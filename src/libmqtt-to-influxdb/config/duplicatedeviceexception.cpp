#include <libmqtt-to-influxdb/config/duplicatedeviceexception.hpp>

DuplicateDeviceException::DuplicateDeviceException(const YAML::Mark &mark_, const std::string &msg_)
        : Exception(mark_, msg_) {}
