#include <fmt/core.h>
#include <fmt/format.h>
#include <plugin-api/api.h>
#include "rolly/dll/detail/macro.h"
#include "rolly/dll/plugin.h"

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
  [[nodiscard]] std::string_view name() const final { return this->meta().name(); }

  [[nodiscard]] rolly::meta::project_meta const& meta() const final { return this->meta_; }

  [[nodiscard]] rolly::guid const& uuid() const final { return this->uuid_; }

  ExamplePluginImpl() { fmt::println("Plugin: constructor called"); }

  ~ExamplePluginImpl() final { fmt::println("Plugin: destructor called"); }

  [[nodiscard]] std::string hello() const final { return "Hello"; }

 private:
  rolly::guid uuid_ = rolly::guid("00000000-0000-0000-0100-000123000000");
  rolly::meta::project_meta meta_ = rolly::meta::project_meta(
    rolly::version("1.0.1"),
    "Example Plugin",
    "io.github.rolly.example_plugin",
    "Radar-MMS"
  );
};

DECLARE_PLUGIN(ExamplePluginImpl)  // NOLINT(*-pro-type-reinterpret-cast)
