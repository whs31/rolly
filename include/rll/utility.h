#pragma once

#include <type_traits>
#include <utility>
#include <rll/concepts/enum.h>

namespace rll {
  /**
   * @brief Informs the compiler that the current location is unreachable.
   * @details If the compiler can prove that the current location is unreachable, it is free to
   * optimize the program as if the current location did not exist. This is a very powerful
   * optimization hint.
   *
   * This function is very useful when you write code that should never be executed (for example,
   * in a switch-case statement where you have already handled all possible values), as it
   * allows the compiler to optimize away the unreachable code.
   *
   * Example:
   * @code {.cpp}
   * enum class Color { Red, Green, Blue };
   *
   * auto to_string(Color c) -> std::string {
   *   switch (c) {
   *     case Color::Red: return "red";
   *     case Color::Green: return "green";
   *     case Color::Blue: return "blue";
   *     default: rll::unreachable();
   *   }
   * }
   * @endcode
   * @warning Be sure to use this with care, as it can lead to undefined behavior.
   * @sa https://en.cppreference.com/w/cpp/utility/unreachable
   */
  [[noreturn]] inline void unreachable() {
#if defined(_MSC_VER) && ! defined(__clang__)
    __assume(false);
#else   // defined(_MSC_VER) && !defined(__clang__)
    __builtin_unreachable();
#endif  // defined(_MSC_VER) && !defined(__clang__)
  }

  /**
   * @brief Converts an enumeration value to its underlying type.
   * @details This is a convenience function for converting an enumeration value to its underlying
   * type. It is a generic version of the C++23 std::to_underlying function.
   * @tparam T The enumeration type. Must be an enumeration type.
   * @param t The enumeration value to convert.
   * @return The underlying value of the enumeration.
   * @see https://en.cppreference.com/w/cpp/utility/to_underlying
   */
  template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
  constexpr auto to_underlying(T t) noexcept {
    return static_cast<std::underlying_type_t<T>>(t);
  }
}  // namespace rll

#define DECLARE_ENUM_FLAG(E)                                                              \
  [[nodiscard]] constexpr E operator|(E const lhs, E const rhs) noexcept {                \
    return static_cast<E>(rll::to_underlying(lhs) | rll::to_underlying(rhs));             \
  }                                                                                       \
  [[nodiscard]] constexpr E operator&(E const lhs, E const rhs) noexcept {                \
    return static_cast<E>(rll::to_underlying(lhs) & rll::to_underlying(rhs));             \
  }                                                                                       \
  [[nodiscard]] constexpr E operator^(E const lhs, E const rhs) noexcept {                \
    return static_cast<E>(rll::to_underlying(lhs) ^ rll::to_underlying(rhs));             \
  }                                                                                       \
  [[nodiscard]] constexpr E operator~(E const value) noexcept {                           \
    return static_cast<E>(~rll::to_underlying(value));                                    \
  }                                                                                       \
  [[nodiscard]] constexpr E& operator|=(E& lhs, E const rhs) noexcept {                   \
    lhs = lhs | rhs;                                                                      \
    return lhs;                                                                           \
  }                                                                                       \
  [[nodiscard]] constexpr E& operator&=(E& lhs, E const rhs) noexcept {                   \
    lhs = lhs & rhs;                                                                      \
    return lhs;                                                                           \
  }                                                                                       \
  [[nodiscard]] constexpr E& operator^=(E& lhs, E const rhs) noexcept {                   \
    lhs = lhs ^ rhs;                                                                      \
    return lhs;                                                                           \
  }
