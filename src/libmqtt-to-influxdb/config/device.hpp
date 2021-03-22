#pragma once
#include <string>
#include <vector>
#include <libmqtt-to-influxdb/config/topic.hpp>

struct device {
    std::string name{};
    std::vector<topic> topics{};
};
