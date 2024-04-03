#pragma once

#include <limits>
#include <leaf/global/types.h>
#include <leaf/global/definitions.h>

/**
 * \brief Пространство имен для функций и классов, связанных с платформами, ОС и архитектурой
 */
namespace leaf::os
{
  enum class OperatingSystem
  {
    Windows,
    Linux,
    Darwin,
    Android,
    Unknown
  };

  [[nodiscard]] constexpr auto system() -> OperatingSystem
  {
    #ifdef _WIN32
    return OperatingSystem::Windows;
    #elif defined(__APPLE__) || defined(__MACH__) || defined(__MACOS__) || defined(__APPLE_CC__)
    return OperatingSystem::Darwin;
    #elif defined(__ANDROID__) || defined(__ANDROID) || defined(__ANDROID_API__)
    return OperatingSystem::Android;
    #elif defined (__linux__) || defined (__linux) || defined(linux) || defined(__gnu_linux__)
    return OperatingSystem::Linux;
    #else
    return OperatingSystem::Unknown;
    #endif
  }

  [[nodiscard]] constexpr auto path_separator() -> char {
    switch(system())
    {
      case OperatingSystem::Windows: return '\\';
      case OperatingSystem::Linux: [[fallthrough]];
      case OperatingSystem::Darwin: [[fallthrough]];
      case OperatingSystem::Android: [[fallthrough]];
      case OperatingSystem::Unknown: return '/';
      default: unreachable();
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
      types::u8 c[sizeof(T)];
    } source{}, dest{};
    source.u = u;
    for(size_t k = 0; k < sizeof(T); k++)
      dest.c[k] = source.c[sizeof(T) - k - 1];
    return dest.u;
  }
}