#include <tinyxml2.h>

#include <game/assets/tiled-editor.hpp>
#include <game/logging.hpp>

using namespace entt::literals;

namespace game::assets::tiled_editor {
  tileset::loader_type::result_type tileset::loader_type::operator()(
    SDL_Renderer* renderer,
    const std::filesystem::path& path
  ) const {
    auto& animation_cache = tw::asset_manager<tw::sdl::assets::aseprite::animation>::cache();

    auto tileset_rw = tw::sdl::assets::open_file(path);
    if (tileset_rw == nullptr) {
      logging::logger().error(
        "Could not open tileset",
        logfmtxx::field{"path", path.c_str()}
      );
      return nullptr;
    }

    auto tileset_doc = tinyxml2::XMLDocument{};
    auto xml_data = SDL_RWreadAll(tileset_rw, true);
    if (tileset_doc.Parse(xml_data.data(), xml_data.size()) != tinyxml2::XML_SUCCESS) {
      logging::logger().error(
        "Could not parse tileset",
        logfmtxx::field{"reason", tileset_doc.ErrorName()}
      );
      return nullptr;
    }

    auto result = std::make_shared<data::tileset>();

    for (
      auto tile_elt = tileset_doc.FirstChildElement("tileset")->FirstChildElement("tile");
      tile_elt != nullptr;
      tile_elt = tile_elt->NextSiblingElement("tile")
    ) {
      auto tile_id = tile_elt->IntAttribute("id", -1);
      if (tile_id != -1) {
        auto sheet_path = (
          path.parent_path() / std::filesystem::path{
            tile_elt->FirstChildElement("properties")
                    ->FirstChildElement("property")
                    ->Attribute("value")
          }
        ).lexically_normal();

        auto [it, _loaded] = animation_cache.load(
          entt::hashed_string{sheet_path.c_str()},
          renderer,
          sheet_path
        );
        auto [_id, sheet] = *it;
        result->tiles[tile_id] = sheet;
      }
    }

    return result;
  }
}
