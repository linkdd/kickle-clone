#pragma once

#include <filesystem>
#include <memory>

#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

#include <game/data.hpp>

namespace game::assets::tiled_editor {
  struct tileset {
    using resource_type = data::tileset;

    struct loader_type {
      using result_type = std::shared_ptr<resource_type>;

      result_type operator()(
        SDL_Renderer* renderer,
        const std::filesystem::path& path
      ) const;
    };
  };

  struct level {
    using resource_type = data::level;

    struct loader_type {
      using result_type = std::shared_ptr<resource_type>;

      result_type operator()(
        SDL_Renderer* renderer,
        const std::filesystem::path& path
      ) const;
    };
  };
}
