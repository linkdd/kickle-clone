#pragma once

#include <functional>

namespace game::ui {
  struct system {
    struct component_type {
      std::function<void()> renderer;
    };

    void execute();
  };

  namespace archetypes {
    system::component_type menu();
    system::component_type hud();
  }
}
