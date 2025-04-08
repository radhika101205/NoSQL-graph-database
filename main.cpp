#include <iostream>
#include "node.hpp"
#include "edge.hpp"

int main() {
    Node node1(1);
    node1.setProperty("name", PropertyValue("Alice"));
    node1.setProperty("age", PropertyValue(25));

    Edge edge1(100, 1, 2);
    edge1.setProperty("weight", PropertyValue(3.14f));
    edge1.setProperty("type", PropertyValue("connection"));

    std::cout << "Node " << node1.id << " - Name: " 
              << node1.getProperty("name").asString()
              << ", Age: " << node1.getProperty("age").asInt() << std::endl;

    std::cout << "Edge " << edge1.id << " - Type: " 
              << edge1.getProperty("type").asString()
              << ", Weight: " << edge1.getProperty("weight").asFloat() << std::endl;

    return 0;
}
