#pragma once

#include <optional>
#include <vector>

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
}
