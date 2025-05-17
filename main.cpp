#include <iostream>
#include <unordered_map>
#include "graphdb.hpp"

using namespace std;

int main() {
    GraphDB graph;

    cout << "=== GRAPH INITIALIZATION ===\n";

    // Add nodes
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5);

    

    cout << "Nodes added with properties.\n";

    // Add edges with weights
    graph.addEdge(1, 1, 2, 2.0);     // 1 -> 2
    graph.addEdge(2, 1, 3, 1.0);     // 1 -> 3
    graph.addEdge(3, 2, 3, 3.0);     // 2 -> 3
    graph.addEdge(4, 2, 4, 1.0);     // 2 -> 4
    graph.addEdge(5, 3, 4, 1.0);     // 3 -> 4
    graph.addEdge(6, 4, 5, 1.0);     // 4 -> 5

    graph.setNodeProperty(1, "type", "Person");
    graph.setNodeProperty(2, "type", "Company");
    graph.setNodeProperty(3, "type", "Person");
    graph.setNodeProperty(4, "type", "Company");
    graph.setNodeProperty(5, "type", "Location");

    graph.setEdgeProperty(1, "relation", "works_at");
    graph.setEdgeProperty(2, "relation", "owns");
    graph.setEdgeProperty(3, "relation", "partner");
    graph.setEdgeProperty(4, "relation", "connects");
    graph.setEdgeProperty(5, "relation", "linked");
    graph.setEdgeProperty(6, "relation", "located");

    cout << "Edges added with properties and weights.\n";

    // -------------------------
    cout << "\n=== SHORTEST PATH (DIJKSTRA) ===\n";

    unordered_map<int, vector<int>> paths; 
    
    unordered_map<int, double> distances = graph.dijkstra(1, paths);

    for (auto& pair : paths) {
        cout << "Shortest path to node " << pair.first << ": ";
        for (int id : pair.second) {
            cout << id << " ";
        }
        cout << "(Distance: " << distances[pair.first] << ")\n";
    }
    
    

    // -------------------------
    cout << "\n=== INDEXING LOOKUP ===\n";

    vector<int> personNodes = graph.findNodesByProperty("type", "Person");
    cout << "Nodes with type 'Person': ";
    for (int id : personNodes) cout << id << " ";
    cout << endl;

    vector<int> ownsEdges = graph.findEdgesByProperty("relation", "owns");
    cout << "Edges with relation 'owns': ";
    for (int id : ownsEdges) cout << id << " ";
    cout << endl;

    cout << "\n=== DEMO COMPLETE ===\n";

    return 0;
}