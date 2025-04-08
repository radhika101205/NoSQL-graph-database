#pragma once
#include <string>
#include <unordered_map>
#include "property_value.hpp"

struct Edge {
    int id;
    int sourceId;
    int targetId;
    std::unordered_map<std::string, PropertyValue> properties;

    Edge(int edgeId, int from, int to);
    void setProperty(const std::string& key, const PropertyValue& value);
    PropertyValue getProperty(const std::string& key) const;
};
