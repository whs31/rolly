#pragma once

#include <cassert>
#include <type_traits>
#include <sstream>
#include <iostream>
#include <fmt/ostream.h>
#include <stdexcept>
#include "../bit.h"
#include "../global/definitions.h"
#include "./stdint.h"
#include "./optional.h"

namespace rolly {
  namespace detail {

    template <typename T>
    struct char_reader;

    template <typename T>
    struct char_or_throw {
      [[nodiscard]] static constexpr ___inline___ char
        next(T const*& it, T const* begin, T const* end) noexcept {
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
        auto symbol = utf8_code_point(uint8_t(*it), correct);
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

    template <>
    struct char_reader<char> : utf8_reader<char>,
                               char_or_throw<char>

    {
      [[nodiscard]] static constexpr ___inline___ char
        next_or_error(char const*& it, char const* end, bool& error, u32& error_symbol) noexcept {
        return utf8_next_or_error(it, end, error, error_symbol);
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
  }  // namespace detail

  /**
   * @brief 128-bit unsigned integer.
   * @details u128 offers a comprehensive suite of operations and utilities for handling
   * 128-bit unsigned integers. It supports basic arithmetic operations, bitwise logic,
   * comparison operators, and specialized functions for advanced integer manipulation.
   * The class is optimized for both space and time efficiency, ensuring minimal overhead
   * and maximum performance.
   *
   * The design of u128 is focused on ease of use and integration. It provides
   * constructors for seamless conversion from built-in integer types, as well as explicit
   * methods for converting to and from other numeric representations. The class also
   * includes support for standard library features like hashing and optional values,
   * enhancing its utility in a wide range of programming scenarios.
   *
   * The implementation is based on algorithms from the .NET `UInt128`
   * structure, ensuring reliable and efficient operations.
   *
   * @see
   * [UInt128](https://source.dot.net/#System.Private.CoreLib/src/libraries/System.Private.CoreLib/src/System/UInt128.cs)
   * .NET Implementation.
   */
  class u128 final {  // NOLINT(cppcoreguidelines-special-member-functions)
   public:
    /**
     * @brief String formats available for u128.
     * @details This enumeration defines the possible string representations of u128 values.
     * It allows users to specify the desired format when converting u128 instances to
     * strings.
     */
    enum class format {
      decimal = 0,  ///< Represents the number in decimal format. *This is the default format.*
      octal,        ///< Represents the number in octal format.
      hexadecimal   ///< Represents the number in hexadecimal format.
    };

    /**
     * @brief Default constructor.
     * @details Constructs a new u128 instance with default values.
     */
    constexpr ___inline___ u128() noexcept = default;

    /**
     * @brief Copy constructor.
     * @details Constructs a new u128 instance by copying the value from another instance.
     * @param[in] other The u128 instance to copy.
     */
    constexpr ___inline___ u128(u128 const& other) noexcept = default;

    /**
     * @brief Move constructor.
     * @details Constructs a new u128 instance by moving the value from another instance.
     * @param[in,out] other The u128 instance to move.
     */
    constexpr ___inline___ u128(u128&& other) noexcept = default;

    /**
     * @brief Assignment operator.
     * @details Assigns the value of one u128 instance to another.
     * @param[in] other The u128 instance to assign from.
     * @return A reference to the assigned u128 instance.
     */
    constexpr ___inline___ u128& operator=(u128 const& other) noexcept = default;

    /**
     * @brief Move assignment operator.
     * @details Moves the value of one u128 instance to another.
     * @param[in,out] other The u128 instance to move from.
     * @return A reference to the moved u128 instance.
     */
    constexpr ___inline___ u128& operator=(u128&& other) noexcept = default;

    /**
     * @brief Constructs a u128 instance from upper and lower parts.
     * @details This constructor initializes a u128 instance using separate upper and lower 64-bit
     * integers.
     * @param[in] upper The upper 64 bits of the u128 value.
     * @param[in] lower The lower 64 bits of the u128 value.
     */
    constexpr ___inline___ u128(u64 upper, u64 lower) noexcept
#if ROLLY_ENDIAN == ROLLY_BIG_ENDIAN
      : upper_(upper)
      , lower_(lower)
#else
      : lower_(lower)
      , upper_(upper)
#endif
    {
    }

    /**
     * @brief Constructs an u128 instance from an unsigned integer.
     * @details This constructor initializes a u128 instance using an unsigned integer,
     * setting it as the lower part of the u128 value.
     * @param[in] lower The unsigned integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(ulonglong lower) noexcept
      : lower_(static_cast<u64>(lower)) {}

    /**
     * @brief Constructs a u128 instance from an unsigned integer.
     * @details This constructor initializes a u128 instance using an unsigned integer,
     * setting it as the lower part of the u128 value.
     * @param[in] lower The unsigned integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(unsigned long lower) noexcept
      : u128(static_cast<ulonglong>(lower)) {}

    /**
     * @brief Constructs a u128 instance from an unsigned integer.
     * @details This constructor initializes a u128 instance using an unsigned integer,
     * setting it as the lower part of the u128 value.
     * @param[in] lower The unsigned integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(unsigned int lower) noexcept
      : u128(static_cast<ulonglong>(lower)) {}

    /**
     * @brief Constructs a u128 instance from an unsigned integer.
     * @details This constructor initializes a u128 instance using an unsigned integer,
     * setting it as the lower part of the u128 value.
     * @param[in] lower The unsigned integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(unsigned short lower) noexcept
      : u128(static_cast<ulonglong>(lower)) {}

    /**
     * @brief Constructs a u128 instance from a signed integer.
     * @details This constructor initializes a u128 instance using a signed integer,
     * setting it as the lower part of the u128 value and extending the sign to the upper
     * part.
     * @param[in] lower The signed integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(long long lower) noexcept
#if ROLLY_ENDIAN == ROLLY_BIG_ENDIAN
      : upper_(static_cast<u64>(static_cast<i64>(lower) >> 63))
      , lower_(static_cast<u64>(lower))
#else
      : upper_(static_cast<u64>(lower))
      , lower_(static_cast<u64>(static_cast<i64>(lower) >> 63))
#endif
    {
    }

    /**
     * @brief Constructs a u128 instance from a signed integer.
     * @details This constructor initializes a u128 instance using a signed integer,
     * setting it as the lower part of the u128 value and extending the sign to the upper
     * part.
     * @param[in] lower The signed integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(long lower) noexcept
      : u128(static_cast<long long>(lower)) {}

    /**
     * @brief Constructs a u128 instance from a signed integer.
     * @details This constructor initializes a u128 instance using a signed integer,
     * setting it as the lower part of the u128 value and extending the sign to the upper
     * part.
     * @param[in] lower The signed integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(int lower) noexcept
      : u128(static_cast<long long>(lower)) {}

    /**
     * @brief Constructs a u128 instance from a signed integer.
     * @details This constructor initializes a u128 instance using a signed integer,
     * setting it as the lower part of the u128 value and extending the sign to the upper
     * part.
     * @param[in] lower The signed integer to initialize the u128 instance with.
     */
    constexpr ___inline___ u128(short lower) noexcept
      : u128(static_cast<long long>(lower)) {}

    /**
     * @brief Constructs a u128 instance from a floating-point value.
     * @details This constructor initializes a u128 instance by converting a floating-point value to
     * its 128-bit unsigned integer representation. The conversion is explicit to prevent unintended
     * implicit conversions.
     * @param[in] value The floating-point value to convert.
     */
    ___inline___ explicit u128(double value) noexcept {
      auto const result = from_double(value);
      this->upper_ = result.upper_;
      this->lower_ = result.lower_;
    }

    /**
     * @brief Constructs a u128 instance from a floating-point value.
     * @details This constructor initializes a u128 instance by converting a floating-point value to
     * its 128-bit unsigned integer representation. The conversion is explicit to prevent unintended
     * implicit conversions.
     * @param[in] value The floating-point value to convert.
     */
    ___inline___ explicit u128(long double value) noexcept
      : u128(static_cast<f64>(value)) {}

    /**
     * @brief Constructs a u128 instance from a floating-point value.
     * @details This constructor initializes a u128 instance by converting a floating-point value to
     * its 128-bit unsigned integer representation. The conversion is explicit to prevent unintended
     * implicit conversions.
     * @param[in] value The floating-point value to convert.
     */
    ___inline___ explicit u128(float value) noexcept
      : u128(static_cast<f64>(value)) {}

    /**
     * @brief Retrieves the lower 64 bits of the u128 value.
     * @details This method returns the lower part of the u128 instance, allowing access to the
     * least significant bits.
     * @return The lower 64 bits as a u64.
     */
    [[nodiscard]] constexpr ___inline___ u64 lower() const noexcept { return this->lower_; }

    /**
     * @brief Retrieves the upper 64 bits of the u128 value.
     * @details This method returns the upper part of the u128 instance, allowing access to the most
     * significant bits.
     * @return The upper 64 bits as a u64.
     */
    [[nodiscard]] constexpr ___inline___ u64 upper() const noexcept { return this->upper_; }

    /**
     * @brief Computes the hash value of the u128 instance.
     */
    [[nodiscard]] ___inline___ std::size_t hash() const noexcept {
      return std::hash<u64>()(this->lower_) ^ std::hash<u64>()(this->upper_);
    }

    /**
     * @brief Swaps the values of two u128 instances.
     * @param[in,out] other The other u128 instance to swap with.
     */
    constexpr ___inline___ void swap(u128& other) noexcept {
      auto const tmp = *this;
      *this = other;
      other = tmp;
    }

    /**
     * @brief Converts the u128 value to a string representation.
     * @details Supports decimal, octal, and hexadecimal formats.
     * @param[in] fmt The format to use for the conversion, with `format::decimal` as the default.
     * @return A std::string holding the converted value.
     */
    [[nodiscard]] ___inline___ std::string to_string(format fmt = format::decimal) const {
      if(this->upper_ == 0) {
        std::ostringstream ss;
        switch(fmt) {
          case format::octal: ss << std::oct; break;
          case format::hexadecimal: ss << std::hex; break;
          default: ss << std::dec; break;
        }
        ss << this->lower_;
        return ss.str();
      }

      switch(fmt) {
        case format::octal: return u128o_oct_str(*this);
        case format::hexadecimal: return u128o_hex_str(*this);
        default: return u128o_dec_str(*this);
      }
    }

    /**
     * @brief Parses a string to a u128 instance.
     * @details Attempts to parse a given string as a u128 value in the specified
     * format. If the parsing is successful, it returns an optional containing the parsed
     * u128 value.
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed u128 value if successful, otherwise an empty
     * optional.
     */
    [[nodiscard]] static ___inline___ optional<u128>
      from_string(std::string const& str, format fmt = format::decimal) noexcept {
      return str_to_uint128(
        str.data(),
        str.data() + str.length(),  // NOLINT(*-pro-bounds-pointer-arithmetic)
        fmt
      );
    }

    /**
     * @brief Parses a string to a u128 instance.
     * @details This static method attempts to parse a given string as a u128 value in the specified
     * format. If the parsing is successful, it returns an optional containing the parsed
     * u128 value.
     * @tparam T The character type of the array.
     * @tparam N The size of the character array.
     * @param[in] str The input string to parse.
     * @param[in] fmt The string format to interpret the input string (*defaults to decimal*).
     * @return An optional containing the parsed u128 value if successful, otherwise an empty
     * optional.
     */
    template <typename T, size_t N>
    [[nodiscard]] static constexpr ___inline___ optional<u128>
      from_string(T const (&str)[N], format fmt = format::decimal) noexcept {
      return str_to_uint128(str, str + N, fmt);
    }

    /**
     * @brief Checks if the u128 value is non-zero.
     * @details This method allows u128 instances to be contextually converted to a boolean to check
     * for non-zero values.
     * @return `true` if the u128 value is non-zero, `false` otherwise.
     */
    [[nodiscard]] constexpr ___inline___ explicit operator bool() const noexcept {
      return this->upper_ or this->lower_;
    }

    [[nodiscard]] constexpr ___inline___ explicit operator ulonglong() const noexcept {
      return static_cast<ulonglong>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator long long() const noexcept {
      return static_cast<long long>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator unsigned long() const noexcept {
      return static_cast<unsigned long>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator long() const noexcept {
      return static_cast<long>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator unsigned int() const noexcept {
      return static_cast<unsigned int>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator int() const noexcept {
      return static_cast<int>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator unsigned short() const noexcept {
      return static_cast<unsigned short>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator short() const noexcept {
      return static_cast<short>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator unsigned char() const noexcept {
      return static_cast<char>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator char() const noexcept {
      return static_cast<char>(this->lower_);
    }

    [[nodiscard]] constexpr ___inline___ explicit operator signed char() const noexcept {
      return static_cast<signed char>(this->lower_);
    }

    [[nodiscard]] ___inline___ explicit operator long double() const noexcept {
      return static_cast<long double>(to_double(*this));
    }

    [[nodiscard]] ___inline___ explicit operator double() const noexcept {
      return to_double(*this);
    }

    [[nodiscard]] ___inline___ explicit operator float() const noexcept {
      return static_cast<float>(to_double(*this));
    }

    constexpr ___inline___ u128& operator+=(u128 const& other) noexcept {
      *this = *this + other;
      return *this;
    }

    constexpr ___inline___ u128& operator-=(u128 const& other) noexcept {
      *this = *this - other;
      return *this;
    }

    constexpr ___inline___ u128& operator*=(u128 const& other) noexcept {
      *this = *this * other;
      return *this;
    }

    constexpr ___inline___ u128& operator/=(u128 const& other) noexcept {
      *this = *this / other;
      return *this;
    }

    constexpr ___inline___ u128& operator%=(u128 const& other) noexcept {
      *this = *this % other;
      return *this;
    }

    constexpr ___inline___ u128& operator&=(u128 const& other) noexcept {
      this->upper_ &= other.upper_;
      this->lower_ &= other.lower_;
      return *this;
    }

    constexpr ___inline___ u128& operator|=(u128 const& other) noexcept {
      this->upper_ |= other.upper_;
      this->lower_ |= other.lower_;
      return *this;
    }

    constexpr ___inline___ u128& operator^=(u128 const& other) noexcept {
      this->upper_ ^= other.upper_;
      this->lower_ ^= other.lower_;
      return *this;
    }

    /**
     * @brief Bitwise left shift assignment operator.
     * @details Shifts the bits of this instance to the left by the specified number of places and
     * assigns the result to this instance.
     *
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the left.
     * @return A reference to this instance after the shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    constexpr ___inline___ u128& operator<<=(T shift) noexcept {
      *this = *this << shift;
      return *this;
    }

    /**
     * @brief Bitwise right shift assignment operator.
     * @details Shifts the bits of this instance to the right by the specified number of places and
     * assigns the result to this instance.
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the right.
     * @return A reference to this instance after the shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    constexpr ___inline___ u128& operator>>=(T shift) noexcept {
      *this = *this >> shift;
      return *this;
    }

    [[nodiscard]] constexpr ___inline___ u128 operator+() const noexcept { return *this; }

    [[nodiscard]] constexpr ___inline___ u128 operator-() const noexcept { return ++(~*this); }

    [[nodiscard]] constexpr ___inline___ u128 operator~() const noexcept {
      return {~this->upper_, ~this->lower_};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator+(u128 const& other) const noexcept {
      u64 const lower = this->lower_ + other.lower_;
      u64 const carry = lower < this->lower_ ? 1 : 0;
      return {this->upper_ + other.upper_ + carry, lower};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator-(u128 const& other) const noexcept {
      u64 const lower = this->lower_ - other.lower_;
      u64 const borrow = lower > this->lower_ ? 1 : 0;
      return {this->upper_ - other.upper_ - borrow, lower};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator*(u128 const& other) const noexcept {
      u64 lower = 0;
      u64 upper = big_mul(this->lower_, other.lower_, lower);
      upper += (this->upper_ * other.lower_) + (this->lower_ * other.upper_);
      return {upper, lower};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator/(u128 const& other) const noexcept {
      return divide(*this, other);
    }

    [[nodiscard]] constexpr ___inline___ u128 operator%(u128 const& other) const noexcept {
      auto const quotient = divide(*this, other);
      return *this - quotient * other;
    }

    [[nodiscard]] constexpr ___inline___ u128 operator&(u128 const& other) const noexcept {
      return {this->upper_ & other.upper_, this->lower_ & other.lower_};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator|(u128 const& other) const noexcept {
      return {this->upper_ | other.upper_, this->lower_ | other.lower_};
    }

    [[nodiscard]] constexpr ___inline___ u128 operator^(u128 const& other) const noexcept {
      return {this->upper_ ^ other.upper_, this->lower_ ^ other.lower_};
    }

    /**
     * @brief Bitwise left shift operator with an integral type.
     * @details Shifts the bits of this instance to the left by the specified number of places and
     * returns a new u128 instance with the result.
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the left.
     * @return A new u128 instance representing the result of the left shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ u128 operator<<(T shift) const noexcept {
      if(not shift)
        return *this;
      if(shift >= 64 and shift <= 128)
        return {this->lower_ << (shift - 64), 0};
      if(shift < 64 and shift > 0)
        return {(this->upper_ << shift) + (this->lower_ >> (64 - shift)), this->lower_ << shift};
      return {0, 0};
    }

    /**
     * @brief Bitwise right shift operator with an integral type.
     * @details Shifts the bits of this instance to the right by the specified number of places and
     * returns a new u128 instance with the result.
     * @tparam T An integral type representing the number of places to shift.
     * @param[in] shift The number of bits to shift to the right.
     * @return A new u128 instance representing the result of the right shift operation.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ u128 operator>>(T shift) const noexcept {
      if(not shift)
        return *this;
      if(shift >= 64 and shift <= 128)
        return {0, this->upper_ >> (shift - 64)};
      if(shift < 64 and shift > 0)
        return {this->upper_ >> shift, (this->lower_ >> shift) + (this->upper_ << (64 - shift))};
      return {0, 0};
    }

    constexpr ___inline___ u128& operator++() noexcept {
      *this += 1;
      return *this;
    }

    constexpr ___inline___ u128& operator--() noexcept {
      *this -= 1;
      return *this;
    }

    constexpr ___inline___ u128 operator++(int) noexcept {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr ___inline___ u128 operator--(int) noexcept {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    [[nodiscard]] constexpr ___inline___ bool operator!() const noexcept {
      return not this->upper_ and not this->lower_;
    }

    [[nodiscard]] constexpr ___inline___ bool operator&&(u128 const& other) const noexcept {
      return (this->upper_ or this->lower_) and (other.upper_ or other.lower_);
    }

    [[nodiscard]] constexpr ___inline___ bool operator||(u128 const& other) const noexcept {
      return (this->upper_ or this->lower_) or (other.upper_ or other.lower_);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator==(T lower) const noexcept {
      return *this == u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator==(u128 const& other) const noexcept {
      return this->upper_ == other.upper_ and this->lower_ == other.lower_;
    }

    /**
     * @brief Inequality operator with an integral type.
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is not equal to the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator!=(T lower) const noexcept {
      return *this != u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator!=(u128 const& other) const noexcept {
      return not (*this == other);
    }

    /**
     * @brief Less than operator with an integral type.
     * @details Checks if the value of this instance is lexicographically less than the provided
     * integral value.
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is less than the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator<(T lower) const noexcept {
      return *this < u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator<(u128 const& other) const noexcept {
      return this->upper_ < other.upper_
          or (this->upper_ == other.upper_ and this->lower_ < other.lower_);
    }

    /**
     * @brief Greater than operator with an integral type.
     * @details Checks if the value of this instance is lexicographically greater than the provided
     * integral value.
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is greater than the integral value, `false` otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator>(T lower) const noexcept {
      return *this > u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator>(u128 const& other) const noexcept {
      return other < *this;
    }

    /**
     * @brief Less than or equal to operator with an integral type.
     * @details Checks if the value of this instance is lexicographically less than or equal to the
     * provided integral value.
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is less than or equal to the integral value, `false`
     * otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator<=(T lower) const noexcept {
      return *this <= u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator<=(u128 const& other) const noexcept {
      return not (other < *this);
    }

    /**
     * @brief Greater than or equal to operator with an integral type.
     * @details Checks if the value of this instance is lexicographically greater than or equal to
     * the provided integral value.
     * @tparam T An integral type to compare with.
     * @param[in] lower The integral value to compare with.
     * @return `true` if this instance is greater than or equal to the integral value, `false`
     * otherwise.
     */
    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    [[nodiscard]] constexpr ___inline___ bool operator>=(T lower) const noexcept {
      return *this >= u128(lower);
    }

    [[nodiscard]] constexpr ___inline___ bool operator>=(u128 const& other) const noexcept {
      return not (*this < other);
    }

   private:
    [[nodiscard]] static constexpr ___inline___ u64 big_mul(u64 a, u64 b, u64& lower) noexcept {
      auto al = static_cast<u32>(a);
      auto ah = static_cast<u32>(a >> 32);
      auto bl = static_cast<u32>(b);
      auto bh = static_cast<u32>(b >> 32);
      u64 const mull = static_cast<u64>(al) * bl;
      u64 const t = static_cast<u64>(ah) * bl + (mull >> 32);
      u64 const tl = static_cast<u64>(al) * bh + static_cast<u32>(t);
      lower = tl << 32 | static_cast<u32>(mull);
      return static_cast<u64>(ah) * bh + (t >> 32) + (tl >> 32);
    }

    [[nodiscard]] static constexpr ___inline___ u128
      divide(u128 const& lhs, u128 const& rhs) noexcept {
      if(rhs.upper_ == 0) {
        if(rhs.lower_ == 0)
          return {};
        if(lhs.upper_ == 0)
          return lhs.lower_ / rhs.lower_;
      }
      if(rhs >= lhs)
        return rhs == lhs ? 1 : 0;
      return divide_slow(lhs, rhs);
    }

    [[nodiscard]] static constexpr ___inline___ u128
      divide_slow(u128 const& quotient, u128 const& divisor) noexcept {
      u32 left[4] {
        static_cast<u32>(quotient.lower_),
        static_cast<u32>(quotient.lower_ >> 32),
        static_cast<u32>(quotient.upper_),
        static_cast<u32>(quotient.upper_ >> 32)
      };
      i32 const left_size = 4 - static_cast<i32>(leading_zero_count(quotient)) / 32;

      u32 const right[4] {
        static_cast<u32>(divisor.lower_),
        static_cast<u32>(divisor.lower_ >> 32),
        static_cast<u32>(divisor.upper_),
        static_cast<u32>(divisor.upper_ >> 32)
      };
      i32 const right_size = 4 - static_cast<i32>(leading_zero_count(divisor)) / 32;

      u32 bits[4] {};
      i32 const bits_size = left_size - right_size + 1;

      assert(left_size >= 1);
      assert(right_size >= 1);
      assert(left_size >= right_size);

      u32 div_hi = right[right_size - 1];
      u32 div_lo = right_size > 1 ? right[right_size - 2] : 0;

      u32 const shift = leading_zero_count(div_hi);
      u32 const back_shift = 32 - shift;

      if(shift > 0) {
        u32 const div_nx = right_size > 2 ? right[right_size - 3] : 0;
        div_hi = (div_hi << shift) | (div_lo >> back_shift);
        div_lo = (div_lo << shift) | (div_nx >> back_shift);
      }

      for(i32 i = left_size; i >= right_size; --i) {
        i32 const n = i - right_size;
        u32 const t = static_cast<u32>(i) < static_cast<u32>(left_size) ? left[i] : 0;

        u64 val_hi = (static_cast<u64>(t) << 32) | left[i - 1];
        u32 val_lo = (i > 1) ? left[i - 2] : 0;  // NOLINT(clang-analyzer-core.uninitialized.Assign)

        if(shift > 0) {
          u32 const val_nx = i > 2 ? left[i - 3] : 0;
          val_hi = (val_hi << shift) | (val_lo >> back_shift);
          val_lo = (val_lo << shift) | (val_nx >> back_shift);
        }

        u64 digit = val_hi / div_hi;

        if(digit > 0xFFFFFFFF)
          digit = 0xFFFFFFFF;
        while(divide_guess_too_big(digit, val_hi, val_lo, div_hi, div_lo))
          --digit;
        if(digit > 0) {
          u32 carry = subtract_divisor(left + n, left_size - n, right, right_size, digit);
          if(carry != t) {
            assert(carry == t + 1);
            // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores): carry is not used in the release
            // build
            carry = add_divisor(left + n, left_size - n, right, right_size);
            --digit;
            assert(carry == 1);
          }
        }

        if(static_cast<u32>(n) < static_cast<u32>(bits_size))
          bits[n] = static_cast<u32>(digit);
        if(static_cast<u32>(i) < static_cast<u32>(left_size))
          left[i] = 0;
      }

      return {u64(bits[3]) << 32 | bits[2], u64(bits[1]) << 32 | bits[0]};
    }

    [[nodiscard]] static constexpr ___inline___ bool
      divide_guess_too_big(u64 q, u64 val_hi, u32 val_lo, u32 div_hi, u32 div_lo) noexcept {
      assert(q <= 0xFFFFFFFF);

      u64 chk_hi = div_hi * q;
      u64 chk_lo = div_lo * q;
      chk_hi += chk_lo >> 32;
      chk_lo = static_cast<u32>(chk_lo);
      return (chk_hi > val_hi) or (chk_hi == val_hi and chk_lo > val_lo);
    }

    [[nodiscard]] static constexpr ___inline___ u32 subtract_divisor(
      u32* left,
      i32 ls /* NOLINT(misc-unused-parameters) */,
      u32 const (&right)[4],
      i32 rs,
      u64 q
    ) noexcept {
      assert(ls >= rs);
      assert(q <= 0xFFFFFFFF);

      u64 carry = 0;

      for(i32 i = 0; i < rs; ++i) {
        carry += right[i] * q;
        auto digit = u32(carry);
        carry >>= 32;
        if(left[i] < digit)
          ++carry;
        left[i] -= digit;
      }

      return static_cast<u32>(carry);
    }

    [[nodiscard]] static constexpr ___inline___ u32 add_divisor(
      u32* left,
      i32 ls /* NOLINT(misc-unused-parameters) */,
      u32 const (&right)[4],
      i32 rs
    ) noexcept {
      assert(ls >= rs);
      u64 carry = 0;
      for(i32 i = 0; i < rs; ++i) {
        u64 const digit = left[i] + carry + right[i];
        left[i] = static_cast<u32>(digit);
        carry = digit >> 32;
      }
      return static_cast<u32>(carry);
    }

    [[nodiscard]] static ___inline___ u128 from_double(double value) noexcept {
      constexpr double two_pow_128 = 340282366920938463463374607431768211456.0;

      if(value < 0 or std::isnan(value))
        return u128(0, 0);
      if(value >= two_pow_128)
        return u128(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);

      if(value >= 1.0) {
        u64 const bits = double_to_uint64_bits(value);
        u128 result((bits << 12) >> 1 | 0x8000000000000000, 0x0000000000000000);
        result >>= (1'023 + 128 - 1 - static_cast<i32>(bits >> 52));
        return result;
      }

      return u128(0, 0);
    }

    [[nodiscard]] static ___inline___ double to_double(u128 value) noexcept {
      constexpr double two_pow_52 = 4503599627370496.0;
      constexpr double two_pow_76 = 75557863725914323419136.0;
      constexpr double two_pow_104 = 20282409603651670423947251286016.0;
      constexpr double two_pow_128 = 340282366920938463463374607431768211456.0;

      constexpr u64 two_pow_52_bits = 0x4330000000000000;
      constexpr u64 two_pow_76_bits = 0x44B0000000000000;
      constexpr u64 two_pow_104_bits = 0x4670000000000000;
      constexpr u64 two_pow_128_bits = 0x47F0000000000000;

      if(value.upper_ == 0)
        return double(value.lower_);

      if((value.upper_ >> 24) == 0) {
        double const lower = uint64_bits_to_double(two_pow_52_bits | ((value.lower_ << 12) >> 12))
                           - two_pow_52;
        double const upper = uint64_bits_to_double(two_pow_104_bits | static_cast<u64>(value >> 52))
                           - two_pow_104;
        return double(lower + upper);
      }

      double const lower = uint64_bits_to_double(
                             two_pow_76_bits | (static_cast<u64>(value >> 12) >> 12)
                             | (value.lower_ & 0xFFFFFF)
                           )
                         - two_pow_76;
      double const upper = uint64_bits_to_double(two_pow_128_bits | static_cast<u64>(value >> 76))
                         - two_pow_128;
      return double(lower + upper);
    }

    [[nodiscard]] static ___inline___ double uint64_bits_to_double(u64 bits) noexcept {
      double result;  // NOLINT(cppcoreguidelines-init-variables)
      std::memcpy(&result, &bits, sizeof(double));
      return result;
    }

    [[nodiscard]] static ___inline___ u64 double_to_uint64_bits(double value) noexcept {
      u64 result;  // NOLINT(cppcoreguidelines-init-variables)
      std::memcpy(&result, &value, sizeof(u64));
      return result;
    }

    [[nodiscard]] static constexpr ___inline___ u32 leading_zero_count(u128 const& value) noexcept {
      if(value.upper_ != 0) {
        for(u32 i = 0; i < 64; ++i) {
          if(value.upper_ & (1ULL << (63 - i)))
            return i;
        }
      }
      if(value.lower_ != 0) {
        for(u32 i = 0; i < 64; ++i) {
          if(value.lower_ & (1ULL << (63 - i)))
            return 64 + i;
        }
      }
      return 128;
    }

    [[nodiscard]] static constexpr ___inline___ u32 leading_zero_count(u32 value) noexcept {
      i32 shift = 32;
      if(value != 0) {
        for(shift = 0; shift < 32; ++shift) {
          if(value & (1ULL << (31 - shift)))
            break;
        }
      }
      return shift;
    }

    [[nodiscard]] static ___inline___ std::string u128o_dec_str(u128 value) {
      std::ostringstream ss;
      do {
        auto const q = value / 10;
        auto const r = value - q * 10;
        ss << r.lower_;
        value = q;
      } while(value);

      auto result = ss.str();
      std::reverse(result.begin(), result.end());  // NOLINT(*-use-ranges)
      return result;
    }

    [[nodiscard]] static ___inline___ std::string u128o_oct_str(u128 value) {
      std::ostringstream ss;

      do {
        auto const q = value / 8;
        auto const r = value - q * 8;

        ss << r.lower_;

        value = q;
      } while(value);

      auto result = ss.str();
      std::reverse(result.begin(), result.end());  // NOLINT(*-use-ranges)
      return result;
    }

    [[nodiscard]] static ___inline___ std::string u128o_hex_str(u128 value) {
      constexpr char digits[] = "0123456789abcdef";

      std::ostringstream ss;
      do {
        auto const q = value / 16;
        auto const r = value - q * 16;

        ss << digits[r.lower_];

        value = q;
      } while(value);

      auto result = ss.str();
      std::reverse(result.begin(), result.end());  // NOLINT(*-use-ranges)
      return result;
    }

#pragma warning(push, 3)
#ifdef __clang__
    _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wundefined-inline\"")
#endif
      template <typename T>
      [[nodiscard]] static constexpr ___inline___ optional<u128> str_to_uint128(
        const T* begin,
        const T* end,
        format fmt
      ) noexcept {
      switch(fmt) {
        case format::octal: return oct_str_to_uint128(begin, end);
        case format::hexadecimal: return hex_str_to_uint128(begin, end);
        default: return dec_str_to_uint128(begin, end);
      }
    }
#ifdef __clang__
    _Pragma("clang diagnostic pop")
#endif
#pragma warning(pop)

      template <typename T>
      [[nodiscard]] static constexpr ___inline___ optional<u128> dec_str_to_uint128(
        const T* begin,
        const T* end
      ) noexcept {
      bool error = false;
      u32 error_symbol = 0;
      u128 result = 0;
      T const* it = begin;
      while(it < end) {
        auto const c = detail::next_char_or_error(it, end, error, error_symbol);
        if(error)  // NOLINT(bugprone-branch-clone)
          return none;
        else if(c == '\0')
          break;
        else if(c < '0' || c > '9')
          return none;
        result = result * 10 + (c - '0');
      }
      return result;
    }

    template <typename T>
    [[nodiscard]] static constexpr ___inline___ optional<u128>
      oct_str_to_uint128(T const* begin, T const* end) noexcept {
      bool error = false;
      u32 error_symbol = 0;
      u128 result = 0;
      T const* it = begin;
      while(it < end) {
        auto const c = detail::next_char_or_error(it, end, error, error_symbol);
        if(error)  // NOLINT(bugprone-branch-clone)
          return none;
        else if(c == '\0')
          break;
        else if(c < '0' || c > '7')
          return none;

        result = result * 8 + (c - '0');
      }
      return result;
    }

    template <typename T>
    [[nodiscard]] static constexpr ___inline___ optional<u128>
      hex_str_to_uint128(T const* begin, T const* end) noexcept {
      bool error = false;
      u32 error_symbol = 0;
      u128 result = 0;
      int digit = 0;
      T const* it = begin;
      while(it < end) {
        auto const c = detail::next_char_or_error(it, end, error, error_symbol);
        if(error)  // NOLINT(bugprone-branch-clone)
          return none;
        else if(c == '\0')
          break;
        else if(c >= '0' && c <= '9')
          digit = c - '0';
        else if(c >= 'A' && c <= 'F')
          digit = c - 55;
        else if(c >= 'a' && c <= 'f')
          digit = c - 87;
        else
          return none;

        result = result * 16 + digit;
      }
      return result;
    }

#if ROLLY_ENDIAN == ROLLY_BIG_ENDIAN
    u64 upper_ {};
    u64 lower_ {};
#else
    u64 lower_ {};
    u64 upper_ {};
#endif
  };

  /**
   * Performs addition between an integer and a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value to be added.
   * @param[in] value The u128 value to be added.
   * @return u128 the result of the addition.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator+(T lower, u128 const& value) noexcept {
    return u128(lower) + value;
  }

  /**
   * Performs subtraction of a u128 value from an integer.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value from which the u128 value is subtracted.
   * @param[in] value The u128 value to be subtracted.
   * @return u128 the result of the subtraction.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator-(T lower, u128 const& value) noexcept {
    return u128(lower) - value;
  }

  /**
   * Performs multiplication between an integer and a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value to be multiplied.
   * @param[in] value The u128 value to be multiplied.
   * @return u128 the result of the multiplication.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator*(T lower, u128 const& value) noexcept {
    return u128(lower) * value;
  }

  /**
   * Performs division of an integer by a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value to be divided.
   * @param[in] value The u128 value that divides the integer.
   * @return u128 the result of the division.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator/(T lower, u128 const& value) noexcept {
    return u128(lower) / value;
  }

  /**
   * Calculates the remainder of division of an integer by a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value to be divided.
   * @param[in] value The u128 value that divides the integer.
   * @return u128 the remainder of the division.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator%(T lower, u128 const& value) noexcept {
    return u128(lower) % value;
  }

  /**
   * Performs bitwise AND operation between an integer and a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value.
   * @param[in] value The u128 value.
   * @return u128 the result of the bitwise AND operation.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator&(T lower, u128 const& value) noexcept {
    return u128(lower) & value;
  }

  /**
   * Performs bitwise OR operation between an integer and a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value.
   * @param[in] value The u128 value.
   * @return u128 the result of the bitwise OR operation.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator|(T lower, u128 const& value) noexcept {
    return u128(lower) | value;
  }

  /**
   * Performs bitwise XOR operation between an integer and a u128 value.
   *
   * @tparam T Integer type.
   * @param[in] lower The integer value.
   * @param[in] value The u128 value.
   * @return u128 the result of the bitwise XOR operation.
   */
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ u128 operator^(T lower, u128 const& value) noexcept {
    return u128(lower) ^ value;
  }

  /**
   * Checks if an integer value is equal to a u128 value.
   *
   * @tparam T Integral type.
   * @param[in] lower The integer value to compare.
   * @param[in] other The u128 value to compare.
   * @return `true` if the integer value is equal to the u128 value, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator==(T lower, u128 const& other) noexcept {
    return u128(lower) == other;
  }

  /**
   * Checks if an integer value is not equal to a u128 value.
   *
   * @tparam T Integral type.
   * @param[in] lower The integer value to compare.
   * @param[in] other The u128 value to compare.
   * @return `true` if the integer value is not equal to the u128 value, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator!=(T lower, u128 const& other) noexcept {
    return u128(lower) != other;
  }

  /**
   * Compares if an integer value is less than a u128 value.
   *
   * This function template overloads the less than operator (<) to compare an integer of type T
   * with a u128 value. It checks if the integer value is lexicographically less than the
   * u128 value.
   *
   * @tparam T An integral type parameter that is checked at compile-time to ensure it is an
   * integer type.
   * @param[in] lower The integer value of type T to be compared.
   * @param[in] other The u128 value to compare against.
   * @return `true` if \a lower is less than \a other, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator<(T lower, u128 const& other) noexcept {
    return u128(lower) < other;
  }

  /**
   * Compares if an integer value is greater than a u128 value.
   *
   * This function template overloads the greater than operator (>) to compare an integer of type
   * T with a u128 value. It checks if the integer value is lexicographically greater than the
   * u128 value.
   *
   * @tparam T An integral type parameter that is checked at compile-time to ensure it is an
   * integer type.
   * @param[in] lower The integer value of type T to be compared.
   * @param[in] other The u128 value to compare against.
   * @return `true` if \a lower is greater than \a other, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator>(T lower, u128 const& other) noexcept {
    return u128(lower) > other;
  }

  /**
   * Compares if an integer value is less than or equal to a u128 value.
   *
   * This function template overloads the less than or equal to operator (<=) to compare an
   * integer of type T with a u128 value. It checks if the integer value is lexicographically less
   * than or equal to the u128 value.
   *
   * @tparam T An integral type parameter that is checked at compile-time to ensure it is an
   * integer type.
   * @param[in] lower The integer value of type T to be compared.
   * @param[in] other The u128 value to compare against.
   * @return `true` if \a lower is less than or equal to \a other, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator<=(T lower, u128 const& other) noexcept {
    return u128(lower) <= other;
  }

  /**
   * Compares if an integer value is greater than or equal to a u128 value.
   *
   * This function template overloads the greater than or equal to operator (>=) to compare an
   * integer of type T with a u128 value. It checks if the integer value is lexicographically
   * greater than or equal to the u128 value.
   *
   * @tparam T An integral type parameter that is checked at compile-time to ensure it is an
   * integer type.
   * @param[in] lower The integer value of type T to be compared.
   * @param[in] other The u128 value to compare against.
   * @return `true` if \a lower is greater than or equal to \a other, `false` otherwise.
   */
  template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
  [[nodiscard]] constexpr ___inline___ bool operator>=(T lower, u128 const& other) noexcept {
    return u128(lower) >= other;
  }

}  // namespace rolly

namespace std {

#pragma warning(push, 3)
#if defined(_MSC_VER)
#  pragma warning(disable : 4'996)
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__clang__)
  _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated\"")
#endif

  template <typename T>
  struct _numeric_limits_uint128 {
    static constexpr bool is_bounded = true;
    static constexpr bool is_exact = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_modulo = true;
    static constexpr bool is_specialized = true;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_signed = false;
    static constexpr bool has_denorm_loss = false;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool tinyness_before = false;
    static constexpr bool traps = false;
    static constexpr int max_digits10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int digits = 128;
    static constexpr int digits10 = 38;
    static constexpr int radix = 2;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr std::float_round_style round_style = std::round_toward_zero;

    [[nodiscard]] static constexpr ___inline___ T(min)() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T(max)() noexcept {
      return T(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
    }

    [[nodiscard]] static constexpr ___inline___ T lowest() noexcept { return (min)(); }

    [[nodiscard]] static constexpr ___inline___ T epsilon() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T round_error() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T denorm_min() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T infinity() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T quiet_NaN() noexcept { return 0; }

    [[nodiscard]] static constexpr ___inline___ T signaling_NaN() noexcept { return 0; }
  };

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_bounded;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_exact;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_integer;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_modulo;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_specialized;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_iec559;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::is_signed;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::has_denorm_loss;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::has_infinity;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::has_quiet_NaN;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::has_signaling_NaN;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::tinyness_before;

  template <typename T>
  constexpr bool _numeric_limits_uint128<T>::traps;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::max_digits10;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::max_exponent;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::max_exponent10;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::min_exponent;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::min_exponent10;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::digits;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::digits10;

  template <typename T>
  constexpr int _numeric_limits_uint128<T>::radix;

  template <typename T>
  constexpr std::float_denorm_style _numeric_limits_uint128<T>::has_denorm;

  template <typename T>
  constexpr std::float_round_style _numeric_limits_uint128<T>::round_style;

#if defined(_MSC_VER)
#  pragma warning(default : 4'996)
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#elif defined(__clang__)
  _Pragma("clang diagnostic pop")
#endif
#pragma warning(pop)

  template <>
  struct numeric_limits<rolly::u128> : _numeric_limits_uint128<rolly::u128> {};

  template <>
  struct hash<rolly::u128> {
    [[nodiscard]] ___inline___ size_t operator()(rolly::u128 const& value) const noexcept {
      return value.hash();
    }
  };

  constexpr ___inline___ void swap(rolly::u128& value1, rolly::u128& value2) noexcept {
    auto const tmp = value1;
    value1 = value2;
    value2 = tmp;
  }

  [[nodiscard]] constexpr ___inline___ rolly::u128 abs(rolly::u128 const& value) noexcept {
    return value;
  }

  [[nodiscard]] ___inline___ std::string to_string(rolly::u128 const& value) {
    return value.to_string();
  }

  template <typename T>
  ___inline___ std::basic_istream<T, std::char_traits<T>>&
    operator>>(std::basic_istream<T, std::char_traits<T>>& stream, rolly::u128& value) {
    auto fmt = rolly::u128::format::decimal;
    if(stream.flags() & ios_base::hex) {
      fmt = rolly::u128::format::hexadecimal;
    } else if(stream.flags() & ios_base::oct) {
      fmt = rolly::u128::format::octal;
    }
    std::basic_string<T, std::char_traits<T>, std::allocator<T>> str;
    stream >> str;
    auto const result = rolly::u128::from_string(str, fmt);
    if(result) {
      value = result.value();
    } else {
      stream.setstate(std::ios_base::failbit);
    }
    return stream;
  }
}  // namespace std

template <>
struct fmt::formatter<rolly::u128> : fmt::ostream_formatter {};
