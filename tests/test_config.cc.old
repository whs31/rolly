#include <cstdint>
#include <gtest/gtest.h>
#include <toml++/toml.h>
#include <floppy/configuration.h>

using std::string;
using std::string_view;
using namespace fl::types;
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
auto fl::serialization::serialize<fl::serialization::format::toml>(DummyConfiguration const& value) -> std::basic_string<char>
{
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
  return (std::stringstream() << out).str();
}

template <>
auto fl::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char> const& value) -> DummyConfiguration
{
  auto self = DummyConfiguration();
  toml::table in;
  try {
    in = toml::parse(value);
  } catch(toml::parse_error const& err) {
    throw serialization::serialization_error(fl::serialization::format::toml, err.what());
  }
  try {
    self.test = in["test"].value<u32>().value();
    self.ip_address = {
      .ip = in["ip_address"]["ip"].value<string>().value(),
      .port = in["ip_address"]["port"].value<u16>().value(),
      .sock_mode = {
        .tcp = in["ip_address"]["sock_mode"]["tcp"].value<bool>().value(),
        .udp = in["ip_address"]["sock_mode"]["udp"].value<bool>().value()
      }
    };
  } catch(std::bad_optional_access const& err) {
    throw serialization::serialization_error(fl::serialization::format::toml, err.what());
  }
  return self;
}

TEST(Configuration, ExampleConfig)
{
  using fl::serialization::format;
  using enum fl::saving_policy;

  auto config = fl::configuration_file<format::toml, DummyConfiguration>(
    "test.toml",
    fs::current_path() / "test-cfg",
    explicit_
  );

  ASSERT_TRUE(config.valid());
  ASSERT_EQ(config().test, 0);
  ASSERT_EQ(config().ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config().ip_address.port, 25565);
  ASSERT_EQ(config().ip_address.sock_mode.tcp, true);
  ASSERT_EQ(config().ip_address.sock_mode.udp, false);
  config().ip_address = {
    .ip = "127.0.0.1",
    .port = 45555,
    .sock_mode = {
      .tcp = false,
      .udp = true
    }
  };
  config.save();
  ASSERT_EQ(config().test, 0);
  ASSERT_EQ(config().ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config().ip_address.port, 45555);
  ASSERT_EQ(config().ip_address.sock_mode.tcp, false);
  ASSERT_EQ(config().ip_address.sock_mode.udp, true);
  config.load();
  ASSERT_EQ(config().test, 0);
  ASSERT_EQ(config().ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config().ip_address.port, 45555);
  ASSERT_EQ(config().ip_address.sock_mode.tcp, false);
  ASSERT_EQ(config().ip_address.sock_mode.udp, true);

  fs::remove_all(fs::current_path() / "test-cfg");
}