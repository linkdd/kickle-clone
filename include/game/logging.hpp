#pragma once

#include <logfmtxx.hpp>

namespace game::logging {
  namespace details {
    struct printer {
      void operator()(const std::string& message);
    };
  }

  using logger_type = logfmtxx::logger<std::chrono::system_clock, details::printer>;

  logger_type& logger();
}
