#pragma once

#include "global/definitions.h"

#ifdef log
# undef log
#endif // log

#ifdef ROLLY_SPDLOG
# include <spdlog/spdlog.h>

namespace rolly {
  [[nodiscard]] inline spdlog::logger& log() {
    return *spdlog::default_logger();
  }

  [[nodiscard]] inline spdlog::logger& log(std::string_view const name) {
    return *spdlog::get(name.data());
  }
} // namespace rolly
#endif // ROLLY_SPDLOG