#include <string>
#include <sstream>
#include <toml++/toml.h>
#include <lf/leaf.h>
#include <lf/config.h>
#include <gtest/gtest.h>

using std::string;
using std::string_view;
using namespace lf::types;
namespace fs = std::filesystem;

struct DummyConfiguration : public lf::trait::i::Serializable,
                            public lf::trait::i::Deserializable
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
    } sock_mode;
  } ip_address = { .ip = "127.0.0.1", .port = 25565 };

  [[nodiscard]] virtual auto deserialize(string_view data) -> Result<> override {
    toml::table in;
    try {
      in = toml::parse(data);
    } catch(toml::parse_error const& err) {
      return Error(err.what());
    }
    try {
      this->test = in["test"].value<u32>().value();
      this->ip_address = {
        .ip = in["ip_address"]["ip"].value<string>().value(),
        .port = in["ip_address"]["port"].value<u16>().value(),
        .sock_mode = {
          .tcp = in["ip_address"]["sock_mode"]["tcp"].value<bool>().value(),
          .udp = in["ip_address"]["sock_mode"]["udp"].value<bool>().value()
        }
      };
    } catch(std::bad_optional_access const& err) {
      return Error("error when deserializing: {}", err.what());
    }
    return Ok();
  }

  [[nodiscard]] virtual auto serialize() const -> Result<string> override
  {
    auto const out = toml::table {
      {"test", test},
      {"ip_address", toml::table{
        {"ip", ip_address.ip},
        {"port", ip_address.port},
        {"sock_mode", toml::table{
          {"tcp", ip_address.sock_mode.tcp},
          {"udp", ip_address.sock_mode.udp}
        }}
      }}
    };
    std::stringstream out_stream;
    out_stream << out;
    return Ok(out_stream.str());
  }
};

TEST(Config, ConfigurationFile)
{
  using enum lf::config::SavingPolicy;

  auto config = lf::config::ConfigurationFile<DummyConfiguration>(
    "test.toml",
    fs::current_path() / "test-cfg",
    Explicit
  );

  ASSERT_EQ(config.values.test, 0);
  ASSERT_EQ(config.values.ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config.values.ip_address.port, 25565);
  ASSERT_EQ(config.values.ip_address.sock_mode.tcp, true);
  ASSERT_EQ(config.values.ip_address.sock_mode.udp, false);
  config.values.ip_address = {
    .ip = "127.0.0.1",
    .port = 45555,
    .sock_mode = {
      .tcp = false,
      .udp = true
    }
  };
  const auto save = config.save();
  ASSERT_TRUE(save);
  ASSERT_EQ(config.values.test, 0);
  ASSERT_EQ(config.values.ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config.values.ip_address.port, 45555);
  ASSERT_EQ(config.values.ip_address.sock_mode.tcp, false);
  ASSERT_EQ(config.values.ip_address.sock_mode.udp, true);
  const auto load = config.load();
  ASSERT_TRUE(load);
  ASSERT_EQ(config.values.test, 0);
  ASSERT_EQ(config.values.ip_address.ip, "127.0.0.1");
  ASSERT_EQ(config.values.ip_address.port, 45555);
  ASSERT_EQ(config.values.ip_address.sock_mode.tcp, false);
  ASSERT_EQ(config.values.ip_address.sock_mode.udp, true);

  fs::remove_all(fs::current_path() / "test-cfg");
}