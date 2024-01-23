#pragma once

#include <optional>
#include <memory>
#include <vector>

#include <tinyxml2.h>

#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

#include "./tileset.hpp"

namespace game::assets {
  struct level {
    struct layer {
      int width;
      int height;
      std::vector<std::optional<int>> tiles;
    };

    struct object {
      int row;
      int col;

      struct enemy_spawner {
        std::string type;
      };

      struct player_spawn {};
    };

    using objectgroup = entt::registry;

    entt::resource<tileset> tileset;
    std::vector<layer> layers;
    std::vector<objectgroup> objectgroups;

    struct loader_type {
      using result_type = std::shared_ptr<level>;

      result_type operator()(
        entt::resource<game::assets::tileset> tileset,
        const tinyxml2::XMLDocument& doc
      ) const;
    };
  };
}
