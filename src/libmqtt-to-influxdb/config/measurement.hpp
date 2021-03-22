#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/config/field.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>

struct measurement {
    std::string name{};
    std::vector<field> fields{};
};

namespace YAML {
    template<>
    struct convert<measurement> {
        static Node encode(const measurement &rhs) {
            Node node;
            node["name"] = rhs.name;
            node["fields"] = rhs.fields;
            return node;
        }

        static bool decode(const Node &node, measurement &rhs) {
            if (!node["name"].IsDefined()) {
                spdlog::error("Missing name (line: {})", node.Mark().line);
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            if (stringhelper::is_empty_or_whitespace(rhs.name)){
                spdlog::error("Measurement name must not be empty (line: {})", node.Mark().line);
                return false;
            }
            if (!node["fields"].IsDefined()) {
                spdlog::error("Missing fields (line: {})", node.Mark().line);
                return false;
            }
            if (!node["fields"].IsSequence()) {
                spdlog::error("Fields must be a sequence (line: {})", node.Mark().line);
                return false;
            }
            rhs.fields = node["fields"].as<std::vector<field>>();
            return true;
        }
    };
}
