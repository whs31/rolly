#pragma once

#if __cplusplus < 201703L
#error "wtf???? ranges is not available!"
#endif

#include <ranges>

namespace leaf::utils::ranges {
  namespace detail {
    template <typename C> struct to_helper
    {};

    template <typename C, std::ranges::range R>
      requires std::convertible_to<std::ranges::range_value_t<R>, typename C::value_type>
    auto operator|(R&& r, to_helper<C>) -> C
    {
      return C {r.begin(), r.end()};
    }
  } // namespace detail

  template <std::ranges::range C>
    requires(not std::ranges::view<C>)
  auto collect()
  {
    return detail::to_helper<C> {};
  }
} // namespace leaf::utils::ranges