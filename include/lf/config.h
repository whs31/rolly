#pragma once

#include <filesystem>
#include <utility>
#include <fstream>
#include <lf/leaf.h>

namespace lf::config
{
  enum class SavingPolicy
  {
    Autosave,
    Explicit
  };

  template <trait::SerializableDeserializable T>
  class ConfigurationFile
  {
   public:
    T values;
    T defaults;

    explicit ConfigurationFile(std::filesystem::path path, SavingPolicy policy);
    explicit ConfigurationFile(std::string_view filename, std::filesystem::path const& folder, SavingPolicy policy);
    virtual ~ConfigurationFile();

    [[nodiscard]] auto path() const -> std::filesystem::path const&;
    [[nodiscard]] auto saving_policy() const -> SavingPolicy;

    [[nodiscard]] auto load() -> Result<>;
    [[nodiscard]] auto save() const -> Result<>;
    auto revert_to_defaults() -> void;

   protected:
    [[nodiscard]] auto read_from_file() const -> Result<std::string>;
    [[nodiscard]] auto write_to_file(std::string_view content) const -> Result<>;

   private:
    std::filesystem::path m_path;
    SavingPolicy m_policy;
  };
}

namespace lf::config
{
  template <trait::SerializableDeserializable T>
  ConfigurationFile<T>::ConfigurationFile(
    std::filesystem::path path,
    SavingPolicy policy
  )
    : defaults(T())
    , m_policy(policy)
    , m_path(std::move(path))
  {
    this->load()
      .map_error([this](auto const& e) {
        llog::error("failed to load configuration file from path: {}, reason: {}", this->path().string(), e);
      });
  }

  template <trait::SerializableDeserializable T>
  ConfigurationFile<T>::ConfigurationFile(
    std::string_view filename,
    std::filesystem::path const& folder,
    SavingPolicy policy
  )
    : ConfigurationFile(folder / filename, policy)
  {}

  template <trait::SerializableDeserializable T>
  ConfigurationFile<T>::~ConfigurationFile() {
    if(this->saving_policy() == SavingPolicy::Autosave) {
      this->save()
        .map_error([this](auto const& e) {
          llog::error("failed to preform automatic saving of configuration file to path: {}, reason: {}", this->path().string(), e);
        });
    }
  }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::path() const -> std::filesystem::path const& { return this->m_path; }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::saving_policy() const -> SavingPolicy { return this->m_policy; }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::load() -> Result<>
  {
    namespace fs = std::filesystem;
    if(not fs::exists(this->path())) {
      llog::debug("config: file does not exists, using defaults");
      this->revert_to_defaults();
      return Ok();
    }
    return this->read_from_file()
      .and_then([this](auto const&& content) -> Result<> {
        llog::debug("config: loaded config file from path: {}", this->path().string());
        return this->values.deserialize(content);
      });
  }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::save() const -> Result<>
  {
    llog::debug("config: saving config file to path: {}", this->path().string());
    return this->values.serialize()
      .and_then([this](auto const&& content) -> Result<> {
        return this->write_to_file(content);
      });
  }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::revert_to_defaults() -> void
  {
    this->values = this->defaults;
    llog::debug("config: reverted to defaults");
    this->save()
      .map_error([](auto const& e) {
        llog::error("failed to save configuration file when reverting to defaults, reason: {}", e);
      });
  }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::read_from_file() const -> Result<std::string>
  {
    auto handle = std::ifstream(this->path());
    if(not handle.is_open())
      return Error("failed to open file handle for reading at {}", this->path().string());
    auto content = std::string((std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>());
    return Ok(std::move(content));
  }

  template <trait::SerializableDeserializable T>
  auto ConfigurationFile<T>::write_to_file(std::string_view content) const -> Result<>
  {
    namespace fs = std::filesystem;

    if(not fs::exists(this->path().parent_path()))
      fs::create_directories(this->path().parent_path());
    auto handle = std::ofstream(this->path());
    if(not handle.is_open())
      return Error("failed to open file handle for writing at {}", this->path().string());
    if(fs::exists(this->path())) {
      auto const permissions = fs::perms::owner_read
        | fs::perms::owner_write
        | fs::perms::group_read
        | fs::perms::others_read
        | fs::perms::others_exec;
      fs::permissions(this->path(), permissions);
      llog::trace("config: rw file permissions set to: {}", this->path().string());
    }
    handle << content;
    if(not handle.good())
      return Error("failed to write to file at {}", this->path().string());
    llog::trace("config: file ({} bytes) written to path: {}", content.size(), this->path().string());
    return Ok();
  }
}