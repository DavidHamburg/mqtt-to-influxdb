#pragma once
#include <string>
#include <vector>
#include <libmqtt-to-influxdb/database/iiotrepository.hpp>

class iot_repository : public iiot_repository {
public:
    iot_repository(const std::string &user, const std::string& password, const std::string &host, int port, const std::string &protocol, const std::string &database_name);
    void write_mqtt_result(const std::string &measurement, const std::vector<mqtt_parse_result> &data) override;
private:
    std::string m_connection_string{};
};
