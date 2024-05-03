#pragma once

#include <cassert>
#include <exception>
#include <leaf/global.h>
#include <leaf/os.h>

#if defined(leaf_compiler_clang)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Winvalid-noreturn"
#endif

#if defined(leaf_compiler_clang)
# define leaf_suppress(x) [[gsl::suppress(#x)]]
#else
# if defined(leaf_compiler_msvc) && !defined(leaf_compiler_intel) && !defined(leaf_compiler_nvidia)
#   define leaf_suppress(x) [[gsl::suppress(x)]]
# else
#   define leaf_suppress(x)
# endif
#endif

#if defined(leaf_compiler_clang) || defined(leaf_compiler_gnu)
# define leaf_likely(expr) __builtin_expect(!!(expr), 1)
# define leaf_unlikely(expr) __builtin_expect(!!(expr), 0)
#else
# define leaf_likely(expr) (!!(expr))
# define leaf_unlikely(expr) (!!(expr))
#endif

#if defined(leaf_compiler_msvc)
# define leaf_assume(expr) __assume(expr)
#elif defined(leaf_compiler_gnu)
# define leaf_assume(expr) ((expr)) ? static_cast<void>(0) : __builtin_unreachable()
#else
# define leaf_assume(expr) static_cast<void>((cond) ? 0 : 0)
#endif

namespace leaf::details
{
  [[noreturn]] inline auto terminate() noexcept -> void { std::terminate(); }
}

#define leaf_contract_check(type, expr) (leaf_likely(expr) ? static_cast<void>(0) : leaf::details::terminate())
#define assert_expects(expr) leaf_contract_check("Precondition", expr)
#define assert_ensures(expr) leaf_contract_check("Postcondition", expr)
#define assert_condition(expr, message) assert(expr and message)

#if defined(leaf_compiler_clang)
# pragma clang diagnostic pop
#endif