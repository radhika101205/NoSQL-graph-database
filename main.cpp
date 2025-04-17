#include "graphdb.hpp"
#include <iostream>

using namespace std;

int main() {
    GraphDB db;

    // Add nodes
    db.addNode(1);
    db.addNode(2);

    // Add properties to nodes
    db.updateNodeProperty(1, "name", PropertyValue("Node A"));
    db.updateNodeProperty(1, "age", PropertyValue(25));
    db.updateNodeProperty(2, "name", PropertyValue("Node B"));
    db.updateNodeProperty(2, "temperature", PropertyValue(36.6f));

    // Add an edge
    db.addEdge(100, 1, 2);
    db.updateEdgeProperty(100, "weight", PropertyValue(10));
    db.updateEdgeProperty(100, "label", PropertyValue("A_to_B"));

    // Display Node 1 Properties
    cout << "Properties of Node 1:\n";
    auto node1 = db.getNode(1);
    if (node1) {
        for (const auto& p : node1->getProperties()) {
            cout << "  " << p.first << " = ";
            switch (p.second.type) {
                case PropertyType::INT: cout << p.second.intValue; break;
                case PropertyType::FLOAT: cout << p.second.floatValue; break;
                case PropertyType::STRING: cout << p.second.stringValue; break;
            }
            cout << endl;
        }
    }

    // Display Edge 100 Properties
    cout << "\nProperties of Edge 100:\n";
    auto edge = db.getEdge(100);
    if (edge) {
        for (const auto& p : edge->getProperties()) {
            cout << "  " << p.first << " = ";
            switch (p.second.type) {
                case PropertyType::INT: cout << p.second.intValue; break;
                case PropertyType::FLOAT: cout << p.second.floatValue; break;
                case PropertyType::STRING: cout << p.second.stringValue; break;
            }
            cout << endl;
        }
    }

    return 0;
}
