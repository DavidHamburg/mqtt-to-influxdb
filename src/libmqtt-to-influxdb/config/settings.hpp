#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/stringhelper.hpp>

struct settings {
    std::string broker_ip{"127.0.0.1"};
    int broker_port{1883};
    std::string influxdb_host{"127.0.0.1"};
    int influxdb_port{8086};
    std::string influxdb_user{};
    std::string influxdb_password{};
    std::string influxdb_database{};
    std::string influxdb_protocol{"http"};
};

namespace YAML {
    template<>
    struct convert<settings> {
        static bool decode(const Node &node, settings &rhs) {
            if (node["broker"].IsDefined()) {
                auto &broker = node["broker"];
                if (broker["ip"].IsDefined()) {
                    rhs.broker_ip = broker["ip"].as<std::string>();
                    if (stringhelper::is_empty_or_whitespace(rhs.broker_ip)) {
                        spdlog::error("Broker ip must not be empty (line: {})", node.Mark().line);
                        return false;
                    }
                }
                if (broker["port"].IsDefined()) {
                    rhs.broker_port = broker["port"].as<int>();
                }
            }
            if (!node["influxdb"].IsDefined()) {
                spdlog::error("Missing influxdb (line: {})", node.Mark().line);
                return false;
            }
            auto &influxdb = node["influxdb"];
            if (!influxdb["database"].IsDefined()) {
                spdlog::error("Missing influxdb database (line: {})", node.Mark().line);
                return false;
            }
            if (influxdb["port"].IsDefined()) {
                rhs.influxdb_port = influxdb["port"].as<int>();
            }
            if (influxdb["host"].IsDefined()) {
                rhs.influxdb_host = influxdb["host"].as<std::string>();
            }
            if (stringhelper::is_empty_or_whitespace(rhs.influxdb_host)) {
                spdlog::error("Influxdb host must not be empty (line: {})", node.Mark().line);
                return false;
            }
            rhs.influxdb_database = influxdb["database"].as<std::string>();

            if (influxdb["user"].IsDefined()){
                rhs.influxdb_user = influxdb["user"].as<std::string>();
                if (stringhelper::is_empty_or_whitespace(rhs.influxdb_user)) {
                    spdlog::error("Influxdb user must not be empty (line: {})", node.Mark().line);
                    return false;
                }
            }
            if (influxdb["password"].IsDefined()){
              rhs.influxdb_password = influxdb["password"].as<std::string>();
            }
            return true;
        }
    };
}
