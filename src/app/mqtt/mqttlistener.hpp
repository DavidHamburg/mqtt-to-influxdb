#pragma once
#include <app/mqtt/imqttlistener.hpp>
#include <eventpp/eventdispatcher.h>
#include <memory>

class mqttclientcallback;
namespace mqtt{
    class async_client;
}

class mqttlistener : public imqttlistener {
public:
    mqttlistener(const std::string &mqtt_server_ip, const int port, const std::string &name, const std::string &topic);
    ~mqttlistener();
    void subscribe(std::function<void (mqtt::const_message_ptr)> action) override;
private:
    static constexpr auto event = 1;
    eventpp::EventDispatcher<int, void (mqtt::const_message_ptr msg)> m_dispatcher{};
    std::unique_ptr<mqtt::async_client> m_client;
    std::unique_ptr<mqttclientcallback> m_callback;

    std::string get_unique_mqtt_client_name(const std::string &name) const;
};
