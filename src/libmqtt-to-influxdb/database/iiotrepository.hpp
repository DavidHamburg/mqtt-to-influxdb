#pragma once
#include <string>
#include <libmqtt-to-influxdb/mqttparseresult.hpp>

class iiot_repository {
public:
  virtual ~iiot_repository() = default;

  virtual void write_mqtt_result(const mqtt_parse_result &data) = 0;
};
