#pragma once

#include <cstddef>
#include <cstdint>

namespace lf // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Inline namespace for types and type aliases.
  inline namespace types
  {
    using u8 = uint8_t;       ///< 8-bit unsigned integer
    using u16 = uint16_t;     ///< 16-bit unsigned integer
    using u32 = uint32_t;     ///< 32-bit unsigned integer
    using u64 = uint64_t;     ///< 64-bit unsigned integer
    using i8 = int8_t;        ///< 8-bit signed integer
    using i16 = int16_t;      ///< 16-bit signed integer
    using i32 = int32_t;      ///< 32-bit signed integer
    using i64 = int64_t;      ///< 64-bit signed integer
    using usize = size_t;     ///< Unsigned integer with pointer size
    using isize = intptr_t;   ///< Signed integer with pointer size
    using f32 = float;        ///< Float with 32-bit precision
    using f64 = double;       ///< Float with 64-bit precision
    using f128 = long double; ///< Float with 128-bit precision
  }

  /// \brief Inline namespace for literal operators.
  inline namespace literals
  {
    inline auto operator"" _u8(unsigned long long int value) { return u8(value); }
    inline auto operator"" _u16(unsigned long long int value) { return u16(value); }
    inline auto operator"" _u32(unsigned long long int value) { return u32(value); }
    inline auto operator"" _u64(unsigned long long int value) { return u64(value); }
    inline auto operator"" _i8(unsigned long long int value) { return i8(value); }
    inline auto operator"" _i16(unsigned long long int value) { return i16(value); }
    inline auto operator"" _i32(unsigned long long int value) { return i32(value); }
    inline auto operator"" _i64(unsigned long long int value) { return i64(value); }
    inline auto operator"" _sz(unsigned long long int value) { return usize(value); }
    inline auto operator"" _iptr(unsigned long long int value) { return isize(value); }
    inline auto operator"" _uptr(unsigned long long int value) { return usize(value); }
    inline auto operator"" _ptrdiff(unsigned long long int value) { return ptrdiff_t(value); }
  }
} // namespace leaf::inline types
