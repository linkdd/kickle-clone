#include <game/scenes/level.hpp>
#include <game/assets/level.hpp>
#include <game/state.hpp>

namespace game::scenes {
  void make_camera(entt::registry& registry) {
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
  }

  entt::entity make_tile(
    entt::registry& registry,
    SDL_Texture* texture,
    SDL_Rect source,
    assets::level::layer& layer,
    size_t layer_idx,
    int row,
    int col
  ) {
    auto e_tile = registry.create();

    auto& c_transform = registry.emplace<tw::sdl::transform>(e_tile);
    c_transform.position = SDL_FPoint{.x = col * 32.0f, .y = row * 32.0f};
    c_transform.scale = SDL_FPoint{.x = 1.0f, .y = 1.0f};

    auto& c_sprite = registry.emplace<tw::sdl::sprite>(e_tile);
    c_sprite.texture = texture;
    c_sprite.source = source;
    c_sprite.layer = static_cast<Uint32>(layer_idx);
    c_sprite.order_in_layer = static_cast<Uint32>(row * layer.width + col);

    return e_tile;
  }

  void level::load(entt::registry& registry) {
    make_camera(registry);

    // create level entities
    auto& level_cache = tw::asset_manager<assets::level>::cache();
    auto level = level_cache[m_level_id];

    auto tilemap_state = state::tilemap{};
    tilemap_state.layers.resize(level->layers.size());

    for (size_t layer_idx = 0; layer_idx < level->layers.size(); layer_idx++) {
      auto& layer = level->layers[layer_idx];

      auto layer_state = state::layer{};
      layer_state.tiles.resize(layer.width * layer.height);

      for (int row = 0; row < layer.height; row++) {
        for (int col = 0; col < layer.width; col++) {
          auto& tile = layer.tiles[row * layer.width + col];

          auto tile_state = state::tile{
            .layer = static_cast<int>(layer_idx),
            .row = row,
            .col = col,
            .id = std::nullopt,
            .entity = std::nullopt
          };

          if (tile.has_value()) {
            auto tile_id = tile.value();
            auto tile_name = level->tileset->tiles[tile_id];
            auto spritesheet = level->tileset->spritesheet;
            auto frame = spritesheet->get_frame(tile_name);

            if (frame.has_value()) {
              auto [texture, source] = frame.value();

              tile_state.entity = make_tile(
                registry,
                texture,
                source,
                layer,
                layer_idx,
                row,
                col
              );
            }

            tile_state.id = tile_id;
          }

          layer_state.tiles[row * layer.width + col] = tile_state;
        }
      }

      tilemap_state.layers[layer_idx] = layer_state;
    }

    registry.sort<tw::sdl::sprite>([](auto& lhs, auto& rhs) {
      if (lhs.layer == rhs.layer) {
        return lhs.order_in_layer < rhs.order_in_layer;
      }

      return lhs.layer < rhs.layer;
    });

    registry.ctx().emplace<state::tilemap>(tilemap_state);
  }

  void level::unload(entt::registry& registry) {
    registry.clear();
    registry.ctx().erase<state::tilemap>();
  }
}
