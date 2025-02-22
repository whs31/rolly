#pragma once

#include <cstddef>
#include <array>
#include <iosfwd>
#include <utility>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <rll/global.h>
#include <rll/stdint.h>

namespace rll  // NOLINT(*-concat-nested-namespaces)
{
#ifndef DOXYGEN
  namespace detail {
    [[nodiscard]] constexpr inline std::uint8_t _d(char const* str, int const index) {
      return ((hex_to_char(str[index]) << 4) + hex_to_char(str[index + 1]));  // NOLINT
    }
  }  // namespace detail
#endif

  /**
   * @brief 128-bit globally unique identifier (GUID).
   * @details Based on std::array container.
   * @sa https://en.wikipedia.org/wiki/Globally_unique_identifier
   */
  class RLL_API uuid {
   public:
    /**
     * @brief Length of the guid string representation in the form of
     * `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`.
     */
    constexpr static inline auto short_guid_string_length = static_cast<usize>(36);

    /**
     * @brief Length of the guid string representation in the form of
     * `{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}`.
     */
    constexpr static inline auto long_guid_string_length = static_cast<usize>(38);

    /**
     * @brief Creates an empty guid.
     * @see empty
     */
    constexpr uuid()
      : bytes_ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

    ~uuid() = default;

    /**
     * @brief Creates a guid from an array of bytes.
     * @param bytes Array of bytes.
     */
    constexpr explicit uuid(std::array<u8, 16> const& bytes)
      : bytes_ {bytes} {}

    /**
     * @brief Creates a <tt>guid</tt> from an array of <tt>std::byte</tt>.
     * @param bytes Array of <tt>std::byte</tt>.
     */
    explicit uuid(std::array<std::byte, 16> const& bytes);

    /**
     * @brief Creates a guid from a string.
     * @details String must satisfy the following requirements:
     * - Length of string must be exactly 36 or 38 characters (see @ref short_guid_string_length
     * and @ref long_guid_string_length).
     * - String must be in the form of `{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}` or
     * `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx` (with or without braces).
     * - Bytes must be separated by hyphens.
     * @param str String representation of the guid.
     */
    constexpr explicit uuid(std::string_view const str)
      : bytes_ {} {
      if(str.size() != uuid::short_guid_string_length
         and str.size() != uuid::long_guid_string_length)
        throw std::invalid_argument(
          "guid string initializer must have exactly 36 or 38 characters (see "
          "guid::short_guid_string_length and guid::long_guid_string_length)"
        );
      auto const* data = (str.size() == uuid::short_guid_string_length) ? str.data()
                                                                        : str.data() + 1;
      this->bytes_ =
        {detail::_d(data, 0),
         detail::_d(data, 2),
         detail::_d(data, 4),
         detail::_d(data, 6),
         detail::_d(data, 9),
         detail::_d(data, 11),
         detail::_d(data, 14),
         detail::_d(data, 16),
         detail::_d(data, 19),
         detail::_d(data, 21),
         detail::_d(data, 24),
         detail::_d(data, 26),
         detail::_d(data, 28),
         detail::_d(data, 30),
         detail::_d(data, 32),
         detail::_d(data, 34)};
    }

    uuid(uuid const&) = default;
    uuid(uuid&&) = default;
    uuid& operator=(uuid const&) = default;
    uuid& operator=(uuid&&) = default;

    /**
     * @brief Checks whether the guid is valid or not.
     * @return `true` if the guid is valid, `false` otherwise.
     */
    [[nodiscard]] bool valid() const noexcept { return *this != uuid::empty(); }

    /**
     * @brief Converts the guid to a string.
     * @details String representation is in the form of
     * `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`. All hex digits are lowercase. Bytes are
     * separated by hyphens.
     * @return String representation of the <tt>guid</tt>.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * @brief Gets the bytes of the guid.
     * @return Constant reference to the array of bytes.
     */
    [[nodiscard]] constexpr std::array<u8, 16> const& bytes() const noexcept {
      return this->bytes_;
    }

    /**
     * @brief Gets the mutable bytes of the guid.
     * @return Mutable reference to the array of bytes.
     */
    [[nodiscard]] constexpr std::array<u8, 16>& bytes_mut() noexcept { return this->bytes_; }

    /**
     * @brief Hashes the guid to an unsigned 64-bit integer.
     * @return Hash value.
     */
    [[nodiscard]] u64 to_u64() const noexcept;

    /**
     * @brief Checks whether the guid is valid or not.
     * @return `true` if the guid is valid, `false` otherwise.
     * @see valid
     */
    [[nodiscard]] operator bool() const noexcept {  // NOLINT(*-explicit-constructor)
      return this->valid();
    }

    /**
     * @brief Checks whether two guids are equal or not.
     * @param other Other guid.
     * @return `true` if the guids are equal, `false` otherwise.
     */
    [[nodiscard]] bool operator==(uuid const& other) const noexcept {
      return this->bytes() == other.bytes();
    }

    /**
     * @brief Checks whether two guids are not equal.
     * @param other Other guid.
     * @return `true` if the guids are __not__ equal, `false` otherwise.
     */
    [[nodiscard]] bool operator!=(uuid const& other) const noexcept { return not (*this == other); }

    /**
     * @brief Array-like less comparator for guid.
     * @param lhs First guid.
     * @param rhs Second guid.
     * @return `true` if lhs is less than rhs, `false` otherwise.
     */
    friend bool operator<(uuid const& lhs, uuid const& rhs) noexcept {
      return lhs.bytes() < rhs.bytes();
    }

    /**
     * @brief Prints the guid to an output stream.
     * @param os Output stream
     * @param guid GUID
     * @return Output stream
     * @see to_string
     */
    RLL_API friend std::ostream& operator<<(std::ostream& os, uuid const& guid);

    /**
     * @brief Creates an empty uuid.
     * @return New empty uuid.
     */
    [[nodiscard]] constexpr static uuid empty() noexcept { return {}; }

    /**
     * @brief Creates a random uuid
     * @return New random uuid.
     */
    [[nodiscard]] static uuid random() noexcept;

   private:
    std::array<u8, 16> bytes_;
  };

  inline namespace literals {
    /**
     * @brief Literal operator for the uuid.
     * @param str String representation of the uuid.
     * @param size Size of the string.
     * @return uuid object.
     */
    constexpr inline uuid
      operator""_uuid(char const* str, [[maybe_unused]] std::size_t const size) {
      return uuid(std::string_view(str, size));
    }
  }  // namespace literals

#ifndef DOXYGEN
  namespace detail {
    template <typename...>
    struct hash;

    template <typename T>
    struct hash<T> : public std::hash<T> {
      using std::hash<T>::hash;
    };

    template <typename T, typename... Rest>
    struct hash<T, Rest...> {
      constexpr std::size_t operator()(T const& v, Rest const&... rest) {
        auto seed = static_cast<std::size_t>(hash<Rest...> {}(rest...));
        seed ^= hash<T> {}(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };
  }  // namespace detail
#endif
}  // namespace rll

namespace std {
  /**
   * @brief Swaps two `uuid`s.
   * @param a First uuid.
   * @param b Second uuid.
   * @sa http://en.cppreference.com/w/cpp/memory/swap
   * @relates rll::uuid
   */
  [[maybe_unused]] inline void swap(rll::uuid& a, rll::uuid& b) noexcept {
    std::swap(a.bytes_mut(), b.bytes_mut());
  }

  /**
   * @brief Hashes an `uuid`.
   * @tparam T Underlying type of the `hash`.
   * @param b `uuid` to hash.
   * @return Hash value.
   * @relates rll::uuid
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <>
  struct [[maybe_unused]] hash<rll::uuid> {
    [[nodiscard]] std::size_t operator()(rll::uuid const& b) const noexcept {
      return rll::detail::hash<rll::u64, rll::u64> {}(b.bytes()[0], b.bytes()[1]);
    }
  };
}  // namespace std

/**
 * @brief Specialization of the `fmt::formatter` for the rll::uuid class.
 * @relates rll::uuid
 */
template <>
struct [[maybe_unused]] fmt::formatter<rll::uuid> : ostream_formatter {};
