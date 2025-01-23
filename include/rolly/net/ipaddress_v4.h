#pragma once

#include <cstring>
#include <array>
#include <string>
#include <string_view>
#include <functional>
#include <iosfwd>
#include <utility>
#include <fmt/format.h>
#include "../types/stdint.h"
#include "../types/result.h"

namespace rolly {
  namespace net {
    class ip_address;
    class ip_address_v4;
    class ip_address_v6;

    /**
     * @brief Pair of ip_address_v4 and netmask.
     */
    using cidr_address_v4 = std::pair<ip_address_v4, u8>;

    /**
     * @brief A representation of an IPv4 address.
     * @details Implementation is based on `folly::IpAddressV4` class (see [source code
     * reference](https://github.com/facebook/folly/blob/main/folly/IPAddressV4.h))
     * @see ip_address
     * @see ip_address_v6
     */
    class ip_address_v4 {
     public:
      [[nodiscard]] static bool validate(std::string_view str) noexcept;
      [[nodiscard]] static ip_address_v4 from_u32(u32 value) noexcept;
      [[nodiscard]] static ip_address_v4 from_u32_with_host_byte_order(u32 value) noexcept;
      // [[nodiscard]] static ip_address_v4 from_binary(std::array<u8, 4>) todo: span is needed here
      // https://github.com/facebook/folly/blob/main/folly/IPAddressV4.h
    };
  }  // namespace net
}  // namespace rolly
