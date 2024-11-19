#pragma once

#include <utility>
#include <fstream>
#include "../types/result.h"
#ifndef Q_MOC_RUN
#  include <filesystem>
#endif

namespace rolly {
  /**
   * @brief IO-related functions and classes.
   */
  namespace io {
    class filedevice {
     public:
      static constexpr auto write_permissions = std::filesystem::perms::owner_read
                                              | std::filesystem::perms::owner_write
                                              | std::filesystem::perms::group_read
                                              | std::filesystem::perms::others_read
                                              | std::filesystem::perms::others_exec
                                              | std::filesystem::perms::owner_all
                                              | std::filesystem::perms::group_all;

      explicit filedevice(std::filesystem::path path)
        : path_(std::move(path)) {}

      [[nodiscard]] std::filesystem::path const& path() const { return this->path_; }

      [[nodiscard]] std::filesystem::path suffixed_path(std::string_view suffix) const {
        auto copy = this->path_;
        return copy.concat(suffix);
      }

      [[nodiscard]] bool exists() const { return std::filesystem::exists(this->path_); }

      [[nodiscard]] bool is_regular_file() const {
        return std::filesystem::is_regular_file(this->path_);
      }

      [[nodiscard]] std::string read() const { return filedevice::read_from(this->path()); }

      [[nodiscard]] result<std::string> try_read() const noexcept {
        return filedevice::try_read_from(this->path());
      }

      void write(std::string_view content) const noexcept(false) {
        filedevice::write_to(this->path(), content);
      }

      [[nodiscard]] result<> try_write(std::string_view content) const noexcept {
        return filedevice::try_write_to(this->path(), content);
      }

      [[nodiscard]] static std::string read_from(std::filesystem::path const& path) {
        auto handle = std::ifstream(path);
        if(not handle.is_open())
          throw std::runtime_error(
            "failed to open file handle for reading at " + path.generic_string()
          );
        return {(std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>()};
      }

      [[nodiscard]] static result<std::string> try_read_from(std::filesystem::path const& path
      ) noexcept {
        auto handle = std::ifstream(path);
        if(not handle.is_open())
          return error("failed to open file handle for reading at \'{}\'", path.generic_string());
        return ok<std::string>(
          {(std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>()}
        );
      }

      static void write_to(std::filesystem::path const& path, std::string_view content) noexcept(
        false
      ) {
        namespace fs = std::filesystem;

        if(not fs::exists(path.parent_path()))
          fs::create_directories(path.parent_path());
        auto handle = std::ofstream(path);
        if(not handle.is_open())
          throw std::runtime_error(
            "failed to open file handle for writing at " + path.generic_string()
          );
        if(fs::exists(path))
          fs::permissions(path, filedevice::write_permissions);
        handle << content;
        if(not handle.good())
          throw std::runtime_error("failed to write to file at " + path.generic_string());
      }

      [[nodiscard]] static result<>
        try_write_to(std::filesystem::path const& path, std::string_view content) noexcept {
        namespace fs = std::filesystem;

        if(not fs::exists(path.parent_path()))
          fs::create_directories(path.parent_path());
        auto handle = std::ofstream(path);
        if(not handle.is_open())
          return error("failed to open file handle for writing at \'{}\'", path.generic_string());
        if(fs::exists(path))
          fs::permissions(path, filedevice::write_permissions);
        handle << content;
        if(not handle.good())
          return error("failed to write to file at \'{}\'", path.generic_string());
        return ok();
      }

     private:
      std::filesystem::path path_;
    };
  }  // namespace io
}  // namespace rolly
