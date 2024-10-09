#pragma once

#include <type_traits>
#include <utility>
#include "concepts/enum.h"

namespace rolly
{
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
   *     default: rolly::unreachable();
   *   }
   * }
   * @endcode
   * @warning Be sure to use this with care, as it can lead to undefined behavior.
   * @sa https://en.cppreference.com/w/cpp/utility/unreachable
   */
  [[noreturn]] inline void unreachable() {
    #if defined(_MSC_VER) && !defined(__clang__)
    __assume(false);
    #else // defined(_MSC_VER) && !defined(__clang__)
    __builtin_unreachable();
    #endif // defined(_MSC_VER) && !defined(__clang__)
  }

  /**
   * @brief Converts an enumeration value to its underlying type.
   * @details This is a convenience function for converting an enumeration value to its underlying type.
   * It is a generic version of the C++23 std::to_underlying function.
   * @tparam T The enumeration type. Must be an enumeration type.
   * @param t The enumeration value to convert.
   * @return The underlying value of the enumeration.
   * @see https://en.cppreference.com/w/cpp/utility/to_underlying
   */
  template <___concept___(concepts::enum_) T ___sfinae_requirement___(std::is_enum_v<T>)>
  constexpr auto to_underlying(T t) noexcept {
    return static_cast<std::underlying_type_t<T>>(t);
  }

  /**
   * @brief Generic bitwise <b>or</b> operator for enum types.
   * @details Requires opt-ins <code>enable_bitwise_or</code> or <code>enum_flag</code> to be defined
   * for the enum type.
   *
   * Example:
   * @code {.cpp}
   * enum class Example {
   *   A = 1,
   *   B = 2,
   *   C = 4
   * };
   *
   * // opt-in bitwise or
   * void consteval enable_bitwise_or(Example);
   *
   * // alternatively, opt-in all operators
   * void consteval enum_flag(Example);
   *
   * auto value = Example::A | Example::B;
   * @endcode
   * @tparam T Enum type
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise or operation
   */
  template <typename T>
#ifdef ___rolly_cxx20___
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_or(e);
  } or requires(T e) {
    enum_flag(e);
  })
#endif // ___rolly_cxx20___
  constexpr T operator|(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) | to_underlying(rhs));
  }

  /**
   * @brief Generic bitwise <b>and</b> operator for enum types.
   * @details Requires opt-ins <code>enable_bitwise_and</code> or <code>enum_flag</code> to be defined
   * for the enum type.
   *
   * Example:
   * @code {.cpp}
   * enum class Example {
   *   A = 1,
   *   B = 2,
   *   C = 4
   * };
   *
   * // opt-in bitwise and
   * void consteval enable_bitwise_and(Example);
   *
   * // alternatively, opt-in all operators
   * void consteval enum_flag(Example);
   *
   * auto value = Example::A & Example::B;
   * @endcode
   * @tparam T Enum type
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise and operation
   */
  template <typename T>
#ifdef ___rolly_cxx20___
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_and(e);
  } or requires(T e) {
    enum_flag(e);
  })
#endif // ___rolly_cxx20___
  constexpr T operator&(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) & to_underlying(rhs));
  }

  /**
   * @brief Generic bitwise <b>xor</b> operator for enum types.
   * @details Requires opt-ins <code>enable_bitwise_xor</code> or <code>enum_flag</code> to be defined
   * for the enum type.
   *
   * Example:
   * @code {.cpp}
   * enum class Example {
   *   A = 1,
   *   B = 2,
   *   C = 4
   * };
   *
   * // opt-in bitwise or
   * void consteval enable_bitwise_xor(Example);
   *
   * // alternatively, opt-in all operators
   * void consteval enum_flag(Example);
   *
   * auto value = Example::A ^ Example::B;
   * @endcode
   * @tparam T Enum type
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise xor operation
   */
  template <typename T>
#ifdef ___rolly_cxx20___
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_xor(e);
  } or requires(T e) {
    enum_flag(e);
  })
#endif // ___rolly_cxx20___
  constexpr T operator^(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) ^ to_underlying(rhs));
  }

  /**
   * @brief Generic bitwise <b>not</b> operator for enum types.
   * @details Requires opt-ins <code>enable_bitwise_not</code> or <code>enum_flag</code> to be defined
   * for the enum type.
   *
   * Example:
   * @code {.cpp}
   * enum class Example {
   *   A = 1,
   *   B = 2,
   *   C = 4
   * };
   *
   * // opt-in bitwise not
   * void consteval enable_bitwise_not(Example);
   *
   * // alternatively, opt-in all operators
   * void consteval enum_flag(Example);
   *
   * auto value = ~Example::A;
   * @endcode
   * @tparam T Enum type
   * @param lhs Left operand
   * @param rhs Right operand
   * @return Result of bitwise not operation
   */
  template <typename T>
#ifdef ___rolly_cxx20___
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_not(e);
  } or requires(T e) {
    enum_flag(e);
  })
#endif // ___rolly_cxx20___
  constexpr T operator~(T const value) noexcept {
    return static_cast<T>(~to_underlying(value));
  }
} // namespace rolly