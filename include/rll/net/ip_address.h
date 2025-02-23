/**
 * @file      ip_address.h
 * @brief     The main include file for the IP-address related functions and types
 * @author    Vladimir Shaleev
 * @copyright MIT License
 *
 * This file includes all the necessary components for working with IP addresses
 * and networks, as well as providing functionality for their manipulation.
 */

#pragma once

#include <ipaddress/ipaddress.hpp>

namespace rll {
  namespace net {
    using ipaddress::ipv4_address;
    using ipaddress::ipv4_network;
    using ipaddress::ipv6_address;
    using ipaddress::ipv6_network;
    using ipaddress::ip_version;
    using ipaddress::format;
    using ipaddress::ip_address;
    using ipaddress::operator!=;
    using ipaddress::operator==;
    using ipaddress::operator%;
    using ipaddress::operator&;
    using ipaddress::operator|;
    using ipaddress::operator*;
    using ipaddress::operator+;
    using ipaddress::operator-;
    using ipaddress::operator/;
    using ipaddress::operator^;
  }  // namespace net

  inline namespace literals {
    using ipaddress::operator""_ip;
    using ipaddress::operator""_ipv4;
    using ipaddress::operator""_ipv4_net;
    using ipaddress::operator""_ipv6;
    using ipaddress::operator""_ipv6_net;
    using ipaddress::operator""_net;
  }  // namespace literals
}  // namespace rll
