#pragma once
#include <libmqtt-to-influxdb/mqttparseresult.hpp>
#include <string>
#include <vector>

class imqttmessageparser {
public:
    virtual ~imqttmessageparser() = default;
    virtual std::vector<mqtt_parse_result> parse(const std::string &msg, const std::string &payload) const = 0;
};
