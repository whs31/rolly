#pragma once

#include "base_v4.h"

namespace rolly::net::detail {

  class ipv4_address_base : public base_v4<ipv4_address_base> {
   public:
    using base_type = typename base_v4<ipv4_address_base>::base_type;
    using uint_type = typename base_v4<ipv4_address_base>::uint_type;

    template <uint_type Ip>
    [[nodiscard]] static constexpr ___inline___ ip_address_base<ipv4_address_base> from_uint(
    ) noexcept {
      return from_uint(Ip);
    }

    [[nodiscard]] static constexpr ___inline___ ip_address_base<ipv4_address_base> from_uint(
      uint_type ip
    ) noexcept {
      return ip_from_uint32(ip);
    }

    [[nodiscard]] constexpr ___inline___ uint_type to_uint() const noexcept {
      return ip_to_uint32(_bytes);
    }

    [[nodiscard]] constexpr ___inline___ const base_type& bytes() const noexcept { return _bytes; }

   protected:
    constexpr ___inline___ ipv4_address_base() noexcept = default;

    constexpr ___inline___ explicit ipv4_address_base(base_type const& bytes) noexcept
      : _bytes(bytes) {}

    static ___inline___ void swap(
      ip_address_base<ipv4_address_base>& lhs,
      ip_address_base<ipv4_address_base>& rhs
    ) noexcept {
      lhs._bytes.swap(rhs._bytes);
    }

    [[nodiscard]] static ___inline___ size_t hash(base_type const& bytes) noexcept {
      return std::hash<uint_type> {}(ip_to_uint32(bytes));
    }

    [[nodiscard]] static ___inline___ bool equals(
      ip_address_base<ipv4_address_base> const& lhs,
      ip_address_base<ipv4_address_base> const& rhs
    ) noexcept {
      return lhs._bytes == rhs._bytes;
    }

    [[nodiscard]] static ___inline___ bool less(
      ip_address_base<ipv4_address_base> const& lhs,
      ip_address_base<ipv4_address_base> const& rhs
    ) noexcept {
      return lhs._bytes < rhs._bytes;
    }

   private:
    template <typename>
    friend class ip_network_base;

    base_type _bytes {};
  };  // ipv4_address_base
}  // namespace rolly::net::detail

namespace rolly {
  namespace net {
    /**
     * @brief Alias for the base class specialized for IPv4 address manipulation.
     * @details This alias provides a convenient shorthand for the specialized `ip_address_base`
     * class tailored for IPv4 address handling. It inherits all functionalities from the
     * `ipv4_address_base` class, allowing for operations such as conversion, comparison, and
     * property querying specific to IPv4 addresses.
     */
    using ipv4_address = ip_address_base<detail::ipv4_address_base>;
  }  // namespace net

  inline namespace literals {
    /**
     * @brief User-defined literal for creating an ipv4_address from a string literal.
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    [[nodiscard]] constexpr ___inline___ net::ipv4_address
      operator""_ipv4(char const* address, size_t size) {
      return net::detail::parse_ip_from_literal<
        net::detail::ipv4_address_base,
        char,
        net::ipv4_address::base_max_string_len>(address, size);
    }

    /**
     * @brief User-defined literal for creating an ipv4_address from a wide string literal.
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    [[nodiscard]] constexpr ___inline___ net::ipv4_address
      operator""_ipv4(wchar_t const* address, size_t size) noexcept {
      return net::detail::parse_ip_from_literal<
        net::detail::ipv4_address_base,
        wchar_t,
        net::ipv4_address::base_max_string_len>(address, size);
    }

    /**
     * @brief User-defined literal for creating an ipv4_address from a UTF-16 string literal.
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    [[nodiscard]] constexpr ___inline___ net::ipv4_address
      operator""_ipv4(char16_t const* address, size_t size) noexcept {
      return net::detail::parse_ip_from_literal<
        net::detail::ipv4_address_base,
        char16_t,
        net::ipv4_address::base_max_string_len>(address, size);
    }

    /**
     * @brief User-defined literal for creating an ipv4_address from a UTF-32 string literal.
     * @param[in] address A pointer to a character array representing the IPv4 address.
     * @param[in] size The size of the character array.
     * @return An ipv4_address object parsed from the string literal.
     */
    [[nodiscard]] constexpr ___inline___ net::ipv4_address
      operator""_ipv4(char32_t const* address, size_t size) noexcept {
      return net::detail::parse_ip_from_literal<
        net::detail::ipv4_address_base,
        char32_t,
        net::ipv4_address::base_max_string_len>(address, size);
    }

    /**
     * @brief User-defined literal for creating an ipv4_address from an unsigned long long integer.
     * @param[in] value An unsigned long long integer representing the IPv4 address in host byte
     * order.
     * @return An ipv4_address object created from the integer.
     */
    [[nodiscard]] constexpr ___inline___ net::ipv4_address operator""_ipv4(unsigned long long value
    ) noexcept {
      return net::ipv4_address::from_uint(u32(value));
    }
  }  // namespace literals
}  // namespace rolly
