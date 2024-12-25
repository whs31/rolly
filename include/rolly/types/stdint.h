#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace rolly {
  using u8 = std::uint8_t;                        ///< 8-bit unsigned integer
  using u16 = std::uint16_t;                      ///< 16-bit unsigned integer
  using u32 = std::uint32_t;                      ///< 32-bit unsigned integer
  using u64 = std::uint64_t;                      ///< 64-bit unsigned integer
  using i8 = std::int8_t;                         ///< 8-bit signed integer
  using i16 = std::int16_t;                       ///< 16-bit signed integer
  using i32 = std::int32_t;                       ///< 32-bit signed integer
  using i64 = std::int64_t;                       ///< 64-bit signed integer
  using usize = std::size_t;                      ///< Unsigned integer with pointer size
  using isize = std::make_signed_t<std::size_t>;  ///< Signed integer with pointer size
  using f32 = float;                              ///< Float with 32-bit precision
  using f64 = double;                             ///< Float with 64-bit precision
  using f128 [[maybe_unused]] = long double;      ///< Float with 128-bit precision
  using uptr = std::uintptr_t;                    ///< Unsigned integer with pointer size

  // NOLINTNEXTLINE(*-runtime-int)
  using ulonglong
    [[maybe_unused]] = unsigned long long;  ///< Alias for <tt>unsigned long long</tt>.

  // NOLINTNEXTLINE(*-runtime-int)
  using uint [[maybe_unused]] = unsigned int;  ///< Alias for <tt>unsigned int</tt>.

  /**
   * @brief Inline namespace for literal operators.
   */
  inline namespace literals {
    /**
     * @brief Literal operator for @ref u8.
     */
    constexpr u8 operator""_u8(ulonglong value) { return static_cast<u8>(value); }

    /**
     * @brief Literal operator for @ref u16.
     */
    constexpr u16 operator""_u16(ulonglong value) { return static_cast<u16>(value); }

    /**
     * @brief Literal operator for @ref u32.
     */
    constexpr u32 operator""_u32(ulonglong value) { return static_cast<u32>(value); }

    /**
     * @brief Literal operator for @ref u64.
     */
    constexpr u64 operator""_u64(ulonglong value) { return static_cast<u64>(value); }

    /**
     * @brief Literal operator for @ref i8.
     */
    constexpr i8 operator""_i8(ulonglong value) { return static_cast<i8>(value); }

    /**
     * @brief Literal operator for @ref i16.
     */
    constexpr i16 operator""_i16(ulonglong value) { return static_cast<i16>(value); }

    /**
     * @brief Literal operator for @ref i32.
     */
    constexpr i32 operator""_i32(ulonglong value) { return static_cast<i32>(value); }

    /**
     * @brief Literal operator for @ref i64.
     */
    constexpr i64 operator""_i64(ulonglong value) { return static_cast<i64>(value); }

    /**
     * @brief Literal operator for <code>std::ptrdiff_t</code>.
     */
    constexpr std::ptrdiff_t operator""_ptrdiff(ulonglong value) {
      return static_cast<std::ptrdiff_t>(value);
    }

    // NOLINTBEGIN(*-pro-type-reinterpret-cast, *-no-int-to-ptr)

    /**
     * @brief Literal operator for <code>void*</code>.
     * @note This operator is not constexpr.
     */
    inline void* operator""_pvoid(unsigned long long value) {
      return reinterpret_cast<void*>(value);
    }

    // NOLINTEND(*-pro-type-reinterpret-cast, *-no-int-to-ptr)
  }  // namespace literals
}  // namespace rolly
