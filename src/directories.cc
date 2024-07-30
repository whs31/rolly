#include <floppy/directories.h>

#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <floppy/detail/predefs.h>
#include <floppy/detail/platform.h>

#if defined(_WIN32)
# include <windows.h>
# if defined(_MSC_VER)
#   if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#     define _AMD64_
#   elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#     define _X86_
#   elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#     define _ARM_
#   endif // __amd64__
# endif // _MSC_VER
# include <Shlobj.h>
#endif // _WIN32

using std::string;
using std::string_view;
using std::vector;
namespace fs = std::filesystem;

namespace
{
#if defined(_WIN32)
  [[nodiscard]] auto known_folder_path(::KNOWNFOLDERID id) -> fs::path {
    auto* buf = ::PWSTR();
    auto const result = ::SHGetKnownFolderPath(id, 0, nullptr, &buf);
    if(result != S_OK) {
      ::CoTaskMemFree(buf);
      throw std::system_error(std::make_error_code(static_cast<std::errc>(result)));
    }
    auto const path = fs::path(buf);
    ::CoTaskMemFree(buf);
    return path;
  }
#endif // _WIN32

  auto split(const string& input) -> vector<string> {
    auto buffer = std::istringstream(input);
    return {std::istream_iterator<string>(buffer), std::istream_iterator<string>()};
  }

  auto to_lower(const string_view name) -> string {
    auto str = string();
    str.reserve(name.length());
    for(auto const c : name)
      str.push_back(static_cast<char>(::tolower(c)));
    return str;
  }

  auto trim(const string_view name, const string_view repl) -> string {
    auto str = string();
    str.reserve(name.length());
    auto parts = split(string(name));
    auto current_part = parts.begin();
    auto const replace = not repl.empty();
    while(current_part != parts.end()) {
      auto const value = to_lower(*current_part);
      str.append(value);
      ++current_part;
      if(replace && current_part != parts.end())
        str.append(repl);
    }
    return str;
  }
} // anonymous namespace

namespace floppy::filesystem
{
  application_dirs::application_dirs(meta::project_meta const& meta) noexcept(false)
    : application_dirs(meta.domain(), meta.organization(), meta.name())
  {}

  application_dirs::application_dirs(
    string_view qualifier,
    string_view organization,
    string_view application
  ) noexcept(false)
  {
    if constexpr(current_platform.operating_system == platform::operating_system::linux_) {
      auto const path = fs::path(::trim(application, ""));
      auto* const home_c = std::getenv("HOME");
      if(not home_c)
        throw std::runtime_error("no valid home directory could be retrieved from the operating system");
      auto const home = fs::path(home_c);
      auto* const xdg_runtime_dir = std::getenv("XDG_RUNTIME_DIR");
      this->m_project_path = path;
      this->m_cache_dir = home / ".cache" / path;
      this->m_config_dir = home / ".config" / path;
      this->m_config_local_dir = this->m_config_dir;
      this->m_data_dir = home / ".local" / "share" / path;
      this->m_data_local_dir = this->m_data_dir;
      this->m_preference_dir = this->m_config_dir;
      this->m_runtime_dir = xdg_runtime_dir ? option<fs::path>(fs::path(xdg_runtime_dir)) : none;
      this->m_state_dir = home / ".local" / "state" / path;
    } else if constexpr(current_platform.operating_system == platform::operating_system::windows) {
      auto const path = fs::path(organization) / application;
      #if defined(FLOPPY_OS_WINDOWS)
        auto const appdata_roaming = ::known_folder_path(FOLDERID_RoamingAppData);
        auto const appdata_local = ::known_folder_path(FOLDERID_LocalAppData);
        this->m_project_path = path;
        this->m_cache_dir = appdata_local / path / "cache";
        this->m_config_dir = appdata_roaming / path / "config";
        this->m_config_local_dir = appdata_local / path / "config";
        this->m_data_dir = appdata_roaming / path / "data";
        this->m_data_local_dir = appdata_local / path / "data";
        this->m_preference_dir = this->m_config_dir;
        this->m_runtime_dir = none;
        this->m_state_dir = none;
      #endif
    } else if constexpr(current_platform.operating_system == platform::operating_system::macos) {
      auto replaced = [](string_view const str, char const what, char const with) {
        auto owned_str = string(str);
        std::replace(owned_str.begin(), owned_str.end(), what, with);
        return owned_str;
      };
      auto const org = replaced(organization, ' ', '-');
      auto const path = fmt::format("{}{}{}{}{}",
        qualifier,
        qualifier.empty() ? "" : ".",
        org,
        org.empty() ? "" : ".",
        application
      );
      auto* const home_c = std::getenv("HOME");
      if(not home_c)
        throw std::runtime_error("no valid home directory could be retrieved from the operating system");
      auto const home = fs::path(home_c);
      this->m_project_path = path;
      this->m_cache_dir = home / "Library" / "Caches" / path;
      this->m_config_dir = home / "Library" / "Application Support" / path;
      this->m_config_local_dir = this->m_config_dir;
      this->m_data_dir = this->m_config_dir;
      this->m_data_local_dir = this->m_config_dir;
      this->m_preference_dir = this->m_config_dir;
      this->m_runtime_dir = none;
      this->m_state_dir = none;
    } else {
      throw std::runtime_error("unsupported operating system");
    }
  }

  auto application_dirs::create() const -> void {
    for(auto i = 0_ZU; i <= static_cast<usize>(dir::preferences); ++i) {
      if(auto const dir = this->get(static_cast<enum dir>(i)); not fs::exists(dir))
        fs::create_directories(dir);
    }
    if(this->m_runtime_dir and not fs::exists(*this->m_runtime_dir))
      fs::create_directory(*this->m_runtime_dir);
    if(this->m_state_dir and not fs::exists(*this->m_state_dir))
      fs::create_directory(*this->m_state_dir);
  }

  auto application_dirs::remove() const -> void {
    for(auto i = 0_ZU; i <= static_cast<usize>(dir::preferences); ++i) {
      if(auto const dir = this->get(static_cast<enum dir>(i)); fs::exists(dir))
        fs::remove_all(dir);
    }
    if(this->m_runtime_dir and fs::exists(*this->m_runtime_dir))
      fs::remove_all(*this->m_runtime_dir);
    if(this->m_state_dir and fs::exists(*this->m_state_dir))
      fs::remove_all(*this->m_state_dir);
  }

  auto application_dirs::project_path() const -> fs::path const& { return this->m_project_path; }
  auto application_dirs::cache_dir() const -> fs::path const& { return this->m_cache_dir; }
  auto application_dirs::config_dir() const -> fs::path const& { return this->m_config_dir; }
  auto application_dirs::config_local_dir() const -> fs::path const& { return this->m_config_local_dir; }
  auto application_dirs::data_dir() const -> fs::path const& { return this->m_data_dir; }
  auto application_dirs::data_local_dir() const -> fs::path const& { return this->m_data_local_dir; }
  auto application_dirs::preference_dir() const -> fs::path const& { return this->m_preference_dir; }
  auto application_dirs::runtime_dir() const -> option<fs::path> const& { return this->m_runtime_dir; }
  auto application_dirs::state_dir() const -> option<fs::path> const& { return this->m_state_dir; }

  auto application_dirs::get(application_dirs::dir directory_type) const noexcept(false) -> fs::path
  {
    switch(directory_type) {
      case dir::cache: return this->m_cache_dir;
      case dir::config: return this->m_config_dir;
      case dir::config_local: return this->m_config_local_dir;
      case dir::data: return this->m_data_dir;
      case dir::data_local: return this->m_data_local_dir;
      case dir::preferences: return this->m_preference_dir;
      case dir::runtime:
        if(this->m_runtime_dir)
          return *this->m_runtime_dir;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      case dir::state:
        if(this->m_state_dir)
          return *this->m_state_dir;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      default: throw std::system_error(std::make_error_code(std::errc::invalid_argument));
    }
  }

  auto application_dirs::operator[](application_dirs::dir directory_type) const noexcept(false) -> fs::path {
    return this->get(directory_type);
  }
} // namespace floppy::filesystem