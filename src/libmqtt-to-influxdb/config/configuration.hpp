#pragma once
#include <string>
#include <tuple>
#include <memory>
#include <libmqtt-to-influxdb/config/document.hpp>

namespace YAML{
    class Node;
}

class configuration {
public:
    std::tuple<bool, std::shared_ptr<document>> load(const std::string &input);
    std::tuple<bool, std::shared_ptr<document>> load_file(const std::string &filename);
private:
    bool validate(const document &document);
    std::tuple<bool, std::shared_ptr<document>> load_yaml(const YAML::Node &node);
};