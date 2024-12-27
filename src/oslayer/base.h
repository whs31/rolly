#pragma once

#include <rolly/global/definitions.h>
#include <rolly/global/platform_definitions.h>
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly::oslayer {
  namespace fs = std::filesystem;
}  // namespace rolly::oslayer

#ifdef ROLLY_OS_WINDOWS
// NOLINTNEXTLINE
#  define ___os___ win
#else
// NOLINTNEXTLINE
#  define ___os___ linux
#endif  // ROLLY_OS_WINDOWS
