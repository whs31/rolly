#pragma once

#include <cstddef>
#include <cstdint>

/**
 * \brief Пространство имен с алиасами для типов данных.
 */
namespace leaf::types
{
  using u8 = uint8_t;             ///< 8-bit беззнаковое целое
  using u16 = uint16_t;           ///< 16-bit беззнаковое целое
  using u32 = uint32_t;           ///< 32-bit беззнаковое целое
  using u64 = uint64_t;           ///< 64-bit беззнаковое целое
  using i8 = int8_t;              ///< 8-bit знаковое целое
  using i16 = int16_t;            ///< 16-bit знаковое целое
  using i32 = int32_t;            ///< 32-bit знаковое целое
  using i64 = int64_t;            ///< 64-bit знаковое целое
  using usize = size_t;           ///< Беззнаковое целое размером с указатель на целевой системе
  using isize = intptr_t;         ///< Знаковое целое размером с указатель на целевой системе
  using f32 = float;              ///< 32-bit число с плавающей точкой (`float`)
  using f64 = double;             ///< 64-bit число с плавающей точкой (`double`)
  using f128 = long double;       ///< 128-bit число с плавающей точкой (`long double`)
}
