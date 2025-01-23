#include <rolly/net.h>

#include <catch2/catch_all.hpp>

using namespace rolly;
using namespace rolly::net;

TEST_CASE("Net", "[net]") {
  SECTION("IPv4") {
    SECTION("Is loopback") {
      REQUIRE(ip_address_v4("127.0.0.1").is_loopback());
      REQUIRE(ip_address_v4("127.0.0.0").is_loopback());
      REQUIRE(ip_address_v4("127.0.0.2").is_loopback());
      REQUIRE_FALSE(ip_address_v4("0.0.0.0").is_loopback());
    }  // SECTION("Is loopback")

    SECTION("Is nonroutable") {
      REQUIRE(ip_address_v4("0.0.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.0.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.1.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("0.255.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.0.255").is_nonroutable());
      REQUIRE(not ip_address_v4("192.0.1.0").is_nonroutable());
      REQUIRE(not ip_address_v4("192.0.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.2.0").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.2.1").is_nonroutable());
      REQUIRE(ip_address_v4("192.0.2.255").is_nonroutable());
      REQUIRE(not ip_address_v4("192.0.3.0").is_nonroutable());
      // 198.18.0.0-198.19.255.255
      REQUIRE(ip_address_v4("198.18.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.18.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.19.255.255").is_nonroutable());

      // 198.51.100.0-198.51.100.255
      REQUIRE(ip_address_v4("198.51.100.0").is_nonroutable());
      REQUIRE(ip_address_v4("198.51.100.1").is_nonroutable());
      REQUIRE(ip_address_v4("198.51.100.255").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.101.0").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.101.1").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.101.255").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.255.0").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.255.1").is_nonroutable());
      REQUIRE(not ip_address_v4("198.51.255.255").is_nonroutable());

      // 203.0.113.0-203.0.113.255
      REQUIRE(ip_address_v4("203.0.113.0").is_nonroutable());
      REQUIRE(ip_address_v4("203.0.113.1").is_nonroutable());
      REQUIRE(ip_address_v4("203.0.113.255").is_nonroutable());
      REQUIRE(not ip_address_v4("203.0.114.0").is_nonroutable());
      REQUIRE(not ip_address_v4("203.0.114.1").is_nonroutable());

      // 224.0.0.0-255.255.255.255
      REQUIRE(ip_address_v4("224.0.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.0.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.1.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.1.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.1.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.255.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.255.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.1.255.255").is_nonroutable());
      REQUIRE(ip_address_v4("224.255.0.0").is_nonroutable());
      REQUIRE(ip_address_v4("224.255.0.1").is_nonroutable());
      REQUIRE(ip_address_v4("224.255.0.255").is_nonroutable());
      REQUIRE(ip_address_v4("255.255.255.255").is_nonroutable());
    }  // SECTION("Is nonroutable")

    SECTION("Is private") {
      // 10.0.0.0-10.255.255.255
      REQUIRE(not ip_address_v4("9.0.0.0").is_private());
      REQUIRE(not ip_address_v4("9.255.255.255").is_private());
      REQUIRE(ip_address_v4("10.0.0.0").is_private());
      REQUIRE(ip_address_v4("10.0.255.255").is_private());
      REQUIRE(ip_address_v4("10.255.255.255").is_private());
      REQUIRE(not ip_address_v4("11.0.0.0").is_private());

      // 127.0.0.0-127.255.255.255
      REQUIRE(not ip_address_v4("126.255.255.255").is_private());
      REQUIRE(ip_address_v4("127.0.0.0").is_private());
      REQUIRE(ip_address_v4("127.0.255.255").is_private());
      REQUIRE(ip_address_v4("127.255.255.255").is_private());
      REQUIRE(not ip_address_v4("128.0.0.0").is_private());

      // 169.254.0.0-169.254.255.255
      REQUIRE(not ip_address_v4("169.253.255.255").is_private());
      REQUIRE(ip_address_v4("169.254.0.0").is_private());
      REQUIRE(ip_address_v4("169.254.255.255").is_private());
      REQUIRE(not ip_address_v4("169.255.0.0").is_private());

      // 172.16.0.0-172.31.255.255
      REQUIRE(not ip_address_v4("172.15.255.255").is_private());
      REQUIRE(ip_address_v4("172.16.0.0").is_private());
      REQUIRE(ip_address_v4("172.31.255.255").is_private());
      REQUIRE(not ip_address_v4("172.32.0.0").is_private());

      // 192.168.0.0-192.168.255.255
      REQUIRE(not ip_address_v4("192.167.255.255").is_private());
      REQUIRE(ip_address_v4("192.168.0.0").is_private());
      REQUIRE(ip_address_v4("192.168.255.255").is_private());
      REQUIRE(not ip_address_v4("192.169.0.0").is_private());

      // 100.64.0.0-100.127.255.255
      REQUIRE(not ip_address_v4("100.63.255.255").is_private());
      REQUIRE(ip_address_v4("100.64.0.0").is_private());
      REQUIRE(ip_address_v4("100.127.255.255").is_private());
      REQUIRE(not ip_address_v4("100.128.0.0").is_private());
    }  // SECTION("Is private")

    SECTION("Is multicast") {
      REQUIRE(not ip_address_v4("0.0.0.0").is_multicast());
      REQUIRE(not ip_address_v4("127.0.0.1").is_multicast());
      REQUIRE(not ip_address_v4("89.207.132.170").is_multicast());
      REQUIRE(not ip_address_v4("89.207.132.170").is_multicast());
      REQUIRE(ip_address_v4("237.84.2.178").is_multicast());
      REQUIRE(ip_address_v4("237.84.2.178").is_multicast());
    }  // SECTION("Is multicast")

    SECTION("Is zero") {
      REQUIRE(ip_address_v4("0.0.0.0").is_zero());
      REQUIRE(not ip_address_v4("127.0.0.1").is_zero());
    }  // SECTION("Is zero")

    SECTION("Format") { REQUIRE(fmt::format("{}", ip_address_v4("127.0.0.1")) == "127.0.0.1"); }

    SECTION("To json") {
      REQUIRE(
        ip_address_v4("127.0.0.1").to_json()
        == R"({family:'AF_INET', addr:'127.0.0.1', hash:7785339717392670637})"
      );
    }

    SECTION("Validate") {
      REQUIRE(ip_address_v4::validate("127.0.0.1"));
      REQUIRE(ip_address_v4::validate("0.0.0.0"));
      REQUIRE_FALSE(ip_address_v4::validate(""));
      REQUIRE_FALSE(ip_address_v4::validate("127.0.0.1.1"));
      REQUIRE_FALSE(ip_address_v4::validate("127.0.0"));
      REQUIRE_FALSE(ip_address_v4::validate("127.0.0.1.1.1"));
      REQUIRE_FALSE(ip_address_v4::validate("127.0.0.1.1.1.1"));
    }  // SECTION("Validate")
  }  // SECTION("IPv4")
}  // TEST_CASE("Net")
