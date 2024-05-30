#pragma once

#include <string>
#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

#include <game/state.hpp>

namespace game::assets {
  struct resource_loader {
    const std::string argv0;

    void setup(tw::controlflow&);
    void teardown();
  };
}
