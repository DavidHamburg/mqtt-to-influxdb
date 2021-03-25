#pragma once

#include <mqtt/callback.h>
#include <functional>
#include <mqtt/async_client.h>
#include "logactionlistener.hpp"

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class mqttclientcallback : public virtual mqtt::callback,
                           public virtual mqtt::iaction_listener {
public:
  std::function<void(mqtt::const_message_ptr msg)> on_message{nullptr};
  mqttclientcallback(mqtt::async_client &cli, mqtt::connect_options &connOpts,
                     const std::string &clientid, const std::string &topic);

private:
  int nretry_;
  mqtt::async_client &m_cli;
  mqtt::connect_options &connOpts_;
  // An action listener to display the result of actions.
  logactionlistener subListener_;
  std::string m_clientid, m_topic;

  void reconnect();
  void on_failure(const mqtt::token &tok) override;

  void on_success(const mqtt::token &tok) override;
  void connected(const std::string &cause) override;
  void connection_lost(const std::string &cause) override;
  void message_arrived(mqtt::const_message_ptr msg) override;
  void delivery_complete(mqtt::delivery_token_ptr token) override;
};
