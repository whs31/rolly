#pragma once

#include <rolly/global/definitions.h>
#include <rolly/global/platform_definitions.h>
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

/**
 * @brief Internal namespace for OS-agnostic wrapper functions and classes.
 */
namespace rolly::oslayer {}  // namespace rolly::oslayer

#ifdef ROLLY_OS_WINDOWS
// NOLINTNEXTLINE
#  define ___os___ win
#else
// NOLINTNEXTLINE
#  define ___os___ linux
#endif  // ROLLY_OS_WINDOWS
