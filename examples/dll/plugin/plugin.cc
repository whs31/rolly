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

  [[nodiscard]] rolly::guid const& uuid() const final { return this->uuid_; }

  ExamplePluginImpl() { fmt::println("Plugin: constructor called"); }

  ~ExamplePluginImpl() final { fmt::println("Plugin: destructor called"); }

  [[nodiscard]] std::string hello() const final { return "Hello"; }

 private:
  rolly::guid uuid_ = rolly::guid("00000000-0000-0000-0100-000123000000");
};

DECLARE_PLUGIN(ExamplePluginImpl)  // NOLINT(*-pro-type-reinterpret-cast)
