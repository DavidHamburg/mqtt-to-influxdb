#pragma once
#include <string>
#include <vector>
#include <libmqtt-to-influxdb/database/iiotrepository.hpp>

namespace influxdb_cpp {
class server_info;
}

class iot_repository : public iiot_repository {
public:
    iot_repository(const std::string &user, const std::string& password, const std::string &ip, int port);
    void write_mqtt_result(const std::string &measurement, const std::vector<mqtt_parse_result> &data) override;
private:
    std::string m_user, m_password{}, m_ip;
    int m_port{};
};
