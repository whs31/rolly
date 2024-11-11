#pragma once

#include "../global/definitions.h"
#include "../type_traits.h"
#if defined(___rolly_cxx20___)
#  include <concepts>
#endif  // defined(___rolly_cxx20___)

namespace rolly::concepts {
#if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  template <typename T, typename... U>
  concept any_of = (std::same_as<T, U> or ...);
#endif  // defined(___rolly_cxx20___) || defined(ROLLY_DOC)
}  // namespace rolly::concepts

namespace rolly {
  template <typename T, typename... U>
  struct is_any_of : std::disjunction<std::is_same<T, U>...> {};

  template <typename T, typename... U>
  inline constexpr bool is_any_of_v = is_any_of<T, U...>::value;
}  // namespace rolly
