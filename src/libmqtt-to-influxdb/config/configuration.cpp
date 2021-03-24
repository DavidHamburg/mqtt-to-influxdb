#include <libmqtt-to-influxdb/config/configuration.hpp>
#include <libmqtt-to-influxdb/config/document.hpp>

std::tuple<bool, std::shared_ptr<document>> configuration::load(const std::string &input) {
    if (input.empty()){
        auto doc = std::make_unique<document>();
        return std::make_tuple(false, std::move(doc));
    }
    auto node = YAML::Load(input);
    return load_yaml(node);
}

bool configuration::validate(const document &document) {
    if (document.devices.empty()) {
        spdlog::warn("No device configuration found.");
        return false;
    }
    return true;
}

std::tuple<bool, std::shared_ptr<document>> configuration::load_file(const std::string &filename) {
    auto node = YAML::LoadFile(filename);
    return load_yaml(node);
}

std::tuple<bool, std::shared_ptr<document>> configuration::load_yaml(const YAML::Node &node) {
    auto doc = std::make_unique<document>();
    try{
        auto d = node.as<document>();
        doc = std::make_unique<document>(d);
    } catch(YAML::Exception &exc) {
        spdlog::error("Parsing configuration failed with: {}", exc.what());
        return std::make_tuple(false, std::move(doc));
    }
    auto success =  validate(*doc);
    return std::make_tuple(success, std::move(doc));
}
