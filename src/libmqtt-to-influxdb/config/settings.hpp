#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/stringhelper.hpp>

struct settings {
    std::string broker_ip{};
    int broker_port{};
    std::string influxdb_host{};
    int influxdb_port{};
    std::string influxdb_user{};
    std::string influxdb_password{};
    std::string influxdb_database{"iot"};
    std::string influxdb_protocol{"http"};
};

namespace YAML {
    template<>
    struct convert<settings> {
        static bool decode(const Node &node, settings &rhs) {
            if (!node["broker"].IsDefined()) {
                spdlog::error("Missing broker (line: {})", node.Mark().line);
                return false;
            }
            auto &broker = node["broker"];
            if (!broker["ip"].IsDefined()) {
                spdlog::error("Missing broker ip (line: {})", node.Mark().line);
                return false;
            }
            if (!broker["port"].IsDefined()) {
                spdlog::error("Missing broker port (line: {})", node.Mark().line);
                return false;
            }

            rhs.broker_ip = broker["ip"].as<std::string>();
            if (stringhelper::is_empty_or_whitespace(rhs.broker_ip)) {
                spdlog::error("Broker ip must not be empty (line: {})", node.Mark().line);
                return false;
            }

            rhs.broker_port = broker["port"].as<int>();

            if (!node["influxdb"].IsDefined()) {
                spdlog::error("Missing influxdb (line: {})", node.Mark().line);
                return false;
            }
            auto &influxdb = node["influxdb"];
            if (!influxdb["host"].IsDefined()) {
                spdlog::error("Missing influxdb host (line: {})", node.Mark().line);
                return false;
            }
            if (!influxdb["port"].IsDefined()) {
                spdlog::error("Missing influxdb port (line: {})", node.Mark().line);
                return false;
            }

            rhs.influxdb_host = influxdb["host"].as<std::string>();
            if (stringhelper::is_empty_or_whitespace(rhs.influxdb_host)) {
                spdlog::error("Influxdb host must not be empty (line: {})", node.Mark().line);
                return false;
            }

            rhs.influxdb_port = influxdb["port"].as<int>();

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
