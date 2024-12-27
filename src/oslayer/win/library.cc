#include <oslayer/win/library.h>

#ifdef ROLLY_OS_WINDOWS
#  include <windows.h>

namespace rolly::oslayer::win {
  void* lib_resolve(std::atomic<void*>& handle, std::string_view const symbol) {
    auto const hm = static_cast<::HMODULE>(handle.load(std::memory_order_acquire));
    if(hm == nullptr)
      return nullptr;
    return reinterpret_cast<void*>(::GetProcAddress(hm, symbol.data()));
  }
}  // namespace rolly::oslayer::win
#endif
