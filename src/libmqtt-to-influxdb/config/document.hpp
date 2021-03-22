#pragma once
#include <string>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <spdlog/spdlog.h>
#include <libmqtt-to-influxdb/config/device.hpp>
#include <libmqtt-to-influxdb/config/duplicatedeviceexception.hpp>
#include <set>

struct document {
    std::vector<device> devices{};
};

namespace YAML {
    template<>
    struct convert<document> {

        static bool decode(const Node& node, document& rhs) {
            std::set<std::string> names{};
            for(auto it : node) {
                device d;
                d.name = it.first.as<std::string>();

                if (names.find(d.name) != names.end()) {
                    std::stringstream msg{};
                    msg << "duplicated device '" << d.name << "' found";
                    throw DuplicateDeviceException(it.first.Mark(), msg.str());
                }

                d.topics = it.second.as<std::vector<topic>>();
                rhs.devices.push_back(d);

                names.insert(d.name);
            }

            return true;
        }
    };
}

