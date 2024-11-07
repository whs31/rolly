#pragma once

#include <filesystem>
#include <utility>
#include <fstream>
#include "serialization.h"
#include "types/stdint.h"

namespace rolly {
  /**
   * @brief Determines the policy used for autosaving configuration files.
   */
  enum class saving_policy : u8 {
    autosave,  ///< Saves configuration files automatically on closing.
    explicit_  ///< Saves configuration files only explicitly.
  };

  /**
   * @brief Configuration file that can be loaded and saved.
   * @details This class is used to store configuration values in a configuration file in given format.
   * Accepts any type that implements @ref rolly::serialization::serializable_and_deserializable with character type
   * <code>char</code> and given format.
   *
   * Example usage:
   * @code {.cpp}
   * struct DummyConfiguration
   * {
   *   u32 test = 0;
   *   struct IpAddress
   *   {
   *     string ip;
   *     u16 port;
   *     struct SockMode
   *     {
   *       bool tcp = true;
   *       bool udp = false;
   *     } sock_mode{};
   *   } ip_address = { .ip = "127.0.0.1", .port = 25565 };
   * };
   * template <>
   * auto rolly::serialization::serialize<fl::serialization::format::toml>(DummyConfiguration const& value) ->
   * std::basic_string<char> { / implementation / }
   *
   * template <>
   * auto rolly::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char> const& value) ->
   * DummyConfiguration { / implementation / }
   *
   * auto main() -> int
   * {
   *   using enum rolly::saving_policy;
   *   using namespace rolly::serialization::format;
   *
   *   auto config = rolly::configuration_file<format::toml, DummyConfiguration>(
   *     "test.toml",
   *     fs::current_path() / "test-cfg",
   *     explicit_
   *   );
   *   if(not config)
   *     return 1;
   *   std::cout << config().ip_address.ip << std::endl;
   *   try {
   *     config().ip_address.sock_mode.udp = true;
   *     config.save();
   *   } catch(...) {
   *     return 1;
   *   }
   *   return 0;
   * }
   * @endcode
   * @tparam F Format type. Must be value of tag \ref rolly::serialization::format or your own format tag.
   * @tparam T Type to serialize and deserialize. Must implement \ref
   * rolly::serialization::serializable_and_deserializable with character type <code>char</code>.
   * @see rolly::serialization::format
   * @see rolly::serialization::serializable_and_deserializable
   * @see rolly::saving_policy
   */
  template <typename F, typename T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) 
#endif
  class configuration_file {
   public:
    /**
     * @brief Creates or loads configuration file from given path with saving policy.
     * @note This constructor will not throw any error, but will return an invalid configuration file.
     * Don't forget to check if configuration file is valid before using it.
     * @param path Path to configuration file.
     * @param policy Saving policy.
     * @see saving_policy
     * @see valid
     */
    explicit configuration_file(std::filesystem::path path, saving_policy policy);

    /**
     * @brief Creates or loads configuration file from given path and folder with saving policy.
     * @note This constructor will not throw any error, but will return an invalid configuration file.
     * Don't forget to check if configuration file is valid before using it.
     * @param filename Filename of configuration file.
     * @param folder Folder where configuration file is located.
     * @param policy Saving policy.
     * @see saving_policy
     * @see valid
     */
    explicit configuration_file(std::string_view filename, std::filesystem::path const& folder, saving_policy policy);
    configuration_file(configuration_file const&) = default;
    configuration_file(configuration_file&&) = default;

    /**
     * @brief Closes configuration file.
     * @details If saving policy is set to @ref saving_policy::autosave, file will be saved automatically on closing.
     */
    virtual ~configuration_file() noexcept;

    /**
     * @brief Returns whether configuration file is valid or not.
     */
    [[nodiscard]] bool valid() const;

    /**
     * @brief Path to the configuration file.
     */
    [[nodiscard]] std::filesystem::path const& path() const;

    /**
     * @brief Saving policy.
     */
    [[nodiscard]] enum saving_policy saving_policy() const;

    /**
     * @brief Constant reference to the current values of configuration file.
     */
    [[nodiscard]] T const& values() const;

    /**
     * @brief Mutable reference to the current values of configuration file.
     */
    [[nodiscard]] T& values_mut();

    /**
     * @brief Default values of configuration file.
     */
    [[nodiscard]] T const& default_values() const;

    /**
     * @brief Mutable reference to default values of configuration file.
     */
    [[nodiscard]] T& default_values_mut();

    /**
     * @brief Loads configuration file from file.
     * @throws std::exception if loading fails.
     */
    void load() noexcept(false);

    /**
     * @brief Saves configuration file to file.
     * @throws std::exception if saving fails.
     */
    void save() const noexcept(false);

    /**
     * @brief Reverts configuration file to default values.
     * @throws std::exception if reverting fails.
     */
    void revert_to_default() noexcept(false);

    /**
     * @brief Returns whether configuration file is valid or not.
     * @see valid
     */
    [[nodiscard]] explicit operator bool() const noexcept;

    /**
     * @brief Constant reference to the current values of configuration file.
     * @see values
     */
    [[nodiscard]] T const& operator()() const;

    /**
     * @brief Mutable reference to the current values of configuration file.
     * @see values_mut
     */
    [[nodiscard]] T& operator()();

    /**
     * @brief Default copy assignment.
     */
    configuration_file& operator=(configuration_file const&) = default;

    /**
     * @brief Default move assignment.
     */
    configuration_file& operator=(configuration_file&&) = default;

   protected:
    /**
     * @brief Reads content of configuration file from file.
     */
    [[nodiscard]] std::string read_from_file() const noexcept(false);

    /**
     * @brief Writes content of configuration file to a file.
     * @param content Content to write.
     */
    void write_to_file(std::string_view content) const noexcept(false);

   private:
    T values_;
    T default_values_;
    std::filesystem::path path_;
    enum saving_policy saving_policy_;
    bool valid_;
  };

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
    configuration_file<F, T>::configuration_file(std::filesystem::path path, enum saving_policy policy)
    : default_values_(T())
    , path_(std::move(path))
    , saving_policy_(policy) {
    try {
      this->load();
      this->valid_ = true;
    } catch(std::exception const& ex) {
      this->valid_ = false;
    }
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
    configuration_file<F, T>::configuration_file(
      std::string_view filename,
      std::filesystem::path const& folder,
      enum saving_policy policy
    )
    : configuration_file(folder / filename, policy) {}

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
    configuration_file<F, T>::~configuration_file() noexcept {
    if(this->saving_policy() == saving_policy::autosave) {
      try {
        this->save();
      } catch(std::exception const& ex) {
      }  // NOLINT(*-empty-catch)
    }
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) bool configuration_file<F, T>::valid(
  ) const {
    return this->valid_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) std::filesystem::path const& configuration_file<F, T>::path() const {
    return this->path_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) enum saving_policy
    configuration_file<F, T>::saving_policy() const {
    return this->saving_policy_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T& configuration_file<F, T>::values_mut(
  ) {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) T const& configuration_file<F, T>::values() const {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) T const& configuration_file<F, T>::default_values() const {
    return this->default_values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) T& configuration_file<F, T>::default_values_mut() {
    return this->default_values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void configuration_file<F, T>::load(
  ) noexcept(false) {
    namespace fs = std::filesystem;
    if(not fs::exists(this->path())) {
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

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) void configuration_file<F, T>::save(
  ) const noexcept(false) try {
    auto str = serialization::serialize<F, T>(this->values_);
    this->write_to_file(str);
  } catch(std::exception const& ex) {
    throw;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) void configuration_file<F, T>::revert_to_default() noexcept(false) try {
    this->values_ = this->default_values_;
    this->save();
  } catch(std::exception const& ex) {
    throw;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
    configuration_file<F, T>::operator bool() const noexcept {
    return this->valid();
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) T const& configuration_file<F, T>::operator()() const {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) T& configuration_file<F, T>::operator()(
  ) {
    return this->values_;
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)) std::string
    configuration_file<F, T>::read_from_file() const noexcept(false) {
    auto handle = std::ifstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for reading at " + this->path().string());
    return {(std::istreambuf_iterator<char>(handle)), std::istreambuf_iterator<char>()};
  }

  template <typename F, typename T>
  ___requires___((serialization::serializable_and_deserializable<F, T, char>)
  ) void configuration_file<F, T>::write_to_file(std::string_view content) const noexcept(false) {
    namespace fs = std::filesystem;

    if(not fs::exists(this->path().parent_path()))
      fs::create_directories(this->path().parent_path());
    auto handle = std::ofstream(this->path());
    if(not handle.is_open())
      throw std::runtime_error("failed to open file handle for writing at " + this->path().string());
    if(fs::exists(this->path())) {
      auto const permissions = fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read |
                               fs::perms::others_read | fs::perms::others_exec;
      fs::permissions(this->path(), permissions);
    }
    handle << content;
    if(not handle.good())
      throw std::runtime_error("failed to write to file at " + this->path().string());
  }
}  // namespace rolly
