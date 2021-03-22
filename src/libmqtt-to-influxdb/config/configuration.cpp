#include <libmqtt-to-influxdb/config/configuration.hpp>
#include <libmqtt-to-influxdb/config/document.hpp>

std::tuple<bool, std::shared_ptr<document>> configuration::load(const std::string &input) {
    auto doc = std::make_unique<document>();
    if (input.empty()){
        return std::make_tuple(false, std::move(doc));
    }
    YAML::Node node = YAML::Load(input);
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

bool configuration::validate(const document &document) {
    if (document.devices.empty()) {
        spdlog::warn("No device configuration found.");
        return false;
    }
    return true;
}
