#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/settings.hpp>

class settingsfixture { };

TEST_CASE_METHOD(settingsfixture, "broker is optional") {
    std::string sample = R"(
influxdb:
  database: iot
)";
    YAML::Node node = YAML::Load(sample);
    REQUIRE_NOTHROW(node.as<settings>());
}

TEST_CASE_METHOD(settingsfixture, "deserializes broker ip") {
    std::string sample = R"(
influxdb:
  database: iot
broker:
  port: 1883
  )";
    SECTION("ip is optional, default 127.0.0.1") {
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.broker_ip == "127.0.0.1");
    }
    SECTION("must not be empty") {
        sample += "ip: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("must not be whitespace") {
        sample += "ip: \"   \"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("ip") {
        sample += "ip: \"192.168.178.201\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.broker_ip == "192.168.178.201");
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes broker port") {
    std::string sample = R"(
influxdb:
  database: iot
broker:
  ip: 192.168.178.201
  )";
    SECTION("is optional, default 1883") {
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.broker_port == 1883);
    }
    SECTION("must not be empty") {
        sample += "port: ";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("port") {
        sample += "port: 1234";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.broker_port == 1234);
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes influxdb host") {
    std::string sample = R"(
broker:
  ip: 192.168.178.102
  port: 1883
influxdb:
  database: iot
  )";
    SECTION("host is optional, default 127.0.0.1") {
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_host == "127.0.0.1");
    }
    SECTION("must not be empty") {
        sample += "host: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("must not be whitespace") {
        sample += "host: \"   \"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("host") {
        sample += "host: \"192.168.178.201\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_host == "192.168.178.201");
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes influxdb port") {
    std::string sample = R"(
broker:
  ip: 192.168.178.201
  port: 1883
influxdb:
  database: iot
  )";
    SECTION("is optional, default 8086") {
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_port == 8086);
    }
    SECTION("must not be empty") {
        sample += "port: ";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("port") {
        sample += "port: 1234";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_port == 1234);
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes influxdb user") {
    std::string sample = R"(
broker:
  ip: 192.168.178.102
  port: 1883
influxdb:
  database: iot
  )";
    SECTION("user is optional") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_NOTHROW(node.as<settings>());
    }
    SECTION("must not be empty") {
        sample += "user: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("must not be whitespace") {
        sample += "user: \"   \"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("user") {
        sample += "user: \"my user\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_user == "my user");
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes influxdb password") {
    std::string sample = R"(
broker:
  ip: 192.168.178.102
  port: 1883
influxdb:
  database: iot
  )";
    SECTION("password is optional") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_NOTHROW(node.as<settings>());
    }
    SECTION("password") {
        sample += "password: \"my password\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_password == "my password");
    }
}

TEST_CASE_METHOD(settingsfixture, "deserializes influxdb database") {
    std::string sample = R"(
broker:
  ip: 192.168.178.201
  port: 1883
influxdb:
  host: 192.168.178.201
  )";
    SECTION("is required") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("must not be empty") {
        sample += "database: ";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<settings>(), YAML::RepresentationException);
    }
    SECTION("database") {
        sample += "database: mydb";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<settings>();
        REQUIRE(m.influxdb_database == "mydb");
    }
}
