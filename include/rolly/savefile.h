#pragma once

#pragma once

#include <filesystem>
#include <utility>
#include <fstream>
#include "serialization.h"
#include "types/stdint.h"

namespace rolly {
  template <typename F, typename T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
#endif
    class savefile {

   public:
    explicit savefile(std::filesystem::path path);

    explicit savefile(std::string_view filename, std::filesystem::path const& folder);
    savefile(savefile const&) = default;
    savefile(savefile&&) = default;

    /**
     * @brief Closes the savefile.
     * @details File will be saved automatically on closing.
     */
    virtual ~savefile() noexcept;

    /**
     * @brief Returns whether savefile is valid or not.
     */
    [[nodiscard]] bool valid() const;

    /**
     * @brief Path to the savefile.
     */
    [[nodiscard]] std::filesystem::path const& path() const;

    /**
     * @brief Path to the savefile's backing file.
     */
    [[nodiscard]] std::filesystem::path const& backing_path() const;

    [[nodiscard]] bool has_backup() const noexcept { return std::filesystem::exists(this->backing_path()); }

    /**
     * @brief Constant reference to the current values of savefile.
     */
    [[nodiscard]] T const& values() const;

    /**
     * @brief Mutable reference to the current values of savefile.
     */
    [[nodiscard]] T& values_mut();

    void load() noexcept(false);

    void save() const noexcept(false);

    /**
     * @brief Swaps the current savefile with its backup.
     */
    void invalidate() const noexcept;

    void commit() const noexcept;

    /**
     * @brief Returns whether the savefile is valid or not.
     * @see valid
     */
    [[nodiscard]] explicit operator bool() const noexcept;

    /**
     * @brief Constant reference to the current values of the savefile.
     * @see values
     */
    [[nodiscard]] T const& operator()() const;

    /**
     * @brief Mutable reference to the current values of the savefile.
     * @see values_mut
     */
    [[nodiscard]] T& operator()();

    /**
     * @brief Default copy assignment.
     */
    savefile& operator=(savefile const&) = default;

    /**
     * @brief Default move assignment.
     */
    savefile& operator=(savefile&&) = default;

   protected:
    /**
     * @brief Reads content of savefile from file.
     */
    [[nodiscard]] std::string read_from_file() const noexcept(false);

    /**
     * @brief Writes content of savefile to a file.
     * @param content Content to write.
     */
    void write_to_file(std::string_view content) const noexcept(false);

   private:
    T values_;
    mutable std::filesystem::path path_;
    mutable std::filesystem::path backing_path_;
    bool valid_;
  };

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) savefile<F, T>::savefile(std::filesystem::path path)
    : path_(std::move(path))
    , backing_path_([this]() -> std::filesystem::path {
      auto path = this->path_;
      path.replace_extension(".bak");
      return path;
    }()) {
    try {
      this->load();
      this->valid_ = true;
    } catch(std::exception const& ex) {
      this->valid_ = false;
    }
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) savefile<F, T>::savefile(std::string_view filename, std::filesystem::path const& folder)
    : savefile(folder / filename) {}

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) savefile<F, T>::~savefile() noexcept {
    try {
      this->save();
    } catch(std::exception const& ex) {
    }  // NOLINT(*-empty-catch)
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) bool savefile<F, T>::valid() const {
    return this->valid_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) std::filesystem::path const& savefile<F, T>::path() const {
    return this->path_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) std::filesystem::path const& savefile<F, T>::backing_path() const {
    return this->backing_path_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T& savefile<F, T>::values_mut() {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T const& savefile<F, T>::values() const {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void savefile<F, T>::load(
  ) noexcept(false) {
    namespace fs = std::filesystem;
    if(not fs::exists(this->path())) {
      this->values_ = T();
      this->save();
      return;
    }
    try {
      auto str = this->read_from_file();
      this->values_ = serialization::deserialize<F, T>(str);
    } catch(std::exception const& ex) {
      this->invalidate();
      try {
        auto str = this->read_from_file();
        this->values_ = serialization::deserialize<F, T>(str);
      } catch(std::exception const& ex2) {
        throw ex2;
      }
    }
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void savefile<F, T>::save() const
    noexcept(false) try {
    auto str = serialization::serialize<F, T>(this->values_);
    this->write_to_file(str);
    this->commit();
  } catch(std::exception const& ex) {
    throw;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void savefile<F, T>::invalidate(
  ) const noexcept {
    if(exists(this->path()))
      std::filesystem::remove_all(this->path());
    std::filesystem::copy(this->backing_path(), this->path());
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void savefile<F, T>::commit(
  ) const noexcept {
    if(exists(this->backing_path()))
      std::filesystem::remove_all(this->backing_path());
    std::filesystem::copy(this->path(), this->backing_path());
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) savefile<F, T>::operator bool(
  ) const noexcept {
    return this->valid();
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T const& savefile<F, T>::operator()(
  ) const {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T& savefile<F, T>::operator()() {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) std::string savefile<F, T>::read_from_file() const noexcept(false) {
    auto handle = std::ifstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for reading at " + this->path().string());
    return {(std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>()};
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) void savefile<F, T>::write_to_file(std::string_view content) const noexcept(false) {
    namespace fs = std::filesystem;

    if(not fs::exists(this->path().parent_path()))
      fs::create_directories(this->path().parent_path());
    auto handle = std::ofstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for writing at " + this->path().string());
    if(fs::exists(this->path())) {
      auto const permissions = fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read |
                               fs::perms::others_read | fs::perms::others_exec | fs::perms::owner_all |
                               fs::perms::group_all;
      fs::permissions(this->path(), permissions);
    }
    handle << content;
    if(not handle.good())
      throw std::runtime_error("failed to write to file at " + this->path().string());
  }
}  // namespace rolly
