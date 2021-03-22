#pragma once
#include <string>
#include <tuple>
#include <memory>
#include <libmqtt-to-influxdb/config/document.hpp>

class configuration {
public:
    std::tuple<bool, std::shared_ptr<document>> load(const std::string &input);
private:
    bool validate(const document &document);
};