#include <iostream>

#include <trollworks.hpp>

#include <game/logging.hpp>

namespace game::logging {
  namespace details {
    void printer::operator()(const std::string& message) {
      std::cout << message << std::endl;
    }
  }

  logger_type& logger() {
    if (!entt::locator<logger_type>::has_value()) {
      entt::locator<logger_type>::emplace(details::printer{});
    }

    return entt::locator<logger_type>::value();
  }
}
