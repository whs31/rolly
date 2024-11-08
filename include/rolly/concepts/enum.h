#pragma once

#include "../global/definitions.h"
#include "../type_traits.h"

namespace rolly::concepts {
#if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  /**
   * @brief Concept that is true if T is an enum type.
   * @details Will yield <code>true</code> for any type that is a scoped or unscoped enum.
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   */
  template <typename T>
  concept enum_ = std::is_enum_v<plain_type_t<T>>;
#endif  // defined(___rolly_cxx20___)

  using std::is_enum;
  using std::is_enum_v;
}  // namespace rolly::concepts
