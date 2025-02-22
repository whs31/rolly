#pragma once

#include <type_traits>
#include <utility>
#if defined(_MSC_VER)
#  include <ciso646>
#endif

namespace rll {
#ifndef DOXYGEN
  namespace detail {
    template <typename T, typename = void>
    struct is_noarg_callable_t : std::false_type {};

    template <typename T>
    struct is_noarg_callable_t<T, decltype(std::declval<T&&>()())> : std::true_type {};

    template <typename T>
    struct returns_void_t : std::is_same<void, decltype(std::declval<T&&>()())> {};

    template <typename A, typename B, typename... C>
    struct and_t : and_t<A, and_t<B, C...>> {};

    template <typename A, typename B>
    struct and_t<A, B> : std::conditional_t<A::value, B, A> {};

    template <typename T>
    struct is_proper_sg_callback_t : and_t<
                                       is_noarg_callable_t<T>,
                                       returns_void_t<T>,
                                       std::is_nothrow_invocable<T>,
                                       std::is_nothrow_destructible<T>> {};

    template <
      typename Callback,
      typename = std::enable_if_t<is_proper_sg_callback_t<Callback>::value>>
    class scope_guard;

    template <typename Callback>
    detail::scope_guard<Callback> make_scope_guard(
      Callback&& callback
    ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>);

    template <typename Callback>
    detail::scope_guard<Callback> finally(
      Callback&& callback
    ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>);

    template <typename Callback>
    class [[nodiscard]] scope_guard<Callback> final {
     public:
      using callback_type = Callback;

      scope_guard(
        scope_guard&& other
      ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>)
        : callback_(std::forward<Callback>(other.callback_))
        , active_(std::move(other.active_)) {
        other.active_ = false;
      }

      ~scope_guard() noexcept {
        if(this->active_)
          this->callback_();
      }

      void dismiss() noexcept { this->active_ = false; }

      scope_guard() = delete;
      scope_guard(scope_guard const&) = delete;
      scope_guard& operator=(scope_guard const&) = delete;
      scope_guard& operator=(scope_guard&&) = delete;

     private:
      explicit scope_guard(
        Callback&& callback
      ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>)
        : callback_(std::forward<Callback>(callback))
        , active_(true) {}

      friend scope_guard<Callback> make_scope_guard<
        Callback>(Callback&&) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>);

      friend scope_guard<Callback> finally<
        Callback>(Callback&&) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>);

      Callback callback_;
      bool active_;
    };

    template <typename Callback>
    inline scope_guard<Callback> make_scope_guard(
      Callback&& callback
    ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>) {
      return detail::scope_guard<Callback> {std::forward<Callback>(callback)};
    }

    template <typename Callback>
    inline scope_guard<Callback> finally(
      Callback&& callback
    ) noexcept(std::is_nothrow_constructible_v<Callback, Callback&&>) {
      return detail::scope_guard<Callback> {std::forward<Callback>(callback)};
    }
  }  // namespace detail
#endif

  template <typename Callback>
  [[nodiscard]] inline detail::scope_guard<Callback> make_scope_guard(Callback&& callback) noexcept(
    noexcept(detail::make_scope_guard<Callback>(std::forward<Callback>(callback)))
  ) {
    return detail::make_scope_guard<Callback>(std::forward<Callback>(callback));
  }

  template <typename Callback>
  [[nodiscard]] inline detail::scope_guard<Callback> finally(Callback&& callback) noexcept(
    noexcept(detail::finally<Callback>(std::forward<Callback>(callback)))
  ) {
    return detail::finally<Callback>(std::forward<Callback>(callback));
  }
}  // namespace rll
