#pragma once

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
      default: $unreachable();
    }
  }

}