#pragma once

#include <cstddef>
#include <array>
#include <iosfwd>
#include <utility>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include "../global/export.h"
#include "../global/char_utils.h"
#include "./stdint.h"

namespace rolly  // NOLINT(*-concat-nested-namespaces)
{
#ifndef DOXYGEN_GENERATING_OUTPUT
  namespace detail {
    [[nodiscard]] constexpr inline std::uint8_t _d(char const* str, int const index) {
      return ((hex_to_char(str[index]) << 4) + hex_to_char(str[index + 1]));  // NOLINT
    }
  }  // namespace detail
#endif

  inline namespace types {
    /**
     * @brief 128-bit globally unique identifier (GUID).
     * @details Based on std::array container.
     * @sa https://en.wikipedia.org/wiki/Globally_unique_identifier
     */

    class ___rolly_api___ guid {
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
      constexpr guid()
        : bytes_ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

      ~guid() = default;

      /**
       * @brief Creates a guid from an array of bytes.
       * @param bytes Array of bytes.
       */
      constexpr explicit guid(std::array<u8, 16> const& bytes)
        : bytes_ {bytes} {}

      /**
       * @brief Creates a <tt>guid</tt> from an array of <tt>std::byte</tt>.
       * @param bytes Array of <tt>std::byte</tt>.
       */
      explicit guid(std::array<std::byte, 16> const& bytes);

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
      constexpr explicit guid(std::string_view const str) {
        if(str.size() != guid::short_guid_string_length
           and str.size() != guid::long_guid_string_length)
          throw std::invalid_argument(
            "guid string initializer must have exactly 36 or 38 characters (see "
            "guid::short_guid_string_length and guid::long_guid_string_length)"
          );
        auto const* data = (str.size() == guid::short_guid_string_length) ? str.data()
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

      guid(guid const&) = default;
      guid(guid&&) = default;
      guid& operator=(guid const&) = default;
      guid& operator=(guid&&) = default;

      /**
       * @brief Checks whether the guid is valid or not.
       * @return `true` if the guid is valid, `false` otherwise.
       */
      [[nodiscard]] constexpr bool valid() const noexcept { return *this != guid::empty(); }

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
      [[nodiscard]] constexpr operator bool() const noexcept {  // NOLINT(*-explicit-constructor)
        return this->valid();
      }

      /**
       * @brief Checks whether two guids are equal or not.
       * @param other Other guid.
       * @return `true` if the guids are equal, `false` otherwise.
       */
      [[nodiscard]] constexpr bool operator==(guid const& other) const noexcept {
        return this->bytes() == other.bytes();
      }

      /**
       * @brief Checks whether two guids are not equal.
       * @param other Other guid.
       * @return `true` if the guids are __not__ equal, `false` otherwise.
       */
      [[nodiscard]] constexpr bool operator!=(guid const& other) const noexcept {
        return not (*this == other);
      }

      /**
       * @brief Array-like less comparator for guid.
       * @param lhs First guid.
       * @param rhs Second guid.
       * @return `true` if lhs is less than rhs, `false` otherwise.
       */
      friend constexpr bool operator<(guid const& lhs, guid const& rhs) noexcept {
        return lhs.bytes() < rhs.bytes();
      }

      /**
       * @brief Prints the guid to an output stream.
       * @param os Output stream
       * @param guid GUID
       * @return Output stream
       * @see to_string
       */
#ifndef DOXYGEN_GENERATING_OUTPUT
      ___rolly_api___
#endif
        friend std::ostream&
        operator<<(std::ostream& os, guid const& guid);

      /**
       * @brief Creates an empty guid.
       * @return New empty guid.
       */
      [[nodiscard]] constexpr static guid empty() noexcept { return {}; }

      /**
       * @brief Creates a random guid
       * @return New random guid.
       */
      [[nodiscard]] static guid random() noexcept;

     private:
      std::array<u8, 16> bytes_;
    };
  }  // namespace types

  inline namespace literals {
    /**
     * @brief Literal operator for guid.
     * @param str String representation of the guid.
     * @return guid object.
     */
    constexpr inline types::guid
      operator""_guid(char const* str, [[maybe_unused]] std::size_t size) {
      return types::guid(std::string_view(str, size));
    }
  }  // namespace literals

#ifndef DOXYGEN_GENERATING_OUTPUT
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
}  // namespace rolly

namespace std {
  /**
   * @brief Swaps two <tt>guid</tt>s.
   * @param a First <tt>guid</tt>.
   * @param b Second <tt>guid</tt>.
   * @sa http://en.cppreference.com/w/cpp/memory/swap
   * @relates rolly::types::guid
   */
  [[maybe_unused]] inline void swap(rolly::guid& a, rolly::guid& b) noexcept {
    std::swap(a.bytes_mut(), b.bytes_mut());
  }

  /**
   * @brief Hashes a <tt>guid</tt>.
   * @tparam T Underlying type of the <tt>hash</tt>.
   * @param b <tt>guid</tt> to hash.
   * @return Hash value.
   * @relates rolly::types::guid
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <>
  struct [[maybe_unused]] hash<rolly::guid> {
    [[nodiscard]] std::size_t operator()(rolly::guid const& b) const noexcept {
      return rolly::detail::hash<rolly::u64, rolly::u64> {}(b.bytes()[0], b.bytes()[1]);
    }
  };
}  // namespace std

/**
 * @brief Specialization of the <code>fmt::formatter</code> for the @ref rolly::guid class.
 * @relates rolly::guid
 */
template <>
struct [[maybe_unused]] fmt::formatter<rolly::guid> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  auto format(rolly::guid const& v, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}", v.to_string());
  }
};
