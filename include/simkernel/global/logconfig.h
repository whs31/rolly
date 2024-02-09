#pragma once

#include <string>
#include <simkernel/global/types.h>

/// \brief Пространство имен для классов и функций, связанных с логгером
namespace simkernel::log
{
  using std::string;
  using simkernel::types::usize;

  struct LogFileConfiguration
  {
    [[nodiscard]] auto initialize() -> bool;

    string filename;
    usize max_size;
    usize max_files;
  };
}