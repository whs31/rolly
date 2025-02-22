#pragma once

#include <rll/global/definitions.h>
#include <rll/type_traits.h>

#if __cplusplus >= 202'002L
#  include <concepts>
#endif

namespace rll {
#if _cplusplus >= 202'002L || defined(RLL_DOC)
  template <typename T, typename... U>
  concept any_of = (std::same_as<T, U> or ...);
#endif
}  // namespace rll

namespace rll {
  template <typename T, typename... U>
  struct is_any_of : std::disjunction<std::is_same<T, U>...> {};

  template <typename T, typename... U>
  inline constexpr bool is_any_of_v = is_any_of<T, U...>::value;
}  // namespace rll
