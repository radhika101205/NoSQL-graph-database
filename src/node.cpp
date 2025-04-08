#include "node.hpp"
#include <stdexcept>

Node::Node(int nodeId) : id(nodeId) {}

void Node::setProperty(const std::string& key, const PropertyValue& value) {
    properties[key] = value;
}

PropertyValue Node::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) return it->second;
    throw std::runtime_error("Property not found");
}
