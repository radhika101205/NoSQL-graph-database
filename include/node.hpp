#pragma once
#include <string>
#include <unordered_map>
#include "property_value.hpp"

struct Node {
    int id;
    std::unordered_map<std::string, PropertyValue> properties;

    Node(int nodeId);
    void setProperty(const std::string& key, const PropertyValue& value);
    PropertyValue getProperty(const std::string& key) const;
};
