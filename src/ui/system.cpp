#include <trollworks.hpp>

#include <game/ui/system.hpp>
#include <game/ui/components/menu.hpp>
#include <game/ui/components/hud.hpp>

using namespace entt::literals;

namespace game::ui {
  void system::execute() {
    auto& registry = tw::scene_manager::main().registry();

    auto renderers = registry.view<component_type>();

    for (auto &e_renderer : renderers) {
      auto& c_renderer = renderers.get<component_type>(e_renderer);
      c_renderer.renderer();
    }
  }

  namespace archetypes {
    system::component_type menu() {
      return system::component_type{
        .renderer = []() { tw::ui::h<game::ui::menu>("menu"_hs); }
      };
    }

    system::component_type hud() {
      return system::component_type{
        .renderer = []() { tw::ui::h<game::ui::hud>("hud"_hs); }
      };
    }
  }
}
