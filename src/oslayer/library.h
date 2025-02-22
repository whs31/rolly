#pragma once

#include <atomic>
#include <string>
#include <string_view>
#include "base.h"

#ifdef RLL_OS_WINDOWS
#  include <oslayer/win/library.h>
#else
#  include <oslayer/linux/library.h>
#endif

namespace rll::oslayer {
  [[nodiscard]] ___inline___ void*
    lib_resolve(std::atomic<void*>& handle, std::string_view const symbol) {
    return ___os___::lib_resolve(handle, symbol);
  }
}  // namespace rll::oslayer
