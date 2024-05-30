#include <cstdlib>
#include <filesystem>

#include <physfs.h>
#include <physfsrwops.h>

#include <game/assets/loader.hpp>
#include <game/logging.hpp>

namespace game::assets {
  class physfs_loader : public tw::sdl::assets::file_loader {
    public:
      SDL_RWops* open(const std::filesystem::path& filepath) override {
        logging::logger().info("hello");
        return PHYSFSRWOPS_openRead(filepath.c_str());
      }
  };

  void resource_loader::setup(tw::controlflow&) {
    logging::logger().debug("Initialize PhysicsFS");
    if (!PHYSFS_init(argv0.c_str())) {
      logging::logger().error(
        "Could not initialize PhysicsFS",
        logfmtxx::field{"reason", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())}
      );
      std::exit(1);
    }

    auto data_path = std::filesystem::path{PHYSFS_getBaseDir()} / "data.zip";
    logging::logger().debug(
      "Mount data archive",
      logfmtxx::field{"path", data_path.c_str()}
    );
    if (!PHYSFS_mount(data_path.string().c_str(), nullptr, 1)) {
      logging::logger().error(
        "Could not mount data archive",
        logfmtxx::field{"reason", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())}
      );
      std::exit(1);
    }

    tw::sdl::assets::register_file_loader<physfs_loader>();
  }

  void resource_loader::teardown() {
    logging::logger().debug("Teardown PhysicsFS");
    PHYSFS_deinit();
  }
}
