#include <simkernel/utils/rtti.h>

#ifdef __GNUG__
# include <cstdlib>
# include <memory>
# include <cxxabi.h>

using std::unique_ptr;
using std::free;

namespace simkernel::utils
{
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
  auto detail::demangle(const char* name) -> string { return name }
#endif
}