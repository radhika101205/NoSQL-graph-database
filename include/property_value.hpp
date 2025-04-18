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

    // Add this constructor to handle double values
    PropertyValue(double val) : type(PropertyType::FLOAT), floatValue(static_cast<float>(val)) {}

    PropertyValue(const std::string& val) : type(PropertyType::STRING), stringValue(val) {}

    // Getters
    int getInt() const {
        if (type == PropertyType::INT) return intValue;
        throw std::invalid_argument("Incorrect type");
    }

    float getFloat() const {
        if (type == PropertyType::FLOAT) return floatValue;
        throw std::invalid_argument("Incorrect type");
    }

    std::string getString() const {
        if (type == PropertyType::STRING) return stringValue;
        throw std::invalid_argument("Incorrect type");
    }
};
