#pragma once

#include <string>
#include <string_view>
#include "../global/export.h"
#include "../global/platform_definitions.h"
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly::dll {
#ifndef DOXYGEN
  ___rolly_api___
#endif
    [[nodiscard]] std::string
    library_name(std::string_view name);

#ifndef DOXYGEN
  ___rolly_api___
#endif
    [[nodiscard]] void*
    load_library(std::string_view name);

#ifndef DOXYGEN
  ___rolly_api___
#endif
    [[nodiscard]] void*
    load_library(std::string_view name, std::filesystem::path const& path);

#ifndef DOXYGEN
  ___rolly_api___
#endif
    void
    unload_library(void* handle);

#ifndef DOXYGEN
  ___rolly_api___
#endif
    [[nodiscard]] void*
    proc_address(void* handle, std::string_view name);
}  // namespace rolly::dll
