#include <game/scenes/level.hpp>
#include <game/assets/level.hpp>

namespace game::scenes {
  void level::load(entt::registry& registry) {
    // create camera entity
    auto e_camera = registry.create();

    auto& c_camera = registry.emplace<tw::sdl::camera>(e_camera);
    c_camera.depth = 0;
    c_camera.view = SDL_FRect{
      .x = 15 * 32.0f,
      .y = 10 * 32.0f,
      .w = 30 * 32.0f,
      .h = 20 * 32.0f
    };
    c_camera.viewport = SDL_FRect{
      .x = 32.0f,
      .y = 96.0f,
      .w = 30 * 32.0f,
      .h = 20 * 32.0f
    };

    // create level entities
    auto& level_cache = tw::asset_manager<assets::level>::cache();
    auto level = level_cache[m_level_id];

    for (size_t layer_idx = 0; layer_idx < level->layers.size(); layer_idx++) {
      auto& layer = level->layers[layer_idx];

      for (int y = 0; y < layer.height; y++) {
        for (int x = 0; x < layer.width; x++) {
          auto& tile = layer.tiles[y * layer.width + x];

          if (tile.has_value()) {
            auto tile_id = tile.value();
            auto tile_name = level->tileset->tiles[tile_id];
            auto spritesheet = level->tileset->spritesheet;
            auto sprite = spritesheet->get_sprite(tile_name);

            if (sprite.has_value()) {
              auto e_tile = registry.create();

              auto& c_transform = registry.emplace<tw::sdl::transform>(e_tile);
              c_transform.position = SDL_FPoint{.x = x * 32.0f, .y = y * 32.0f};
              c_transform.scale = SDL_FPoint{.x = 1.0f, .y = 1.0f};

              auto& c_sprite = registry.emplace<tw::sdl::sprite>(e_tile);
              c_sprite.texture = sprite.value().texture;
              c_sprite.source = sprite.value().source;
              c_sprite.layer = static_cast<Uint32>(layer_idx);
              c_sprite.order_in_layer = static_cast<Uint32>(y * layer.width + x);
            }
          }
        }
      }
    }

    registry.sort<tw::sdl::sprite>([](auto& lhs, auto& rhs) {
      if (lhs.layer == rhs.layer) {
        return lhs.order_in_layer < rhs.order_in_layer;
      }

      return lhs.layer < rhs.layer;
    });
  }

  void level::unload(entt::registry& registry) {
    registry.clear();
  }
}
