#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <trollworks-backend-sdl.hpp>
#include <tinyxml2.h>

namespace game::assets {
  struct tileset {
    entt::resource<tw::sdl::aseprite::spritesheet> spritesheet;
    std::unordered_map<int, std::string> tiles;

    struct loader_type {
      using result_type = std::shared_ptr<tileset>;

      result_type operator()(
        entt::resource<tw::sdl::aseprite::spritesheet> spritesheet,
        const tinyxml2::XMLDocument& doc
      ) const;
    };
  };
}
