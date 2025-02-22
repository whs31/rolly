#pragma once

#include "../base.h"

#if defined(RLL_OS_LINUX) || defined(DOXYGEN)

namespace rll::oslayer::linux {
  [[nodiscard]] std::filesystem::path home_dir();
  [[nodiscard]] std::filesystem::path xdg_runtime_dir();
}  // namespace rll::oslayer::linux

#endif
