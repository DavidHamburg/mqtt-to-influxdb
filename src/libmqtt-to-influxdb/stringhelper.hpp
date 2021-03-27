#pragma once
#include <string>
#include <vector>

class stringhelper {
public:
    static bool is_empty_or_whitespace(const std::string &input);
    static bool is_equal(const std::string &a, const std::string &b, bool case_sensitive = true);
    static bool to_bool(std::string value);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};