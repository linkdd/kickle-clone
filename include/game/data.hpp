#pragma once

#include <unordered_map>
#include <optional>
#include <variant>
#include <vector>
#include <string>

#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

namespace game::data {
  struct tileset {
    std::unordered_map<int, entt::resource<tw::sdl::rendering::animation>> tiles;
  };

  struct object {
    struct enemy_spawner {
      std::string type;
    };

    struct player_spawn {};

    using object_type = std::variant<
      enemy_spawner,
      player_spawn
    >;

    int row;
    int col;
    object_type type;
  };

  struct level {
    struct tile_layer {
      std::vector<int> tiles;
    };

    struct object_layer {
      std::unordered_map<int, object> objects;
    };

    using layer_type = std::variant<
      tile_layer,
      object_layer
    >;

    struct tileset_ref {
      entt::resource<tileset> resource;
      int firstgid;
    };

    int width;
    int height;
    std::vector<layer_type> layers;
    std::vector<tileset_ref> tilesets;
  };
}
