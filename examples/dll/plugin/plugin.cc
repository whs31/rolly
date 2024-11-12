#include <fmt/core.h>
#include <fmt/format.h>
#include <plugin-api/api.h>
#include "rolly/dll/detail/macro.h"

class ExamplePluginImpl final : public ExamplePlugin {
 private:
  [[nodiscard]] bool init(std::any& init_interface) final {
    fmt::println("Plugin: plugin loaded");
    // cast any to string
    auto const s = std::any_cast<std::string const&>(init_interface);
    fmt::println("Plugin: init called with {}", s);
    return true;
  }

  [[nodiscard]] bool quit() final {
    fmt::println("Plugin: plugin unloaded");
    return true;
  }

 public:
  [[nodiscard]] std::string_view name() const final { return "example_plugin_impl"; }

  ExamplePluginImpl() { fmt::println("Plugin: constructor called"); }

  ~ExamplePluginImpl() final { fmt::println("Plugin: destructor called"); }

  [[nodiscard]] std::string hello() const final { return "Hello"; }
};

DECLARE_PLUGIN(ExamplePluginImpl)  // NOLINT(*-pro-type-reinterpret-cast)
