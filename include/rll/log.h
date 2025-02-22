#pragma once

#include <rll/global/definitions.h>

// NOLINTBEGIN
#  include <spdlog/spdlog.h>

namespace rll {
  [[nodiscard]] inline spdlog::logger& logger() { return *spdlog::default_logger(); }

  [[nodiscard]] inline spdlog::logger& logger(std::string_view const name) {
    return *spdlog::get(name.data());
  }
}  // namespace rll
// NOLINTEND
