#include <trollworks-backend-sdl.hpp>

#include <game/scenes/titlescreen.hpp>

using namespace entt::literals;

namespace game::scenes::archetypes {
  entt::entity background(entt::registry& registry) {
    auto* backend = entt::locator<tw::sdl::sdl_backend*>::value();
    auto& spritesheet_cache = tw::asset_manager<tw::sdl::assets::aseprite::spritesheet>::cache();
    auto [it, _loaded] = spritesheet_cache.load(
      "titlescreen"_hs,
      backend->renderer(),
      "titlescreen.json"
    );
    auto [_id, res] = *it;

    auto entity = registry.create();

    auto& c_bg = registry.emplace<tw::sdl::background>(entity);
    c_bg.stretch = true;

    registry.emplace<tw::sdl::drawable>(entity, tw::sdl::drawable{
      .pipeline = tw::sdl::rendering::pipeline{
        {1024, 768},
        std::make_shared<tw::sdl::rendering::sprite_node>(res, 0)
      }
    });

    registry.emplace<tw::sdl::ordering>(entity, tw::sdl::ordering{
      .layer = 0,
      .order_in_layer = 0
    });

    return entity;
  }
}
