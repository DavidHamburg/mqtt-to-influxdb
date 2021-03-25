#include <app/mqtt/mqttlistenerfactory.hpp>
#include <app/mqtt/mqttlistener.hpp>

std::shared_ptr<imqttlistener> mqttlistenerfactory::create(const std::string &mqtt_server_ip, const std::string& name, const std::string &topic) const {
    auto listener = std::make_shared<mqttlistener>(mqtt_server_ip, name, topic);
    return listener;
}
