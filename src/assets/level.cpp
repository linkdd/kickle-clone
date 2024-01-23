#include <algorithm>
#include <charconv>
#include <ranges>
#include <cctype>

#include <game/assets/level.hpp>

using std::operator""sv;

namespace game::assets {
  std::string_view trim(std::string_view sv) {
    auto is_space = [](char c) { return std::isspace(c); };
    auto start = std::ranges::find_if_not(sv, is_space);
    auto end = std::ranges::find_if_not(sv | std::views::reverse, is_space).base();

    if (start == sv.end() || end == sv.begin()) {
      return {};
    }

    return std::string_view{
      &*start,
      static_cast<size_t>(std::distance(start, end))
    };
  }

  std::vector<std::string_view> splitlines(std::string_view sv, int height) {
    auto lines = std::vector<std::string_view>{};
    lines.reserve(height);

    auto line_start = sv.begin();
    for (auto it = sv.begin(); it != sv.end(); it++) {
      if (*it == '\n') {
        auto line = std::string_view{line_start, it};

        if (!line.empty() && line.back() == '\r') {
          line.remove_suffix(1);
        }

        lines.push_back(line);
        line_start = it + 1;
      }
    }

    if (line_start != sv.end()) {
      lines.emplace_back(line_start, sv.end());
    }

    return lines;
  }

  std::vector<int> splittiles(std::string_view sv, int width) {
    auto tiles = std::vector<int>{};
    tiles.reserve(width);

    int current_n = 0;
    bool n_started = false;

    if (sv.empty()) {
      return tiles;
    }

    if (sv.back() == ',') {
      sv.remove_suffix(1);
    }

    for (char c : sv) {
      if (c == ',') {
        if (n_started) {
          tiles.push_back(current_n);
          current_n = 0;
          n_started = false;
        }
      }
      else if (std::isdigit(c)) {
        n_started = true;
        current_n = current_n * 10 + (c - '0');
      }
      else {
        throw std::runtime_error("Invalid tilemap data");
      }
    }

    if (n_started) {
      tiles.push_back(current_n);
    }

    return tiles;
  }

  level::loader_type::result_type level::loader_type::operator()(
    entt::resource<game::assets::tileset> tileset,
    const tinyxml2::XMLDocument& doc
  ) const {
    auto result = std::make_shared<level>();
    result->tileset = tileset;

    auto map = doc.FirstChildElement("map");
    auto width = map->IntAttribute("width");
    auto height = map->IntAttribute("height");
    auto tilewidth = map->IntAttribute("tilewidth");
    auto tileheight = map->IntAttribute("tileheight");

    auto firstgid = map->FirstChildElement("tileset")->IntAttribute("firstgid");

    for (
      auto layer = map->FirstChildElement("layer");
      layer;
      layer = layer->NextSiblingElement("layer")
    ) {
      auto data = layer->FirstChildElement("data");
      auto encoding = data->Attribute("encoding");
      auto compression = data->Attribute("compression");

      if (encoding != std::string_view("csv")) {
        throw std::runtime_error("Unsupported encoding");
      }

      if (compression != nullptr) {
        throw std::runtime_error("Unsupported compression");
      }

      auto tiles = std::vector<std::optional<int>>{};
      tiles.resize(width * height);

      auto tilemap_data = std::string_view{data->GetText()};
      auto tilemap = trim(tilemap_data);

      int y = 0;
      for (auto row : splitlines(tilemap, height)) {
        int x = 0;
        for (auto col : splittiles(row, width)) {
          if (col == 0) {
            tiles[y * width + x] = std::nullopt;
          }
          else {
            tiles[y * width + x] = col - firstgid;
          }

          x++;
        }

        y++;
      }

      result->layers.push_back(level::layer{
        .width = width,
        .height = height,
        .tiles = std::move(tiles),
      });
    }

    for (
      auto objectgroup = map->FirstChildElement("objectgroup");
      objectgroup;
      objectgroup = objectgroup->NextSiblingElement("objectgroup")
    ) {
      auto group = level::objectgroup{};

      for (
        auto object = objectgroup->FirstChildElement("object");
        object;
        object = object->NextSiblingElement("object")
      ) {
        auto row = object->IntAttribute("y") / tileheight;
        auto col = object->IntAttribute("x") / tilewidth;
        auto name = std::string_view{object->Attribute("name")};
        auto properties = object->FirstChildElement("properties");

        auto e_object = group.create();
        auto& c_object = group.emplace<level::object>(e_object);
        c_object.row = row;
        c_object.col = col;

        if (name == "player_spawn") {
          group.emplace<level::object::player_spawn>(e_object);
        }
        else if (name == "enemy_spawner") {
          auto& c_spawner = group.emplace<level::object::enemy_spawner>(e_object);

          for (
            auto property = properties->FirstChildElement("property");
            property;
            property = property->NextSiblingElement("property")
          ) {
            auto name = std::string_view{property->Attribute("name")};

            if (name == "type") {
              c_spawner.type = std::string_view{property->Attribute("value")};
            }
          }
        }
      }
    }

    return result;
  }
}
