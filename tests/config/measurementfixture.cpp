#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/measurement.hpp>

class measurementfixture { };

TEST_CASE_METHOD(measurementfixture, "deserializes measurement name") {
    std::string sample = R"(
    fields:
    - name: "battery"
      json-field: battery
    )";
    SECTION("is required") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<measurement>(), YAML::RepresentationException);
    }
    SECTION("must not be empty") {
        sample += "name: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<measurement>(), YAML::RepresentationException);
    }
    SECTION("name") {
        sample += "name: \"abc\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<measurement>();
        REQUIRE(m.name == "abc");
    }
}

TEST_CASE_METHOD(measurementfixture, "deserializes measurement fields") {
    std::string sample = R"(
    name: "plug"
    )";
    SECTION("is required") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<measurement>(), YAML::RepresentationException);
    }
    SECTION("must not be empty") {
        sample += "fields: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<measurement>(), YAML::RepresentationException);
    }
    SECTION("must be a sequence") {
        std::string sample = R"(
        name: "plug"
        fields:
          name: "battery"
          json-field: battery
        )";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<measurement>(), YAML::RepresentationException);
    }
    SECTION("vector of field") {
        std::string sample = R"(
        name: "plug"
        fields:
        - name: "battery"
          json-field: battery
        )";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<measurement>();
        REQUIRE(m.fields.size() == 1);
    }
}

