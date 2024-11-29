/**
 * @file fl/global/version_definitions.h
 * @brief Version number macro definitions, used by <i>meta::library_metadata</i> global constant.
 */

#pragma once

// NOLINTBEGIN(*-macro-to-enum, *-macro-usage)
#ifndef ROLLY_VERSION_STRING
/**
 * @ingroup meta
 * @brief Major version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#define ROLLY_VERSION_MAJOR 2

/**
 * @ingroup meta
 * @brief Minor version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#define ROLLY_VERSION_MINOR 1

/**
 * @ingroup meta
 * @brief Patch version number of this library build.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @version 2.1.32
 */
#define ROLLY_VERSION_PATCH 39

/**
 * @internal
 */
#  define ROLLY_STRINGIFY2(x) #x

/**
 * @internal
 */
#  define ROLLY_STRINGIFY(x) ROLLY_STRINGIFY2(x)

/**
 * @ingroup meta
 * @brief String representation of this library version number.
 * @note Consider using <i>meta::library_metadata</i> variable to access this value.
 * @see ROLLY_VERSION_MAJOR, ROLLY_VERSION_MINOR, ROLLY_VERSION_PATCH
 */
#  define ROLLY_VERSION_STRING ROLLY_STRINGIFY(ROLLY_VERSION_MAJOR) "." ROLLY_STRINGIFY(ROLLY_VERSION_MINOR) "." ROLLY_STRINGIFY(ROLLY_VERSION_PATCH)

#  undef ROLLY_STRINGIFY2
#  undef ROLLY_STRINGIFY
#endif  // ROLLY_VERSION_STRING
        // NOLINTEND(*-macro-to-enum, *-macro-usage)
