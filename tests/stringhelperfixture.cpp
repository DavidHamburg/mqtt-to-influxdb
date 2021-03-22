#include <catch2/catch.hpp>
#include <string>
#include <libmqtt-to-influxdb/stringhelper.hpp>

class stringhelperfixture{};

TEST_CASE_METHOD(stringhelperfixture, "is_empty_or_whitespace") {
    SECTION("empty string") {
        auto result = stringhelper::is_empty_or_whitespace("");
        REQUIRE(result == true);
    }
    SECTION("whitespace") {
        auto result = stringhelper::is_empty_or_whitespace(" ");
        REQUIRE(result == true);
    }
    SECTION("several whitespaces") {
        auto result = stringhelper::is_empty_or_whitespace("           ");
        REQUIRE(result == true);
    }
    SECTION("text") {
        auto result = stringhelper::is_empty_or_whitespace("text");
        REQUIRE(result == false);
    }
    SECTION("text with whitespaces") {
        auto result = stringhelper::is_empty_or_whitespace(" text  ");
        REQUIRE(result == false);
    }
}
