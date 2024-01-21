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

    return result;
  }
}
