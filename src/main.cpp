#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>


int SDL_main(int, char**) {
  auto backend = tw::sdl::sdl_backend{"Kickle Clone"};
  auto loop = tw::game_loop{}
    .with_fps(60)
    .with_ups(50)
    .with_backend(backend);

  loop.run();
  return 0;
}