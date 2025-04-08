#include "edge.hpp"
#include <stdexcept>

Edge::Edge(int edgeId, int from, int to) : id(edgeId), sourceId(from), targetId(to) {}

void Edge::setProperty(const std::string& key, const PropertyValue& value) {
    properties[key] = value;
}

PropertyValue Edge::getProperty(const std::string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) return it->second;
    throw std::runtime_error("Property not found");
}
