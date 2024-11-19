#pragma once

#include <type_traits>
#include "../global/definitions.h"

#if defined(___rolly_cxx20___)
#  include <concepts>
#endif  // defined(___rolly_cxx20___)

namespace rolly::concepts {
#if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  /**
   * @brief Concept that is true if T is a standard copyable and movable type.
   * @note Available only in C++20 mode.
   * @tparam T Type to check.
   */
  template <typename T>
  concept standard_copyable_and_movable = std::is_nothrow_move_constructible_v<T>
                                      and std::is_nothrow_move_assignable_v<T> and std::copyable<T>;
#endif  // defined(___rolly_cxx20___)
}  // namespace rolly::concepts
