#pragma once

#include <trollworks.hpp>

namespace game::messages {
  struct quit_game {};

  struct load_level {
    entt::id_type level_id;
  };
}
