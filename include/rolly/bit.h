#pragma once

#include <array>
#include <algorithm>
#include "global.h"
#include "concepts/num.h"

// NOLINTBEGIN(*-reserved-identifier)
#ifdef ROLLY_COMPILER_MSVC
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif  // _MSC_VER

#if defined(ROLLY_COMPILER_MSVC)
#  include <cstdlib>
#  define ___rolly_byteswap16 _byteswap_ushort
#  define ___rolly_byteswap32 _byteswap_ulong
#  define ___rolly_byteswap64 _byteswap_uint64
#else
#  define ___rolly_byteswap16 __builtin_bswap16
#  define ___rolly_byteswap32 __builtin_bswap32
#  define ___rolly_byteswap64 __builtin_bswap64
#endif
// NOLINTEND(*-reserved-identifier)

#if (__cplusplus >= 202'002L) && defined(__has_include)
#  if __has_include(<bit>)
#    define ROLLY_HAS_STD_ENDIAN
#    include <bit>
#  endif
#endif

#define ROLLY_LITTLE_ENDIAN 0
#define ROLLY_BIG_ENDIAN 1

#ifndef ROLLY_ENDIAN
//  GCC 4.6
#  ifdef __BYTE_ORDER__
#    if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#      define ROLLY_ENDIAN ROLLY_LITTLE_ENDIAN
#    elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#      define ROLLY_ENDIAN ROLLY_BIG_ENDIAN
#    else
#      error Unknown endianness detected. Needs to define ROLLY_ENDIAN
#    endif
// GLIBC
#  elif defined(__GLIBC__)
#    include <endian.h>
#    if (__BYTE_ORDER == __LITTLE_ENDIAN)
#      define ROLLY_ENDIAN ROLLY_LITTLE_ENDIAN
#    elif (__BYTE_ORDER == __BIG_ENDIAN)
#      define ROLLY_ENDIAN ROLLY_BIG_ENDIAN
#    else
#      error Unknown endianness detected. Needs to define ROLLY_ENDIAN
#    endif
// _LITTLE_ENDIAN and _BIG_ENDIAN
#  elif defined(_LITTLE_ENDIAN) && ! defined(_BIG_ENDIAN)
#    define ROLLY_ENDIAN ROLLY_LITTLE_ENDIAN
#  elif defined(_BIG_ENDIAN) && ! defined(_LITTLE_ENDIAN)
#    define ROLLY_ENDIAN ROLLY_BIG_ENDIAN
// For architecture
#  elif defined(__hppa) || defined(__hpux) || defined(__powerpc__) || defined(__ppc__) \
    || defined(__s390__)
defined(__sparc__) || defined(__sparc) || defined(_MIPSEB)
  || defined(_POWER)
#    define ROLLY_ENDIAN ROLLY_BIG_ENDIAN
#  elif defined(__alpha__) || defined(__amd64__) || defined(__amd64) || defined(__bfin__) \
    || defined(__i386__) || defined(__ia64__) || defined(__ia64) || defined(__x86_64__)   \
    || defined(__x86_64) || defined(_M_ALPHA) || defined(_M_AMD64) || defined(_M_IA64)    \
    || defined(_M_IX86) || defined(_M_X64)
#    define ROLLY_ENDIAN ROLLY_LITTLE_ENDIAN
#  elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
#    define ROLLY_ENDIAN ROLLY_LITTLE_ENDIAN
#  elif ! defined(IPADDRESS_HAS_STD_ENDIAN)
#    error Unknown endianness detected. Needs to define ROLLY_ENDIAN
#  endif
#endif

namespace rolly {
  /**
   * @brief Returns the number of 1 bits in the value.
   * @tparam T Type of the value. Must be an unsigned integral type.
   * @param x Value to count the number of 1 bits in.
   * @return The number of 1 bits in the value.
   */
#ifdef DOXYGEN
  template <std::integral T>
#else
  template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
#endif
  [[nodiscard]] constexpr int popcount(T x) noexcept {
    return __builtin_popcount(x);
  }

  /**
   * @brief Checks if the value is an integral power of two.
   * @tparam T Type of the value. Must be an integral type.
   * @param x Value to check.
   * @return True if the value is an integral power of two, false otherwise.
   * @see is_pow2
   */
#ifdef DOXYGEN
  template <std::integral T>
#else
  template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
#endif
  [[nodiscard]] constexpr bool has_single_bit(T x) noexcept {
    return popcount(static_cast<unsigned>(x)) == 1;
  }

  /**
   * @brief Checks if the value is an integral power of two.
   * @details This is an alias for @ref has_single_bit function.
   * @tparam T Type of the value. Must be an integral type.
   * @param x Value to check.
   * @return True if the value is an integral power of two, false otherwise.
   * @see has_single_bit
   */
#ifdef DOXYGEN
  template <std::integral T>
#else
  template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
#endif
  [[nodiscard]] constexpr bool is_pow2(T x) noexcept {
    return x and not (x bitand (x - 1));
  }

  /**
   * @brief Returns the next power of two of the value.
   * @tparam T Type of the value. Must be an unsigned integral type.
   * @param x Value.
   * @return The next power of two of the value.
   */
#ifdef DOXYGEN
  template <std::unsigned_integral T>
#else
  template <___concept___(std::unsigned_integral) T ___sfinae_requirement___(std::is_unsigned_v<T>)>
#endif
  [[nodiscard]] constexpr T bit_ceil(T x) noexcept {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
  }

  /**
   * @brief Endianness.
   * @details This is the backport of <tt>std::endian</tt>.
   * @version 2.1.32
   * @sa https://en.cppreference.com/w/cpp/types/endian
   */
  enum class endian : signed int {
#if defined(ROLLY_OS_WINDOWS) or defined(DOXYGEN)
    little = 0,      ///< Little endian
    big = 1,         ///< Big endian
    native = little  ///< Native endian. Either little or big.
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
  };

  /**
   * @version 2.1.32
   */
  using big_endian_t = std::integral_constant<int, static_cast<signed int>(endian::big)>;

  /**
   * @version 2.1.32
   */
  using little_endian_t = std::integral_constant<int, static_cast<signed int>(endian::little)>;

  /**
   * @version 2.1.32
   */
  using native_endian_t = std::integral_constant<int, static_cast<signed int>(endian::native)>;

  /**
   * @brief Casts value from one type to another by bit copy.
   * @details This is the backport of <tt>std::bit_cast</tt>.
   *
   * Obtain a value of type To by reinterpreting the object representation of From. Every bit in the
   * value representation of the returned To object is equal to the corresponding bit in the object
   * representation of from.
   *
   * The values of padding bits in the returned To object are unspecified.
   *
   * If there is no value of type To corresponding to the value representation produced, the
   * behavior is undefined. If there are multiple such values, which value is produced is
   * unspecified.
   * @tparam To Destination type. Must be a trivially copyable type and have the same size as the
   * source type.
   * @tparam From Source type. Must be a trivially copyable type and have the same size as the
   * destination type.
   * @param src Source value.
   * @return Destination value.
   * @sa https://en.cppreference.com/w/cpp/numeric/bit_cast
   * @version 2.1.32
   */
  template <class To, class From>
#ifndef DOXYGEN
  std::enable_if_t<
    sizeof(To) == sizeof(From)
      and std::is_trivially_copyable_v<From> and std::is_trivially_copyable_v<To>,
    To>
#endif
    bit_cast(From const& src) noexcept {
    static_assert(
      std::is_trivially_constructible_v<To>,
      "This implementation additionally requires destination type to be trivially constructible"
    );

    auto dst = To {};
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
  }

  /**
   * @brief Reverses the byte order of the value.
   * @details This is the backport of <tt>std::byteswap</tt>.
   * @tparam T Type of the value. Must be a numeric type.
   * @param x Value.
   * @return Reversed value.
   * @version 2.1.32
   */
#ifdef DOXYGEN
  template <concepts::num T>
#else
  template <___concept___(concepts::num) T ___sfinae_requirement___(is_num_v<T>)>
#endif
  [[nodiscard]] constexpr T byteswap(T x) noexcept {
    static_assert(
      std::has_unique_object_representations_v<T>,
      "T must have unique object representations"
    );
    auto value_repr = bit_cast<std::array<std::byte, sizeof(T)>>(x);
    std::reverse(std::begin(value_repr), std::end(value_repr));
    return bit_cast<T>(value_repr);
  }

  /**
   * @brief Reverses the byte order of the value if the native endianness is little.
   * @details If the native endianness is big, the value is returned unchanged.
   * @tparam T Type of the value. Must be a numeric type.
   * @param x Value.
   * @return Reversed value.
   * @version 2.1.32
   */
#ifdef DOXYGEN
  template <concepts::num T>
#else
  template <___concept___(concepts::num) T ___sfinae_requirement___(is_num_v<T>)>
#endif
  [[nodiscard]] constexpr T to_big_endian(T x) noexcept {
    if constexpr(endian::native == endian::little)
      return byteswap(x);
    else
      return x;
  }

  /**
   * @brief Reverses the byte order of the value if the native endianness is big.
   * @details If the native endianness is little, the value is returned unchanged.
   * @tparam T Type of the value. Must be a numeric type.
   * @param x Value.
   * @return Reversed value.
   * @version 2.1.32
   */
#ifdef DOXYGEN
  template <concepts::num T>
#else
  template <___concept___(concepts::num) T ___sfinae_requirement___(is_num_v<T>)>
#endif
  [[nodiscard]] constexpr T to_little_endian(T x) noexcept {
    if constexpr(endian::native == endian::big)
      return byteswap(x);
    else
      return x;
  }
}  // namespace rolly
