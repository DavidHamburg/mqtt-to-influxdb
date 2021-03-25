#include "mqttclientcallback.hpp"
#include "logactionlistener.hpp"
#include <iostream>

mqttclientcallback::mqttclientcallback(mqtt::async_client &cli,
                                       mqtt::connect_options &connOpts,
                                       const std::string &clientid,
                                       const std::string &topic)
    : nretry_(0), m_cli(cli), connOpts_(connOpts),
      subListener_(logactionlistener{"Subscription"}), m_clientid(clientid),
      m_topic(topic) {}

void mqttclientcallback::reconnect() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  try {
    m_cli.connect(connOpts_, nullptr, *this);
  } catch (const mqtt::exception &exc) {
    std::cerr << "Error: " << exc.what() << std::endl;
    exit(1);
  }
}

void mqttclientcallback::on_failure(const mqtt::token &) {
  spdlog::error("Connection attempt failed");
  if (++nretry_ > N_RETRY_ATTEMPTS)
    exit(1);
  reconnect();
}

void mqttclientcallback::on_success(const mqtt::token &) {}

void mqttclientcallback::connected(const std::string &) {
  spdlog::info("Subscribing to topic '{}' for client {} using QoS {}", m_topic, m_clientid, QOS);
  m_cli.subscribe(m_topic, QOS, nullptr, subListener_);
}

void mqttclientcallback::connection_lost(const std::string &cause) {
  std::stringstream ss;
  ss << "\nConnection lost" << std::endl;
  if (!cause.empty())
    ss << "\tcause: " << cause << std::endl;

  ss << "Reconnecting..." << std::endl;
  spdlog::error(ss.str());
  nretry_ = 0;
  reconnect();
}

void mqttclientcallback::message_arrived(mqtt::const_message_ptr msg) {
  std::stringstream ss;
  ss << "Message arrived" << std::endl;
  ss << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
  ss << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
  spdlog::debug(ss.str());
  if (on_message) {
    on_message(msg);
  }
}

void mqttclientcallback::delivery_complete(mqtt::delivery_token_ptr) {}
