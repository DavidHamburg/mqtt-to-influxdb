#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/datatype.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>

struct field {
    std::string name{};
    std::string value{};
    std::string match{};
    std::string json_field{};
    datatype data_type{datatype::string_type};
    bool ignore_case{true};
};

namespace YAML {
    template<>
    struct convert<field> {
        static Node encode(const field &rhs) {
            Node node;
            node["name"] = rhs.name;
            node["value"] = rhs.value;
            node["match"] = rhs.match;
            switch (rhs.data_type) {
                case datatype::string_type:
                    node["data-type"] = "string";
                    break;
                case datatype::boolean_type:
                    node["data-type"] = "bool";
                    break;
                case datatype::float_type:
                    node["data-type"] = "float";
                    break;
                case datatype::integer_type:
                    node["data-type"] = "int";
                    break;
                default:
                    break;
            }
            return node;
        }

        static bool decode(const Node &node, field &rhs) {
            if (!node["name"].IsDefined()) {
                spdlog::error("Missing name (line: {})", node.Mark().line);
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            if (stringhelper::is_empty_or_whitespace(rhs.name)){
                spdlog::error("Measurement name must not be empty (line: {})", node.Mark().line);
                return false;
            }
            if (node["value"].IsDefined()) {
                rhs.value = node["value"].as<std::string>();
                if (stringhelper::is_empty_or_whitespace(rhs.value)) {
                    spdlog::error("Measurement value must not be empty (line: {})", node.Mark().line);
                    return false;
                }
            }
            if (node["match"].IsDefined()) {
                rhs.match = node["match"].as<std::string>();
            }
            if (node["json-field"].IsDefined()) {
                rhs.json_field = node["json-field"].as<std::string>();
            }
            if (node["ignore-case"].IsDefined()) {
                rhs.ignore_case = node["ignore-case"].as<bool>();
            }

            if (node["data-type"].IsDefined()) {
                auto data_type = node["data-type"].as<std::string>();
                if (data_type == "string") {
                    rhs.data_type = datatype::string_type;
                } else if (data_type == "bool") {
                    rhs.data_type = datatype::boolean_type;
                } else if (data_type == "float") {
                    rhs.data_type = datatype::float_type;
                } else if (data_type == "int") {
                    rhs.data_type = datatype::integer_type;
                } else {
                    spdlog::error("Unknown data type '{}' specified", data_type);
                    return false;
                }
            }
            return true;
        }
    };
}
