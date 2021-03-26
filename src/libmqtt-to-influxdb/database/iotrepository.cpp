#include <libmqtt-to-influxdb/database/iotrepository.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>
#include <influxdb.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

iot_repository::iot_repository(const std::string &user, const std::string& password, const std::string &ip, int port) :m_user(user), m_password(password), m_ip(ip), m_port(port) { }

void iot_repository::write_mqtt_result(const mqtt_parse_result &data) {
    influxdb_cpp::server_info db(m_ip, m_port, "iot", m_user, m_password, "ns");
    spdlog::debug("Storing measurement '{}'.", data.measurement);

    auto measurement = std::move(influxdb_cpp::builder().meas(data.measurement));
    switch (data.data_type) {
        case datatype::float_type:{
            float floatvalue = std::stof(data.value);
            measurement.field(data.dbfield, floatvalue).post_http(db);
            break;
        }
        case datatype::string_type: {
            measurement.field(data.dbfield, data.value).post_http(db);
            break;
        }
        case datatype::integer_type:{
            int intvalue = std::stoi(data.value);
            measurement.field(data.dbfield, intvalue).post_http(db);
            break;
        }
        case datatype::boolean_type: {
            bool boolvalue = stringhelper::to_bool(data.value);
            measurement.field(data.dbfield, (int)boolvalue).post_http(db);
            break;
        }
    default:
            throw std::runtime_error("unknown data type");
    }
}

