#include <catch2/catch.hpp>
#include <sstream>
#include <libmqtt-to-influxdb/datatype.hpp>

class datatypefixture {
};

TEST_CASE_METHOD(datatypefixture, "can write to ostream") {
    std::stringstream s;
    SECTION("bool") {
        s << datatype::boolean_type;
        REQUIRE(s.str() == "bool");
    }
    SECTION("int") {
        s << datatype::integer_type;
        REQUIRE(s.str() == "int");
    }
    SECTION("string") {
        s << datatype::string_type;
        REQUIRE(s.str() == "string");
    }
    SECTION("float") {
        s << datatype::float_type;
        REQUIRE(s.str() == "float");
    }
}
