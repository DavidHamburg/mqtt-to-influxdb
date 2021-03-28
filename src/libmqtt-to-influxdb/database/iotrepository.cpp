#include <libmqtt-to-influxdb/database/iotrepository.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
#include <sstream>

iot_repository::iot_repository(const std::string &user, const std::string &password, const std::string &host, int port, const std::string &protocol, const std::string &databaseName) {
    std::stringstream ss;
    ss << protocol << "://";
    if (!stringhelper::is_empty_or_whitespace(user) && !stringhelper::is_empty_or_whitespace(password)) {
        ss << user << ":" << password << "@";
    }
    ss << host << ":" << port << "?db=" << databaseName;
    m_connection_string = ss.str();
}

void iot_repository::write_mqtt_result(const std::string &measurement, const std::vector<mqtt_parse_result> &data) {
    if (data.empty()) {
        return;
    }
    auto influxdb = influxdb::InfluxDBFactory::Get(m_connection_string);
    auto point = influxdb::Point{measurement};
    std::vector<influxdb::Point> points{};
    for(const auto &d : data) {
        switch (d.data_type) {
            case datatype::float_type: {
                float f = std::stof(d.value);
                point.addField(d.dbfield, f);
                break;
            }
            case datatype::string_type: {
                point.addField(d.dbfield, d.value);
                break;
            }
            case datatype::integer_type: {
                int i = std::stoi(d.value);
                point.addField(d.dbfield, i);
                break;
            }
            case datatype::boolean_type: {
                bool boolean = stringhelper::to_bool(d.value);
                point.addField(d.dbfield, boolean);
                break;
            }
            default:
                throw std::runtime_error("unknown data type");
        }
    }
    influxdb->write(std::move(point));
}


