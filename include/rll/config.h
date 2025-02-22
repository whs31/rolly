#pragma once

#include "serialization.h"
#include "types/stdint.h"
#include "io/filedevice.h"

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
   * @details This class is used to store configuration values in a configuration file in given
   * format. Accepts any type that implements @ref
   * rolly::serialization::serializable_and_deserializable with character type <code>char</code> and
   * given format.
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
   * auto rolly::serialization::serialize<fl::serialization::format::toml>(DummyConfiguration const&
   * value) -> std::basic_string<char> { / implementation / }
   *
   * template <>
   * auto rolly::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char>
   * const& value) -> DummyConfiguration { / implementation / }
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
   * @tparam F Format type. Must be value of tag \ref rolly::serialization::format or your own
   * format tag.
   * @tparam T Type to serialize and deserialize. Must implement \ref
   * rolly::serialization::serializable_and_deserializable with character type <code>char</code>.
   * @see rolly::serialization::format
   * @see rolly::serialization::serializable_and_deserializable
   * @see rolly::saving_policy
   */
  template <typename F, typename T>
#ifndef DOXYGEN
  ___requires___((serialization::serializable_and_deserializable<F, T, char>))
#endif
    class configuration_file : public io::filedevice {

   public:
    /**
     * @brief Creates or loads configuration file from given path with saving policy.
     * @note This constructor will not throw any error, but will return an invalid configuration
     * file. Don't forget to check if configuration file is valid before using it.
     * @param path Path to configuration file.
     * @param policy Saving policy.
     * @see saving_policy
     * @see valid
     */
    explicit configuration_file(std::filesystem::path path, saving_policy policy)
      : io::filedevice(std::move(path))
      , default_values_(T())
      , saving_policy_(policy) {
      try {
        this->load();
        this->valid_ = true;
      } catch(std::exception const& ex) {
        this->valid_ = false;
      }
    }

    /**
     * @brief Creates or loads configuration file from given path and folder with saving policy.
     * @note This constructor will not throw any error, but will return an invalid configuration
     * file. Don't forget to check if configuration file is valid before using it.
     * @param filename Filename of configuration file.
     * @param folder Folder where configuration file is located.
     * @param policy Saving policy.
     * @see saving_policy
     * @see valid
     */
    explicit configuration_file(
      std::string_view filename,
      std::filesystem::path const& folder,
      saving_policy policy
    )
      : configuration_file(folder / filename, policy) {}

    configuration_file(configuration_file const&) = default;
    configuration_file(configuration_file&&) = default;

    /**
     * @brief Closes configuration file.
     * @details If saving policy is set to @ref saving_policy::autosave, file will be saved
     * automatically on closing.
     */
    virtual ~configuration_file() noexcept {
      if(this->saving_policy() == saving_policy::autosave) {
        try {
          this->save();
        } catch(std::exception const& ex) {
        }  // NOLINT(*-empty-catch)
      }
    }

    /**
     * @brief Returns whether configuration file is valid or not.
     */
    [[nodiscard]] bool valid() const { return this->valid_; }

    /**
     * @brief Saving policy.
     */
    [[nodiscard]] enum saving_policy saving_policy() const { return this->saving_policy_; }

    /**
     * @brief Constant reference to the current values of configuration file.
     */
    [[nodiscard]] T const& values() const { return this->values_; }

    /**
     * @brief Mutable reference to the current values of configuration file.
     */
    [[nodiscard]] T& values_mut() { return this->values_; }

    /**
     * @brief Default values of configuration file.
     */
    [[nodiscard]] T const& default_values() const { return this->default_values_; }

    /**
     * @brief Mutable reference to default values of configuration file.
     */
    [[nodiscard]] T& default_values_mut() { return this->default_values_; }

    /**
     * @brief Loads configuration file from file.
     * @throws std::exception if loading fails.
     */
    void load() noexcept(false) {
      namespace fs = std::filesystem;
      if(not fs::exists(this->path())) {
        this->revert_to_default();
        return;
      }
      try {
        auto str = this->read();
        this->values_ = serialization::deserialize<F, T>(str);
      } catch(std::exception const& ex) {
        throw;
      }
    }

    /**
     * @brief Saves configuration file to file.
     * @throws std::exception if saving fails.
     */
    void save() const noexcept(false) {
      auto str = serialization::serialize<F, T>(this->values_);
      this->write(str);
    }

    /**
     * @brief Reverts configuration file to default values.
     * @throws std::exception if reverting fails.
     */
    void revert_to_default() noexcept(false) {
      this->values_ = this->default_values_;
      this->save();
    }

    /**
     * @brief Returns whether configuration file is valid or not.
     * @see valid
     */
    [[nodiscard]] explicit operator bool() const noexcept { return this->valid(); }

    /**
     * @brief Constant reference to the current values of configuration file.
     * @see values
     */
    [[nodiscard]] T const& operator()() const { return this->values_; }

    /**
     * @brief Mutable reference to the current values of configuration file.
     * @see values_mut
     */
    [[nodiscard]] T& operator()() { return this->values_; }

    configuration_file& operator=(configuration_file const&) = default;
    configuration_file& operator=(configuration_file&&) = default;

   private:
    T values_;
    T default_values_;
    enum saving_policy saving_policy_;
    bool valid_;
  };
}  // namespace rolly
