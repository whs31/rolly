#pragma once

// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#if defined(DOXYGEN)
#  define RLL_API
#else
#  if defined(_WIN32)
#    if defined(RLL_LIBRARY)
#      define RLL_API __declspec(dllexport)
#    elif defined(RLL_STATIC_LIBRARY)
#      define RLL_API
#    else   // defined(RLL_LIBRARY)
#      define RLL_API __declspec(dllimport)
#    endif  // defined(RLL_LIBRARY)
#  else     // defined(_WIN32)
#    define RLL_API
#  endif    // defined(_WIN32)
#endif
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)

/**
 * @brief Main namespace for the **rolly** library.
 */
namespace rll {}  // namespace rll
