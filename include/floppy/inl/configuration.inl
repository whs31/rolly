#pragma once

#ifndef Q_MOC_RUN

namespace floppy
{
  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  configuration_file<F, T>::configuration_file(std::filesystem::path path, enum saving_policy policy)
    : default_values_(T())
    , path_(std::move(path))
    , saving_policy_(policy)
  {
    try {
      this->load();
      this->valid_ = true;
    } catch(std::exception const& ex) {
      log::error("config: failed to load configuration file: {}", ex.what());
      this->valid_ = false;
    }
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  configuration_file<F, T>::configuration_file(
    std::string_view filename,
    std::filesystem::path const& folder,
    enum saving_policy policy
    ) : configuration_file(folder / filename, policy)
  {}

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  configuration_file<F, T>::~configuration_file() noexcept
  {
    if(this->saving_policy() == saving_policy::autosave) {
      try {
        this->save();
      } catch(std::exception const& ex) {
        log::error("config: failed to save configuration file: {}", ex.what());
      }
    }
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::valid() const -> bool { return this->valid_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::path() const -> std::filesystem::path const& { return this->path_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::saving_policy() const -> enum saving_policy { return this->saving_policy_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::values_mut() -> T& { return this->values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::values() const -> T const& { return this->values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::default_values() const -> T const& { return this->default_values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::default_values_mut() -> T& { return this->default_values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::load() noexcept(false) -> void {
    namespace fs = std::filesystem;
    if(not fs::exists(this->path())) {
      log::debug("config: file does not exists, creating default one");
      this->revert_to_default();
      return;
    }
    try {
      auto str = this->read_from_file();
      this->values_ = serialization::deserialize<F, T>(str);
    } catch(std::exception const& ex) {
      throw;
    }
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::save() const noexcept(false) -> void try {
    log::trace("config: saving to {}", this->path().string());
    auto str = serialization::serialize<F, T>(this->values_);
    this->write_to_file(str);
  } catch(std::exception const& ex) {
    throw;
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::revert_to_default() noexcept(false) -> void try {
    this->values_ = this->default_values_;
    this->save();
  } catch(std::exception const& ex) {
    throw;
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  configuration_file<F, T>::operator bool() const noexcept { return this->valid(); }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::operator()() const -> T const& { return this->values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::operator()() -> T& { return this->values_; }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::read_from_file() const noexcept(false) -> std::string {
    auto handle = std::ifstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for reading at " + this->path().string());
    return {(std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>()};
  }

  template <auto F, typename T> requires serialization::serializable_and_deserializable<F, T, char>
  auto configuration_file<F, T>::write_to_file(std::string_view content) const noexcept(false) -> void
  {
    namespace fs = std::filesystem;

    if(not fs::exists(this->path().parent_path()))
      fs::create_directories(this->path().parent_path());
    auto handle = std::ofstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for writing at " + this->path().string());
    if(fs::exists(this->path())) {
      auto const permissions = fs::perms::owner_read
        | fs::perms::owner_write
        | fs::perms::group_read
        | fs::perms::others_read
        | fs::perms::others_exec;
      fs::permissions(this->path(), permissions);
      log::trace("config: rw file permissions set to: {}", this->path().string());
    }
    handle << content;
    if(not handle.good())
      throw std::runtime_error("failed to write to file at " + this->path().string());
    log::trace("config: file ({} bytes) written to path: {}", content.size(), this->path().string());
  }
} // namespace floppy

#endif // Q_MOC_RUN