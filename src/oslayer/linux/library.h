#pragma once

#include "../library.h"

#ifndef ROLLY_OS_WINDOWS
namespace rolly::oslayer::linux {
  [[nodiscard]] void* lib_resolve(std::atomic<void*>& handle, std::string_view symbol);
}  // namespace rolly::oslayer::linux
#endif
