#pragma once

#include <any>
#include <string>
#include "detail/macro.h"

namespace rolly {  // NOLINT

  /**
   * @brief Dynamic libraries and plugins.
   */
  namespace dll {}
}  // namespace rolly

//* <!-- @image html ./calling_graph.svg width=35% -->
namespace rolly::dll {
  /**
   * @brief Plugin interface.
   * @details Plugin interface is a base class for your extension plugins.
   *
   * Calling graph during plugin loading/unloading routine:
   * @image html include/rolly/dll/detail/calling_graph.png
   *
   * @paragraph plugin_usage Usage basics
   * To implement the plugin system, you need to implement the following entities:
   * <ul>
   *   <li>Plugin interface, which will derive from the @ref rolly::dll::plugin class . This interface will be used
   *   by both the plugin loader and the plugin. Best practice is to put it into a separate header file or the
   *   <b>INTERFACE</b> library in CMake.</li>
   *   <li>Plugin implementation, which will derive from the interface, presented above. This code will not be linked
   *   anywhere, but it is required to compile it as a <b>SHARED</b> library, which links to the <i>INTERFACE</i>
   *   library described above.</li>
   *   <li>Application with a plugin system (@ref rolly::dll::plugin_loader class). Must be compiled as the
   *   <b>EXECUTABLE</b>, which links to the </i>INTERFACE</i> library described above.</li>
   * </ul>
   *
   * @paragraph plugin_example_plugin_interface Example plugin interface
   * Here we declare an <b>INTERFACE</b> CMake library with a single header file, which contains your plugin interface.
   *
   * In the header file we simply derive from the @ref rolly::dll::plugin without overriding any functions.
   *
   * <b>CMakeLists.txt</b>
   * @code {.cmake}
   * cmake_minimum_required(VERSION 3.16)
   *
   * include_guard(GLOBAL)
   *
   * add_library(plugin-interface INTERFACE)
   * target_sources(plugin-interface INTERFACE include/plugin-api/api.h)
   *
   * target_link_libraries(plugin-interface INTERFACE rolly::rolly)
   * target_include_directories(${PROJECT_NAME}
   *    INTERFACE
   *    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
   *    $<INSTALL_INTERFACE:include>
   * )
   * @endcode
   *
   * <b>include/plugin-api/api.h</b>
   * @code {.cpp}
   * #pragma once
   *
   * #include <rolly/dll/plugin.h>
   *
   * class ExamplePlugin : public rolly::dll::plugin {
   *  public:
   *   [[nodiscard]] virtual std::string hello() const = 0; // example function - part of your plugin's public API.
   *                                                        // we will call this function in the application later.
   * };
   * @endcode
   *
   * @paragraph plugin_example_plugin_impl Example plugin implementation
   * Implementation of the plugin interface must be compiled as a <b>SHARED</b> library, which links to the
   * <b>INTERFACE</b> library described above.
   *
   * @warning
   * @ref rolly::dll::plugin_loader expects libraries on Linux to be named with the prefix <tt>lib</tt>, e.g.
   * <tt>libplugin-impl</tt>. In Windows, it expects the same, but without the prefix. By default, CMake with MingW
   * compiles the libraries with the prefix <tt>lib</tt>, so don't forget to set the prefix to <tt>""</tt>., e.g.
   * @code {.cmake}
   * if(WIN32)
   *   set_target_properties(plugin-impl PROPERTIES PREFIX "")
   * endif()
   * @endcode
   *
   * <b>CMakeLists.txt</b>
   * @code {.cmake}
   * cmake_minimum_required(VERSION 3.16)
   *
   * add_library(plugin-impl SHARED plugin.cc)
   * target_link_libraries(plugin-impl PUBLIC plugin-interface rolly::rolly)
   *
   * if(WIN32)
   *   set_target_properties(plugin-impl PROPERTIES PREFIX "")
   * endif()
   * @endcode
   *
   * The plugin implementation is a class, which implements the plugin interface and overrides the
   * @ref rolly::dll::plugin interface methods.
   *
   * @remark
   * Notice the macro <code>DECLARE_PLUGIN(...)</code> in the plugin implementation code.
   * This macro is used to register the plugin implementation with the plugin loader and basically expands to something
   * like:
   * @code {.cpp}
   * extern "C" rolly::dll::plugin* __rolly_identifier_for_calling() {
   *   return reinterpret_cast<rolly::dll::plugin*>(new IMPL_NAME);
   * }
   * @endcode
   *
   * @note Do not forget to fully qualify your implementation class typename when passing it to the
   * <code>DECLARE_PLUGIN(...)</code> macro! For example, if your plugin resides in the <code>example</code> namespace
   * and is named <code>ExamplePluginImpl</code>, qualify it like:
   * @code {.cpp}
   * DECLARE_PLUGIN(example::ExamplePluginImpl)
   * @endcode
   *
   * <b>plugin.cc</b>
   * @code {.cpp}
   * #include <fmt/core.h>
   * #include <fmt/format.h>
   * #include <plugin-api/api.h>
   * #include "rolly/dll/detail/macro.h"
   *
   * // example plugin implementation
   * class ExamplePluginImpl final : public ExamplePlugin {
   *  private:
   *   [[nodiscard]] bool init(std::any& init_interface) final {
   *     fmt::println("Plugin: plugin loaded");
   *     // extract data passed by loader in init method.
   *     // in this case, we expect a string
   *     auto const s = std::any_cast<std::string const&>(init_interface);
   *     fmt::println("Plugin: init called with {}", s);
   *     return true;
   *   }
   *
   *   [[nodiscard]] bool quit() final {
   *     fmt::println("Plugin: plugin unloaded");
   *     return true;
   *   }
   *
   *  public:
   *   [[nodiscard]] std::string_view name() const final { return "example_plugin_impl"; }
   *
   *   ExamplePluginImpl() { fmt::println("Plugin: constructor called"); }
   *
   *   ~ExamplePluginImpl() final { fmt::println("Plugin: destructor called"); }
   *
   *   [[nodiscard]] std::string hello() const final { return "Hello"; }
   * };
   *
   * // notice this macro call. see remarks above for details
   * DECLARE_PLUGIN(ExamplePluginImpl)  // NOLINT(*-pro-type-reinterpret-cast)
   * @endcode
   *
   * @paragraph plugin_example_plugin_app Example application
   * See @ref rolly::dll::plugin_loader documentation for method reference.
   *
   * <b>CMakeLists.txt</b>
   * @code {.cmake}
   * cmake_minimum_required(VERSION 3.16)
   *
   * include_guard(GLOBAL)
   *
   * add_executable(plugin-host main.cc)
   * target_link_libraries(plugin-host PUBLIC plugin-interface rolly::rolly spdlog::spdlog)
   *
   * if (WIN32)
   *   add_custom_command(TARGET plugin-host
   *     POST_BUILD
   *     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:plugin-host> $<TARGET_FILE_DIR:plugin-host>
   *     COMMAND_EXPAND_LISTS
   *   )
   * endif ()
   * @endcode
   *
   * <b>main.cc</b>
   * @code {.cpp}
   * #include <fmt/core.h>
   * #include <rolly/dll/plugin_loader.h>
   * #include <plugin-api/api.h>
   * #include <rolly/log.h>
   *
   * int main() {
   *   spdlog::default_logger()->set_level(spdlog::level::trace);
   *   auto init_data = std::make_any<std::string>("Hi from std::any!");
   *   auto loader = rolly::dll::plugin_loader(init_data);
   *   auto res = loader.load_all(std::filesystem::current_path().parent_path() / "plugin");
   *   if(not res) {
   *     fmt::println(stderr, "Failed to load plugin: {}", res.error());
   *     return -1;
   *   }
   *   auto const plugin = loader.query_interface<ExamplePlugin>("example_plugin_impl");
   *   if(not plugin) {
   *     fmt::println(stderr, "Failed to load plugin: plugin is nullptr");
   *     return -1;
   *   }
   *   fmt::println("{}", (*plugin).get().hello());
   *   fmt::println("Success!");
   *   return 0;
   * }
   * @endcode
   *
   * @paragraph plugin_example_plugin_output Output
   * @code {.shell-session}
   * 17:20:14  .\plugin-host.exe
   * Plugin: constructor called
   * [2024-11-12 17:20:17.008] [info] rolly::dll: loaded plugin 'plugin-impl.dll'
   * Plugin: plugin loaded
   * Plugin: init called with Hi from std::any!
   * Hello
   * Success!
   * Plugin: plugin unloaded
   * Plugin: destructor called
   *
   * 17:20:17  ls
   * D:\dev\radar2\rolly-radar\build\Debug\examples\dll\host
   * Mode                LastWriteTime         Length Name
   * ----                -------------         ------ ----
   * d-----       12.11.2024     14:37                  CMakeFiles
   * -a----       12.11.2024     14:37           1197   cmake_install.cmake
   * -a----       12.11.2024     14:37            317   CTestTestfile.cmake
   * -a----       12.11.2024     15:33        2036629   libfmtd.dll
   * -a----       12.11.2024     15:33           2404   libplugin-host.dll.a
   * -a----       12.11.2024     15:33       14017647   librolly.dll
   * -a----       12.11.2024     15:33       10949254   plugin-host.exe
   * @endcode
   * @see rolly::dll::plugin_loader
   */
  class plugin {
   public:
    plugin() = default;
    plugin(plugin const&) = default;
    plugin(plugin&&) = default;
    plugin& operator=(plugin const&) = default;
    plugin& operator=(plugin&&) = default;
    virtual ~plugin() = default;

    /**
     * @brief Plugin internal interface name.
     * @details This function should return unique interface name.
     *
     * It is recommended to document this function's return value in your plugin documentation.
     *
     * Used in @ref rolly::dll::plugin_loader::query_interface method for iterating over plugins.
     * @return Internal interface name.
     */
    [[nodiscard]] virtual std::string_view name() const = 0;

    /**
     * @brief Called directly after plugin constructor during plugin loading.
     * @param init_interface Data structure passed to plugin from the @ref rolly::dll::plugin_loader class. Can be
     * used to pass configuration data to plugin, such as pointer to Application object.
     * @return True if plugin was successfully loaded.
     */
    [[nodiscard]] virtual bool init(std::any& init_interface) = 0;

    /**
     * @brief Called directly before plugin destructor during plugin unloading.
     * @return True if plugin was successfully unloaded.
     */
    [[nodiscard]] virtual bool quit() = 0;
  };
}  // namespace rolly::dll
