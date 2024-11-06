#include <optional>
#include <rolly/directories.h>

#include <stdexcept>
#include <system_error>
#include <rolly/string_util.h>
#include "oslayer/base.h"

#ifdef ROLLY_OS_WINDOWS
#  include "oslayer/win/known_folder.h"
#else
#  include "oslayer/linux/dirs.h"
#endif  // ROLLY_OS_WINDOWS

using std::string;
using std::string_view;
using std::filesystem::path;

#if defined(ROLLY_OS_LINUX)
namespace {
  string trim(string_view const name, string_view const repl) {
    auto str = string();
    str.reserve(name.length());
    auto parts = rolly::split(name);
    auto current_part = parts.begin();
    auto const replace = not repl.empty();
    while(current_part != parts.end()) {
      auto const value = rolly::to_lower(*current_part);
      str.append(value);
      ++current_part;
      if(replace && current_part != parts.end())
        str.append(repl);
    }
    return str;
  }
}  // namespace
#endif  // ROLLY_OS_LINUX

namespace rolly {
  dirs::dirs()
    : user_home_(oslayer::___os___::home_dir()) {}

  path const& dirs::user_home() const { return this->user_home_; }

  application_dirs::application_dirs(
    [[maybe_unused]] string_view const qualifier,
    [[maybe_unused]] string_view const vendor,
    [[maybe_unused]] string_view const app
  ) {
#if defined(ROLLY_OS_WINDOWS)
    auto const p = path(vendor) / app;
    auto const appdata = oslayer::___os___::appdata_dir();
    auto const local_appdata = oslayer::___os___::local_appdata_dir();

    this->project_path_ = p;
    this->cache_dir_ = local_appdata / p / "cache";
    this->config_dir_ = appdata / p / "config";
    this->config_local_dir_ = local_appdata / p / "config";
    this->data_dir_ = appdata / p / "data";
    this->data_local_dir_ = local_appdata / p / "data";
    this->preference_dir_ = this->config_dir_;  // NOLINT
    this->runtime_dir_ = std::nullopt;          // NOLINT
    this->state_dir_ = std::nullopt;            // NOLINT
#elif defined(ROLLY_OS_LINUX)
    auto const p = path(::trim(app, ""));
    auto const home = oslayer::___os___::home_dir();

    this->project_path_ = p;
    this->cache_dir_ = home / ".cache" / p;
    this->config_dir_ = home / ".config" / p;
    this->config_local_dir_ = this->config_dir_;  // NOLINT
    this->data_dir_ = home / ".local" / "share" / p;
    this->data_local_dir_ = this->data_dir_;      // NOLINT
    this->preference_dir_ = this->config_dir_;
    try {
      this->runtime_dir_ = oslayer::___os___::xdg_runtime_dir();
    } catch(std::exception const&) {
      this->runtime_dir_ = std::nullopt;
    }
    this->state_dir_ = home / ".local" / "state" / p;
#elif defined(ROLLY_OS_DARWIN)
    auto replaced = [](string_view const str, char const what, char const with) {
      auto owned_str = string(str);
      std::replace(owned_str.begin(), owned_str.end(), what, with);
      return owned_str;
    };
    auto const org = replaced(vendor, ' ', '-');
    auto const path =
      fmt::format("{}{}{}{}{}", qualifier, qualifier.empty() ? "" : ".", org, org.empty() ? "" : ".", app);
    auto const home = oslayer::___os___::home_dir();
    this->project_path_ = path;
    this->cache_dir_ = home / "Library" / "Caches" / path;
    this->config_dir_ = home / "Library" / "Application Support" / path;
    this->config_local_dir_ = this->config_dir_;  // NOLINT
    this->data_dir_ = this->config_dir_;          // NOLINT
    this->data_local_dir_ = this->config_dir_;    // NOLINT
    this->preference_dir_ = this->config_dir_;    // NOLINT
    this->runtime_dir_ = std::nullopt;            // NOLINT
    this->state_dir_ = std::nullopt;              // NOLINT
#endif                                          // OS
  }

  application_dirs::application_dirs(meta::project_meta const& meta)
    : application_dirs(meta.domain(), meta.organization(), meta.name()) {}

  void application_dirs::create() const {
    for(auto i = 0; i <= static_cast<int>(dir::preferences); ++i)
      if(auto const directory = (*this)[static_cast<enum dir>(i)]; not exists(directory))
        create_directories(directory);
    if(this->runtime_dir_ and not exists(*this->runtime_dir_))
      create_directories(*this->runtime_dir_);
    if(this->state_dir_ and not exists(*this->state_dir_))
      create_directories(*this->state_dir_);
  }

  void application_dirs::remove() const {
    for(auto i = 0; i <= static_cast<int>(dir::preferences); ++i)
      if(auto const directory = (*this)[static_cast<enum dir>(i)]; exists(directory))
        remove_all(directory);
    if(this->runtime_dir_ and exists(*this->runtime_dir_))
      remove_all(*this->runtime_dir_);
    if(this->state_dir_ and exists(*this->state_dir_))
      remove_all(*this->state_dir_);
  }

  path const& application_dirs::get(dir d) const {
    switch(d) {
      case dir::cache: return this->cache_dir_;
      case dir::config: return this->config_dir_;
      case dir::config_local: return this->config_local_dir_;
      case dir::data: return this->data_dir_;
      case dir::data_local: return this->data_local_dir_;
      case dir::preferences: return this->preference_dir_;
      case dir::runtime:
        if(this->runtime_dir_)
          return *this->runtime_dir_;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      case dir::state:
        if(this->state_dir_)
          return *this->state_dir_;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      default: throw std::system_error(std::make_error_code(std::errc::invalid_argument));
    }
  }

  path const& application_dirs::project_path() const { return this->project_path_; }

  path const& application_dirs::cache_dir() const { return this->cache_dir_; }

  path const& application_dirs::config_dir() const { return this->config_dir_; }

  path const& application_dirs::config_local_dir() const { return this->config_local_dir_; }

  path const& application_dirs::data_dir() const { return this->data_dir_; }

  path const& application_dirs::data_local_dir() const { return this->data_local_dir_; }

  path const& application_dirs::preference_dir() const { return this->preference_dir_; }

  std::optional<path> const& application_dirs::runtime_dir() const { return this->runtime_dir_; }

  std::optional<path> const& application_dirs::state_dir() const { return this->state_dir_; }
}  // namespace rolly
