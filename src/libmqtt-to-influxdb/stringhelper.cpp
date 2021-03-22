#include <libmqtt-to-influxdb/stringhelper.hpp>

bool stringhelper::is_empty_or_whitespace(const std::string &input) {
    if (input == "") {
        return true;
    }
    return input.find_first_not_of(' ') == std::string::npos;
}
