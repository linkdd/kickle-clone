#pragma once

#include <string>
#include <trollworks-backend-sdl.hpp>

std::string SDL_RWreadAll(SDL_RWops* rw, bool freesrc);
