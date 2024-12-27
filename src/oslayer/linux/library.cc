#include <oslayer/linux/library.h>

#ifndef ROLLY_OS_WINDOWS
#  include <dlfcn.h>

namespace rolly::oslayer::linux {
  void* lib_resolve(std::atomic<void*>& handle, std::string_view const symbol) {
    return ::dlsym(handle.load(std::memory_order_acquire), symbol.data());
  }
}  // namespace rolly::oslayer::linux
#endif
