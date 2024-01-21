#pragma once

#include <string>
#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

struct resource_loader {
  const std::string argv0;
  tw::sdl::sdl_backend& backend;

  void setup(tw::controlflow&);
  void teardown();
};
