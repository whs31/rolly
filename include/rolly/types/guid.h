#pragma once

#include <cstddef>
#include <array>
#include <iosfwd>
#include <utility>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include "./stdint.h"

namespace rolly // NOLINT(*-concat-nested-namespaces)
{
  inline namespace types
  {
    class guid
    {
     public:
      guid();
      explicit guid(std::array<u8, 16> const& bytes);
      explicit guid(std::array<std::byte, 16> const& bytes);
      explicit guid(std::string_view str);

      guid(guid const&) = default;
      guid(guid&&) = default;
      guid& operator=(guid const&) = default;
      guid& operator=(guid&&) = default;

      [[nodiscard]] bool valid() const noexcept;

      [[nodiscard]] std::string to_string() const;
      [[nodiscard]] std::array<u8, 16> const& bytes() const noexcept;
      [[nodiscard]] std::array<u8, 16>& bytes_mut() noexcept;
      [[nodiscard]] u64 to_u64() const noexcept;

      [[nodiscard]] operator bool() const noexcept { // NOLINT(*-explicit-constructor)
        return this->valid();
      }

      [[nodiscard]] bool operator==(guid const& other) const noexcept;
      [[nodiscard]] bool operator!=(guid const& other) const noexcept;

      friend bool operator<(guid const& lhs, guid const& rhs) noexcept;
      friend std::ostream& operator<<(std::ostream& os, guid const& guid);

      [[nodiscard]] static guid empty() noexcept;
      [[nodiscard]] static guid random() noexcept;

     private:
      std::array<u8, 16> bytes_;
    };
  } // namespace types

  inline namespace literals
  {
    /**
     * @brief Literal operator for <tt>guid</tt>.
     * @param str String representation of the <tt>guid</tt>.
     * @return <tt>guid</tt> object.
     * @relates rolly::types::guid
     */
    inline types::guid operator""_guid(char const* str, std::size_t) { return types::guid(str); }
  } // namespace literals

  namespace detail
  {
    template <typename...> struct hash;
    template <typename T> struct hash<T> : public std::hash<T> { using std::hash<T>::hash; };

    template <typename T, typename... Rest>
    struct hash<T, Rest...> {
      inline std::size_t operator()(T const& v, Rest const&... rest) {
        auto seed = std::size_t(hash<Rest...>{}(rest...));
        seed ^= hash<T>{}(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };
  } // namespace detail
} // namespace rolly

namespace std
{
  /**
   * @brief Swaps two <tt>guid</tt>s.
   * @param a First <tt>guid</tt>.
   * @param b Second <tt>guid</tt>.
   * @sa http://en.cppreference.com/w/cpp/memory/swap
   * @relates rolly::types::guid
   */
  [[maybe_unused]] void swap(rolly::guid& a, rolly::guid& b) noexcept {
    std::swap(a.bytes_mut(), b.bytes_mut());
  }

  /**
   * @brief Hashes a <tt>guid</tt>.
   * @tparam T Underlying type of the <tt>guid</tt>.
   * @param b <tt>guid</tt> to hash.
   * @return Hash value.
   * @sa http://en.cppreference.com/w/cpp/utility/hash
   */
  template <>
  struct [[maybe_unused]] hash<rolly::guid>
  {
    [[nodiscard]] std::size_t operator()(rolly::guid const& b) const {
      return rolly::detail::hash<rolly::u64, rolly::u64>{}(b.bytes()[0], b.bytes()[1]);
    }
  };
} // namespace std

/**
 * @brief Specialization of std::formatter for the <tt>guid</tt> class.
 */
template <>
struct [[maybe_unused]] fmt::formatter<rolly::guid> : fmt::formatter<std::string_view>
{
  template <typename FormatContext>
  auto format(rolly::guid const& v, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}", v.to_string());
  }
};