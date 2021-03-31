#include <libmqtt-to-influxdb/stringhelper.hpp>
#include <algorithm>
#include <sstream>

bool stringhelper::is_empty_or_whitespace(const std::string &input) {
    if (input == "") {
        return true;
    }
    return input.find_first_not_of(' ') == std::string::npos;
}

bool stringhelper::is_equal(const std::string &a, const std::string &b, bool case_sensitive) {
    if (a.size() != b.size()) {
        return false;
    }
    if (case_sensitive) {
        return std::equal(a.begin(), a.end(), b.begin());
    }
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](const char &a, const char &b) {
                          return (std::tolower(a) == std::tolower(b));
                      });
}

bool stringhelper::to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "true" || str == "on") {
        return true;
    }
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    if (!is.good()){
        return false;
    }
    return b;
}

std::vector<std::string> stringhelper::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}