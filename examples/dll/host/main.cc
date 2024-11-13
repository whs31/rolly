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
  fmt::println(
    "Loaded plugin: {} ({})",
    loader.query_raw([](auto const& p) { return p.name() == "example_plugin_impl"; })->name(),
    loader.query_raw([](auto const& p) { return p.name() == "example_plugin_impl"; })->uuid()
  );
  auto const plugin = loader.query_interface<ExamplePlugin>("example_plugin_impl");
  if(not plugin) {
    fmt::println(stderr, "Failed to load plugin: plugin is nullptr");
    return -1;
  }
  fmt::println("{}", (*plugin).get().hello());
  fmt::println("Success!");
  return 0;
}
