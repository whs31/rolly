#include <fmt/core.h>
#include <rolly/dll/plugin_loader.h>
#include <plugin-api/api.h>
#include <rolly/log.h>

int main() {
  spdlog::default_logger()->set_level(spdlog::level::trace);
  auto init_data = std::make_any<std::string>("Hi from std::any!");
  auto loader = rolly::dll::plugin_loader(init_data);
  auto res = loader.load_all(std::filesystem::current_path().parent_path() / "plugin");
  if(not res) {
    fmt::println(stderr, "Failed to load plugin: {}", res.error());
    return -1;
  }
  auto plugin = loader.query_raw([](auto const& p) { return p.name() == "Example Plugin"; });
  fmt::println(
    "Loaded plugin: {} ({}), soname: {}, description: {}",
    plugin->name(),
    plugin->uuid(),
    plugin->soname(),
    plugin->description()
  );
  auto const plugin_ = loader.query_interface<ExamplePlugin>("Example Plugin");
  if(not plugin_) {
    fmt::println(stderr, "Failed to load plugin: plugin is nullptr");
    return -1;
  }
  fmt::println("{}", (*plugin_).get().hello());
  fmt::println("Success!");
  return 0;
}
