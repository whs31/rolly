#include <catch2/catch_all.hpp>
#include <toml++/toml.h>
#include <rolly/config.h>

using std::string;
using std::string_view;
using namespace rolly;
namespace fs = std::filesystem;

struct DummyConfiguration
{
  u32 test = 0;
  struct IpAddress
  {
    string ip;
    u16 port;
    struct SockMode
    {
      bool tcp = true;
      bool udp = false;
    } sock_mode{};
  } ip_address = { .ip = "127.0.0.1", .port = 25565 };
};


template <>
std::basic_string<char> rolly::serialization::serialize<rolly::serialization::format::toml>(DummyConfiguration const& value) {
  auto const out = toml::table {
    {"test", value.test},
    {"ip_address", toml::table{
      {"ip", value.ip_address.ip},
      {"port", value.ip_address.port},
      {"sock_mode", toml::table{
        {"tcp", value.ip_address.sock_mode.tcp},
        {"udp", value.ip_address.sock_mode.udp}
      }}
    }}
  };
  auto ss = std::stringstream();
  ss << out;
  return ss.str();
}

template <>
DummyConfiguration rolly::serialization::deserialize<rolly::serialization::format::toml>(std::basic_string<char> const& str) {
  auto self = DummyConfiguration();
  toml::table in;
  try {
    in = toml::parse(str);
  } catch(toml::parse_error const& err) {
    throw serialization_error<serialization::format::toml>(err.what());
  }
  try {
    self.test = in["test"].value<u32>().value();
    self.ip_address = {
      in["ip_address"]["ip"].value<string>().value(),
      in["ip_address"]["port"].value<u16>().value(),
      {
        in["ip_address"]["sock_mode"]["tcp"].value<bool>().value(),
        in["ip_address"]["sock_mode"]["udp"].value<bool>().value()
      }
    };
  } catch(std::bad_optional_access const& err) {
    throw serialization_error<serialization::format::toml>(err.what());
  }
  return self;
}

TEST_CASE("Config") {
  SECTION("Basic Test") {
    namespace format = serialization::format;

    auto config = configuration_file<format::toml, DummyConfiguration>(
      "test.toml",
      fs::current_path() / "test-cfg",
      saving_policy::explicit_
    );

    REQUIRE(config.valid());
    REQUIRE(config().test == 0);
    REQUIRE(config().ip_address.ip == "127.0.0.1");
    REQUIRE(config().ip_address.port == 25565);
    REQUIRE(config().ip_address.sock_mode.tcp == true);
    REQUIRE(config().ip_address.sock_mode.udp == false);

    config().ip_address = {
      "127.0.0.1",
      45555,
      { false, true }
    };
    config.save();
    REQUIRE(config().test == 0);
    REQUIRE(config().ip_address.ip == "127.0.0.1");
    REQUIRE(config().ip_address.port == 45555);
    REQUIRE(config().ip_address.sock_mode.tcp == false);
    REQUIRE(config().ip_address.sock_mode.udp == true);
    config.load();
    REQUIRE(config().test == 0);
    REQUIRE(config().ip_address.ip == "127.0.0.1");
    REQUIRE(config().ip_address.port == 45555);
    REQUIRE(config().ip_address.sock_mode.tcp == false);
    REQUIRE(config().ip_address.sock_mode.udp == true);

    fs::remove_all(fs::current_path() / "test-cfg");
  }
}