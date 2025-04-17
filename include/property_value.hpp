#pragma once

#include <string>

enum class PropertyType { INT, FLOAT, STRING };

struct PropertyValue {
    PropertyType type;
    int intValue;
    float floatValue;
    std::string stringValue;

    PropertyValue() = default;

    PropertyValue(int val) : type(PropertyType::INT), intValue(val) {}
    PropertyValue(float val) : type(PropertyType::FLOAT), floatValue(val) {}
    PropertyValue(const std::string& val) : type(PropertyType::STRING), stringValue(val) {}
};
