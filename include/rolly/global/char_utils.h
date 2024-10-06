#pragma once

#include <string>
#include <string_view>
#include <fmt/format.h>

namespace rolly
{
  [[nodiscard]] constexpr char to_lower(char c) noexcept {
    return (c >= 'A' and c <= 'Z')
      ? static_cast<char>(c + ('a' - 'A'))
      : c;
  }

  [[nodiscard]] constexpr bool is_digit(char c) noexcept {
    return c >= '0' and c <= '9';
  }

  [[nodiscard]] constexpr bool is_space(char c) noexcept {
    return c == ' ';
  }

  [[nodiscard]] constexpr bool is_operator(char c) noexcept {
    return c == '<' or c == '>' or c == '=';
  }

  [[nodiscard]] constexpr bool is_dot(char c) noexcept {
    return c == '.';
  }

  [[nodiscard]] constexpr bool is_logical_or(char c) noexcept {
    return c == '|';
  }

  [[nodiscard]] constexpr bool is_hyphen(char c) noexcept {
    return c == '-';
  }

  [[nodiscard]] constexpr bool is_letter(char c) noexcept {
    return (c >= 'A' and c <= 'Z') or (c >= 'a' and c <= 'z');
  }

  [[nodiscard]] constexpr std::uint16_t to_digit(char c) noexcept {
    return static_cast<std::uint16_t>(c - '0');
  }
} // namespace rolly