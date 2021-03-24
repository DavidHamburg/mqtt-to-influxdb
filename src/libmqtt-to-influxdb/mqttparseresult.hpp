#pragma once
#include <libmqtt-to-influxdb/datatype.hpp>
#include <string>

struct mqtt_parse_result {
    std::string measurement;
    std::string value;
    std::string dbfield;
    datatype data_type;
};