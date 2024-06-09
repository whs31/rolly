#pragma once

#include <string>
#include <filesystem>
#include <floppy/detail/types.h>
#include <floppy/detail/export.h>

/// \brief Filesystem namespace.
namespace floppy::filesystem
{
  /**
   * \brief Class for getting location of system directories for a specific application.
   * \details ProjectDirs computes the location of cache, config or data directories for a specific application,
   * which are derived from the standard directories and the name of the project/organization.
   *
   * \details
   * For example if user named <i>Alice</i>, the following code:
   * \code {.cpp}
   * using floppy::filesystem::application_dirs;
   * auto dirs = application_dirs("com", "Foo Corp", "Bar App");
   * std::cout << dirs.config_dir() << std::endl;
   * \endcode
   * will produce the following output:
   * \code {.ansi}
   * Linux:   '/home/alice/.config/barapp'
   * Windows: 'C:\Users\Alice\AppData\Roaming\Foo Corp\Bar App'
   * MacOS:   '/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App'
   * \endcode
   */
  class FLOPPY_EXPORT application_dirs
  {
   public:
      /// \brief Supported directory types.
      enum class dir
      {
        cache,        ///< Cache directory
        config,       ///< Config directory
        config_local, ///< Local config directory
        data,         ///< Data directory
        data_local,   ///< Local data directory
        preferences,  ///< Preferences directory
        runtime,      ///< Runtime directory. Can be unavailable on some platforms
        state         ///< State directory. Can be unavailable on some platforms
      };

      /**
       * \brief Creates an application_dirs class from values describing the project.
       * \note Constuctor can fail if no valid home directory could be retrieved from the operating system.
       * \note Directories will be created if they do not exist.
       * \param qualifier The reverse domain name notation of the application, excluding the organization or application name itself.
       * Example values: <tt>"com.example"</tt>, <tt>"org"</tt>, <tt>"uk.co"</tt>, <tt>"io"</tt>, <tt>""</tt>.
       * \param organization The name of the organization that develops this application, or for which the application is developed.
       * Example values: <tt>"Foo Corp"</tt>, <tt>"Bar Inc."</tt>, <tt>""</tt>.
       * \param application The name of the application itself.
       * Example values: <tt>"Bar App"</tt>, <tt>"ExampleProgram"</tt>.
       * \throws std::runtime_error if no valid home directory could be retrieved from the operating system.
       */
     explicit application_dirs(
       std::string_view qualifier,
       std::string_view organization,
       std::string_view application
     ) noexcept(false);

     ~application_dirs() = default;

     /// \brief Returns the path to the directory.
     /// \param directory_type The directory type.
     /// \return The path to the directory.
     /// \throws std::system_error If the directory does not exist on this platform.
     /// \note Use explicit functions such as \ref config_dir() to avoid this exception.
     /// \see operator[]
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto get(dir directory_type) const noexcept(false) -> std::filesystem::path;
#else
     [[nodiscard]] std::filesystem::path get(dir directory_type) const noexcept(false);
#endif

     /// \brief Creates the directories if they do not exist.
#ifndef DOXYGEN_GENERATING_OUTPUT
     auto create() const -> void;
#else
     void create();
#endif

     /// \brief Removes the directories from the filesystem.
#ifndef DOXYGEN_GENERATING_OUTPUT
     auto remove() const -> void;
#else
     void remove();
#endif

     /**
      * \brief Returns the project path fragment used to compute the project's cache/config/data directories.
      * \details The value is derived from the ProjectDirs::from call and is platform-dependent.
      * \return The project path.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto project_path() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path project_path() const;
#endif

     /**
      * \brief Returns the path to the project's cache directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_CACHE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.cache/<tt>project_path</tt>, for example: <tt>/home/alice/.cache/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME/Library/Caches</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Caches/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\cache, for example: <tt>C:\\Users\\Alice\\AppData\\Local\\Foo Corp\\Bar App\\cache</tt>
      * \return The path to the project's cache directory.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto cache_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path cache_dir() const;
#endif

     /**
      * \brief Returns the path to the project's config directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%APPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
      * \return The path to the project's config directory.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto config_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path config_dir() const;
#endif

     /**
      * \brief Returns the path to the project's config_local directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
      * \return The path to the project's config_local directory.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto config_local_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path config_local_dir() const;
#endif

     /**
      * \brief Returns the path to the project's data directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt>
      * \return The path to the project's data directory.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto data_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path data_dir() const;
#endif

     /**
      * \brief Returns the path to the project's data_local directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt>
      * \return The path to the project's data_local directory.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto data_local_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path data_local_dir() const;
#endif

     /**
      * \brief Returns the path to the project's preference directory.
      * \details Values on different platforms:
      * - <b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt>
      * - <b>MacOS</b>: <tt>$HOME</tt>/Library/Preferences/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Preferences/com.Foo-Corp.Bar-App</tt>
      * - <b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt>
      * \return
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto preference_dir() const -> std::filesystem::path const&;
#else
     [[nodiscard]] std::filesystem::path preference_dir() const;
#endif

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
      * \return The path to the project's runtime directory or <tt>none</tt> if it is not available.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto runtime_dir() const -> option<std::filesystem::path> const&;
#else
     [[nodiscard]] option<std::filesystem::path> runtime_dir() const;
#endif

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
      * \return The path to the project's state directory or <tt>none</tt> if it is not available.
      */
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto state_dir() const -> option<std::filesystem::path> const&;
#else
     [[nodiscard]] option<std::filesystem::path> state_dir() const;
#endif

     /// \brief Shortcut for \ref get() function.
     /// \param directory_type The directory type.
     /// \return The path to the directory.
     /// \throws std::system_error If the directory does not exist on this platform.
     /// \note Use explicit functions such as \ref config_dir() to avoid this exception.
     /// \see get
#ifndef DOXYGEN_GENERATING_OUTPUT
     [[nodiscard]] auto operator[](dir directory_type) const noexcept(false) -> std::filesystem::path;
#else
     [[nodiscard]] std::filesystem::path operator[](dir directory_type) const;
#endif

    private:
     std::filesystem::path m_project_path;
     std::filesystem::path m_cache_dir;
     std::filesystem::path m_config_dir;
     std::filesystem::path m_config_local_dir;
     std::filesystem::path m_data_dir;
     std::filesystem::path m_data_local_dir;
     std::filesystem::path m_preference_dir;
     option<std::filesystem::path> m_runtime_dir;
     option<std::filesystem::path> m_state_dir;
  };
} // namespace floppy::filesystem