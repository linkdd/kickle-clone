#include <trollworks-backend-sdl.hpp>

#include <game/scenes/titlescreen.hpp>

namespace game::scenes::archetypes {
  entt::entity camera(entt::registry& registry) {
    auto entity = registry.create();

    registry.emplace<tw::sdl::rendering::camera>(entity, tw::sdl::rendering::camera{
      .depth = 0,
      .view = SDL_FRect{
        .x = 0,
        .y = 0,
        .w = 1024,
        .h = 768
      },
      .viewport = SDL_FRect{
        .x = 0,
        .y = 0,
        .w = 1024,
        .h = 768
      }
    });

    return entity;
  }
}
