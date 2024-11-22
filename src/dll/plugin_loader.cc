#include <rolly/dll/plugin_loader.h>

#include <algorithm>
#include <cstdint>
#include "rolly/dll/shared_library.h"
#include <rolly/contracts.h>
#include <rolly/log.h>
#include <rolly/string_util.h>

namespace rolly::dll {
  plugin_loader::plugin_loader(std::any init_data)
    : init_data_(std::move(init_data)) {}

  plugin_loader::~plugin_loader() {
    for(auto& p : this->plugins_)
      std::ignore = p->quit();
  }

  bool plugin_loader::is_loaded(std::string_view name) const {
    return std::any_of(this->plugins_.cbegin(), this->plugins_.cend(), [&](auto const& p) {
      return p->name() == name;
    });
  }

  bool plugin_loader::is_loaded(guid const& uuid) const {
    return std::any_of(this->plugins_.cbegin(), this->plugins_.cend(), [&](auto const& p) {
      return p->uuid() == uuid;
    });
  }

  usize plugin_loader::loaded_count() const { return this->plugins_.size(); }

  plugin* plugin_loader::operator[](std::string_view name) const {
    auto it = std::find_if(this->plugins_.cbegin(), this->plugins_.cend(), [&](auto const& p) {
      return p->name() == name;
    });
    return it != this->plugins_.end() ? it->get() : nullptr;
  }

  plugin* plugin_loader::operator[](guid const& uuid) const {
    auto it = std::find_if(this->plugins_.cbegin(), this->plugins_.cend(), [&](auto const& p) {
      return p->uuid() == uuid;
    });
    return it != this->plugins_.end() ? it->get() : nullptr;
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

    if(this->query_raw([&plugin_ptr](auto const& p) { return p.uuid() == (*plugin_ptr)->uuid(); })
       != nullptr)
      return error("plugin with uuid '{}' is already loaded", (*plugin_ptr)->uuid());

    auto _ = std::unique_ptr<plugin>(*plugin_ptr);
    _->soname_ = lib.soname();
    lib.loaded_ = true;
    this->plugins_.push_back(std::move(_));
    // NOLINTBEGIN(*-pro-type-reinterpret-cast)
    contracts::postcondition(
      reinterpret_cast<std::uintptr_t>(*plugin_ptr)
      == reinterpret_cast<std::uintptr_t>(this->plugins_.back().get())
    );
    logger().info("rolly::dll: loaded plugin '{}'", name);
    std::ignore = (**plugin_ptr).init(this->init_data_);
    logger().debug("rolly::dll: initialized plugin '{}'", name);
    return *plugin_ptr;
    // NOLINTEND(*-pro-type-reinterpret-cast)
  }

  result<plugin*>
    plugin_loader::load(std::filesystem::path const& path, std::string_view const name) {
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

    if(this->query_raw([&plugin_ptr](auto const& p) { return p.uuid() == (*plugin_ptr)->uuid(); })
       != nullptr)
      return error("plugin with uuid '{}' is already loaded", (*plugin_ptr)->uuid());

    auto _ = std::unique_ptr<plugin>(*plugin_ptr);
    _->soname_ = lib.soname();
    lib.loaded_ = true;
    this->plugins_.push_back(std::move(_));
    // NOLINTBEGIN(*-pro-type-reinterpret-cast)
    contracts::postcondition(
      reinterpret_cast<std::uintptr_t>(*plugin_ptr)
      == reinterpret_cast<std::uintptr_t>(this->plugins_.back().get())
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

  result<>
    plugin_loader::load_all(std::filesystem::path const& path, std::string_view const extension) {
    logger().debug("rolly::dll: searching for plugins in '{}'", path.generic_string());
    auto found_any = false;
    for(auto const& entry : std::filesystem::directory_iterator(path)) {
      if(entry.path().extension() != fmt::format(".{}", extension)
         and entry.path().extension() != extension)
        continue;
      found_any = true;
      logger().trace("rolly::dll: found plugin candidate '{}'", entry.path().generic_string());
      auto name = entry.path().filename().replace_extension("").filename().string();
      if(starts_with(name, "lib"))
        name = name.substr(3);
      this->load(path, name).map_error([](auto const& err) {
        logger().error("rolly::dll: {}", err);
      });
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

  result<> plugin_loader::unload(std::string_view name) {
    auto it = std::find_if(this->plugins_.cbegin(), this->plugins_.cend(), [&](auto const& p) {
      return p->name() == name;
    });
    if(it == this->plugins_.cend())
      return error("plugin '{}' not found", name);
    std::ignore = it->get()->quit();
    this->plugins_.erase(it);
    auto it_ = this->libraries_.find(name.data());
    if(it_ != this->libraries_.end())
      it_->second.loaded_ = false;
    return ok();
  }
}  // namespace rolly::dll
