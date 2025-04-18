#pragma once
#include <string>
#include <unordered_map>
#include "property_value.hpp"

using namespace std;

class Edge {
private:
    int id;
    int sourceId;
    int targetId;
    unordered_map<string, PropertyValue> properties;

public:
    Edge(int edgeId, int from, int to);

    void setProperty(const string& key, const PropertyValue& value);
    PropertyValue getProperty(const string& key) const;
    void addProperty(const std::string& key, const PropertyValue& value);  // Alias for setProperty

    const unordered_map<string, PropertyValue>& getProperties() const;

    int getId() const { return id; }
    int getSource() const { return sourceId; }
    int getTarget() const { return targetId; }
};
