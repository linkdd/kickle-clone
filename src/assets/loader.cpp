#include <cstdlib>
#include <filesystem>

#include <physfs.h>
#include <physfsrwops.h>
#include <trollworks-backend-sdl/assets.hpp>

#include <game/assets/utils.hpp>
#include <game/assets/loader.hpp>
#include <game/assets/tileset.hpp>

using namespace entt::literals;

namespace game::assets {
  void resource_loader::setup(tw::controlflow&) {
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

    SDL_Log("Load resources");
    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();
    auto& tileset_cache = tw::asset_manager<tileset>::cache();

    auto spritesheet_texture = PHYSFSRWOPS_openRead("spritesheet.png");
    auto spritesheet_meta = nlohmann::json::parse(
      SDL_RWreadAll(PHYSFSRWOPS_openRead("spritesheet.json"), true)
    );

    auto spritesheet = spritesheet_cache.load(
      "spritesheet"_hs,
      backend.renderer(),
      spritesheet_texture,
      spritesheet_meta
    ).first->second;

    auto tileset_doc = tinyxml2::XMLDocument{};
    auto xml_data = SDL_RWreadAll(PHYSFSRWOPS_openRead("tilemaps/tileset.tsx"), true);
    if (tileset_doc.Parse(xml_data.data(), xml_data.size()) != tinyxml2::XML_SUCCESS) {
      SDL_Log("Could not parse tileset XML: %s", tileset_doc.ErrorName());
      std::exit(1);
    }

    tileset_cache.load(
      "tileset"_hs,
      spritesheet,
      tileset_doc
    );
  }

  void resource_loader::teardown() {
    SDL_Log("Release resources");
    auto& tileset_cache = tw::asset_manager<tileset>::cache();
    tileset_cache.clear();

    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();
    spritesheet_cache.clear();

    SDL_Log("Teardown PhysicsFS");
    PHYSFS_deinit();
  }
}
