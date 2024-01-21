#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>
#include <game/assets/loader.hpp>

int SDL_main(int, char** argv) {
  SDL_Log("ARGV0=%s", argv[0]);
  SDL_Log("BASE_PATH=%s", SDL_GetBasePath());

  auto backend = tw::sdl::sdl_backend{"Kickle Clone"}
    .with_logical_size({1024, 768})
    .with_fullscreen(false);

  auto loader = resource_loader{
    .argv0 = argv[0],
    .backend = backend
  };

  auto loop = tw::game_loop{}
    .with_fps(60)
    .with_ups(50)
    .with_backend(backend)
    .on_setup<&resource_loader::setup>(loader)
    .on_teardown<&resource_loader::teardown>(loader);

  loop.run();
  return 0;
}
