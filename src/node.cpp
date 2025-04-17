#include "../include/node.hpp"
#include <stdexcept>


Node::Node(int id) : id(id) {}

void Node::setProperty(const string& key, const PropertyValue& value) {
    properties[key] = value;
}

PropertyValue Node::getProperty(const string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) return it->second;
    throw runtime_error("Property not found");
}

// ✅ Add this implementation
const unordered_map<string, PropertyValue>& Node::getProperties() const {
    return properties;
}
