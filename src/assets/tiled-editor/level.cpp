#include <cstring>
#include <charconv>
#include <string_view>
#include <string>
#include <ranges>

#include <tinyxml2.h>

#include <game/assets/tiled-editor.hpp>
#include <game/logging.hpp>

using namespace entt::literals;

namespace game::assets::tiled_editor {
  level::loader_type::result_type level::loader_type::operator()(
    SDL_Renderer* renderer,
    const std::filesystem::path& path
  ) const {
    auto& tileset_cache = tw::asset_manager<tileset>::cache();

    auto level_rw = tw::sdl::assets::open_file(path);
    if (level_rw == nullptr) {
      logging::logger().error(
        "Could not open level",
        logfmtxx::field{"path", path.c_str()}
      );
      return nullptr;
    }

    auto level_doc = tinyxml2::XMLDocument{};
    auto xml_data = SDL_RWreadAll(level_rw, true);
    if (level_doc.Parse(xml_data.data(), xml_data.size()) != tinyxml2::XML_SUCCESS) {
      logging::logger().error(
        "Could not parse level",
        logfmtxx::field{"reason", level_doc.ErrorName()}
      );
      return nullptr;
    }

    auto result = std::make_shared<data::level>();

    auto map_elt   = level_doc.FirstChildElement("map");
    result->width  = map_elt->IntAttribute("width", 0);
    result->height = map_elt->IntAttribute("height", 0);

    for (
      auto child_elt = map_elt->FirstChildElement();
      child_elt != nullptr;
      child_elt = child_elt->NextSiblingElement()
    ) {
      auto name = std::string{child_elt->Name()};

      if (name == "tileset") {
        auto firstgid = child_elt->IntAttribute("firstgid", 0);
        auto tileset_path = (
          path.parent_path() / std::filesystem::path{
            child_elt->Attribute("source")
          }
        ).lexically_normal();

        auto [it, _loaded] = tileset_cache.load(
          entt::hashed_string{tileset_path.c_str()},
          renderer,
          tileset_path
        );
        auto [_id, res] = *it;

        result->tilesets.push_back(data::level::tileset_ref{
          .resource = res,
          .firstgid = firstgid
        });
      }
      else if (name == "layer") {
        auto tiles_csv = std::string{child_elt->FirstChildElement("data")->GetText()};
        auto tile_layer = data::level::tile_layer{};

        using std::operator""sv;
        for (auto line : std::views::split(tiles_csv, "\n"sv)) {
          auto row = line
            | std::views::drop_while(isspace)
            | std::views::reverse
            | std::views::drop_while(isspace)
            | std::views::reverse;

          if (!row.empty()) {
            for (auto cell : std::views::split(row, ","sv)) {
              auto view = std::string_view(&*cell.begin(), std::ranges::distance(cell));
              int tile_id = 0;

              std::from_chars(view.data(), view.data() + view.size(), tile_id);

              tile_layer.tiles.push_back(tile_id);
            }
          }
        }

        result->layers.push_back(tile_layer);
      }
      else if (name == "objectgroup") {
        auto obj_layer = data::level::object_layer{};

        for (
          auto obj_elt = child_elt->FirstChildElement("object");
          obj_elt != nullptr;
          obj_elt = obj_elt->NextSiblingElement("object")
        ) {
          auto name = std::string{obj_elt->Attribute("name")};
          auto obj_id = obj_elt->IntAttribute("id", -1);
          auto obj_row = obj_elt->IntAttribute("y", 0);
          auto obj_col = obj_elt->IntAttribute("x", 0);

          if (name == "enemy-spawner") {
            obj_layer.objects[obj_id] = data::object{
              .row = obj_row,
              .col = obj_col,
              .type = data::object::enemy_spawner{
                .type = obj_elt->Attribute("type")
              }
            };
          }
          else if (name == "player-spawn") {
            obj_layer.objects[obj_id] = data::object{
              .row = obj_row,
              .col = obj_col,
              .type = data::object::player_spawn{}
            };
          }
        }

        result->layers.push_back(obj_layer);
      }
    }

    // sort by firstgid, bigger first
    std::ranges::sort(result->tilesets, {}, &data::level::tileset_ref::firstgid);
    std::ranges::reverse(result->tilesets);

    return result;
  }
}
