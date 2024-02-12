#pragma once

#include <limits>
#include <simkernel/global/types.h>
#include <simkernel/global/definitions.h>

/**
 * \brief Пространство имен для функций и классов, связанных с платформами, ОС и архитектурой
 */
namespace simkernel::os
{
  enum class OperatingSystem
  {
    Windows,
    Linux
  };

  [[nodiscard]] constexpr auto system() -> OperatingSystem
  {
    #ifdef _WIN32
    return OperatingSystem::Windows;
    #else
    return OperatingSystem::Linux;
    #endif
  }

  [[nodiscard]] constexpr auto path_separator() -> char {
    switch(system())
    {
      case OperatingSystem::Windows: return '\\';
      case OperatingSystem::Linux: return '/';
      default: simkernel::unreachable();
    }
  }

  using std::numeric_limits;

  template<typename T>
  auto swap_endian(T u) -> T
  {
    static_assert(numeric_limits<unsigned char>::digits == 8, "unsigned char is not 8 bits");
    union
    {
      T u;
      simkernel::types::u8 c[sizeof(T)];
    } source{}, dest{};
    source.u = u;
    for(size_t k = 0; k < sizeof(T); k++)
      dest.c[k] = source.c[sizeof(T) - k - 1];
    return dest.u;
  }
}