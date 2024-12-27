#pragma once

#include "../base.h"

#if defined(ROLLY_OS_WINDOWS) || defined(DOXYGEN)
#  include <windows.h>
#  if defined(_MSC_VER)
#    if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) \
      || defined(_M_X64) || defined(_M_AMD64)
#      define _AMD64_
#    elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) \
      || defined(_M_IX86)
#      define _X86_
#    elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#      define _ARM_
#    endif  // __amd64__
#  endif    // _MSC_VER
#  include <Shlobj.h>

namespace rolly::oslayer::win {
  [[nodiscard]] std::filesystem::path known_folder_path(::KNOWNFOLDERID id);
  [[nodiscard]] std::filesystem::path home_dir();
  [[nodiscard]] std::filesystem::path appdata_dir();
  [[nodiscard]] std::filesystem::path local_appdata_dir();
}  // namespace rolly::oslayer::win

#endif  // ROLLY_OS_WINDOWS
