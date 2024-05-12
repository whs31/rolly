#pragma once

#include <lf/detail/os.h>

namespace lf
{
  /**
   * \brief Invokes undefined behavior.
   * \details https://en.cppreference.com/w/cpp/utility/unreachable
   */
  [[noreturn]] inline auto unreachable() -> void
  {
    #if defined(LF_COMPILER_MSVC) && !defined(LF_COMPILER_CLANG)
      __assume(false);
    #else
      __builtin_unreachable();
    #endif
  }
}