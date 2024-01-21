#include <game/assets/utils.hpp>

std::string SDL_RWreadAll(SDL_RWops* rw, bool freesrc) {
  if (rw == nullptr) {
    return std::string{};
  }

  auto size = SDL_RWsize(rw);
  auto buffer = std::string(size + 1, '\0');

  Sint64 nb_read_total = 0;
  Sint64 nb_read = 1;
  auto* tmp = buffer.data();
  while (nb_read_total < size && nb_read != 0) {
    nb_read = SDL_RWread(rw, tmp, 1, (size - nb_read_total));
    nb_read_total += nb_read;
    tmp += nb_read;
  }

  if (freesrc) {
    SDL_RWclose(rw);
  }

  return buffer;
}