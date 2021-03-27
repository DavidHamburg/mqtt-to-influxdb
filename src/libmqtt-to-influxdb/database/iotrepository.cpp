#include <libmqtt-to-influxdb/database/iotrepository.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>
#include <influxdb.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <memory>

iot_repository::iot_repository(const std::string &user, const std::string &password, const std::string &ip, int port)
        : m_user(user), m_password(password), m_ip(ip), m_port(port) {}

void iot_repository::write_mqtt_result(const std::string &measurement, const std::vector<mqtt_parse_result> &data) {
    if (data.empty()) {
        return;
    }

    influxdb_cpp::server_info db(m_ip, m_port, "iot", m_user, m_password, "ns");
    spdlog::debug("Storing measurement '{}'.", measurement);

    influxdb_cpp::builder builder;
    influxdb_cpp::detail::ts_caller *caller = nullptr;
    for (const auto &d : data) {
        if (caller) {
            switch (d.data_type) {
                case datatype::float_type: {
                    float floatvalue = std::stof(d.value);
                    caller = &caller->meas(measurement)
                            .field(d.dbfield, floatvalue);
                    break;
                }
                case datatype::string_type: {
                    caller = &caller->meas(measurement)
                            .field(d.dbfield, d.value);
                    break;
                }
                case datatype::integer_type: {
                    int intvalue = std::stoi(d.value);
                    caller = &caller->meas(measurement)
                            .field(d.dbfield, intvalue);
                    break;
                }
                case datatype::boolean_type: {
                    bool boolvalue = stringhelper::to_bool(d.value);
                    caller = &caller->meas(measurement)
                            .field(d.dbfield, boolvalue);
                    break;
                }
                default:
                    throw std::runtime_error("unknown data type");
            }
        } else {
            switch (d.data_type) {
                case datatype::float_type: {
                    float floatvalue = std::stof(d.value);
                    caller = &builder.meas(measurement)
                            .field(d.dbfield, floatvalue);
                    break;
                }
                case datatype::string_type: {
                    caller = &builder.meas(measurement)
                            .field(d.dbfield, d.value);
                    break;
                }
                case datatype::integer_type: {
                    int intvalue = std::stoi(d.value);
                    caller = &builder.meas(measurement)
                            .field(d.dbfield, intvalue);
                    break;
                }
                case datatype::boolean_type: {
                    bool boolvalue = stringhelper::to_bool(d.value);
                    caller = &builder.meas(measurement)
                            .field(d.dbfield, boolvalue);
                    break;
                }
                default:
                    throw std::runtime_error("unknown data type");
            }
        }
    }
    if (caller) {
        caller->post_http(db);
    }
}

