#include <rolly/dll/plugin_loader.h>

#include <algorithm>
#include <cstdint>
#include "rolly/dll/shared_library.h"
#include <rolly/contracts.h>
#include <rolly/log.h>

namespace rolly::dll {
  plugin_loader::plugin_loader(std::any init_data)
    : init_data_(std::move(init_data)) {}

  plugin_loader::~plugin_loader() {
    for(auto& p : this->plugins_)
      std::ignore = p->quit();
  }

  result<plugin*> plugin_loader::load(std::string_view const name) {
    auto it = this->libraries_.find(name.data());
    if(it == this->libraries_.end()) {
      this->libraries_[name.data()] = shared_library(std::string(name));
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
    logger().info("rolly::dll: loaded plugin '{}'", name);
    std::ignore = (**plugin_ptr).init(this->init_data_);
    logger().debug("rolly::dll: initialized plugin '{}'", name);
    return *plugin_ptr;
    // NOLINTEND(*-pro-type-reinterpret-cast)
  }

  result<plugin*> plugin_loader::load(std::filesystem::path const& path, std::string_view const name) {
    auto it = this->libraries_.find(name.data());
    if(it == this->libraries_.end()) {
      this->libraries_[name.data()] = shared_library(path, std::string(name));
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
    logger().info("rolly::dll: loaded plugin '{}'", name);
    std::ignore = (**plugin_ptr).init(this->init_data_);
    logger().debug("rolly::dll: initialized plugin '{}'", name);
    return *plugin_ptr;
    // NOLINTEND(*-pro-type-reinterpret-cast)
  }

  std::string_view plugin_loader::native_extension() noexcept {
#ifdef ROLLY_OS_WINDOWS
    return "dll";
#else
    return "so";
#endif
  }

  result<> plugin_loader::load_all(std::filesystem::path const& path, std::string_view const extension) {
    logger().debug("rolly::dll: searching for plugins in '{}'", path.generic_string());
    auto found_any = false;
    for(auto const& entry : std::filesystem::directory_iterator(path)) {
      if(entry.path().extension() != fmt::format(".{}", extension) and entry.path().extension() != extension)
        continue;
      found_any = true;
      logger().trace("rolly::dll: found plugin candidate '{}'", entry.path().generic_string());
      auto name = entry.path().filename().replace_extension("").filename().string();
      if(name.starts_with("lib"))
        name = name.substr(3);
      this->load(path, name).map_error([](auto const& err) { logger().error("rolly::dll: {}", err); });
    }
    if(not found_any)
      return error("no plugins found in '{}'", path.generic_string());
    return ok();
  }

  plugin* plugin_loader::query_raw(std::function<bool(plugin const&)> const& predicate) const {
    for(auto const& p : this->plugins_) {
      if(predicate(*p))
        return p.get();
    }
    return nullptr;
  }
}  // namespace rolly::dll
