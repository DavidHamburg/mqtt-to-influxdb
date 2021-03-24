#pragma once
#include <ostream>

enum class datatype {
    string_type, boolean_type, integer_type, float_type
};

inline std::ostream& operator << (std::ostream& os, const datatype& obj)
{
    switch (obj) {
        case datatype::string_type:
            os << "string";
            break;
        case datatype::boolean_type:
            os << "bool";
            break;
        case datatype::integer_type:
            os << "int";
            break;
        case datatype::float_type:
            os << "float";
            break;
        default:
            os << "unknown data type";
            break;
    }
    return os;
}

