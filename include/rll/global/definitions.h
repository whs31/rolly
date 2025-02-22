#pragma once

#include <rll/global/export.h>
#include <rll/global/platform_definitions.h>

// NOLINTBEGIN(*-macro-usage)
#if defined(DOXYGEN)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Doxygen</b> documentation is being generated.
 */
#  define RLL_DOC

/**
 * @ingroup macros
 * @brief Expands to <code>constexpr</code> in C++20 mode, to nothing in C++17 mode.
 */
#  define ___constexpr___
#endif  // DOXYGEN

#if defined(RLL_DOC)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Core</b> library is available and linked.
 */
#  define RLL_QT_CORE

/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Gui</b> library is available and linked.
 */
#  define RLL_QT_GUI

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>enabled</b>.
 * @details Enables some additional checks, majority of contract functionality and some assertions.
 * @see RLL_NO_DEBUG
 */
#  define RLL_DEBUG

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>disabled</b>.
 * @details Disables some additional checks, majority of contract functionality and some assertions.
 * @see RLL_DEBUG
 */
#  define RLL_NO_DEBUG

/**
 * @ingroup macros
 * @brief Attribute macro intended for <b>no inlining</b> functions.
 * @details Disables inlining for the function.
 *
 * Example usage:
 *
 * @code {.cpp}
 *  ___noinline___ void func() { ... } // func will not be inlined
 * @endcode
 * @see ___inline___
 */
#  define ___noinline___

/**
 * @ingroup macros
 * @brief Attribute macro intended for <b>force inlining</b> functions.
 * @details Forces inlining for the function.
 *
 * Example usage:
 *
 * @code {.cpp}
 *  ___inline___ inline void func() { ... } // func will be inlined, if possible
 * @endcode
 *
 * In some cases, inlining will not be possible, for example, when code contains taking the address
 * of a such function:
 *
 * @code {.cpp}
 *  ___inline___ inline void func() { ... } // func will not be inlined
 *  void* ptr = &func;
 *  ...
 *  func();
 * @endcode
 * @see ___noinline___
 */
#  define ___inline___
#else  // DOXYGEN
#  if defined(QT_CORE_LIB) \
    || __has_include("qtglobal.h") || __has_include("qcoreapplication.h") || defined(DOXYGEN)
#    define RLL_QT_CORE
#  endif
#  if defined(QT_GUI_LIB) \
    || __has_include("qpainter.h") || __has_include("qguiapplication.h") || defined(DOXYGEN)
#    define RLL_QT_GUI
#  endif
#  if defined(NDEBUG) || defined(QT_NO_DEBUG)
#    define RLL_NO_DEBUG
#  else
#    define RLL_DEBUG
#  endif
// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#  if defined(RLL_COMPILER_MSVC)
#    define ___noinline___ __declspec(noinline)
#    define ___inline___ __forceinline
#  else
#    define ___noinline___ __attribute__((noinline))
#    define ___inline___ __attribute__((always_inline)) inline
#  endif
#  define RLL_STRINGIFY_IMPL(x) #x
#  define RLL_STRINGIFY(x) RLL_STRINGIFY_IMPL(x)
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#endif  // RLL_DOC

#if defined(__GXX_RTTI) || defined(__cpp_rtti) || defined(_CPPRTTI)
#  define RLL_RTTI 1
#endif

// NOLINTEND(*-macro-usage)
