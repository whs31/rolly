#pragma once

#include <cassert>
#include <exception>
#include <lf/detail/platform.h>

#if defined(LF_COMPILER_CLANG)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Winvalid-noreturn"
#endif

#if defined(LF_COMPILER_CLANG)
# define leaf_suppress(x) [[gsl::suppress(#x)]]
#else
# if defined(LF_COMPILER_MSVC) && !defined(LF_COMPILER_INTEL) && !defined(LF_COMPILER_NVIDIA)
#   define leaf_suppress(x) [[gsl::suppress(x)]]
# else
#   define leaf_suppress(x)
# endif
#endif

#if defined(LF_COMPILER_CLANG) || defined(LF_COMPILER_GNU)
# define leaf_likely(expr) __builtin_expect(!!(expr), 1)
# define leaf_unlikely(expr) __builtin_expect(!!(expr), 0)
#else
# define leaf_likely(expr) (!!(expr))
# define leaf_unlikely(expr) (!!(expr))
#endif

#if defined(LF_COMPILER_MSVC)
# define leaf_assume(expr) __assume(expr)
#elif defined(LF_COMPILER_GNU)
# define leaf_assume(expr) ((expr)) ? static_cast<void>(0) : __builtin_unreachable()
#else
# define leaf_assume(expr) static_cast<void>((cond) ? 0 : 0)
#endif

#define lf_contract_check(type, expr) (leaf_likely(expr) ? static_cast<void>(0) : std::terminate())
#define assert_expects(expr) lf_contract_check("Precondition", expr)
#define assert_ensures(expr) lf_contract_check("Postcondition", expr)
#define assert_condition(expr, message) assert(expr and message)

#if defined(LF_COMPILER_CLANG)
# pragma clang diagnostic pop
#endif