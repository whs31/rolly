#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "../traits/pin.h"
#include "plugin.h"
#include "shared_library.h"

namespace rolly::dll {
  class
#ifndef DOXYGEN_GENERATING_OUTPUT
    ___rolly_api___
#endif
    /**
     * @brief Dynamic loader for @ref rolly::dll::plugin libraries.
     * @details See @ref rolly::dll::plugin class reference for detailed usage examples and guidelines.
     */
      plugin_loader : pin {

   public:
    /**
     * @brief Constructs new plugin_loader instance.
     * @param init_data Data, which will be passed into plugins during initialization.
     */
    explicit plugin_loader(std::any init_data);
    plugin_loader(plugin_loader const&) = delete;
    plugin_loader(plugin_loader&&) = delete;
    plugin_loader& operator=(plugin_loader const&) = delete;
    plugin_loader& operator=(plugin_loader&&) = delete;

    virtual ~plugin_loader();

    /**
     * @brief Loads a plugin by name.
     * @param name Name of the plugin to load.
     * @return Pointer to the loaded plugin or error if loading fails.
     */
    [[nodiscard]] result<plugin*> load(std::string_view name);

    /**
     * @brief Loads a plugin by name and path.
     * @param path Path to directory where plugin file is located.
     * @param name Name of the plugin to load.
     * @return Pointer to the loaded plugin or error if loading fails.
     */
    [[nodiscard]] result<plugin*> load(std::filesystem::path const& path, std::string_view name);

    /**
     * @brief Loads all possible plugins in a directory.
     * @param path Path to directory where plugin files are located.
     * @return Returns error, if no plugins are located in provided directory.
     */
    [[nodiscard]] result<>
      load_all(std::filesystem::path const& path, std::string_view extension = plugin_loader::native_extension());

    /**
     * @brief Returns native extension for dynamically loaded libraries on this platform.
     * @details Returns <code>dll</code>, <code>so</code> on Linux.
     * @return Native extension without a prefix dot.
     */
    [[nodiscard]] static std::string_view native_extension() noexcept;

    /**
     * @brief Returns the internal vector of loaded plugins.
     * @return Internal vector of loaded plugins.
     */
    [[nodiscard]] std::vector<std::unique_ptr<plugin>> const& plugins() const { return this->plugins_; }

    /**
     * @brief Returns a mutable reference to the internal vector of loaded plugins.
     * @return Mutable reference to the internal vector of loaded plugins.
     */
    [[nodiscard]] std::vector<std::unique_ptr<plugin>>& plugins() { return this->plugins_; }

    /**
     * @brief Returns the internal map of loaded libraries.
     * @return Internal map of loaded libraries.
     */
    [[nodiscard]] std::unordered_map<std::string, shared_library> const& libraries() const { return this->libraries_; }

    /**
     * @brief Returns a mutable reference to the internal map of loaded libraries.
     * @return Mutable reference to the internal map of loaded libraries.
     */
    [[nodiscard]] std::unordered_map<std::string, shared_library>& libraries() { return this->libraries_; }

    /**
     * @brief Queries plugin interface by it's type via RTTI.
     * @details Essentially iterates over all loaded plugins and tries to <code>dynamic_cast</code> their pointer to
     * the requested interface.
     * @warning This function can be slow, especially when many plugins are loaded. Much better solution will be to
     * query by internal plugin interface name using method @ref query_interface
     * @tparam T Interface type.
     * @return Reference to the retrieved interface or none when no such interface is loaded.
     */
    template <typename T>
    [[nodiscard]] std::optional<std::reference_wrapper<T>> query_interface_by_type() const {
      for(auto const& p : this->plugins())
        try {
          return std::ref(*dynamic_cast<T*>(p.get()));
        } catch(...) {  // NOLINT(*-empty-catch)
        }
      return none;
    }

    /**
     * @brief Queries plugin interface by it's internal name.
     * @tparam T Interface type.
     * @param interface_name Internal plugin interface name. See @ref rolly::dll::plugin::name
     * @return Reference to the retrieved interface or none when no such interface is loaded.
     */
    template <typename T>
    [[nodiscard]] std::optional<std::reference_wrapper<T>> query_interface(std::string_view interface_name) const {
      for(auto const& p : this->plugins()) {
        if(p->name() == interface_name)
          try {
            return std::ref(*dynamic_cast<T*>(p.get()));
          } catch(...) {
            return none;
          }
        break;
      }
      return none;
    }

   private:
    std::any init_data_;
    std::unordered_map<std::string, shared_library> libraries_;
    std::vector<std::unique_ptr<plugin>> plugins_;
  };
}  // namespace rolly::dll
