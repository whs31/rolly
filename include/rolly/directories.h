#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <optional>
#include "global/export.h"
#include "global/version.h"
#include "global/definitions.h"
#include "types/stdint.h"

namespace rolly {
  class dirs {
   public:
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      dirs();
    ~dirs() = default;
    dirs(dirs const&) = default;
    dirs(dirs&&) = default;
    dirs& operator=(dirs const&) = default;
    dirs& operator=(dirs&&) = default;

    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& user_home() const;

   private:
    std::filesystem::path user_home_;
  };

  /**
   * @brief Class for getting location of system directories for a specific application.
   * @details ProjectDirs computes the location of cache, config or data directories for a specific application,
   * which are derived from the standard directories and the name of the project/organization.
   *
   * For example if user named <i>Alice</i>, the following code:
   * @code {.cpp}
   * using rolly::application_dirs;
   * using enum rolly::application_dirs::dir;
   *
   * auto dirs = application_dirs("com", "Foo Corp", "Bar App");
   * std::cout << dirs[config].string() << std::endl; // or std::cout << dirs.config_dir() << std::endl;
   * @endcode
   * will produce the following output:
   * @code {.sh}
   * Linux:   '/home/alice/.config/barapp'
   * Windows: 'C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App'
   * MacOS:   '/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App'
   * @endcode
   * @see dirs
   */
  class application_dirs {
   public:
    /**
     * @brief Supported application directory types.
     */
    enum class dir : u8 {
      cache,         ///< Cache directory
      config,        ///< Config directory
      config_local,  ///< Local config directory
      data,          ///< Data directory
      data_local,    ///< Local data directory
      preferences,   ///< Preferences directory
      runtime,       ///< Runtime directory. Can be unavailable on some platforms
      state          ///< State directory. Can be unavailable on some platforms
    };

                     /**
                      * @brief Creates an application_dirs class from values describing the project.
                      * @note Constructor can fail if no valid home directory could be retrieved from the operating system.
                      * @note Directories will be created if they do not exist.
                      * @param qualifier The reverse domain name notation of the application, excluding
                      * the organization or application name itself.
                      *
                      * Example values of qualifier:
                      * <ul>
                      * <li><tt>"com.example"</tt></li>
                      * <li><tt>"org"</tt></li>
                      * <li><tt>"uk.co"</tt></li>
                      * <li><tt>"io"</tt></li>
                      * <li><tt>""</tt></li>
                      * </ul>
                      * @param vendor The name of the organization that develops this application, or for which the application is
                      * developed.
                      *
                      * Example values of vendor:
                      * <ul>
                      * <li><tt>"Foo Corp"</tt></li>
                      * <li><tt>"Bar Ltd"</tt></li>
                      * <li><tt>"Example Inc"</tt></li>
                      * </ul>
                      * @param app The name of the application itself.
                      *
                      * Example values of app:
                      * <ul>
                      * <li><tt>"Bar App"</tt></li>
                      * <li><tt>"Foo App"</tt></li>
                      * </ul>
                      * @throws std::runtime_error if no valid home directory could be retrieved from the operating system.
                      */
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      application_dirs(std::string_view qualifier, std::string_view vendor, std::string_view app);

        /**
         * @brief Creates an application_dirs class from values describing the project.
         * @note Constuctor can fail if no valid home directory could be retrieved from the operating system.
         * @note Directories will be created if they do not exist.
         * @param meta The project meta data.
         * @throws std::runtime_error if no valid home directory could be retrieved from the operating system.
         * @see meta::project_meta
         */
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      explicit application_dirs(meta::project_meta const& meta);

        /**
         * @brief Creates the directories if they do not exist.
         */
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      void
      create() const;

        /**
         * @brief Removes the directories and their contents from the filesystem.
         */
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      void
      remove() const;

    /**
     * @brief Returns the path to the directory.
     * @param d The directory type.
     * @return The path to the directory.
     * @throws std::system_error If the directory does not exist on this platform.
     * @note Use explicit functions such as @ref application_dirs::config_dir to avoid this exception.
     * @see operator[]
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& get(dir d) const;

    /**
     * @brief Returns the path to the directory.
     * @param d The directory type.
     * @return The path to the directory.
     * @throws std::system_error If the directory does not exist on this platform.
     * @note Use explicit functions such as @ref application_dirs::config_dir to avoid this exception.
     * @see get
     */
    [[nodiscard]] inline std::filesystem::path const& operator[](dir d) const { return this->get(d); }

    /**
     * \brief Returns the project path fragment used to compute the project's cache/config/data directories.
     * \details The value is derived from the constructor call and is platform-dependent.
     * \return The project path.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& project_path() const;

    /**
     * @brief Returns the path to the project's cache directory.
     * @details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_CACHE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.cache/<tt>project_path</tt>,
     * for example: <tt>/home/alice/.cache/barapp</tt></li> <li><b>MacOS</b>:
     * <tt>$HOME/Library/Caches</tt>/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Caches/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\cache, for example: <tt>C:\\Users\\Alice\\AppData\\Local\\Foo
     * Corp\\Bar App\\cache</tt></li>
     * </ul>
     * @return The path to the project's cache directory.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& cache_dir() const;

    /**
     * @brief Returns the path to the project's config directory.
     * @details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or
     * <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     * <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%APPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo
     * Corp\\Bar App\\config</tt></li>
     * </ul>
     * @return The path to the project's config directory.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& config_dir() const;

    /**
     * @brief Returns the path to the project's config_local directory.
     * @details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or
     * <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     * <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo
     * Corp\\Bar App\\config</tt></li>
     * </ul>
     * @return The path to the project's config_local directory.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& config_local_dir() const;

    /**
     * @brief Returns the path to the project's data directory.
     * @details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or
     * <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt></li>
     * <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo
     * Corp\\Bar App\\data</tt></li>
     * </ul>
     * @return The path to the project's data directory.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& data_dir() const;

    /**
     * @brief Returns the path to the project's data_local directory.
     * @details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or
     * <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt></li>
     * <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo
     * Corp\\Bar App\\data</tt></li>
     * </ul>
     * @return The path to the project's data_local directory.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& data_local_dir() const;

    /**
     * \brief Returns the path to the project's preference directory.
     * \details Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or
     * <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     * <li><b>MacOS</b>: <tt>$HOME</tt>/Library/Preferences/<tt>project_path</tt>, for example:
     * <tt>/Users/Alice/Library/Preferences/com.Foo-Corp.Bar-App</tt></li> <li><b>Windows</b>:
     * <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo
     * Corp\\Bar App\\config</tt></li>
     * </ul>
     * \return
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::filesystem::path const& preference_dir() const;

    /**
     * @brief Returns the path to the project's runtime directory.
     * @details
     * The runtime directory contains transient, non-essential data (like sockets or named pipes) that
     * is expected to be cleared when the user's session ends.<br>
     * Values on different platforms:
     * <ul>
     * <li><b>Linux</b>: <tt>XDG_RUNTIME_DIR</tt>/<tt>project_path</tt>, for example:
     * <tt>/run/user/1001/barapp</tt></li> <li><b>MacOS</b>: -</li> <li><b>Windows</b>: -</li>
     * </ul>
     * @note Only for Linux.
     * @return The path to the project's runtime directory or <tt>none</tt> if it is not available.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::optional<std::filesystem::path> const& runtime_dir() const;

    /**
     * \brief Returns the path to the project's state directory.
     * \details
     * The state directory contains data that should be retained between sessions (unlike the runtime directory),
     * but may not be important/portable enough to be synchronized across machines (unlike the config/preferences/data
     * directories).<br> Values on different platforms: <ul> <li><b>Linux</b>:
     * <tt>$XDG_STATE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/state/<tt>project_path</tt>, for example:
     * <tt>/home/alice/.local/state/barapp</tt></li> <li><b>MacOS</b>: -</li> <li><b>Windows</b>: -</li>
     * </ul>
     * \note Only for Linux.
     * \return The path to the project's state directory or <tt>none</tt> if it is not available.
     */
    [[nodiscard]]
#ifndef ROLLY_DOC
    ___rolly_api___
#endif  // ROLLY_DOC
      std::optional<std::filesystem::path> const& state_dir() const;

   private:
    std::filesystem::path project_path_;
    std::filesystem::path cache_dir_;
    std::filesystem::path config_dir_;
    std::filesystem::path config_local_dir_;
    std::filesystem::path data_dir_;
    std::filesystem::path data_local_dir_;
    std::filesystem::path preference_dir_;
    std::optional<std::filesystem::path> runtime_dir_;
    std::optional<std::filesystem::path> state_dir_;
  };
}  // namespace rolly
