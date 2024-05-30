#pragma once

#include <trollworks.hpp>
#include <game/messages.hpp>
#include <game/state.hpp>

namespace game::lifecycle {
  class manager {
    public:
      void setup(tw::controlflow& cf);
      void teardown();

    private:
      void on_quit_game(const messages::quit_game& msg);
      void on_load_level(const messages::load_level& msg);
  };
}
