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

TEST_CASE_METHOD(stringhelperfixture, "convert to bool") {
    SECTION("empty string") {
        auto result = stringhelper::to_bool("");
        REQUIRE(result == false);
    }
    SECTION("whitespace") {
        auto result = stringhelper::to_bool(" ");
        REQUIRE(result == false);
    }
    SECTION("true") {
        auto result = stringhelper::to_bool("true");
        REQUIRE(result == true);
    }
    SECTION("false") {
        auto result = stringhelper::to_bool("false");
        REQUIRE(result == false);
    }
    SECTION("case-insensitive") {
        auto result = stringhelper::to_bool("TrUe");
        REQUIRE(result == true);
    }
    SECTION("true with whitespaces") {
        auto result = stringhelper::to_bool(" true ");
        REQUIRE(result == true);
    }
}
