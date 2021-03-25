#define UUID_SYSTEM_GENERATOR
#include <app/mqtt/mqttlistener.hpp>
#include <mqtt/connect_options.h>
#include <mqtt/async_client.h>
#include <app/mqtt/internal/mqttclientcallback.hpp>
#include <uuid.h>
#include <sstream>

mqttlistener::mqttlistener(const std::string &mqtt_server_ip, const std::string &name, const std::string &topic) {
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    const std::string address("tcp://" + mqtt_server_ip);
    const auto clientid = get_unique_mqtt_client_name(name);
    m_client = std::make_unique<mqtt::async_client>(address, clientid);

    m_callback = std::make_unique<mqttclientcallback>(*m_client, connOpts, clientid, topic);
    m_client->set_callback(*m_callback);
    m_client->connect(connOpts)->wait();

    m_callback->on_message = [&](mqtt::const_message_ptr msg) {
        m_dispatcher.dispatch(event, msg);
    };
}

std::string mqttlistener::get_unique_mqtt_client_name(const std::string &name) const {
    auto id = uuids::uuid_system_generator{}();
    std::stringstream ss{};
    ss << name << "-subscriber-" << uuids::to_string(id);
    return ss.str();
}

void mqttlistener::subscribe(std::function<void(mqtt::const_message_ptr)> action) {
    m_dispatcher.appendListener(event, action);
}

mqttlistener::~mqttlistener() {
    if (m_client->is_connected()) {
        m_client->disconnect()->wait();
    }
}