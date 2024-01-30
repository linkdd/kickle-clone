#include <game/lifecycle/manager.hpp>
#include <game/lifecycle/states/titlescreen.hpp>
#include <game/scenes/level.hpp>

namespace game::lifecycle {
  manager::manager(state::global& global_state)
    : m_global_state{global_state}
  {}

  void manager::setup(tw::controlflow&) {
    tw::scene_manager::main().registry().ctx().emplace<state::global*>(&m_global_state);

    tw::message_bus::main().sink<messages::quit_game>().connect<&manager::on_quit_game>(*this);
    tw::message_bus::main().sink<messages::load_level>().connect<&manager::on_load_level>(*this);

  }

  void manager::update(float, tw::controlflow& cf) {
    m_fsm.update(m_global_state);

    if (m_global_state.quit_requested) {
      cf = tw::controlflow::exit;
    }
  }

  void manager::teardown() {
    m_fsm.clear_state(m_global_state);

    tw::message_bus::main().sink<messages::load_level>().disconnect<&manager::on_load_level>(*this);
    tw::message_bus::main().sink<messages::quit_game>().disconnect<&manager::on_quit_game>(*this);
  }

  void manager::on_quit_game(const messages::quit_game&) {
    m_global_state.quit_requested = true;
  }

  void manager::on_load_level(const messages::load_level& msg) {
    tw::scene_manager::main().load(scenes::level{msg.level_id});
  }
}
