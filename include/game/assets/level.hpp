#pragma once

#include <optional>
#include <memory>
#include <vector>

#include <trollworks-backend-sdl.hpp>
#include <trollworks-backend-sdl/assets.hpp>
#include <tinyxml2.h>

#include "./tileset.hpp"

namespace game::assets {
  struct level {
    struct layer {
      int width;
      int height;
      std::vector<std::optional<int>> tiles;
    };

    entt::resource<tileset> tileset;
    std::vector<layer> layers;

    struct loader_type {
      using result_type = std::shared_ptr<level>;

      result_type operator()(
        entt::resource<game::assets::tileset> tileset,
        const tinyxml2::XMLDocument& doc
      ) const;
    };
  };
}
