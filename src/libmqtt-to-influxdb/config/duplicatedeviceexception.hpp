#pragma once
#include <string>
#include <yaml-cpp/exceptions.h>

namespace YAML {
    class Mark;
}

class DuplicateDeviceException : public YAML::Exception {
public:
    DuplicateDeviceException(const YAML::Mark &mark_, const std::string &msg_);
    DuplicateDeviceException(const DuplicateDeviceException &) = default;

    virtual ~DuplicateDeviceException() = default;
};

