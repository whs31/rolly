#pragma once

#include <string>
#include <filesystem>
#include <lf/leaf.h>

namespace lf::directories
{
  /**
   * \brief Class for getting location of system directories for a specific application.
   * \details ProjectDirs computes the location of cache, config or data directories for a specific application,
   * which are derived from the standard directories and the name of the project/organization.
   *
   * \details
   * For example if user named <i>Alice</i>, the following code:
   * \code {.cpp}
   * using directories::ProjectDirs;
   * auto dirs = ProjectDirs::from("com", "Foo Corp", "Bar App");
   * if(dirs.has_value())
   *   std::cout << dirs.config_dir() << std::endl;
   * \endcode
   * will produce the following output:
   * \code
   * Linux:   /home/alice/.config/barapp
   * Windows: C:\Users\Alice\AppData\Roaming\Foo Corp\Bar App
   * MacOS:   /Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App
   * \endcode
   */
  class project_dirs
  {
   public:
    explicit project_dirs(
      std::filesystem::path project_path,
      std::filesystem::path cache_dir,
      std::filesystem::path config_dir,
      std::filesystem::path config_local_dir,
      std::filesystem::path data_dir,
      std::filesystem::path data_local_dir,
      std::filesystem::path preference_dir,
      Option<std::filesystem::path> runtime_dir,
      Option<std::filesystem::path> state_dir
    );

    ~project_dirs() = default;

    /**
     * \brief Creates a ProjectDirs class from values describing the project.
     * \details The returned value is optional and dependes on the operating system and is either:
     * - <tt>ProjectDirs</tt>, containing project directory path based on the state of the system's paths at the time of the call, or
     * - <tt>nullopt</tt>, if no valid home directory could be retrieved from the operating system.
     * \param qualifier The reverse domain name notation of the application, excluding the organization or application name itself.
     * Example values: <tt>"com.example"</tt>, <tt>"org"</tt>, <tt>"uk.co"</tt>, <tt>"io"</tt>, <tt>""</tt>.
     * \param organization The name of the organization that develops this application, or for which the application is developed.
     * Example values: <tt>"Foo Corp"</tt>, <tt>"Bar Inc."</tt>, <tt>""</tt>.
     * \param application The name of the application itself.
     * Example values: <tt>"Bar App"</tt>, <tt>"ExampleProgram"</tt>.
     * \return An optional ProjectDirs class, or <tt>nullopt</tt> if no valid home directory could be retrieved from the operating system.
     */
    [[nodiscard]] static auto from(std::string_view qualifier, std::string_view organization, std::string_view application) -> Result<project_dirs>;

    /**
     * \brief Returns the project path fragment used to compute the project's cache/config/data directories.
     * \details The value is derived from the ProjectDirs::from call and is platform-dependent.
     * \return The project path.
     */
    [[nodiscard]] auto project_path() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's cache directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_CACHE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.cache/<tt>project_path</tt>, for example: <tt>/home/alice/.cache/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME/Library/Caches</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Caches/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\cache, for example: <tt>C:\\Users\\Alice\\AppData\\Local\\Foo Corp\\Bar App\\cache</tt>
     * \return The path to the project's cache directory.
     */
    [[nodiscard]] auto cache_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's config directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%APPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
     * \return The path to the project's config directory.
     */
    [[nodiscard]] auto config_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's config_local directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
     * \return The path to the project's config_local directory.
     */
    [[nodiscard]] auto config_local_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's data directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt>
     * \return The path to the project's data directory.
     */
    [[nodiscard]] auto data_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's data_local directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt>
     * \return The path to the project's data_local directory.
     */
    [[nodiscard]] auto data_local_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's preference directory.
     * \details Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
     * - <b>MacOS</b>: <tt>$HOME</tt>/Library/Preferences/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Preferences/com.Foo-Corp.Bar-App</tt>
     * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
     * \return
     */
    [[nodiscard]] auto preference_dir() const -> std::filesystem::path const&;

    /**
     * \brief Returns the path to the project's runtime directory.
     * \details
     * The runtime directory contains transient, non-essential data (like sockets or named pipes) that
     * is expected to be cleared when the user's session ends.<br>
     * Values on different platforms:
     * - <b>Linux</b>: <tt>XDG_RUNTIME_DIR</tt>/<tt>project_path</tt>, for example: <tt>/run/user/1001/barapp</tt>
     * - <b>MacOS</b>: -
     * - <b>Windows</b>: -
     * \note Only for Linux.
     * \return The path to the project's runtime directory or \c nullopt if it is not available.
     */
    [[nodiscard]] auto runtime_dir() const -> Option<std::filesystem::path> const&;

    /**
     * \brief Returns the path to the project's state directory.
     * \details
     * The state directory contains data that should be retained between sessions (unlike the runtime directory),
     * but may not be important/portable enough to be synchronized across machines (unlike the config/preferences/data directories).<br>
     * Values on different platforms:
     * - <b>Linux</b>: <tt>$XDG_STATE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/state/<tt>project_path</tt>, for example: <tt>/home/alice/.local/state/barapp</tt>
     * - <b>MacOS</b>: -
     * - <b>Windows</b>: -
     * \note Only for Linux.
     * \return The path to the project's state directory or \c nullopt if it is not available.
     */
    [[nodiscard]] auto state_dir() const -> Option<std::filesystem::path> const&;

   private:
    std::filesystem::path m_project_path;

    std::filesystem::path m_cache_dir;
    std::filesystem::path m_config_dir;
    std::filesystem::path m_config_local_dir;
    std::filesystem::path m_data_dir;
    std::filesystem::path m_data_local_dir;
    std::filesystem::path m_preference_dir;
    Option<std::filesystem::path> m_runtime_dir;
    Option<std::filesystem::path> m_state_dir;
  };
}
