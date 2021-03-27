#pragma once
#include <app/mqtt/imqttlistenerfactory.hpp>

class mqttlistenerfactory : public imqttlistenerfactory {
public:
    std::shared_ptr<imqttlistener> create(const std::string &mqtt_server_ip, const int port, const std::string &name, const std::string& topic) const override;
};
