#include <lf/detail/corefn.h>
#include <lf/detail/os.h>

#include <cstdlib>
#include <memory>

#ifdef LF_COMPILER_GNU
# include <cxxabi.h>
#else
# include <windows.h>
# include <DbgHelp.h>
# pragma comment(lib, "dbghelp.lib")
#endif

using std::unique_ptr;
using std::free;

namespace lf::rtti
{
  #ifdef LF_COMPILER_GNU
  auto demangle(const char* name) -> std::string
  {
    auto status = -4;
    const unique_ptr<char, void (*)(void*)> res {
      abi::__cxa_demangle(name, nullptr, nullptr, &status), free};
    return status == 0 ? res.get() : name;
  }
  #else
  auto detail::demangle(const char* name) -> std::string
  {
    constexpr auto max_name_length = DWORD(1024);
    char undecorated[max_name_length];

    if(UnDecorateSymbolName(name, undecorated, max_name_length, UNDNAME_COMPLETE))
      return std::string(undecorated);
    return std::string(name);
  }
  #endif
}