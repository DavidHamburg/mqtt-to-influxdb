#include <libmqtt-to-influxdb/stringhelper.hpp>

bool stringhelper::is_empty_or_whitespace(const std::string &input) {
    if (input == "") {
        return true;
    }
    return input.find_first_not_of(' ') == std::string::npos;
}

bool stringhelper::is_equal(const std::string &a, const std::string &b, bool case_sensitive) {
    if (case_sensitive) {
        return std::equal(a.begin(), a.end(), b.begin());
    }
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](const char &a, const char &b) {
                          return (std::tolower(a) == std::tolower(b));
                      });
}
