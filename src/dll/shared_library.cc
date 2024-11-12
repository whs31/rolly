#include <rolly/dll/shared_library.h>

#include <stdexcept>
#include "oslayer/dll.h"

namespace rolly::dll {
  shared_library::shared_library(std::string_view const name)
    : handle_(oslayer::load_library(name))
    , path_(std::filesystem::current_path())
    , name_(name) {}

  shared_library::shared_library(std::filesystem::path const& path, std::string_view const name)
    : handle_(oslayer::load_library(name, path))
    , path_(path)
    , name_(name) {}

  shared_library::shared_library(shared_library&& other) noexcept { *this = std::move(other); }

  shared_library& shared_library::operator=(shared_library&& other) noexcept {
    this->handle_ = other.handle_;
    other.handle_ = nullptr;
    return *this;
  }

  shared_library::~shared_library() {
    if(this->valid())
      oslayer::unload_library(this->handle_);
    this->handle_ = nullptr;
  }

  plugin* shared_library::load() const {
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
      oslayer::proc_address(this->handle_, ___rolly_dll_proc_name_string___)
    );
  }
}  // namespace rolly::dll
