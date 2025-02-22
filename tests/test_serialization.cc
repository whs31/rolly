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

template <>
struct rll::serializer<TestStruct, serialization::format::json> {
  static result<> serialize(TestStruct const& value, std::ostream& stream) {
    fmt::print(stream, R"({{"a": {}, "b": {}}})", value.a, value.b);
    return {};
  }

  static result<TestStruct> deserialize(std::istream& stream) {
    return {
      TestStruct {1, 2}
    };
  }
};

template <>
struct rll::serializer<DummyConfiguration, serialization::format::toml> {
  static result<> serialize(DummyConfiguration const& value, std::ostream& stream) {
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
    stream << out;
    return ok();
  }

  static result<DummyConfiguration> deserialize(std::istream& stream) {
    auto self = DummyConfiguration();
    toml::table in;
    try {
      in = toml::parse(stream);
    } catch(toml::parse_error const& err) {
      return error(err.what());
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
      return error(err.what());
    }
    return self;
  }
};

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

      auto ss = std::stringstream();
      auto res = serializer<TestStruct, format::json>::serialize(test, ss);

      REQUIRE(res.has_value());
      REQUIRE(ss.str() == R"({"a": 1, "b": 2})");
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
