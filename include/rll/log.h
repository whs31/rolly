#pragma once

#include "global/definitions.h"

// NOLINTBEGIN
#  include <spdlog/spdlog.h>

namespace rolly {
  [[nodiscard]] inline spdlog::logger& logger() { return *spdlog::default_logger(); }

  [[nodiscard]] inline spdlog::logger& logger(std::string_view const name) {
    return *spdlog::get(name.data());
  }
}  // namespace rolly
// NOLINTEND
