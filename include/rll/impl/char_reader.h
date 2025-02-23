#pragma once

#include <stdexcept>
#include <rll/global/definitions.h>
#include <rll/stdint.h>

namespace rll::detail {
  template <typename T>
  struct char_reader;

  template <typename T>
  struct char_or_throw {
    [[nodiscard]] static constexpr ___inline___ char
      next(T const*& it, T const* begin, T const* end) {
      u32 error_symbol = 0;
      auto error = false;
      auto const result = char_reader<T>::next_or_error(it, end, error, error_symbol);
      if(error)
        throw std::domain_error("unexpected symbol");
      return result;
    }

    static constexpr ___inline___ void has_throw() noexcept;
  };

  struct symbol {
    u32 value;
    u32 length;
  };

  template <typename T>
  struct utf8_reader {
    [[nodiscard]] static constexpr ___inline___ char
      utf8_next_or_error(T const*& it, T const* end, bool& error, u32& error_symbol) {
      error = false;
      error_symbol = 0;
      auto correct = true;
      auto symbol = utf8_code_point(static_cast<u8>(*it), correct);
      if(correct) {
        switch(symbol.length) {
          case 1: break;

          case 2:
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            break;

          case 3:
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            break;

          case 4:
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            symbol.value = (symbol.value << 6) | trailing_utf8_code_point(it, end, correct);
            break;

          default: correct = false; break;
        }
      }
      ++it;
      if(! correct) {
        error = true;
        return '\0';
      } else if(symbol.value > 127) {
        error = true;
        error_symbol = symbol.value;
        return '\0';
      }
      return char(symbol.value);
    }

    [[nodiscard]] static constexpr ___inline___ symbol
      utf8_code_point(uint8_t byte, bool& correct) noexcept {
      if((byte & 0b10000000) == 0b00000000)
        return {static_cast<u32>(byte), 1};
      if((byte & 0b11100000) == 0b11000000)
        return {static_cast<u32>(byte & 0b00011111), 2};
      if((byte & 0b11110000) == 0b11100000)
        return {static_cast<u32>(byte & 0b00001111), 3};
      if((byte & 0b11111000) == 0b11110000)
        return {static_cast<u32>(byte & 0b00000111), 4};
      correct = false;
      return {0, 0};
    }

    [[nodiscard]] static constexpr ___inline___ u32
      trailing_utf8_code_point(T const*& it, T const* end, bool& correct) noexcept {
      if(++it >= end) {
        correct = false;
        return 0;
      }
      if((uint8_t(*it) & 0b11000000) == 0b10000000) {
        correct = true;
        return uint8_t(*it) & 0b00111111;
      }
      correct = false;
      return 0;
    }
  };

  template <typename T>
  struct utf16_reader {
    [[nodiscard]] static constexpr ___inline___ char utf16_next_or_error(
      T const*& it,
      T const* end,
      bool& error,
      uint32_t& error_symbol
    ) noexcept {
      error = false;
      error_symbol = 0;
      bool correct = true;
      auto symbol = utf16_code_point(static_cast<u16>(*it));
      switch(symbol.length) {
        case 1: symbol.value = static_cast<u32>(*it); break;

        case 2:
          if(++it >= end) {
            correct = false;
            break;
          }
          if((*it & 0b11111100'00000000) == 0b11011100'00000000) {
            symbol.value = ((symbol.value << 10) | (static_cast<u16>(*it) & 0b00000011'11111111))
                         + 0x10000;
          } else {
            correct = false;
          }
          break;

        default: correct = false; break;
      }
      ++it;
      if(! correct) {
        error = true;
        return '\0';
      } else if(symbol.value > 127) {
        error = true;
        error_symbol = symbol.value;
        return '\0';
      }
      return static_cast<char>(symbol.value);
    }

    [[nodiscard]] static constexpr ___inline___ symbol utf16_code_point(uint16_t value) noexcept {
      if((value & 0b11111100'00000000) == 0b11011000'00000000)
        return {static_cast<uint32_t>(value & 0b00000011'11111111), 2};
      else
        return {value, 1};
    }
  };

  template <typename T>
  struct utf32_reader {
    [[nodiscard]] static constexpr ___inline___ char utf32_next_or_error(
      T const*& it,
      T const* /*end*/,
      bool& error,
      uint32_t& error_symbol
    ) noexcept {
      error = false;
      error_symbol = 0;
      if(static_cast<uint32_t>(*it) > 127) {
        error = true;
        error_symbol = uint32_t(*it++);  // NOLINT
        return '\0';
      }
      return char(*it++);
    }
  };

  template <>
  struct char_reader<char> : utf8_reader<char>,
                             char_or_throw<char>

  {
    [[nodiscard]] static constexpr ___inline___ char
      next_or_error(char const*& it, char const* end, bool& error, u32& error_symbol) noexcept {
      return utf8_next_or_error(it, end, error, error_symbol);
    }
  };

  template <>
  struct char_reader<wchar_t> : utf16_reader<wchar_t>,
                                utf32_reader<wchar_t>,
                                char_or_throw<wchar_t> {
    [[nodiscard]] static constexpr ___inline___ char next_or_error(
      wchar_t const*& it,
      wchar_t const* end,
      bool& error,
      uint32_t& error_symbol
    ) noexcept {
      error = false;
      error_symbol = 0;
#ifdef _WIN32
      if constexpr(sizeof(wchar_t) == sizeof(char16_t))
        return utf16_next_or_error(it, end, error, error_symbol);
#else
      if constexpr(sizeof(wchar_t) == sizeof(char32_t))
        return utf32_next_or_error(it, end, error, error_symbol);
#endif
      if(*it > 127) {
        error = true;
        error_symbol = uint32_t(*it++);  // NOLINT
        return '\0';
      }
      return static_cast<char>(*it++);
    }
  };

#if __cpp_char8_t >= 201'811L
  template <>
  struct char_reader<char8_t> : utf8_reader<char8_t>,
                                char_or_throw<char8_t> {
    [[nodiscard]] static constexpr ___inline___ char next_or_error(
      char8_t const*& it,
      char8_t const* end,
      bool& error,
      uint32_t& error_symbol
    ) noexcept {
      return utf8_next_or_error(it, end, error, error_symbol);
    }
  };
#endif  // __cpp_char8_t

  template <>
  struct char_reader<char16_t> : utf16_reader<char16_t>,
                                 char_or_throw<char16_t> {
    [[nodiscard]] static constexpr ___inline___ char next_or_error(
      char16_t const*& it,
      char16_t const* end,
      bool& error,
      u32& error_symbol
    ) noexcept {
      return utf16_next_or_error(it, end, error, error_symbol);
    }
  };

  template <>
  struct char_reader<char32_t> : utf32_reader<char32_t>,
                                 char_or_throw<char32_t> {
    [[nodiscard]] static constexpr ___inline___ char next_or_error(
      char32_t const*& it,
      char32_t const* end,
      bool& error,
      u32& error_symbol
    ) noexcept {
      return utf32_next_or_error(it, end, error, error_symbol);
    }
  };

  template <typename T>
  [[nodiscard]] constexpr ___inline___ char next_char(
    T const*& it,
    T const* begin,
    T const* end
  ) noexcept(noexcept(detail::char_reader<T>::next(it, begin, end))) {
    return detail::char_reader<T>::next(it, begin, end);
  }

  template <typename T>
  [[nodiscard]] constexpr ___inline___ char
    next_char_or_error(T const*& it, T const* end, bool& error, u32& error_symbol) noexcept {
    return detail::char_reader<T>::next_or_error(it, end, error, error_symbol);
  }
}  // namespace rll::detail
