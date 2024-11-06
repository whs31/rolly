#include <rolly/rtti.h>
#include <rolly/global/platform_definitions.h>

#if defined(ROLLY_COMPILER_GNU) || (defined(ROLLY_COMPILER_CLANG) && __has_include(<cxxabi.h>))
#  define ROLLY_USING_CXXABI
#endif

#ifdef ROLLY_USING_CXXABI
#  include <cstdlib>
#  include <memory>
#  include <cxxabi.h>

using std::unique_ptr;
#else
#  include <array>
#  include <windows.h>
#  include <DbgHelp.h>
#  pragma comment(lib, "dbghelp.lib")

using std::array;
#endif

namespace rolly::rtti {
  std::string demangle(char const* name) {
#ifdef ROLLY_USING_CXXABI
    auto status = -4;
    unique_ptr<char, void (*)(void*)> const res {::abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};
    return status == 0 ? res.get() : name;
#else
    auto constexpr max_name_length = static_cast<::DWORD>(1'024);
    auto undecorated = array<char, max_name_length>();
    return ::UnDecorateSymbolName(name, undecorated.data(), max_name_length, UNDNAME_COMPLETE) ? undecorated.data()
                                                                                               : name;
#endif
  }
}  // namespace rolly::rtti
