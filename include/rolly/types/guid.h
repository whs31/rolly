#pragma once

#include <cstddef>
#include <array>
#include <iosfwd>
#include <utility>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include "../global/export.h"
#include "./stdint.h"

namespace rolly  // NOLINT(*-concat-nested-namespaces)
{
  inline namespace types {
    /**
     * @brief 128-bit globally unique identifier (GUID).
     * @details Based on <tt>std::array</tt> container.
     * @sa https://en.wikipedia.org/wiki/Globally_unique_identifier
     */
    class ___rolly_api___ guid {
     public:
      /**
       * @brief Creates an empty <tt>guid</tt>.
       * @see empty
       */
      guid();

      /**
       * @brief Creates a <tt>guid</tt> from an array of bytes.
       * @param bytes Array of bytes.
       */
      explicit guid(std::array<u8, 16> const& bytes);

      /**
       * @brief Creates a <tt>guid</tt> from an array of <tt>std::byte</tt>.
       * @param bytes Array of <tt>std::byte</tt>.
       */
      explicit guid(std::array<std::byte, 16> const& bytes);

      /**
       * @brief Creates a <tt>guid</tt> from a string.
       * @param str String representation of the <tt>guid</tt>.
       */
      explicit guid(std::string_view str);

      guid(guid const&) = default;
      guid(guid&&) = default;
      guid& operator=(guid const&) = default;
      guid& operator=(guid&&) = default;

      /**
       * @brief Checks whether the <tt>guid</tt> is valid or not.
       * @return <tt>true</tt> if the <tt>guid</tt> is valid, <tt>false</tt> otherwise.
       */
      [[nodiscard]] bool valid() const noexcept;

      /**
       * @brief Converts the <tt>guid</tt> to a string.
       * @details String representation is in the form of <tt>xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx</tt>.
       * All hex digits are lowercase.
       * Bytes are separated by hyphens.
       * @return String representation of the <tt>guid</tt>.
       */
      [[nodiscard]] std::string to_string() const;

      /**
       * @brief Gets the bytes of the <tt>guid</tt>.
       * @return Constant reference to the array of bytes.
       */
      [[nodiscard]] std::array<u8, 16> const& bytes() const noexcept;

      /**
       * @brief Gets the mutable bytes of the <tt>guid</tt>.
       * @return Mutable reference to the array of bytes.
       */
      [[nodiscard]] std::array<u8, 16>& bytes_mut() noexcept;

      /**
       * @brief Hashes the <tt>guid</tt> to an unsigned 64-bit integer.
       * @return Hash value.
       */
      [[nodiscard]] u64 to_u64() const noexcept;

      /**
       * @brief Checks whether the <tt>guid</tt> is valid or not.
       * @return <tt>true</tt> if the <tt>guid</tt> is valid, <tt>false</tt> otherwise.
       * @see valid
       */
      [[nodiscard]] operator bool() const noexcept {  // NOLINT(*-explicit-constructor)
        return this->valid();
      }

      /**
       * @brief Checks whether two <tt>guid</tt>s are equal or not.
       * @param other Other <tt>guid</tt>.
       * @return <tt>true</tt> if the <tt>guid</tt>s are equal, <tt>false</tt> otherwise.
       */
      [[nodiscard]] bool operator==(guid const& other) const noexcept;

      /**
       * @brief Checks whether two <tt>guid</tt>s are not equal.
       * @param other Other <tt>guid</tt>.
       * @return <tt>true</tt> if the <tt>guid</tt>s are not equal, <tt>false</tt> otherwise.
       */
      [[nodiscard]] bool operator!=(guid const& other) const noexcept;

      /**
       * @brief Array-like less comparator for <tt>guid</tt>.
       * @param lhs First guid.
       * @param rhs Second guid.
       * @return <tt>true</tt> if lhs is less than rhs, <tt>false</tt> otherwise.
       */
      friend bool operator<(guid const& lhs, guid const& rhs) noexcept;

      /**
       * @brief Prints the <tt>guid</tt> to an output stream.
       * @param os Output stream
       * @param guid <tt>guid</tt>
       * @return Output stream
       * @see to_string
       */
      ___rolly_api___ friend std::ostream& operator<<(std::ostream& os, guid const& guid);

      /**
       * @brief Creates an empty <tt>guid</tt>.
       * @return New empty <tt>guid</tt>.
       */
      [[nodiscard]] static guid empty() noexcept;

      /**
       * @brief Creates a random <tt>guid</tt>.
       * @return New random <tt>guid</tt>.
       */
      [[nodiscard]] static guid random() noexcept;

     private:
      std::array<u8, 16> bytes_;
    };
  }  // namespace types

  inline namespace literals {
    /**
     * @brief Literal operator for <tt>guid</tt>.
     * @param str String representation of the <tt>guid</tt>.
     * @return <tt>guid</tt> object.
     * @relates rolly::types::guid
     */
    inline types::guid operator""_guid(char const* str, [[maybe_unused]] std::size_t unused) {
      return types::guid(str);
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
      inline std::size_t operator()(T const& v, Rest const&... rest) {  // NOLINT(*-redundant-inline-specifier)
        auto seed = std::size_t(hash<Rest...> {}(rest...));
        seed ^= hash<T> {}(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };
  }  // namespace detail
}  // namespace rolly
#endif

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
   * @tparam T Underlying type of the <tt>guid</tt>.
   * @param b <tt>guid</tt> to hash.
   * @return Hash value.
   * @relates rolly::types::guid
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <>
  struct [[maybe_unused]] hash<rolly::guid> {
    [[nodiscard]] std::size_t operator()(rolly::guid const& b) const {
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
