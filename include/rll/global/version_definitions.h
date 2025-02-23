#pragma once

#include <rll/global/definitions.h>

// NOLINTBEGIN(*-macro-to-enum, *-macro-usage)
#ifndef RLL_VERSION_STRING
/**
 * @ingroup meta
 * @brief Major version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#  define RLL_VERSION_MAJOR 2

/**
 * @ingroup meta
 * @brief Minor version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#  define RLL_VERSION_MINOR 4

/**
 * @ingroup meta
 * @brief Patch version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#  define RLL_VERSION_PATCH 0

/**
 * @ingroup meta
 * @brief String representation of this library version number.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @see RLL_VERSION_MAJOR, RLL_VERSION_MINOR, RLL_VERSION_PATCH
 */
#  define RLL_VERSION_STRING RLL_STRINGIFY(RLL_VERSION_MAJOR) "." RLL_STRINGIFY(RLL_VERSION_MINOR) "." RLL_STRINGIFY(RLL_VERSION_PATCH)
#endif  // RLL_VERSION_STRING
        // NOLINTEND(*-macro-to-enum, *-macro-usage)
