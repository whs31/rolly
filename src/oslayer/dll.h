#pragma once

#include "base.h"
#ifdef ROLLY_OS_WINDOWS
#  include "win/dll.h"
#else
#  include "linux/dll.h"
#endif

namespace rolly::oslayer {
  [[nodiscard]] inline std::string library_name(std::string_view name) { return ___os___::library_name(name); }

  [[nodiscard]] inline void* load_library(std::string_view name) { return ___os___::load_library(name); }

  [[nodiscard]] inline void* load_library(std::string_view name, fs::path const& path) {
    return ___os___::load_library(name, path);
  }

  inline void unload_library(void* handle) { ___os___::unload_library(handle); }

  [[nodiscard]] inline void* proc_address(void* handle, std::string_view name) {
    return ___os___::get_proc_address(handle, name);
  }
}  // namespace rolly::oslayer
