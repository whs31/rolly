#pragma once

// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#if defined(DOXYGEN)
#  define ROLLY_API
#else
#  if defined(_WIN32)
#    if defined(ROLLY_LIBRARY)
#      define ROLLY_API __declspec(dllexport)
#    elif defined(ROLLY_STATIC_LIBRARY)
#      define ROLLY_API
#    else   // defined(ROLLY_LIBRARY)
#      define ROLLY_API __declspec(dllimport)
#    endif  // defined(ROLLY_LIBRARY)
#  else     // defined(_WIN32)
#    define ROLLY_API
#  endif    // defined(_WIN32)
#endif
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)

#if ! defined(ROLLY_COMPAT) || defined(DOXYGEN)
#  define ___rolly_cxx20___
#endif  // __cplusplus >= 202002L

/**
 * @brief Main namespace for the **rolly** library.
 */
namespace rolly {}  // namespace rolly
