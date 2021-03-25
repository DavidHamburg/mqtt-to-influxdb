#pragma once
#include <memory>
#include <string>
#include <app/mqtt/imqttlistener.hpp>

class imqttlistenerfactory {
public:
    virtual ~imqttlistenerfactory() = default;
    virtual std::shared_ptr<imqttlistener> create(const std::string &mqtt_server_ip, const std::string& name, const std::string& topic) const = 0;
};
