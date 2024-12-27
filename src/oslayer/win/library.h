#pragma once

#include "../library.h"

#ifdef ROLLY_OS_WINDOWS

namespace rolly::oslayer::win {
  [[nodiscard]] void* lib_resolve(std::atomic<void*>& handle, std::string_view symbol);
}  // namespace rolly::oslayer::win

#endif
