#include <rolly/dll/plugin_loader.h>

#include <algorithm>
#include <cstdint>
#include "rolly/dll/shared_library.h"
#include <rolly/contracts.h>

namespace rolly::dll {
  plugin_loader::~plugin_loader() {
    for(auto& p : this->plugins_)
      std::ignore = p->quit();
  }

  result<plugin*> plugin_loader::load(std::string_view const name) {
    auto it = this->libraries_.find(name.data());
    if(it == this->libraries_.end()) {
      this->libraries_[name.data()] = shared_library(name);
      it = this->libraries_.find(name.data());
      contracts::postcondition(it != this->libraries_.end());
    }
    auto& lib = it->second;
    auto plugin_ptr = lib.try_load();
    if(not plugin_ptr)
      return error(plugin_ptr.error());
    contracts::invariant(*plugin_ptr != nullptr);
    this->plugins_.emplace_back(*plugin_ptr);
    // NOLINTBEGIN(*-pro-type-reinterpret-cast)
    contracts::postcondition(
      reinterpret_cast<std::uintptr_t>(*plugin_ptr) == reinterpret_cast<std::uintptr_t>(this->plugins_.back().get())
    );
    std::ignore = (**plugin_ptr).init();  // todo
    return *plugin_ptr;
    // NOLINTEND(*-pro-type-reinterpret-cast)
  }

  result<plugin*> plugin_loader::load(std::filesystem::path const& path, std::string_view const name) {
    auto it = this->libraries_.find(name.data());
    if(it == this->libraries_.end()) {
      this->libraries_[name.data()] = shared_library(path, name);
      it = this->libraries_.find(name.data());
      contracts::postcondition(it != this->libraries_.end());
    }
    auto& lib = it->second;
    auto plugin_ptr = lib.try_load();
    if(not plugin_ptr)
      return error(plugin_ptr.error());
    contracts::invariant(*plugin_ptr != nullptr);
    this->plugins_.emplace_back(*plugin_ptr);
    // NOLINTBEGIN(*-pro-type-reinterpret-cast)
    contracts::postcondition(
      reinterpret_cast<std::uintptr_t>(*plugin_ptr) == reinterpret_cast<std::uintptr_t>(this->plugins_.back().get())
    );
    std::ignore = (**plugin_ptr).init();  // todo
    return *plugin_ptr;
    // NOLINTEND(*-pro-type-reinterpret-cast)
  }
}  // namespace rolly::dll
