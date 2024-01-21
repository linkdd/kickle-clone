#include <cstdlib>

#include <filesystem>
#include <string>

#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>
#include <trollworks-backend-sdl/assets.hpp>
#include <physfs.h>
#include <physfsrwops.h>

using namespace entt::literals;

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

struct resource_loader {
  const std::string argv0;
  tw::sdl::sdl_backend& backend;

  void setup(tw::controlflow&) {
    SDL_Log("Initialize PhysicsFS");
    if (!PHYSFS_init(argv0.c_str())) {
      SDL_Log(
        "Could not initialize PhysicsFS: %s",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
      );
      std::exit(1);
    }

    SDL_Log("Mount data archive");
    auto data_path = std::filesystem::path{PHYSFS_getBaseDir()} / "data.zip";
    if (!PHYSFS_mount(data_path.string().c_str(), nullptr, 1)) {
      SDL_Log(
        "Could not mount data archive: %s",
        PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
      );
      std::exit(1);
    }

    SDL_Log("Load spritesheet");
    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();

    auto spritesheet_texture = PHYSFSRWOPS_openRead("spritesheet.png");
    auto spritesheet_meta = nlohmann::json::parse(
      SDL_RWreadAll(PHYSFSRWOPS_openRead("spritesheet.json"), true)
    );

    spritesheet_cache.load(
      "spritesheet"_hs,
      backend.renderer(),
      spritesheet_texture,
      spritesheet_meta
    );
  }

  void teardown() {
    SDL_Log("Release spritesheets");
    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();
    spritesheet_cache.clear();

    SDL_Log("Teardown PhysicsFS");
    PHYSFS_deinit();
  }
};

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
