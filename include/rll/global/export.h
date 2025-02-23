#pragma once

// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#if defined(DOXYGEN)
#  define RLL_API
#else
#  if defined(_WIN32)
#    if defined(ROLLY_LIBRARY)
#      define RLL_API __declspec(dllexport)
#    elif defined(ROLLY_STATIC_LIBRARY)
#      define RLL_API
#    else
#      define RLL_API __declspec(dllimport)
#    endif
#  else
#    define RLL_API
#  endif
#endif
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)

/**
 * @brief Main namespace for the **rolly** library.
 */
namespace rll {}  // namespace rll
