#pragma once

#include <exception>

namespace leaf
{
  template <typename T, typename U>
  constexpr auto narrow_cast(U&& u) noexcept -> T
  {
    return static_cast<T>(std::forward<U>(u));
  }

  struct narrowing_error : public std::exception
  {
    [[nodiscard]] auto what() const noexcept -> const char* override
    {
      return "leaf::narrowing_error (loss of precision or data occurred)";
    }
  };

  template <typename T, typename U, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr auto narrow_strict_cast(U u) noexcept(false) -> T
  {
    auto constexpr is_different_signedness = std::is_signed_v<T> != std::is_signed_v<U>;
    auto const t = narrow_cast<T>(u);

    #if defined(__clang__) || defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
    #endif
    if(static_cast<U>(t) != u or (is_different_signedness and ((t < T{}) != (u < U{}))))
      throw narrowing_error{};
    #if defined(__clang__) || defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    return t;
  }

  template <typename T, typename U, typename std::enable_if<not std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr auto narrow_strict_cast(U u) noexcept(false) -> T
  {
    auto const t = narrow_cast<T>(u);
    if(static_cast<U>(t) != u)
      throw narrowing_error{};
    return t;
  }
}