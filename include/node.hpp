#pragma once
#include <string>
#include <unordered_map>
#include "property_value.hpp"

using namespace std;

struct Node {
    int id;
    unordered_map<string, PropertyValue> properties;

    Node(int nodeId);
    void setProperty(const string& key, const PropertyValue& value);
    PropertyValue getProperty(const string& key) const;
    int getId() const { return id; }

    const  unordered_map< string, PropertyValue>& getProperties() const;
};
