#pragma once

#include "export.h"
#include "platform_definitions.h"

#if defined(DOXYGEN_GENERATING_OUTPUT)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Doxygen</b> documentation is being generated.
 */
# define ROLLY_DOC

/**
 * @ingroup macros
 * @brief Expands to <code>constexpr</code> in C++20 mode, to nothing in C++17 mode.
 */
# define ___constexpr___
#endif // DOXYGEN_GENERATING_OUTPUT

#if defined(ROLLY_DOC)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Core</b> library is available and linked.
 */
# define ROLLY_QT_CORE

/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Gui</b> library is available and linked.
 */
# define ROLLY_QT_GUI

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>enabled</b>.
 * @details Enables some additional checks, majority of contract functionality and some assertions.
 * @see ROLLY_NO_DEBUG
 */
# define ROLLY_DEBUG

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>disabled</b>.
 * @details Disables some additional checks, majority of contract functionality and some assertions.
 * @see ROLLY_DEBUG
 */
# define ROLLY_NO_DEBUG

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
# define ___noinline___

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
 * In some cases, inlining will not be possible, for example, when code contains taking the address of a such function:
 *
 * @code {.cpp}
 *  ___inline___ inline void func() { ... } // func will not be inlined
 *  void* ptr = &func;
 *  ...
 *  func();
 * @endcode
 * @see ___noinline___
 */
# define ___inline___
#else // DOXYGEN_GENERATING_OUTPUT
# if defined(QT_CORE_LIB) || __has_include("qtglobal.h") || __has_include("qcoreapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
#  define ROLLY_QT_CORE
# endif
# if defined(QT_GUI_LIB) || __has_include("qpainter.h") || __has_include("qguiapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
#  define ROLLY_QT_GUI
# endif
# if defined(NDEBUG) || defined(QT_NO_DEBUG)
#  define ROLLY_NO_DEBUG
# else
#  define ROLLY_DEBUG
# endif
// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# if defined(ROLLY_COMPILER_MSVC)
#   define ___noinline___ __declspec(noinline)
#   define ___inline___ __forceinline
# else
#   define ___noinline___ __attribute__((noinline))
#   define ___inline___ __attribute__((always_inline)) inline
# endif
# if defined(ROLLY_DEBUG)
#   define ___rolly_debug_only___(...) __VA_ARGS__
#   define ___rolly_release_consteval___
#   define ___rolly_release_unused___
# else // defined(ROLLY_DEBUG)
#   define ___rolly_debug_only___(...)
#   if defined(___rolly_cxx20___)
#     define ___rolly_release_consteval___ consteval
#   else // defined(___rolly_cxx20___)
#     define ___rolly_release_consteval___
#   endif // defined(___rolly_cxx20___)
#   define ___rolly_release_unused___ [[maybe_unused]]
# endif // defined(ROLLY_DEBUG)
# define ___rolly_stringify_impl___(x) #x
# define ___rolly_stringify___(x) ___rolly_stringify_impl___(x)
# if defined(___rolly_cxx20___)
#   define ___concept___(x) x
#   define ___requires___(x) requires x
#   define ___constexpr___ constexpr
#   define ___consteval___ consteval
#   define ___sfinae_requirement___(x)
# else // defined(___rolly_cxx20___)
#   define ___concept___(x) typename
#   define ___requires___(x)
#   define ___constexpr___
#   define ___consteval___ constexpr
#   define ___sfinae_requirement___(x) , typename = std::enable_if_t<x>
# endif // defined(___rolly_cxx20___)
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#endif // ROLLY_DOC

#if __has_include(<spdlog/spdlog.h>)
# define ROLLY_SPDLOG 1
#endif // __has_include(<spdlog/spdlog.h>)