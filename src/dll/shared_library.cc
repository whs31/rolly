#include <rolly/dll/shared_library.h>

#include <stdexcept>
#include "oslayer/base.h"
#ifdef ROLLY_OS_WINDOWS
#  include "oslayer/win/dll.h"
#else
#  include "oslayer/linux/dll.h"
#endif

namespace rolly::dll {
  shared_library::shared_library(std::string name)
    : name_(std::move(name))
    , handle_(oslayer::___os___::load_library(this->name()))
    , path_(std::filesystem::current_path()) {}

  shared_library::shared_library(std::filesystem::path const& path, std::string name)
    : name_(std::move(name))
    , handle_(oslayer::___os___::load_library(this->name(), path))
    , path_(path) {}

  shared_library::shared_library(shared_library&& other) noexcept { *this = std::move(other); }

  shared_library& shared_library::operator=(shared_library&& other) noexcept {
    this->handle_ = other.handle_;
    this->name_ = std::move(other.name_);
    this->path_ = std::move(other.path_);
    other.handle_ = nullptr;
    return *this;
  }

  shared_library::~shared_library() {
    if(this->valid())
      oslayer::___os___::unload_library(this->handle_);
    this->handle_ = nullptr;
  }

  plugin* shared_library::load() const {
    if(not this->valid())
      throw std::runtime_error(
        fmt::format("failed to load library '{}' (not found in path '{}')", this->name(), this->path().generic_string())
      );
    auto* entry_ptr = this->locate_entry();
    if(entry_ptr == nullptr)
      throw std::runtime_error(fmt::format(
        R"(failed to locate procedure '{}' in library '{}' (0x{:X}))",
        ___rolly_dll_proc_name_string___,
        this->name_,
        reinterpret_cast<std::uintptr_t>(this->handle_)  // NOLINT(*-pro-type-reinterpret-cast)
      ));
    return entry_ptr();
  }

  result<plugin*> shared_library::try_load() const noexcept {
    if(not this->valid())
      return error("failed to load library '{}' (not found in path '{}')", this->name(), this->path().generic_string());
    auto* entry_ptr = this->locate_entry();
    if(entry_ptr == nullptr)
      return error(
        R"(failed to locate procedure '{}' in library '{}' (0x{:X}))",
        ___rolly_dll_proc_name_string___,
        this->name_,
        reinterpret_cast<std::uintptr_t>(this->handle_)  // NOLINT(*-pro-type-reinterpret-cast)
      );
    return entry_ptr();
  }

  shared_library::bootstrap_function_type shared_library::locate_entry() const {
    return reinterpret_cast<shared_library::bootstrap_function_type>(  // NOLINT(*-pro-type-reinterpret-cast)
      oslayer::___os___::get_proc_address(this->handle_, ___rolly_dll_proc_name_string___)
    );
  }
}  // namespace rolly::dll
