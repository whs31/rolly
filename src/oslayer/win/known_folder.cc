#include "known_folder.h"

#ifdef RLL_OS_WINDOWS
#  include <array>
#  include <system_error>
#  include <windows.h>
#  include <winerror.h>

namespace rll::oslayer::win {
  std::filesystem::path known_folder_path(::KNOWNFOLDERID id) {
    auto* buf = ::PWSTR();
    auto const result = ::SHGetKnownFolderPath(id, 0, nullptr, &buf);
    if(FAILED(result)) {
      ::CoTaskMemFree(buf);
      throw std::system_error(std::make_error_code(static_cast<std::errc>(result)));
    }
    auto const path = std::filesystem::path(buf);
    ::CoTaskMemFree(buf);
    return path;
  }

  std::filesystem::path home_dir() {
    auto buf = std::array<wchar_t, MAX_PATH>();
    auto const result =
      ::SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, SHGFP_TYPE_CURRENT, buf.data());
    if(FAILED(result))
      throw std::system_error(std::make_error_code(static_cast<std::errc>(result)));
    return {buf.data()};
  }

  std::filesystem::path appdata_dir() { return known_folder_path(::FOLDERID_RoamingAppData); }

  std::filesystem::path local_appdata_dir() { return known_folder_path(::FOLDERID_LocalAppData); }
}  // namespace rll::oslayer::win

#endif  // RLL_OS_WINDOWS
