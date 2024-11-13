#pragma once

#include <string>
#include <string_view>
#include "../base.h"

#ifndef ROLLY_OS_WINDOWS

namespace rolly::oslayer::linux {
  [[nodiscard]] std::string library_name(std::string_view name);
  [[nodiscard]] void* load_library(std::string_view name);
  [[nodiscard]] void* load_library(std::string_view name, fs::path const& path);
  void unload_library(void* handle);
  [[nodiscard]] void* get_proc_address(void* handle, std::string_view name);
}  // namespace rolly::oslayer::linux

#endif
