#include <fmt/core.h>
#include <rolly/dll/plugin_loader.h>
#include <plugin-api/api.h>

int main() {
  auto loader = rolly::dll::plugin_loader();
  auto res = loader.load("plugin-impl");
  if(not res) {
    fmt::println(stderr, "Failed to load plugin: {}", res.error());
    return -1;
  }
  auto const* plugin = dynamic_cast<ExamplePlugin*>(*res);
  if(plugin == nullptr) {
    fmt::println(stderr, "Failed to load plugin: plugin is nullptr");
    return -1;
  }
  fmt::println("{}", plugin->hello());
  fmt::println("Success!");
  return 0;
}
