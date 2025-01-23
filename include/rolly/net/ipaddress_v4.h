#pragma once

#include <cstring>
#include <array>
#include <string>
#include <string_view>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <utility>
#include <fmt/ostream.h>
#include "../span.h"
#include "../global/definitions.h"
#include "../global/export.h"
#include "../types/stdint.h"
#include "../types/result.h"
#if defined(ROLLY_QT_NETWORK)
#  include <qhostaddress.h>
#endif

namespace rolly::net {
  class ip_address;
  class ip_address_v4;
  class ip_address_v6;

  /**
   * @brief Pair of ip_address_v4 and netmask.
   */
  using cidr_address_v4 = std::pair<ip_address_v4, u8>;

  /**
   * @brief A representation of an IPv4 address.
   * @details Implementation is partially based on `folly::IpAddressV4` class (see [source code
   * reference](https://github.com/facebook/folly/blob/main/folly/IPAddressV4.h))
   * @see ip_address
   * @see ip_address_v6
   */
  class ROLLY_API ip_address_v4 {
   public:
    ip_address_v4();
    explicit ip_address_v4(std::string_view str);
    explicit ip_address_v4(u32 value);
    explicit ip_address_v4(std::array<u8, 4> const& bytes);
    explicit ip_address_v4(span<u8 const> bytes);

#if defined(ROLLY_QT_NETWORK)
    explicit ip_address_v4(QHostAddress const& address) {
      auto ok = false;
      auto bytes = address.toIPv4Address(&ok);
      if(not ok)
        throw std::invalid_argument("invalid ip address");
      *this = ip_address_v4(bytes);
    }
#endif

    ~ip_address_v4() = default;
    ip_address_v4(ip_address_v4 const&) = default;
    ip_address_v4(ip_address_v4&&) = default;
    ip_address_v4& operator=(ip_address_v4 const&) = default;
    ip_address_v4& operator=(ip_address_v4&&) = default;

    [[nodiscard]] u32 to_u32() const noexcept;
    [[nodiscard]] u32 to_u32_with_host_byte_order() const noexcept;

    [[nodiscard]] std::array<u8, 4> const& bytes() const noexcept { return this->bytes_; }

    [[nodiscard]] std::array<u8, 4> const& to_binary() const noexcept { return this->bytes_; }

    [[nodiscard]] std::string to_string() const;
    //[[nodiscard]] ip_address_v6 to_ipv6() const noexcept; // todo

#if defined(ROLLY_QT_NETWORK)
    [[nodiscard]] QHostAddress to_qhostaddress() const noexcept {
      return QHostAddress(this->to_u32());
    }
#endif

    /**
     * @brief Returns a json representation of the IP address.
     * @details This prints a string representation of the address, for human consumption
     * or logging. The string will take the form of a JSON object that looks like:
     *  `{family:'AF_INET', addr:'address', hash:long}`.
     */
    [[nodiscard]] std::string to_json() const;

    [[nodiscard]] usize hash() const noexcept;

    /**
     * @brief Checks if the address belongs to a subnet.
     * @param subnet Subnet to check address against (e.g. `192.168.1.0`)
     * @param netmask Netmask to check address against (e.g. `24` for `/24`)
     * @returns `true` if the address belongs to the subnet with the given CIDR
     */
    [[nodiscard]] bool in_subnet(ip_address_v4 const& subnet, u8 const& netmask) const noexcept;

    /**
     * @brief Returns `true` if the IP address qualifies as localhost.
     */
    [[nodiscard]] bool is_loopback() const noexcept;

    /**
     * @brief Returns `true` if the IP address is a special purpose address, as defined per RFC
     * 6890 (i.e. `0.0.0.0`).
     */
    [[nodiscard]] bool is_nonroutable() const noexcept;

    /**
     * @brief Returns `true` if the IP address is a private address, as per RFC 1918 and RFC 4193.
     * @details For example, `192.168.xxx.xxx` is a private address.
     */
    [[nodiscard]] bool is_private() const noexcept;

    /**
     * @brief Returns `true` if the IP address is a multicast address.
     */
    [[nodiscard]] bool is_multicast() const noexcept;

    /**
     * @brief Returns `true` the address is all zeros
     */
    [[nodiscard]] bool is_zero() const noexcept;

    /**
     * @brief Returns `true` the address is a link local broadcast address
     */
    [[nodiscard]] bool is_link_local_broadcast() const noexcept;

    /**
     * @brief Creates a new ip_address_v4 with all but most significant bits set to zero.
     * @param mask_bits Number of bits to mask. Must be between 0 and 32.
     * @pre `mask_bits` <= `bit_count`
     * @returns ip_address_v4 with bits set to zero.
     */
    [[nodiscard]] ip_address_v4 mask(usize mask_bits) const noexcept;

    /**
     * @brief Returns the nth byte of the address (zero-indexed).
     */
    [[nodiscard]] u8 byte_at(usize index) const noexcept;

    /**
     * @brief Returns the version of the IP address.
     * @returns 4
     */
    [[nodiscard]] static constexpr u8 version() noexcept { return 4; }

    /**
     * @brief Returns the number of bits in the address.
     * @returns 32
     */
    [[nodiscard]] static constexpr usize bit_count() noexcept { return 32; }

    /**
     * @brief Returns the number of bytes in the address.
     * @returns 4
     */
    [[nodiscard]] static constexpr usize byte_count() noexcept { return 4; }

    /**
     * @brief Return the mask associated with the given number of bits.
     * @details If for instance mask_bits was 24 (e.g. `/24`) then the V4 mask returned should
     * be `{0xff, 0xff, 0xff, 0x00}`.
     *
     * @param [in] mask_bits bitmask to retrieve
     * @pre `mask_bits` > 0 and `mask_bits` <= `bit_count()`
     * @return mask associated with the given number of bits
     */
    [[nodiscard]] static std::array<u8, 4> fetch_mask(usize mask_bits) noexcept;

    [[nodiscard]] static bool validate(std::string_view str) noexcept;
    [[nodiscard]] static ip_address_v4 from_u32(u32 value) noexcept;
    [[nodiscard]] static ip_address_v4 from_u32_with_host_byte_order(u32 value) noexcept;
    [[nodiscard]] static ip_address_v4 from_binary(std::array<u8, 4> const& bytes) noexcept;
    [[nodiscard]] static result<ip_address_v4> try_from_binary(span<u8 const> bytes) noexcept;
    [[nodiscard]] static result<ip_address_v4> try_from_str(std::string_view str) noexcept;

#if defined(ROLLY_QT_NETWORK)
    [[nodiscard]] static result<ip_address_v4> try_from_qhostaddress(QHostAddress const& address) {
      auto ok = false;
      auto bytes = address.toIPv4Address(&ok);
      if(not ok)
        return error("invalid ip address");
      return rolly::ok(ip_address_v4(bytes));
    }
#endif

    friend bool operator==(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return lhs.to_u32() == rhs.to_u32();
    }

    friend bool operator!=(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return not (lhs == rhs);
    }

    friend bool operator<(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return lhs.to_u32_with_host_byte_order() < rhs.to_u32_with_host_byte_order();
    }

    friend bool operator>(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return rhs < lhs;
    }

    friend bool operator<=(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return not (lhs > rhs);
    }

    friend bool operator>=(ip_address_v4 const& lhs, ip_address_v4 const& rhs) noexcept {
      return not (lhs < rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, ip_address_v4 const& ip) {
      return os << ip.to_string();
    }

   private:
    std::array<u8, 4> bytes_;
  };

  static inline auto const localhost_v4 = ip_address_v4::from_u32(0x7f000001);
  static inline auto const broadcast_v4 = ip_address_v4::from_u32(0xffffffff);
  static inline auto const any_v4 = ip_address_v4::from_u32(0x00000000);
}  // namespace rolly::net

template <>
struct std::hash<rolly::net::ip_address_v4> {
  std::size_t operator()(rolly::net::ip_address_v4 const& ip) const noexcept { return ip.hash(); }
};

template <>
struct fmt::formatter<rolly::net::ip_address_v4> : fmt::ostream_formatter {};
