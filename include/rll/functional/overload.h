#pragma once

namespace rll {
  template <typename... Args>
  struct non_const_overload_t {
    template <typename R, typename T>
    constexpr auto operator()(R (T::*ptr)(Args...)) const noexcept -> decltype(ptr) {
      return ptr;
    }

    template <typename R, typename T>
    static constexpr auto of(R (T::*ptr)(Args...)) noexcept -> decltype(ptr) {
      return ptr;
    }
  };

  template <typename... Args>
  struct const_overload_t {
    template <typename R, typename T>
    constexpr auto operator()(R (T::*ptr)(Args...) const) const noexcept -> decltype(ptr) {
      return ptr;
    }

    template <typename R, typename T>
    static constexpr auto of(R (T::*ptr)(Args...) const) noexcept -> decltype(ptr) {
      return ptr;
    }
  };

  template <typename... Args>
  struct overload_t : const_overload_t<Args...>,
                      non_const_overload_t<Args...> {
    using const_overload_t<Args...>::of;
    using const_overload_t<Args...>::operator();
    using non_const_overload_t<Args...>::of;
    using non_const_overload_t<Args...>::operator();

    template <typename R>
    constexpr auto operator()(R (*ptr)(Args...)) const noexcept -> decltype(ptr) {
      return ptr;
    }

    template <typename R>
    static constexpr auto of(R (*ptr)(Args...)) noexcept -> decltype(ptr) {
      return ptr;
    }
  };

  template <typename... Args>
  constexpr inline overload_t<Args...> overload = {};

  template <typename... Args>
  constexpr inline const_overload_t<Args...> const_overload = {};

  template <typename... Args>
  constexpr inline non_const_overload_t<Args...> non_const_overload = {};
}  // namespace rll
