#include <lf/detail/corefn.h>
#include <lf/detail/os.h>

#include <cstdlib>
#include <memory>

#if defined(LF_COMPILER_GNU) || defined(LF_COMPILER_CLANG)
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
  #if defined(LF_COMPILER_GNU) || defined(LF_COMPILER_CLANG)
  auto demangle(const char* name) -> std::string
  {
    auto status = -4;
    const unique_ptr<char, void (*)(void*)> res {
      abi::__cxa_demangle(name, nullptr, nullptr, &status), free};
    return status == 0 ? res.get() : name;
  }
  #else
  auto demangle(const char* name) -> std::string
  {
    constexpr auto max_name_length = DWORD(1024);
    char undecorated[max_name_length];

    if(UnDecorateSymbolName(name, undecorated, max_name_length, UNDNAME_COMPLETE))
      return std::string(undecorated);
    return std::string(name);
  }
  #endif
}