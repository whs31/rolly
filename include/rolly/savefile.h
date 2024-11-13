#pragma once

#include "serialization.h"
#include "types/stdint.h"
#include "io/filedevice.h"

namespace rolly {
  template <typename F, typename T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
#endif
    class savefile : public io::filedevice {

   public:
    explicit savefile(std::filesystem::path path)
      : io::filedevice(std::move(path))
      , backing_path_(this->suffixed_path(".bak")) {
      try {
        this->load();
        this->valid_ = true;
      } catch(std::exception const& ex) {
        this->valid_ = false;
      }
    }

    explicit savefile(std::string_view filename, std::filesystem::path const& folder)
      : savefile(folder / filename) {}

    savefile(savefile const&) = default;
    savefile(savefile&&) = default;

    /**
     * @brief Closes the savefile.
     * @details File will be saved automatically on closing.
     */
    virtual ~savefile() noexcept {
      this->try_save().map_error([&](auto const& err) { fmt::println(stderr, "savefile::~savefile: {}", err); });
    }

    /**
     * @brief Returns whether savefile is valid or not.
     */
    [[nodiscard]] bool valid() const { return this->valid_; }

    /**
     * @brief Path to the savefile's backing file.
     */
    [[nodiscard]] std::filesystem::path const& backing_path() const { return this->backing_path_; }

    [[nodiscard]] bool has_backup() const noexcept { return std::filesystem::exists(this->backing_path()); }

    /**
     * @brief Constant reference to the current values of savefile.
     */
    [[nodiscard]] T const& values() const { return this->values_; }

    /**
     * @brief Mutable reference to the current values of savefile.
     */
    [[nodiscard]] T& values_mut() { return this->values_; }

    void load() {
      namespace fs = std::filesystem;
      if(not fs::exists(this->path())) {
        this->values_ = T();
        this->save();
        return;
      }
      try {
        auto str = this->read();
        this->values_ = serialization::deserialize<F, T>(str);
      } catch(std::exception const& ex) {
        this->invalidate();
        try {
          auto str = this->read();
          this->values_ = serialization::deserialize<F, T>(str);
        } catch(std::exception const& ex2) {
          throw ex2;
        }
      }
    }

    [[nodiscard]] result<> try_load() const noexcept {
      try {
        this->load();
        return {};
      } catch(std::exception const& ex) {
        return error("savefile::try_load: {}", ex.what());
      }
    }

    void save() const {
      auto str = serialization::serialize<F, T>(this->values_);
      this->write(str);
      this->commit();
    }

    [[nodiscard]] result<> try_save() const noexcept {
      auto try_serialize = [this]() -> result<std::string> {
        try {
          return serialization::serialize<F, T>(this->values_);
        } catch(std::exception const& ex) {
          return error("savefile::try_save: {}", ex.what());
        }
      };
      try_serialize().and_then([this](auto const& str) { return this->try_write(str); }
      ).and_then([this]() { return this->try_commit(); });
      return ok();
    }

    /**
     * @brief Swaps the current savefile with its backup.
     * @details Consider using exception-safe @ref try_invalidate instead.
     */
    void invalidate() const {
      if(this->exists())
        std::filesystem::remove_all(this->path());
      std::filesystem::copy(this->backing_path(), this->path());
    }

    [[nodiscard]] result<> try_invalidate() const noexcept {
      try {
        this->invalidate();
        return {};
      } catch(std::exception const& ex) {
        return error("savefile::try_invalidate: {}", ex.what());
      }
    }

    /**
     * @brief Removes previous backup and replaces it with the current one.
     * @details Consider using exception-safe @ref try_commit instead.
     */
    void commit() const {
      if(std::filesystem::exists(this->backing_path()))
        std::filesystem::remove_all(this->backing_path());
      std::filesystem::copy(this->path(), this->backing_path());
    }

    [[nodiscard]] result<> try_commit() const noexcept {
      try {
        this->commit();
        return {};
      } catch(std::exception const& ex) {
        return error("savefile::try_commit: {}", ex.what());
      }
    }

    /**
     * @brief Returns whether the savefile is valid or not.
     * @see valid
     */
    [[nodiscard]] explicit operator bool() const noexcept { return this->valid(); }

    /**
     * @brief Constant reference to the current values of the savefile.
     * @see values
     */
    [[nodiscard]] T const& operator()() const { return this->values_; }

    /**
     * @brief Mutable reference to the current values of the savefile.
     * @see values_mut
     */
    [[nodiscard]] T& operator()() { return this->values_; }

    /**
     * @brief Default copy assignment.
     */
    savefile& operator=(savefile const&) = default;

    /**
     * @brief Default move assignment.
     */
    savefile& operator=(savefile&&) = default;

   private:
    T values_;
    std::filesystem::path backing_path_;
    bool valid_;
  };
}  // namespace rolly
