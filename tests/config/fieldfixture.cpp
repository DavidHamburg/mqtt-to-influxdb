#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <libmqtt-to-influxdb/config/field.hpp>

class fieldfixture {
public:
};

TEST_CASE_METHOD(fieldfixture, "can deserialize field") {
    auto sample = R"(
    name: "plug"
    value: "is_on"
    match: "on"
    data-type: "bool"
    )";
    YAML::Node node = YAML::Load(sample);
    field m = node.as<field>();

    REQUIRE(m.name == "plug");
}

TEST_CASE_METHOD(fieldfixture, "deserializes name") {
    std::string sample = R"(
    match: "on"
    value: "is_on"
    )";
    SECTION("is required") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<field>(), YAML::RepresentationException);
    }
    SECTION("must not be empty") {
        sample += "name: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<field>(), YAML::RepresentationException);
    }
    SECTION("name") {
        sample += "name: \"abc\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.name == "abc");
    }
}

TEST_CASE_METHOD(fieldfixture, "deserializes value") {
    std::string sample = R"(
    name: "plug"
    match: "on"
    )";
    SECTION("is optional") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_NOTHROW(node.as<field>());
    }
    SECTION("must not be empty") {
        sample += "value: \"\"";
        YAML::Node node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<field>(), YAML::RepresentationException);
    }
    SECTION("string") {
        sample += "value: \"abc\"";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.value == "abc");
    }
    SECTION("bool") {
        sample += "value: true";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.value == "true");
    }
    SECTION("int") {
        sample += "value: 1";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.value == "1");
    }
    SECTION("float") {
        sample += "value: 1.5";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.value == "1.5");
    }
}

TEST_CASE_METHOD(fieldfixture, "deserializes json-field") {
    std::string sample = R"(
    name: "plug"
    value: "is_on"
    )";
    SECTION("is optional") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_NOTHROW(node.as<field>());
    }
    SECTION("value") {
        sample += "json-field: 123";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.json_field == "123");
    }
}

TEST_CASE_METHOD(fieldfixture, "deserializes ignore-case") {
    std::string sample = R"(
    name: "plug"
    value: "is_on"
    match: "on"
    )";
    SECTION("is optional") {
        YAML::Node node = YAML::Load(sample);
        REQUIRE_NOTHROW(node.as<field>());
    }
    SECTION("default is true") {
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.ignore_case);
    }
    SECTION("false") {
        sample += "ignore-case: false";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.ignore_case == false);
    }
    SECTION("true") {
        sample += "ignore-case: true";
        YAML::Node node = YAML::Load(sample);
        auto m = node.as<field>();
        REQUIRE(m.ignore_case);
    }
}

TEST_CASE_METHOD(fieldfixture, "data-type is optional, string is default") {
    auto sample = R"(
    name: "plug"
    value: "is_on"
    match: "on"
    )";
    YAML::Node node = YAML::Load(sample);
    field m = node.as<field>();
    REQUIRE(m.data_type == datatype::string_type);
}

TEST_CASE_METHOD(fieldfixture, "match is optional") {
    auto sample = R"(
    name: "plug"
    value: "is_on"
    )";
    YAML::Node node = YAML::Load(sample);
    REQUIRE_NOTHROW(node.as<field>());
}

TEST_CASE_METHOD(fieldfixture, "supports list of data-types") {
    std::string sample = R"(
    name: "plug"
    value: "is_on"
    match: "on"
    )";

    SECTION("bool") {
        sample += "data-type: \"bool\"";
        auto node = YAML::Load(sample);
        field m = node.as<field>();
        REQUIRE(m.data_type == datatype::boolean_type);
    }
    SECTION("string") {
        sample += "data-type: \"string\"";
        auto node = YAML::Load(sample);
        field m = node.as<field>();
        REQUIRE(m.data_type == datatype::string_type);
    }
    SECTION("float") {
        sample += "data-type: \"float\"";
        auto node = YAML::Load(sample);
        field m = node.as<field>();
        REQUIRE(m.data_type == datatype::float_type);
    }
    SECTION("int") {
        sample += "data-type: \"int\"";
        auto node = YAML::Load(sample);
        field m = node.as<field>();
        REQUIRE(m.data_type == datatype::integer_type);
    }
    SECTION("throws when data-type is unknown"){
        sample += "data-type: \"random\"";
        auto node = YAML::Load(sample);
        REQUIRE_THROWS_AS(node.as<field>(), YAML::RepresentationException);
    }
}
