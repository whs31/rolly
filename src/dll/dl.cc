#include <rolly/dll/dl.h>
#ifdef ROLLY_OS_WINDOWS
#  include "oslayer/win/dll.h"
#else
#  include "oslayer/linux/dll.h"
#endif

namespace rolly::dll {
  std::string library_name(std::string_view name) { return oslayer::___os___::library_name(name); }

  void* load_library(std::string_view name) { return oslayer::___os___::load_library(name); }

  void* load_library(std::string_view name, std::filesystem::path const& path) {
    return oslayer::___os___::load_library(name, path);
  }

  void unload_library(void* handle) { oslayer::___os___::unload_library(handle); }

  void* proc_address(void* handle, std::string_view name) {
    return oslayer::___os___::get_proc_address(handle, name);
  }
}  // namespace rolly::dll
