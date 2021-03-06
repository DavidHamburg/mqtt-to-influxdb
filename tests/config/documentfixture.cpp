#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/document.hpp>

class documentfixture{ };

TEST_CASE_METHOD(documentfixture, "can deserialize settings") {
    auto sample = R"(
    settings:
      influxdb:
        database: iot
    thermostat:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          match: "on"
          data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto d = node.as<document>();
    REQUIRE(d.devices.size() == 1);
    REQUIRE(d.connection.influxdb_database == "iot");
}


TEST_CASE_METHOD(documentfixture, "can deserialize device") {
    auto sample = R"(
    settings:
      influxdb:
        database: iot
    thermostat:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          match: "on"
          data-type: "bool"
    plug:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          match: "on"
          data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto d = node.as<document>();
    REQUIRE(d.devices.size() == 2);
    REQUIRE(d.devices.at(0).name == "thermostat");
    REQUIRE(d.devices.at(1).name == "plug");
}

TEST_CASE_METHOD(documentfixture, "does not allow two devices with same name") {
    auto sample = R"(
    settings:
      influxdb:
        database: iot
    plug:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          match: "on"
          data-type: "bool"
    plug:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          match: "on"
          data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    REQUIRE_THROWS_AS(node.as<document>(), DuplicateDeviceException);
}

