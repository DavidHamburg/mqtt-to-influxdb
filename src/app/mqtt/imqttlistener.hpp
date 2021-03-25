#pragma once
#include <string>
#include <mqtt/message.h>
#include <functional>

class imqttlistener {
public:
  virtual ~imqttlistener() = default;
  virtual void subscribe(std::function<void (mqtt::const_message_ptr)> action) = 0;
};
