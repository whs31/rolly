#include <rll/rtti.h>
#include <rll/global/platform_definitions.h>

#if defined(RLL_COMPILER_GNU) || (defined(RLL_COMPILER_CLANG) && __has_include(<cxxabi.h>))
#  define RLL_USING_CXXABI
#endif

#ifdef RLL_USING_CXXABI
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

namespace rll::rtti {
  std::string demangle(char const* name) {
#ifdef RLL_USING_CXXABI
    auto status = -4;
    unique_ptr<char, void (*)(void*)> const
      res {::abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};
    return status == 0 ? res.get() : name;
#else
    auto constexpr max_name_length = static_cast<::DWORD>(1'024);
    auto undecorated = array<char, max_name_length>();
    return ::UnDecorateSymbolName(name, undecorated.data(), max_name_length, UNDNAME_COMPLETE)
           ? undecorated.data()
           : name;
#endif
  }
}  // namespace rll::rtti
