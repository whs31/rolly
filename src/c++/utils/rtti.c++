#include <leaf/utils/rtti.h>

#include <cstdlib>
#include <memory>
#ifdef __GNUG__
# include <cxxabi.h>
#else 
# include <windows.h>
# include <DbgHelp.h> 
#pragma comment(lib, "dbghelp.lib")
#endif

using std::unique_ptr;
using std::free;

namespace leaf::utils
{
  #ifdef __GNUG__
  auto detail::demangle(const char* name) -> string
  {
    auto status = -4;
    const unique_ptr<char, void(*)(void*)> res {
      abi::__cxa_demangle(name, nullptr, nullptr, &status),
      free
    };
    return status == 0 ? res.get() : name;
  }
  #else
  auto detail::demangle(const char* name) -> string 
  { 
    constexpr max_name_length = DWORD(1024);
    char undecorated[max_name_length];

    if(UnDecorateSymbolName(name, undecorated, max_name_length, UNDNAME_COMPLETE))  
      return std::string(undecorated);
    return std::string(name);
  }
  #endif
}