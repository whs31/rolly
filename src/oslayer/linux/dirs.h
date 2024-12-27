#pragma once

#include "../base.h"

#if defined(ROLLY_OS_LINUX) || defined(DOXYGEN)

namespace rolly::oslayer::linux {
  [[nodiscard]] std::filesystem::path home_dir();
  [[nodiscard]] std::filesystempath xdg_runtime_dir();
}  // namespace rolly::oslayer::linux

#endif
