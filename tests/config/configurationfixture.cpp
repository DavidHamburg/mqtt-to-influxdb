#include <catch2/catch.hpp>
#include <string>
#include <libmqtt-to-influxdb/config/configuration.hpp>

class configurationfixture{};

TEST_CASE_METHOD(configurationfixture, "can load valid configuration") {
    auto sample = R"(
    settings:
      influxdb:
        ip: 127.0.0.1
        port: 1883
      broker:
        ip: 127.0.0.1
        port: 8086
    thermostat:
    - topic: "zigbee2mqtt/0x00158d00053d224e"
      json: true
      measurements:
      - name: "plug"
        fields:
        - name: "dbfield"
          value: "is_on"
          payload: "on"
          data-type: "bool"
    )";
    configuration sut{};
    auto [result, document] = sut.load(sample);
    REQUIRE(result == true);
}

TEST_CASE_METHOD(configurationfixture, "load returns false for invalid input") {
    configuration sut{};
    SECTION("empty input") {
        auto [result, _] = sut.load("");
        REQUIRE(result == false);
    }
    SECTION("no device defined") {
        auto [result, _] = sut.load("<xml>");
        REQUIRE(result == false);
    }
    SECTION("invalid device definition") {
        auto sample = R"(
        thermostat:
        - topic: "zigbee2mqtt/0x00158d00053d224e"
          json: true
        )";
        auto [result, _] = sut.load(sample);
        REQUIRE(result == false);
    }
}
