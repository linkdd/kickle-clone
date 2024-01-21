#include <cstdlib>
#include <filesystem>

#include <physfs.h>
#include <physfsrwops.h>
#include <trollworks-backend-sdl/assets.hpp>

#include <game/assets/utils.hpp>
#include <game/assets/loader.hpp>
#include <game/assets/tileset.hpp>
#include <game/assets/level.hpp>

using namespace entt::literals;

namespace game::assets {
  entt::resource<tw::sdl::aseprite::spritesheet> load_spritesheet(SDL_Renderer *renderer) {
    SDL_Log("  - spritesheet");

    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();

    auto spritesheet_texture = PHYSFSRWOPS_openRead("spritesheet.png");
    auto spritesheet_meta = nlohmann::json::parse(
      SDL_RWreadAll(PHYSFSRWOPS_openRead("spritesheet.json"), true)
    );

    auto res = spritesheet_cache.load(
      "spritesheet"_hs,
      renderer,
      spritesheet_texture,
      spritesheet_meta
    );

    return res.first->second;
  }

  entt::resource<tileset> load_tileset(
    entt::resource<tw::sdl::aseprite::spritesheet> spritesheet
  ) {
    SDL_Log("  - tileset");

    auto& tileset_cache = tw::asset_manager<tileset>::cache();

    auto tileset_doc = tinyxml2::XMLDocument{};
    auto xml_data = SDL_RWreadAll(PHYSFSRWOPS_openRead("tilemaps/tileset.tsx"), true);
    if (tileset_doc.Parse(xml_data.data(), xml_data.size()) != tinyxml2::XML_SUCCESS) {
      SDL_Log("Could not parse tileset XML: %s", tileset_doc.ErrorName());
      std::exit(1);
    }

    auto res = tileset_cache.load(
      "tileset"_hs,
      spritesheet,
      tileset_doc
    );

    return res.first->second;
  }

  entt::resource<level> load_level(
    entt::resource<tileset> tileset,
    const std::string& level_name
  ) {
    SDL_Log("  - %s", level_name.c_str());

    std::string level_path = "tilemaps/" + level_name + ".tmx";
    auto& level_cache = tw::asset_manager<level>::cache();

    auto level_doc = tinyxml2::XMLDocument{};
    auto xml_data = SDL_RWreadAll(PHYSFSRWOPS_openRead(level_path.c_str()), true);
    if (level_doc.Parse(xml_data.data(), xml_data.size()) != tinyxml2::XML_SUCCESS) {
      SDL_Log("Could not parse level XML: %s", level_doc.ErrorName());
      std::exit(1);
    }

    auto res = level_cache.load(
      entt::hashed_string{level_name.c_str()},
      tileset,
      level_doc
    );

    return res.first->second;
  }

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

    SDL_Log("Load resources:");

    auto spritesheet = load_spritesheet(backend.renderer());
    auto tileset = load_tileset(spritesheet);

    auto levels = nlohmann::json::parse(
      SDL_RWreadAll(PHYSFSRWOPS_openRead("levels.json"), true)
    );

    for (auto& level : levels.at("levels")) {
      auto level_name = level.get<std::string>();
      load_level(tileset, level_name);
    }
  }

  void resource_loader::teardown() {
    SDL_Log("Release resources");
    auto& level_cache = tw::asset_manager<level>::cache();
    level_cache.clear();

    auto& tileset_cache = tw::asset_manager<tileset>::cache();
    tileset_cache.clear();

    auto& spritesheet_cache = tw::asset_manager<tw::sdl::aseprite::spritesheet>::cache();
    spritesheet_cache.clear();

    SDL_Log("Teardown PhysicsFS");
    PHYSFS_deinit();
  }
}