#include "dirs.h"

#include <cstdlib>

#ifndef ROLLY_OS_WINDOWS

namespace rolly::oslayer::linux {
  fs::path home_dir() {
    auto* const home_c = std::getenv("HOME");
    if(not home_c)
      throw std::runtime_error("failed to get home directory: $HOME is not set");
    return {home_c};
  }

  fs::path xdg_runtime_dir() {
    auto* const xdg_runtime_dir_c = std::getenv("XDG_RUNTIME_DIR");
    if(not xdg_runtime_dir_c)
      throw std::runtime_error("failed to get XDG_RUNTIME_DIR: $XDG_RUNTIME_DIR is not set");
    return {xdg_runtime_dir_c};
  }
}  // namespace rolly::oslayer::linux

#endif
