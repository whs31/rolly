#pragma once

#include <string>
#include "../traits/noncopyable.h"
#include "plugin.h"
#include "../types/result.h"
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly::dll {
  class
#ifndef DOXYGEN_GENERATING_OUTPUT
    ___rolly_api___
#endif
      shared_library : noncopyable {

   public:
    shared_library() = default;
    explicit shared_library(std::string name);
    explicit shared_library(std::filesystem::path const& path, std::string name);
    shared_library(shared_library&&) noexcept;
    shared_library& operator=(shared_library&&) noexcept;
    virtual ~shared_library();

    [[nodiscard]] void* handle() const { return this->handle_; }

    [[nodiscard]] std::filesystem::path const& path() const { return this->path_; }

    [[nodiscard]] std::string_view name() const { return this->name_; }

    /**
     * @brief Returns name of the shared object file without platform specific extension and
     * <tt>lib</tt> prefix.
     * @details For example, if <tt>libtest.so</tt> is loaded, then <tt>soname</tt> will return
     * <tt>test</tt>.
     * @return Name of the shared object file without platform specific extension and <tt>lib</tt>
     * prefix.
     * @version 2.1.33
     */
    [[nodiscard]] std::string_view soname() const { return this->soname_; }

    [[nodiscard]] bool valid() const { return this->handle_ != nullptr; }

    [[nodiscard]] operator bool() const { return this->valid(); }  // NOLINT(*-explicit-constructor)

    [[nodiscard]] bool is_loaded() const { return this->loaded_; }

    [[nodiscard]] plugin* load() const;

    [[nodiscard]] result<plugin*> try_load() const noexcept;

    friend class plugin_loader;

   private:
    using bootstrap_function_type = plugin* (*)();

    [[nodiscard]] bootstrap_function_type locate_entry() const;

    std::string name_;
    void* handle_ = nullptr;
    std::filesystem::path path_;
    std::string soname_;
    bool loaded_ = false;
  };
}  // namespace rolly::dll
