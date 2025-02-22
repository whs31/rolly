#pragma once

#include <charconv>
#include <string>
#include <string_view>

namespace rll {
  /**
   * @brief Function prototype for converting C++ types to owned strings.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param value Constant reference to the type to convert
   * @return Owned string representation of the type
   */
  template <typename T, typename C = char>
  [[nodiscard]] inline std::basic_string<C> to_string(T const& value) = delete;

  /**
   * @brief Function prototype for converting C++ types to non-owned strings.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param value Constant reference to the type to convert
   * @return Non-owned string representation of the type
   */
  template <typename T, typename C = char>
  [[nodiscard]] constexpr inline std::basic_string_view<C> as_str(T const& type) = delete;

  /**
   * @brief Function prototype for converting strings to C++ types.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param str Non-owned string representation of the type
   * @return Value of the converted type
   */
  template <typename T, typename C = char>
  [[nodiscard]] constexpr inline T from_str(std::basic_string_view<C> const& str) = delete;
}  // namespace rll

// todo: i don't like this anymore...
#define DECLARE_CONVERTIBLE_TO_STRING_VIEW(T) template <> [[nodiscard]] constexpr inline std::basic_string_view<char> rolly::as_str(T const& type)
#define DECLARE_CONVERTIBLE_TO_STRING(T) template <> [[nodiscard]] inline std::basic_string<char> rolly::to_string(T const& value)
#define DECLARE_CONVERTIBLE_TO_STRING_DEFAULT(T) template <> [[nodiscard]] inline std::basic_string<char> rolly::to_string(T const& value) { return std::basic_string<char>(rolly::as_str(value)); }
#define DECLARE_CONVERTIBLE_FROM_STRING(T) template <> [[nodiscard]] constexpr inline T rolly::from_str(std::basic_string_view<char> const& str)
