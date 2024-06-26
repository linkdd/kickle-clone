#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>
#include <trollworks-imgui-sdl.hpp>

#include <game/lifecycle/manager.hpp>
#include <game/assets/loader.hpp>
#include <game/ui/system.hpp>

using namespace entt::literals;

int main(int, char** argv) {
  auto mgr = game::lifecycle::manager{};
  auto gui = game::ui::system{};

  auto loop = tw::game_loop{};
  auto backend = tw::sdl::sdl_backend{"Kickle Clone"};
  auto gui_backend = tw::sdl::ui::imgui_sdl_plugin{backend};
  auto loader = game::assets::resource_loader{.argv0 = argv[0]};

  entt::locator<tw::sdl::sdl_backend*>::emplace(&backend);

  backend
    .with_logical_size({1024, 768})
    .with_fullscreen(false)
    .on_event<&tw::sdl::ui::imgui_sdl_plugin::process_event>(gui_backend)
    .on_gui<&tw::sdl::ui::imgui_sdl_plugin::render>(gui_backend);

  gui_backend
    .on_render<&game::ui::system::execute>(gui);

  loop
    .with_fps(60)
    .with_ups(50)
    .with_backend(backend)
    .on_setup<&tw::sdl::ui::imgui_sdl_plugin::setup>(gui_backend)
    .on_teardown<&tw::sdl::ui::imgui_sdl_plugin::teardown>(gui_backend)
    .on_setup<&game::assets::resource_loader::setup>(loader)
    .on_teardown<&game::assets::resource_loader::teardown>(loader)
    .on_setup<&game::lifecycle::manager::setup>(mgr)
    .on_teardown<&game::lifecycle::manager::teardown>(mgr)
    .run();

  return 0;
}
