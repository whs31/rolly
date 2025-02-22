#pragma once

#include <rll/global/definitions.h>
#include <rll/global/platform_definitions.h>
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

/**
 * @brief Internal namespace for OS-agnostic wrapper functions and classes.
 */
namespace rll::oslayer {}  // namespace rll::oslayer

#ifdef RLL_OS_WINDOWS
// NOLINTNEXTLINE
#  define ___os___ win
#else
// NOLINTNEXTLINE
#  define ___os___ linux
#endif  // RLL_OS_WINDOWS
