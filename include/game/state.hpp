#pragma once

#include <optional>
#include <vector>
#include <string>

#include <trollworks.hpp>

namespace game::state {
  struct tile {
    int layer;
    int row;
    int col;
    std::optional<int> id;
    std::optional<entt::entity> entity;
  };

  struct layer {
    std::vector<tile> tiles;
  };

  struct tilemap {
    std::vector<layer> layers;
  };

  struct global {
    std::vector<std::string> level_names;
    bool quit_requested{false};
  };
}
