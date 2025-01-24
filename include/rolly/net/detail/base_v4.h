#pragma once

#include <array>
#include <limits>
#include "ip_base.h"

namespace rolly::net::detail {
  template <typename Ext>
  class base_v4 {
   public:
    using base_type = std::array<u8, 4>;
    using uint_type = uint32_t;

    static constexpr ip_version base_version = ip_version::v4;
    static constexpr std::size_t base_size = 4;
    static constexpr std::size_t base_max_string_len = 15;
    static constexpr std::size_t base_max_prefixlen = base_size * 8;
    static constexpr uint_type base_all_ones = std::numeric_limits<uint_type>::max();

    [[nodiscard]] constexpr ___inline___ ip_version version() const noexcept {
      return this->base_version;
    }

    [[nodiscard]] constexpr ___inline___ std::size_t size() const noexcept {
      return this->base_size;
    }

   protected:
    template <typename Iter>
    [[nodiscard]] static constexpr ___inline___ ip_address_base<Ext>
      ip_from_string(Iter begin, Iter end, bool& error, uint32_t& index) noexcept {
      if(begin == end) {
        error = true;
        return {};
      }

      base_type octets = {};
      char first_symbol = '\0';
      int digits = 0;
      int octet = 0;

      index = 0;
      error = false;
      Iter it = begin;
      uint32_t error_symbol = 0;

      while(it < end) {
        auto c = rolly::detail::next_char_or_error(it, end, error, error_symbol);
        if(error) {
          index = error_symbol;
          return {};
        }
        if(index >= 4) {
          error = true;
          return {};
        }
        if(c >= '0' && c <= '9') {
          if(digits > 0 && first_symbol == '0') {
            error = true;
            return {};
          } else if(digits == 0)
            first_symbol = c;
          auto d = c - '0';
          octet = octet * 10 + d;
          ++digits;
          if(digits > 3) {
            error = true;
            return {};
          }
        } else if(c == '.' && digits > 0) {
          if(octet > 255) {
            error = true;
            return {};
          }
          octets[index++] = uint8_t(octet & 0xFF);
          digits = 0;
          octet = 0;
        } else if(c == '.') {
          error = true;
          return {};
        } else {
          error = true;
          return {};
        }
      }
      if(index != 3) {
        error = true;
        return {};
      }
      if(digits == 0) {
        error = true;
        return {};
      }
      if(octet > 255) {
        error = true;
        return {};
      }
      octets[index] = uint8_t(octet & 0xFF);
      return ip_address_base<Ext>(octets);
    }

    [[nodiscard]] static constexpr ___inline___ ip_address_base<Ext> ip_from_uint32(
      uint_type ip
    ) noexcept {
      ip = endian::native == endian::little ? byteswap(ip) : ip;
      return ip_address_base<Ext>(
        {static_cast<u8>(ip & 0xFF),
         static_cast<u8>((ip >> 8) & 0xFF),
         static_cast<u8>((ip >> 16) & 0xFF),
         static_cast<u8>((ip >> 24) & 0xFF)}
      );
    }

    [[nodiscard]] static constexpr ___inline___ ip_address_base<Ext> ip_from_prefix(
      size_t prefixlen
    ) {
      return prefixlen != 0 ? ip_address_base<Ext>::ip_from_uint32(
                                base_all_ones ^ (base_all_ones >> (prefixlen - 1) >> 1)
                              )  // NOLINT
                            : ip_address_base<Ext>::ip_from_uint32(0);
    }

    [[nodiscard]] static constexpr ___inline___ uint_type
      ip_to_uint32(base_type const& bytes) noexcept {
      auto const ip = (static_cast<u32>(bytes[3]) << 24) | (static_cast<u32>(bytes[2]) << 16)
                    | (static_cast<u32>(bytes[1]) << 8) | static_cast<u32>(bytes[0]);
      return endian::native == endian::little ? byteswap(ip) : ip;
    }

    [[nodiscard]] static constexpr ___inline___ size_t ip_to_chars(
      base_type const& bytes,
      format /*fmt*/,
      char (&result)[base_max_string_len + 1]
    ) noexcept {
      size_t offset = 0;
      char buffer[4] {};
      for(size_t b = 0; b < 4; ++b) {
        size_t const length = byte_to_string(bytes[b], buffer);
        for(size_t i = 0; i < length; ++i)
          result[offset++] = buffer[i];
        if(b < 3)
          result[offset++] = '.';
      }
      result[offset] = '\0';
      return offset;
    }

    [[nodiscard]] static ___inline___ std::string ip_reverse_pointer(base_type const& bytes) {
      return ip_from_uint32(byteswap(ip_to_uint32(bytes))).to_string() + ".in-addr.arpa";
    }

    template <typename Iter>
    [[nodiscard]] static constexpr ___inline___ std::tuple<ip_address_base<Ext>, size_t>
      parse_netmask(Iter begin, Iter end, bool& error, uint32_t& code_value) noexcept {
      error = false;
      code_value = 0;
      Iter it = begin;

      size_t prefixlen = 0;
      auto is_value = true;
      auto has_prefixlen = false;

      while(it < end) {
        has_prefixlen = true;
        auto const c = rolly::detail::next_char_or_error(it, end, error, code_value);
        if(error)
          return std::make_tuple(ip_address_base<Ext>(), 0);
        if(c >= '0' && c <= '9')
          prefixlen = prefixlen * 10 + (c - '0');
        else {
          is_value = false;
          break;
        }
      }
      if(is_value) {
        if(prefixlen > base_max_prefixlen) {
          error = true;
          return std::make_tuple(ip_address_base<Ext>(), 0);
        }
      } else {
        auto ip = ip_to_uint32(ip_from_string(begin, end, error, code_value).bytes());
        if(error) {
          error = true;
          return std::make_tuple(ip_address_base<Ext>(), 0);
        }

        prefixlen = prefix_from_ip_uint32(ip, error);
        if(error) {
          ip = ip ^ base_all_ones;
          error = false;
          prefixlen = prefix_from_ip_uint32(ip, error);
          if(error)
            return std::make_tuple(ip_address_base<Ext>(), 0);
        }
      }
      prefixlen = has_prefixlen ? prefixlen : base_max_prefixlen;
      auto netmask = ip_from_prefix(prefixlen);
      return std::make_tuple(netmask, prefixlen);
    }

    [[nodiscard]] static constexpr ___inline___ ip_address_base<Ext> strict_netmask(
      ip_address_base<Ext> const& address,
      ip_address_base<Ext> const& netmask,
      bool strict,
      bool& error
    ) noexcept {
      auto pack_address = address.to_uint();
      auto pack_netmask = netmask.to_uint();
      if((pack_address & pack_netmask) != pack_address) {
        if(strict) {
          error = true;
          return ip_address_base<Ext>();
        }
        return ip_from_uint32(pack_address & pack_netmask);
      }
      return address;
    }

   private:
    [[nodiscard]] static constexpr ___inline___ size_t
      prefix_from_ip_uint32(uint_type ip, bool& error) noexcept {
      auto trailing_zeroes = count_righthand_zero_bits(ip, base_max_prefixlen);
      auto prefixlen = base_max_prefixlen - trailing_zeroes;
      auto leading_ones = trailing_zeroes != 32 ? (ip >> trailing_zeroes) : 0;
      auto all_ones = (uint_type(1) << (prefixlen - 1) << 1) - uint_type(1);  // NOLINT
      if(leading_ones != all_ones) {
        error = true;
        return 0;
      }
      return prefixlen;
    }

    [[nodiscard]] static constexpr ___inline___ size_t
      count_righthand_zero_bits(uint32_t number, uint32_t bits) noexcept {
      if(number == 0) {
        return bits;
      } else {
        number = (~number & (number - 1));
        size_t count = 0;
        while(number != 0) {
          count += number & 0x1;
          number >>= 1;
        }
        if(bits < count)
          return bits;
        return count;
      }
    }

    [[nodiscard]] static constexpr ___inline___ size_t
      byte_to_string(uint8_t byte, char (&str)[4]) noexcept {
      size_t length = 0;
      char* p = str;
      do {
        *p++ = char((byte % 10) + '0');
        byte /= 10;
        ++length;
      } while(byte != 0);

      *p = '\0';
      for(size_t i = 0, midle = length / 2; i < midle; ++i) {
        auto tmp = str[i];
        str[i] = *--p;
        *p = tmp;
      }
      return length;
    }
  };

  template <typename Ext>
  constexpr ip_version base_v4<Ext>::base_version;

  template <typename Ext>
  constexpr size_t base_v4<Ext>::base_size;

  template <typename Ext>
  constexpr size_t base_v4<Ext>::base_max_string_len;

  template <typename Ext>
  constexpr size_t base_v4<Ext>::base_max_prefixlen;

  template <typename Ext>
  constexpr typename base_v4<Ext>::uint_type base_v4<Ext>::base_all_ones;
}  // namespace rolly::net::detail
