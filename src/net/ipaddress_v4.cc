#include <rolly/net/ipaddress_v4.h>

#include <sstream>
#include <ctre.hpp>
#include <oslayer/inet.h>

namespace rolly::net {
  ip_address_v4::ip_address_v4()
    : bytes_({0, 0, 0, 0}) {}

  ip_address_v4::ip_address_v4(std::string_view const str)
    : bytes_({0, 0, 0, 0}) {
    if(not validate(str))
      throw std::runtime_error("invalid ipv4 address format");
    auto ss = std::stringstream(std::string(str));
    auto segment = std::string();
    auto idx = 0u;
    while(std::getline(ss, segment, '.')) {
      if(idx >= 4)
        throw std::runtime_error("ipv4 address has more than 4 octets");
      auto const val = std::stoi(segment);
      if(val < 0 or val > 255)
        throw std::runtime_error("invalid ipv4 address: octet out of range");
      this->bytes_[idx++] = static_cast<u8>(val);  // NOLINT(*-pro-bounds-constant-array-index)
    }
    if(idx != 4)
      throw std::runtime_error("ipv4 address has less than 4 octets");
  }

  ip_address_v4::ip_address_v4(u32 const value)
    : bytes_(
        {static_cast<u8>(value >> 24),
         static_cast<u8>(value >> 16 & 0xFF),
         static_cast<u8>(value >> 8 & 0xFF),
         static_cast<u8>(value & 0xFF)}
      ) {}

  ip_address_v4::ip_address_v4(std::array<u8, 4> const& bytes)
    : bytes_(bytes) {}

  ip_address_v4::ip_address_v4(span<u8 const> const bytes) {
    if(bytes.size() != 4)
      throw std::runtime_error("ipv4 address must have 4 octets");
    std::copy(bytes.begin(), bytes.end(), this->bytes_.begin());
  }

  u32 ip_address_v4::to_u32() const noexcept {
    return this->bytes_[0] << 24 | this->bytes_[1] << 16 | this->bytes_[2] << 8 | this->bytes_[3];
  }

  u32 ip_address_v4::to_u32_with_host_byte_order() const noexcept { return ntohl(this->to_u32()); }

  std::string ip_address_v4::to_string() const {
    return fmt::format(
      "{}.{}.{}.{}",
      this->bytes_[0],
      this->bytes_[1],
      this->bytes_[2],
      this->bytes_[3]
    );
  }

  // ip_address_v6 ip_address_v4::to_ipv6() const noexcept {}

  std::string ip_address_v4::to_json() const {
    return fmt::format("{{family:'AF_INET', addr:'{}', hash:{}}}", this->to_string(), this->hash());
  }

  usize ip_address_v4::hash() const noexcept {
    return std::hash<u32> {}(this->to_u32_with_host_byte_order());
  }

  bool ip_address_v4::in_subnet(ip_address_v4 const& subnet, u8 const& netmask) const noexcept {
    auto const ip = this->to_u32_with_host_byte_order();
    auto const subnet_ip = subnet.to_u32_with_host_byte_order();
    return (ip & netmask) == (subnet_ip & netmask);
  }

  bool ip_address_v4::is_loopback() const noexcept {
    static auto const loopback_addr = ip_address_v4("127.0.0.0");
    return this->in_subnet(loopback_addr, 8);
  }

  bool ip_address_v4::is_nonroutable() const noexcept {
    auto const ip = this->to_u32();
    return this->is_private() or ip <= 0x00FFFFFF or   // 0.0.0.0-0.255.255.255
           (ip >= 0xC0000000 and ip <= 0xC00000FF) or  // 192.0.0.0-192.0.0.255
           (ip >= 0xC0000200 and ip <= 0xC00002FF) or  // 192.0.2.0-192.0.2.255
           (ip >= 0xC6120000 and ip <= 0xC613FFFF) or  // 198.18.0.0-198.19.255.255
           (ip >= 0xC6336400 and ip <= 0xC63364FF) or  // 198.51.100.0-198.51.100.255
           (ip >= 0xCB007100 and ip <= 0xCB0071FF) or  // 203.0.113.0-203.0.113.255
           (ip >= 0xE0000000 and ip <= 0xFFFFFFFF)     // 224.0.0.0-255.255.255.255
      ;
  }

  bool ip_address_v4::is_private() const noexcept {
    auto const ip = this->to_u32();
    return (ip >= 0x0A000000 and ip <= 0x0AFFFFFF) or  // 10.0.0.0-10.255.255.255
           (ip >= 0x7F000000 and ip <= 0x7FFFFFFF) or  // 127.0.0.0-127.255.255.255
           (ip >= 0xA9FE0000 and ip <= 0xA9FEFFFF) or  // 169.254.0.0-169.254.255.255
           (ip >= 0xAC100000 and ip <= 0xAC1FFFFF) or  // 172.16.0.0-172.31.255.255
           (ip >= 0xC0A80000 and ip <= 0xC0A8FFFF) or  // 192.168.0.0-192.168.255.255
           (ip >= 0x64400000 and ip <= 0x647fffff)     // 100.64.0.0-100.127.255.255
      ;
  }

  bool ip_address_v4::is_multicast() const noexcept {
    auto const ip = this->to_u32();
    return (ip & 0xF0000000) == 0xE0000000;
  }

  bool ip_address_v4::is_zero() const noexcept {
    auto constexpr zero = std::array<u8, 4> {0, 0, 0, 0};
    return std::memcmp(this->bytes_.data(), zero.data(), 4) == 0;
  }

  bool ip_address_v4::is_link_local_broadcast() const noexcept {
    auto constexpr link_local = std::array<u8, 4> {0xFE, 0x80, 0, 0};
    return std::memcmp(this->bytes_.data(), link_local.data(), 4) == 0;
  }

  ip_address_v4 ip_address_v4::mask(usize const mask_bits) const noexcept {
    contracts::precondition(mask_bits <= ip_address_v4::bit_count(), "mask bits out of range");

    auto const mask = ip_address_v4::fetch_mask(mask_bits);
    auto const mask_as_u32 = static_cast<u32>(mask[0]) << 24 | static_cast<u32>(mask[1]) << 16
                           | static_cast<u32>(mask[2]) << 8 | static_cast<u32>(mask[3]);
    return ip_address_v4(this->to_u32_with_host_byte_order() & mask_as_u32);
  }

  u8 ip_address_v4::byte_at(usize const index) const noexcept {
    contracts::precondition(index < 4, "index out of range");
    return this->bytes_[index];
  }

  std::array<u8, 4> ip_address_v4::fetch_mask(usize const mask_bits) noexcept {
    contracts::precondition(mask_bits <= ip_address_v4::bit_count(), "mask bits out of range");

    auto mask = std::array<u8, 4> {};
    auto const mask_bytes = mask_bits / 8;
    std::memset(mask.data(), 0xFF, mask_bytes);
    if(mask_bits % 8 != 0)
      mask[mask_bytes] = (1 << (mask_bits % 8)) - 1;
    return mask;
  }

  bool ip_address_v4::validate(std::string_view const str) noexcept {
    static auto constexpr pattern = ctll::fixed_string {
      "^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\\.){3}"
      "(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])$"
    };
    return ctre::match<pattern>(str);
  }

  ip_address_v4 ip_address_v4::from_u32(u32 const value) noexcept { return ip_address_v4(value); }

  ip_address_v4 ip_address_v4::from_u32_with_host_byte_order(u32 const value) noexcept {
    return ip_address_v4(htonl(value));
  }

  ip_address_v4 ip_address_v4::from_binary(std::array<u8, 4> const& bytes) noexcept {
    return ip_address_v4(bytes);
  }

  result<ip_address_v4> ip_address_v4::try_from_binary(span<u8 const> const bytes) noexcept {
    try {
      return ip_address_v4(bytes);
    } catch(std::exception const& e) {
      return error("{}", e.what());
    }
  }

  result<ip_address_v4> ip_address_v4::try_from_str(std::string_view const str) noexcept {
    try {
      return ip_address_v4(str);
    } catch(std::exception const& e) {
      return error("{}", e.what());
    }
  }
}  // namespace rolly::net
