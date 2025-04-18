#include <iostream>
#include "graphdb.hpp"

using namespace std;

int main() {
    GraphDB graph;

    // Add nodes
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);

    // Set node properties via GraphDB so indexing is updated
    graph.setNodeProperty(1, "type", "Person");
    graph.setNodeProperty(2, "type", "Company");
    graph.setNodeProperty(3, "type", "Person");

    // Add edges
    graph.addEdge(1, 1, 2, 1.0);
    graph.addEdge(2, 2, 3, 2.0);

    // Set edge properties via GraphDB so indexing is updated
    graph.setEdgeProperty(1, "relation", "works_at");
    graph.setEdgeProperty(2, "relation", "owns");

    // Lookup nodes with property
    vector<int> personNodes = graph.findNodesByProperty("type", "Person");
    cout << "Nodes with type 'Person': ";
    for (int id : personNodes) {
        cout << id << " ";
    }
    cout << endl;

    // Lookup edges with property
    vector<int> ownsEdges = graph.findEdgesByProperty("relation", "owns");
    cout << "Edges with relation 'owns': ";
    for (int id : ownsEdges) {
        cout << id << " ";
    }
    cout << endl;

    return 0;
}
