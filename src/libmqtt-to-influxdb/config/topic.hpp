#pragma once
#include <string>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/config/measurement.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>

struct topic {
    std::string name{};
    std::vector<measurement> measurements{};
};

namespace YAML {
    template<>
    struct convert<topic> {
        static Node encode(const topic& rhs) {
            Node node;
            node["topic"] = rhs.name;
            node["measurements"] = rhs.measurements;
            return node;
        }

        static bool decode(const Node& node, topic& rhs) {
            if (!node["topic"].IsDefined()) {
                spdlog::error("Missing topic (line: {})", node.Mark().line);
                return false;
            }
            if (!node["measurements"].IsDefined()) {
                spdlog::error("Missing measurements (line: {})", node.Mark().line);
                return false;
            }
            rhs.name = node["topic"].as<std::string>();
            if (stringhelper::is_empty_or_whitespace(rhs.name)) {
                spdlog::error("Topic name must not be empty (line: {})", node.Mark().line);
                return false;
            }

            rhs.measurements = node["measurements"].as<std::vector<measurement>>();
            return true;
        }
    };
}
