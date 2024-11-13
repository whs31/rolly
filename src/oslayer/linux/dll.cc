#include "dll.h"

#ifndef ROLLY_OS_WINDOWS
#  include <dlfcn.h>
#  include <fmt/format.h>

namespace rolly::oslayer::linux {
  std::string library_name(std::string_view const name) { return fmt::format("lib{}.so", name); }

  void* load_library(std::string_view name) {
    return dlopen(fmt::format("./{}", library_name(name)).c_str(), RTLD_NOW);
  }

  void* load_library(std::string_view name, fs::path const& path) {
    return dlopen(fmt::format("{}/{}", path.string(), library_name(name)).c_str(), RTLD_NOW);
  }

  void unload_library(void* handle) { dlclose(handle); }

  void* get_proc_address(void* handle, std::string_view name) { return dlsym(handle, name.data()); }
}  // namespace rolly::oslayer::linux

#endif
