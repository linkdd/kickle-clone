#include <trollworks-backend-sdl.hpp>

#include <game/lifecycle/manager.hpp>
#include <game/scenes/titlescreen.hpp>
//#include <game/scenes/level.hpp>

namespace game::lifecycle {
  void manager::setup(tw::controlflow&) {
    tw::message_bus::main().sink<messages::quit_game>().connect<&manager::on_quit_game>(*this);
    tw::message_bus::main().sink<messages::load_level>().connect<&manager::on_load_level>(*this);

    tw::scene_manager::main().load(scenes::titlescreen{});
  }

  void manager::teardown() {
    tw::message_bus::main().sink<messages::load_level>().disconnect<&manager::on_load_level>(*this);
    tw::message_bus::main().sink<messages::quit_game>().disconnect<&manager::on_quit_game>(*this);
  }

  void manager::on_quit_game(const messages::quit_game&) {
    SDL_Event evt;
    SDL_memset(&evt, 0, sizeof(evt));

    evt.type = SDL_QUIT;
    SDL_PushEvent(&evt);
  }

  void manager::on_load_level(const messages::load_level&) {
    //tw::scene_manager::main().load(scenes::level{msg.level_id});
  }
}
