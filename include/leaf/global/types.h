#pragma once

#include <cstddef>
#include <cstdint>

namespace leaf // NOLINT(*-concat-nested-namespaces)
{
  /**
   * \brief Пространство имен с псевдонимами для типов данных.
   */
  inline namespace types
  {
    using u8 = uint8_t;       ///< 8-bit беззнаковое целое
    using u16 = uint16_t;     ///< 16-bit беззнаковое целое
    using u32 = uint32_t;     ///< 32-bit беззнаковое целое
    using u64 = uint64_t;     ///< 64-bit беззнаковое целое
    using i8 = int8_t;        ///< 8-bit знаковое целое
    using i16 = int16_t;      ///< 16-bit знаковое целое
    using i32 = int32_t;      ///< 32-bit знаковое целое
    using i64 = int64_t;      ///< 64-bit знаковое целое
    using usize = size_t;     ///< Беззнаковое целое размером с указатель на целевой системе
    using isize = intptr_t;   ///< Знаковое целое размером с указатель на целевой системе
    using f32 = float;        ///< 32-bit число с плавающей точкой (`float`)
    using f64 = double;       ///< 64-bit число с плавающей точкой (`double`)
    using f128 = long double; ///< 128-bit число с плавающей точкой (`long double`)
  }

  /**
   * \brief Пространство имен с пользовательскими литералами.
   */
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
