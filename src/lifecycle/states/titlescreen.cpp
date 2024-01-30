#include <trollworks.hpp>

#include <game/lifecycle/states/titlescreen.hpp>
#include <game/scenes/titlescreen.hpp>
#include <game/ui/system.hpp>

namespace game::lifecycle {
  void titlescreen::enter(game::state::global&) {
    tw::scene_manager::main().load(scenes::titlescreen{});
  }
}
