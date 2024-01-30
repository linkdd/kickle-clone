#pragma once

#include <trollworks.hpp>
#include <aitoolkit/fsm.hpp>

#include <game/messages.hpp>
#include <game/state.hpp>

namespace game::lifecycle {
  class manager {
    public:
      manager(state::global& global_state);

      void setup(tw::controlflow& cf);
      void update(float delta_time, tw::controlflow& cf);
      void teardown();

    private:
      void on_quit_game(const messages::quit_game& msg);
      void on_load_level(const messages::load_level& msg);

    private:
      state::global& m_global_state;
      aitoolkit::fsm::simple_machine<state::global> m_fsm;
  };
}
