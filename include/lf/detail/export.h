#pragma once

#if defined(_WIN32)
# if defined(LEAF_LIBRARY)
#   define LF_EXPORT __declspec(dllexport)
# elif defined(LEAF_STATIC_LIBRARY)
#   define LF_EXPORT
# else
#   define LF_EXPORT __declspec(dllimport)
# endif
#else
# define LF_EXPORT
#endif

/// \brief Main namespace for leaf library.
namespace lf {};