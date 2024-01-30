#pragma once

#include <trollworks.hpp>

namespace game::ui {
  struct hud {
    void operator()(tw::ui::hooks& h);
  };
}
