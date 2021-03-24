#pragma once
#include <string>

class stringhelper {
public:
    static bool is_empty_or_whitespace(const std::string &input);
    static bool is_equal(const std::string &a, const std::string &b, bool case_sensitive = true);
};