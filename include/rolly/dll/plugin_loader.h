#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "../traits/pin.h"
#include "plugin.h"
#include "shared_library.h"

namespace rolly::dll {
  class plugin_loader : pin {
   public:
    plugin_loader() = default;
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

    [[nodiscard]] std::vector<std::unique_ptr<plugin>> const& plugins() { return this->plugins_; }

    [[nodiscard]] std::unordered_map<std::string, shared_library> const& libraries() { return this->libraries_; }

   private:
    std::unordered_map<std::string, shared_library> libraries_;
    std::vector<std::unique_ptr<plugin>> plugins_;
  };
}  // namespace rolly::dll
