#pragma once

#include <atomic>
#include <string>
#include <string_view>
#include "base.h"

#ifdef ROLLY_OS_WINDOWS
#  include <oslayer/win/library.h>
#else
#  include <oslayer/linux/library.h>
#endif

namespace rolly::oslayer {
  [[nodiscard]] ___inline___ void*
    lib_resolve(std::atomic<void*>& handle, std::string_view const symbol) {
    return ___os___::lib_resolve(handle, symbol);
  }
}  // namespace rolly::oslayer
