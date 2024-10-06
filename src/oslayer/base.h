#pragma once

#include <filesystem>
#include <rolly/global/platform_definitions.h>

namespace rolly::oslayer
{
  namespace fs = std::filesystem;
} // namespace rolly::oslayer

#ifdef ROLLY_OS_WINDOWS
// NOLINTNEXTLINE
# define ___os___ win 
#else 
// NOLINTNEXTLINE
# define ___os___ linux 
#endif // ROLLY_OS_WINDOWS