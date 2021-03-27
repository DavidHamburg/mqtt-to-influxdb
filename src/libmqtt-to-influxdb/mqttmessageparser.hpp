#pragma once
#include <libmqtt-to-influxdb/imqttmessageparser.hpp>
#include <libmqtt-to-influxdb/mqttparseresult.hpp>
#include <libmqtt-to-influxdb/config/document.hpp>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

class mqttmessageparser : public imqttmessageparser {
public:
    explicit mqttmessageparser(const std::shared_ptr<document> &document);
    std::vector<mqtt_parse_result> parse(const std::string &msg, const std::string &payload) const override;

private:
    std::shared_ptr<document> m_document;
    std::tuple<bool, mqtt_parse_result> create_mqtt_parse_result(const measurement &measurement, const field &field, const std::string &mqtt_payload) const;
};