#pragma once

#include <sstream>
#include <rll/serialization.h>
#include <rll/stdint.h>
#include <rll/io/filedevice.h>

namespace rll {
  template <typename F, typename T, typename = std::enable_if_t<is_serializable<T, F>::value>>
  class savefile : public io::filedevice {
   public:
    explicit savefile(std::filesystem::path path)
      : io::filedevice(std::move(path))
      , backing_path_(this->suffixed_path(".bak")) {
      auto const res = this->load();
      this->valid_ = res.has_value();
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
      this->save().map_error([&](auto const& err) {
        fmt::println(stderr, "savefile::~savefile: {}", err);
      });
    }

    /**
     * @brief Returns whether savefile is valid or not.
     */
    [[nodiscard]] bool valid() const { return this->valid_; }

    /**
     * @brief Path to the savefile's backing file.
     */
    [[nodiscard]] std::filesystem::path const& backing_path() const { return this->backing_path_; }

    [[nodiscard]] bool has_backup() const noexcept {
      return std::filesystem::exists(this->backing_path());
    }

    /**
     * @brief Constant reference to the current values of savefile.
     */
    [[nodiscard]] T const& values() const { return this->values_; }

    /**
     * @brief Mutable reference to the current values of savefile.
     */
    [[nodiscard]] T& values_mut() { return this->values_; }

    result<> load() noexcept {
      if(not std::filesystem::exists(this->path())) {
        this->values_ = T();
        return this->save();
      }
      auto str = this->read();
      auto ss = std::stringstream(str);
      auto const res = serializer<T, F, char>::deserialize(ss);
      if(not res)
        return error(res.error());
      this->values_ = *res;
      return ok();
    }

    result<> save() const {
      auto try_serialize = [this]() -> result<std::string> {
        auto ss = std::stringstream();
        auto const res = serializer<T, F, char>::serialize(this->values_, ss);
        if(not res)
          return error(res.error());
        return ok(ss.str());
      };
      try_serialize().and_then([this](auto const& str) { return this->try_write(str); }
      ).and_then([this]() { return this->try_commit(); });
      return ok();
    }

    /**
     * @brief Swaps the current savefile with its backup.
     */
    [[nodiscard]] result<> invalidate() const noexcept {
      try {
        if(this->exists())
          std::filesystem::remove_all(this->path());
        std::filesystem::copy(this->backing_path(), this->path());
        return ok();
      } catch(std::exception const& ex) {
        return error("{}", ex.what());
      }
    }

    /**
     * @brief Removes previous backup and replaces it with the current one.
     */
    [[nodiscard]] result<> try_commit() const noexcept {
      try {
        if(std::filesystem::exists(this->backing_path()))
          std::filesystem::remove_all(this->backing_path());
        std::filesystem::copy(this->path(), this->backing_path());
        return ok();
      } catch(std::exception const& ex) {
        return error("{}", ex.what());
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
}  // namespace rll
