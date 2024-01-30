#include <trollworks-backend-sdl.hpp>

#include <game/scenes/titlescreen.hpp>
#include <game/ui/system.hpp>

using namespace entt::literals;

namespace game::scenes {
  static void make_camera(entt::registry& registry) {
    auto e_camera = registry.create();

    auto& c_camera = registry.emplace<tw::sdl::camera>(e_camera);
    c_camera.depth = 0;
    c_camera.view = SDL_FRect{
      .x = 0,
      .y = 0,
      .w = 1024,
      .h = 768
    };
    c_camera.viewport = SDL_FRect{
      .x = 0,
      .y = 0,
      .w = 1024,
      .h = 768
    };
  }

  static void make_background(entt::registry& registry) {
    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();
    auto titlescreen = spritesheet_cache["titlescreen"_hs];

    auto e_bg = registry.create();
    auto& c_sprite = registry.emplace<tw::sdl::sprite>(e_bg);
    c_sprite.texture = titlescreen->get_texture();
    c_sprite.source = titlescreen->get_frame("background").value().source;

    auto& c_bg = registry.emplace<tw::sdl::background>(e_bg);
    c_bg.stretch = true;
  }

  static void make_ui(entt::registry& registry) {
    auto e_ui = registry.create();
    registry.emplace<ui::system::component_type>(e_ui, ui::archetypes::menu());
  }

  void titlescreen::load(entt::registry& registry) {
    make_camera(registry);
    make_background(registry);
    make_ui(registry);
  }

  void titlescreen::unload(entt::registry& registry) {
    registry.clear();
  }
}
