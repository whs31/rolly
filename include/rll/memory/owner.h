#pragma once

#include "../concepts/ptr.h"

namespace rll {
  /**
   * @brief GSL-like owning raw pointer typedef.
   * @details <code>rll::owner<T></code> is designed as a safety mechanism for code that must deal
   * directly with raw pointers that own memory.
   *
   * Ideally such code should be restricted to the implementation of low-level abstractions.
   * <code>rll::owner<T></code> can also be used as a stepping point in converting legacy code
   * to use more modern RAII constructs, such as smart pointers.
   * @tparam T Pointer type.
   * @sa https://github.com/microsoft/GSL/blob/main/include/gsl/pointers
   */
  template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
  using owner = T;
}  // namespace rll
