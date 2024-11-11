#pragma once

#include <utility>
#include <fstream>
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
      explicit filedevice(std::filesystem::path path)
        : path_(std::move(path)) {}

      [[nodiscard]] std::filesystem::path const& path() const { return this->path_; }

      [[nodiscard]] std::filesystem::path suffixed_path(std::string_view suffix) const {
        auto copy = this->path_;
        return copy.concat(suffix);
      }

      [[nodiscard]] bool exists() const { return std::filesystem::exists(this->path_); }

      [[nodiscard]] bool is_regular_file() const { return std::filesystem::is_regular_file(this->path_); }

     private:
      std::filesystem::path path_;
    };
  }  // namespace io
}  // namespace rolly
