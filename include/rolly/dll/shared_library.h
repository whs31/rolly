#pragma once

#include <string>
#include "../traits/noncopyable.h"
#include "plugin.h"
#include "../types/result.h"
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly::dll {
  class shared_library : noncopyable {
   public:
    shared_library() = default;
    explicit shared_library(std::string_view name);
    explicit shared_library(std::filesystem::path const& path, std::string_view name);
    shared_library(shared_library&&) noexcept;
    shared_library& operator=(shared_library&&) noexcept;
    virtual ~shared_library();

    [[nodiscard]] void* handle() const { return this->handle_; }

    [[nodiscard]] std::filesystem::path const& path() const { return this->path_; }

    [[nodiscard]] std::string_view name() const { return this->name_; }

    [[nodiscard]] bool valid() const { return this->handle_ != nullptr; }

    [[nodiscard]] operator bool() const { return this->valid(); }  // NOLINT(*-explicit-constructor)

    [[nodiscard]] plugin* load() const;

    [[nodiscard]] result<plugin*> try_load() const noexcept;

   private:
    using bootstrap_function_type = plugin* (*)();

    [[nodiscard]] bootstrap_function_type locate_entry() const;

    void* handle_ = nullptr;
    std::filesystem::path path_;
    std::string name_;
  };
}  // namespace rolly::dll
