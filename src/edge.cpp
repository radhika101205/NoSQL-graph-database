#include "../include/edge.hpp"
#include <stdexcept>

using namespace std;

Edge::Edge(int edgeId, int from, int to) : id(edgeId), sourceId(from), targetId(to) {}

void Edge::setProperty(const string& key, const PropertyValue& value) {
    properties[key] = value;
}

void Edge::addProperty(const std::string& key, const PropertyValue& value) {
    setProperty(key, value);  
}


PropertyValue Edge::getProperty(const string& key) const {
    auto it = properties.find(key);
    if (it != properties.end()) return it->second;
    throw runtime_error("Property not found");
}

const unordered_map<string, PropertyValue>& Edge::getProperties() const {
    return properties;
}
