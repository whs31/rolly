#pragma once

#include <type_traits>
#include <rll/global/definitions.h>

#if __cplusplus >= 202'002L
#  include <concepts>
#endif

namespace rll {
#if __cplusplus >= 202'002L || defined(RLL_DOC)
  /**
   * @brief Concept that is true if T is a standard copyable and movable type.
   * @note Available only in C++20 mode.
   * @tparam T Type to check.
   */
  template <typename T>
  concept standard_copyable_and_movable = std::is_nothrow_move_constructible_v<T>
                                      and std::is_nothrow_move_assignable_v<T> and std::copyable<T>;
#endif
}  // namespace rll
