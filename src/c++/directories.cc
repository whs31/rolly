#include <vector>
#include <sstream>
#include <iterator>
#include <lf/directories.h>

#ifdef LF_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#endif

using std::string;
using std::string_view;
using std::vector;
using std::istringstream;
using std::istream_iterator;
namespace fs = std::filesystem;

namespace lf::directories::utl
{
  auto split(const string& input) -> vector<string> {
    istringstream buffer(input);
    return {istream_iterator<string>(buffer), istream_iterator<string>()};
  }

  auto to_lower(const string_view name) -> string {
    auto str = string();
    str.reserve(name.length());
    for(const auto c : name)
      str.push_back(static_cast<char>(::tolower(c)));
    return str;
  }

  auto trim(const string_view name, const string_view repl) -> string
  {
    auto str = string();
    str.reserve(name.length());
    auto parts = split(string(name));
    auto current_part = parts.begin();
    const auto replace = not repl.empty();
    while(current_part != parts.end()) {
      const auto value = to_lower(*current_part);
      str.append(value);
      ++current_part;
      if(replace && current_part != parts.end())
        str.append(repl);
    }
    return str;
  }
}

namespace lf::directories::platform_details
{
  namespace gnu_linux
  {
    auto project_dirs_from_path(const fs::path& project_path) -> Result<project_dirs>
    {
      const auto home_c = std::getenv("HOME");
      if(home_c == nullptr)
        return Error("environment variable HOME is not set");
      const auto home = fs::path(home_c);
      const auto xdg_runtime_dir = std::getenv("XDG_RUNTIME_DIR");
      auto runtime_dir = Option<fs::path>();
      if(xdg_runtime_dir != nullptr)
        runtime_dir = fs::path(xdg_runtime_dir);
      return Ok(project_dirs(
        project_path,
        home / ".cache" / project_path,
        home / ".config" / project_path,
        home / ".config" / project_path,
        home / ".local" / "share" / project_path,
        home / ".local" / "share" / project_path,
        home / ".config" / project_path,
        runtime_dir,
        home / ".local" / "state" / project_path
      ));
    }
  }

  namespace windows
  {
    auto project_dirs_from_path(const fs::path& project_path) -> Result<project_dirs>
    {
      #ifdef LF_OS_WINDOWS
      auto appdata_roaming = fs::path();
      auto appdata_local = fs::path();

      {
        auto tmp = PWSTR();
        if(const auto h_roaming = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &tmp); h_roaming != S_OK) {
          llog::error("failed to get roaming app data path (error code: {})", h_roaming);
          CoTaskMemFree(tmp);
          return Error("failed to get roaming app data path (error code: {})", h_roaming);
        }
        appdata_roaming = fs::path(tmp);
        CoTaskMemFree(tmp);

        if(const auto h_local = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &tmp); h_local != S_OK) {
          llog::error("failed to get local app data path (error code: {})", h_local);
          CoTaskMemFree(tmp);
          return Error("failed to get local app data path (error code: {})", h_local);
        }
        appdata_local = fs::path(tmp);
        CoTaskMemFree(tmp);
      }

      return Ok(project_dirs(
        project_path,
        appdata_local / project_path / "cache",
        appdata_roaming / project_path / "config",
        appdata_local / project_path / "config",
        appdata_roaming / project_path / "data",
        appdata_local / project_path / "data",
        appdata_roaming / project_path / "config",
        None,
        None
      ));
      #else
      return Error("not implemented for this platform");
      #endif
    }
  }

  namespace macos
  {
    auto project_dirs_from_path(const fs::path& project_path) -> Result<project_dirs>
    {
      const auto home_c = std::getenv("HOME");
      if(home_c == nullptr)
        return Error("environment variable HOME is not set");
      const auto home = fs::path(home_c);
      return project_dirs(
        project_path,
        home / "Library" / "Caches" / project_path,
        home / "Library" / "Application Support" / project_path,
        home / "Library" / "Application Support" / project_path,
        home / "Library" / "Application Support" / project_path,
        home / "Library" / "Application Support" / project_path,
        home / "Library" / "Preferences" / project_path,
        None,
        None
      );
    }

    auto project_dirs_from(
      const string_view qualifier,
      const string_view organization,
      const string_view application
      ) -> Result<project_dirs>
    {
      auto replaced = [](const string_view str, const char what, const char with) {
        auto owned_str = string(str);
        std::replace(owned_str.begin(), owned_str.end(), what, with);
        return owned_str;
      };

      const auto org = replaced(organization, ' ', '-');
      const auto bundle_id = fmt::format("{}{}{}{}{}",
        qualifier,
        qualifier.empty() ? "" : ".",
        org,
        org.empty() ? "" : ".",
        replaced(application, ' ', '-')
      );
      return project_dirs_from_path(bundle_id);
    }
  }
}

namespace lf::directories
{
  project_dirs::project_dirs(
    fs::path project_path,
    fs::path cache_dir,
    fs::path config_dir,
    fs::path config_local_dir,
    fs::path data_dir,
    fs::path data_local_dir,
    fs::path preference_dir,
    Option<fs::path> runtime_dir,
    Option<fs::path> state_dir
  )
    : m_project_path(std::move(project_path))
    , m_cache_dir(std::move(cache_dir))
    , m_config_dir(std::move(config_dir))
    , m_config_local_dir(std::move(config_local_dir))
    , m_data_dir(std::move(data_dir))
    , m_data_local_dir(std::move(data_local_dir))
    , m_preference_dir(std::move(preference_dir))
    , m_runtime_dir(std::move(runtime_dir))
    , m_state_dir(std::move(state_dir))
  {}

  auto project_dirs::from(
    [[maybe_unused]] const string_view qualifier,
    [[maybe_unused]] const string_view organization,
    const string_view application
    ) -> Result<project_dirs>
  {
    if constexpr(lf::os::system() == lf::os::operating_system::gnu_linux)
      return platform_details::gnu_linux::project_dirs_from_path(fs::path(utl::trim(application, "")));
    else if constexpr(lf::os::system() == lf::os::operating_system::windows)
      return platform_details::windows::project_dirs_from_path(fs::path(organization) / fs::path(application));
    else if constexpr(lf::os::system() == lf::os::operating_system::darwin)
      return platform_details::macos::project_dirs_from(qualifier, organization, application);
    // else if constexpr(lf::os::system() == lf::os::OperatingSystem::Android)
    //   return nullopt;
    else {
      llog::error("unsupported operating system");
      return Error("unsupported operating system");
    }
  }

  auto project_dirs::project_path() const -> fs::path const& { return this->m_project_path; }
  auto project_dirs::cache_dir() const -> fs::path const& { return this->m_cache_dir; }
  auto project_dirs::config_dir() const -> fs::path const& { return this->m_config_dir; }
  auto project_dirs::config_local_dir() const -> fs::path const& { return this->m_config_local_dir; }
  auto project_dirs::data_dir() const -> fs::path const& { return this->m_data_dir; }
  auto project_dirs::data_local_dir() const -> fs::path const& { return this->m_data_local_dir; }
  auto project_dirs::preference_dir() const -> fs::path const& { return this->m_preference_dir; }
  auto project_dirs::runtime_dir() const -> Option<fs::path> const& { return this->m_runtime_dir; }
  auto project_dirs::state_dir() const -> Option<fs::path> const& { return this->m_state_dir; }
}

