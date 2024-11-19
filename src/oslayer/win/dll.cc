#include "dll.h"

#ifdef ROLLY_OS_WINDOWS
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  include <fmt/format.h>

namespace rolly::oslayer::win {
  std::string library_name(std::string_view const name) { return std::string(name); }

  void* load_library(std::string_view name) {
    return reinterpret_cast<void*>(LoadLibraryA(fmt::format(".\\{}", library_name(name)).c_str()));
  }

  void* load_library(std::string_view name, fs::path const& path) {
    return reinterpret_cast<void*>(
      LoadLibraryA(fmt::format("{}\\{}", path.string(), library_name(name)).c_str())
    );
  }

  void unload_library(void* handle) { FreeLibrary(HMODULE(handle)); }

  void* get_proc_address(void* handle, std::string_view name) {
    return reinterpret_cast<void*>(GetProcAddress(HMODULE(handle), LPCSTR(name.data())));
  }
}  // namespace rolly::oslayer::win

#endif
