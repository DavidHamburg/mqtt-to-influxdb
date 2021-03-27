#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/topic.hpp>

class topicfixture{ };

TEST_CASE_METHOD(topicfixture, "can deserialize topic") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
    measurements:
    - name: "plug"
      fields:
      - name: "dbfield"
        value: "is_on"
        match: "on"
        data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto t = node.as<topic>();

    REQUIRE(t.name == "zigbee2mqtt/0x00158d00053d224e");
    REQUIRE(t.measurements.size() == 1);
}

TEST_CASE_METHOD(topicfixture, "deserializes topic name") {
    std::string sample = R"(
measurements:
- name: "plug"
  fields:
  - name: "dbfield"
    value: "is_on"
    match: "on"
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

TEST_CASE_METHOD(topicfixture, "measurements is required") {
    auto sample = R"(
    topic: "zigbee2mqtt/0x00158d00053d224e"
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
        match: "on"
        data-type: "bool"
    - name: "c"
      fields:
      - name: "dbfield"
        value: "is_on"
        match: "on"
        data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    auto t = node.as<topic>();
    REQUIRE(t.measurements.size() == 2);
}
