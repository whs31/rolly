#include <catch2/catch_all.hpp>
#include <toml++/toml.h>
#include <rll/config.h>
#include <rll/savefile.h>

using std::string;
using std::string_view;
using namespace rll;
namespace fs = std::filesystem;

struct DummyConfiguration {
  u32 test = 0;

  struct IpAddress {
    string ip;
    u16 port;

    struct SockMode {
      bool tcp = true;
      bool udp = false;
    } sock_mode {};
  } ip_address = {"127.0.0.1", 25'565};
};

struct TestStruct {
  int a = 1;
  int b = 2;
};

DECLARE_SERIALIZABLE(TestStruct, rll::serialization::format::json) {
  return fmt::format(R"({{"a": {}, "b": {}}})", value.a, value.b);
}

DECLARE_DESERIALIZABLE(TestStruct, rll::serialization::format::json) { return {1, 2}; }

DECLARE_SERIALIZABLE(DummyConfiguration, rll::serialization::format::toml) {
  auto const out = toml::table {
    {"test",       value.test},
    {"ip_address",
     toml::table {
       {"ip", value.ip_address.ip},
       {"port", value.ip_address.port},
       {"sock_mode",
        toml::table {
          {"tcp", value.ip_address.sock_mode.tcp},
          {"udp", value.ip_address.sock_mode.udp}
        }}
     }                       }
  };
  auto ss = std::stringstream();
  ss << out;
  return ss.str();
}

DECLARE_DESERIALIZABLE(DummyConfiguration, rll::serialization::format::toml) {
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
      {in["ip_address"]["sock_mode"]["tcp"].value<bool>().value(),
                                                   in["ip_address"]["sock_mode"]["udp"].value<bool>().value()}
    };
  } catch(std::bad_optional_access const& err) {
    throw serialization_error<serialization::format::toml>(err.what());
  }
  return self;
}

TEST_CASE("Serialization & filesystem") {
  namespace format = serialization::format;
  SECTION("Config") {
    SECTION("Basic") {
      auto config = configuration_file<format::toml, DummyConfiguration>(
        "test.toml",
        fs::current_path() / "test-cfg",
        saving_policy::explicit_
      );

      REQUIRE(config.valid());
      REQUIRE(config().test == 0);
      REQUIRE(config().ip_address.ip == "127.0.0.1");
      REQUIRE(config().ip_address.port == 25'565);
      REQUIRE(config().ip_address.sock_mode.tcp == true);
      REQUIRE(config().ip_address.sock_mode.udp == false);

      config().ip_address = {
        "127.0.0.1",
        45'555,
        {false, true}
      };
      config.save();
      REQUIRE(config().test == 0);
      REQUIRE(config().ip_address.ip == "127.0.0.1");
      REQUIRE(config().ip_address.port == 45'555);
      REQUIRE(config().ip_address.sock_mode.tcp == false);
      REQUIRE(config().ip_address.sock_mode.udp == true);
      config.load();
      REQUIRE(config().test == 0);
      REQUIRE(config().ip_address.ip == "127.0.0.1");
      REQUIRE(config().ip_address.port == 45'555);
      REQUIRE(config().ip_address.sock_mode.tcp == false);
      REQUIRE(config().ip_address.sock_mode.udp == true);

      fs::remove_all(fs::current_path() / "test-cfg");
    }
  }

  SECTION("Serialization") {
    SECTION("Basic") {
      auto const test = TestStruct {1, 2};
      auto const expected = test;
      auto json = serialization::serialize<format::json>(test);
      auto test2 = serialization::deserialize<format::json, TestStruct>(json);

      REQUIRE(expected.a == test2.a);
      REQUIRE(expected.b == test2.b);
      REQUIRE(json == R"({"a": 1, "b": 2})");
    }
  }

  SECTION("Savefile") {
    SECTION("Basic") {
      {
        auto save =
          savefile<format::toml, DummyConfiguration>("test.toml", fs::current_path() / "test-save");

        REQUIRE(save.valid());
        REQUIRE(save().test == 0);
        REQUIRE(save().ip_address.ip == "127.0.0.1");
        REQUIRE(save().ip_address.port == 25'565);
        REQUIRE(save().ip_address.sock_mode.tcp == true);
        REQUIRE(save().ip_address.sock_mode.udp == false);

        save().ip_address = {
          "127.0.0.1",
          45'555,
          {false, true}
        };
        save.save();
        REQUIRE(fs::exists(save.path()));
        REQUIRE(save.has_backup());
        REQUIRE(fs::exists(fs::current_path() / "test-save" / "test.toml.bak"));
        REQUIRE(save().test == 0);
        REQUIRE(save().ip_address.ip == "127.0.0.1");
        REQUIRE(save().ip_address.port == 45'555);
        REQUIRE(save().ip_address.sock_mode.tcp == false);
        REQUIRE(save().ip_address.sock_mode.udp == true);
        save.load();
        REQUIRE(save().test == 0);
        REQUIRE(save().ip_address.ip == "127.0.0.1");
        REQUIRE(save().ip_address.port == 45'555);
        REQUIRE(save().ip_address.sock_mode.tcp == false);
        REQUIRE(save().ip_address.sock_mode.udp == true);
      }

      fs::remove_all(fs::current_path() / "test-save");
    }
  }
}
