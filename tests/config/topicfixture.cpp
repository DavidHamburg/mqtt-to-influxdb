#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/topic.hpp>

class topicfixture{
public:
};

TEST_CASE_METHOD(topicfixture, "can deserialize topic") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
    json: true
    measurements:
    - name: "plug"
      fields:
      - name: "dbfield"
        value: "is_on"
        payload: "on"
        data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto t = node.as<topic>();

    REQUIRE(t.is_json);
    REQUIRE(t.name == "zigbee2mqtt/0x00158d00053d224e");
    REQUIRE(t.measurements.size() == 1);
}

TEST_CASE_METHOD(topicfixture, "deserializes topic name") {
    std::string sample = R"(
json: true
measurements:
- name: "plug"
  fields:
  - name: "dbfield"
    value: "is_on"
    payload: "on"
    data-type: "bool"
    )";
    SECTION("is required") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<topic>(), YAML::RepresentationException);
    }
    SECTION("must not be empty") {
        sample = "topic: \"\"" + sample;
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<topic>(), YAML::RepresentationException);
    }
    SECTION("must not be empty whitespace only") {
        sample = "topic: \"   \"" + sample;
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<topic>(), YAML::RepresentationException);
    }
    SECTION("text") {
        sample = "topic: \"abc\"" + sample;
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<topic>();
        REQUIRE(m.name == "abc");
    }
}

TEST_CASE_METHOD(topicfixture, "json is optional") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
    measurements:
    - name: "plug"
      fields:
      - name: "dbfield"
        value: "is_on"
        payload: "on"
        data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto t = node.as<topic>();

    REQUIRE_FALSE(t.is_json);
}

TEST_CASE_METHOD(topicfixture, "measurements is required") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
    json: true
    )";
    YAML::Node node = YAML::Load(sample);
    REQUIRE_THROWS_AS(node.as<topic>(), YAML::RepresentationException);
}

TEST_CASE_METHOD(topicfixture, "can deserialize measurements") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
    measurements:
    - name: "a"
      fields:
      - name: "dbfield"
        value: "is_on"
        payload: "on"
        data-type: "bool"
    - name: "c"
      fields:
      - name: "dbfield"
        value: "is_on"
        payload: "on"
        data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto t = node.as<topic>();
    REQUIRE(t.measurements.size() == 2);
}
