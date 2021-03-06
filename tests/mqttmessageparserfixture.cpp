#include <catch2/catch.hpp>
#include <libmqtt-to-influxdb/mqttmessageparser.hpp>
#include <memory>

class messageparserfixture{
public:
    explicit messageparserfixture(): m_document(create_document()), m_sut(mqttmessageparser{m_document}) {
    }

    const std::string sample_topic = "mydevice/POWER";

    auto& topic() {
        return m_document->devices.at(0).topics.at(0);
    }

    auto& measurement() {
        return topic().measurements.at(0);
    }

    auto& field() {
        return topic().measurements.at(0).fields.at(0);
    }

    auto& sut() {
        return m_sut;
    }

private:
    std::shared_ptr<document> m_document;
    mqttmessageparser m_sut;

    mqttmessageparser create_sut() {
        auto doc = std::make_shared<document>();
        mqttmessageparser sut{doc};
        return sut;
    }

    std::shared_ptr<document> create_document() {
        auto doc = std::make_shared<document>();
        auto &device = doc->devices.emplace_back();
        device.name = "plug";
        auto &topic = device.topics.emplace_back();
        topic.name = sample_topic;
        auto &measurement = topic.measurements.emplace_back();
        measurement.fields.emplace_back();
        return doc;
    }
};

TEST_CASE_METHOD(messageparserfixture, "parsing unknown message returns empty list") {
    auto result = sut().parse("unknown topic", "unknown message");
    REQUIRE(result.size() == 0);
}

TEST_CASE_METHOD(messageparserfixture, "matches match") {
    measurement().name = "my measurement";
    field().match = "ON";
    field().value = "true";
    field().data_type = datatype::boolean_type;
    field().name = "power";

    SECTION("exact match") {
        auto result = sut().parse(sample_topic, "ON");
        REQUIRE(result.size() == 1);
    }
    SECTION("ignores-case by default") {
        auto result = sut().parse(sample_topic, "on");
        REQUIRE(result.size() == 1);
    }
    SECTION("respects ignores-case false") {
        field().ignore_case = false;
        auto result = sut().parse(sample_topic, "on");
        REQUIRE(result.size() == 0);
    }
    SECTION("returns data") {
        auto result = sut().parse(sample_topic, "ON");
        auto data = result.at("my measurement").at(0);
        REQUIRE(data.measurement == "my measurement");
        REQUIRE(data.value == "true");
        REQUIRE(data.data_type == datatype::boolean_type);
        REQUIRE(data.dbfield == "power");
    }
}

TEST_CASE_METHOD(messageparserfixture, "parses value") {
    measurement().name = "my measurement";
    field().match = "ON";
    field().name = "power";

    SECTION("uses payload when no value specified") {
        auto result = sut().parse(sample_topic, "ON");
        REQUIRE(result["my measurement"].at(0).value == "ON");
    }
    SECTION("uses value when specified") {
        field().value = "true";
        auto result = sut().parse(sample_topic, "on");
        REQUIRE(result["my measurement"].at(0).value == "true");
    }
}

TEST_CASE_METHOD(messageparserfixture, "parses simple json field as string") {
    measurement().name = "my measurement";
    field().match = "On";
    field().name = "power";
    field().json_field = "state";

    SECTION("uses json-field value when no value specified") {
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result["my measurement"].at(0).value == "ON");
    }
    SECTION("uses value when specified") {
        field().value = "true";
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result["my measurement"].at(0).value == "true");
    }
}

TEST_CASE_METHOD(messageparserfixture, "json field value must match payload") {
    measurement().name = "my measurement";
    field().name = "power";
    field().json_field = "state";

    SECTION("matches exactly") {
        field().match = "ON";
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result.size() == 1);
    }
    SECTION("case-sensitive when specified") {
        field().match = "oN";
        field().ignore_case = false;
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result.size() == 0);
    }
    SECTION("ignores case by default") {
        field().match = "oN";
        field().ignore_case = true;
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result.size() == 1);
    }
    SECTION("no match") {
        field().match = "xyz";
        auto result = sut().parse(sample_topic, "{\"state\": \"ON\"}");
        REQUIRE(result.size() == 0);
    }
}

TEST_CASE_METHOD(messageparserfixture, "json - nested field") {
    measurement().name = "my measurement";
    field().name = "power";
    field().json_field = "battery.value";

    auto result = sut().parse(sample_topic, "{\"battery\":{\"low\":false,\"value\":\"55\"}}");
    REQUIRE(result.size() == 1);
    REQUIRE(result["my measurement"].at(0).value == "55");
}

TEST_CASE_METHOD(messageparserfixture, "json - accepts number") {
    measurement().name = "my measurement";
    field().name = "power";
    field().json_field = "battery";

    auto result = sut().parse(sample_topic, "{\"battery\": 55 }");
    REQUIRE(result.size() == 1);
    REQUIRE(result["my measurement"].at(0).value == "55");
}
