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

    // Add properties to nodes
    graph.getNode(1)->addProperty("type", string("Person"));
    graph.getNode(2)->addProperty("type", string("Company"));
    graph.getNode(3)->addProperty("type", string("Person"));
    graph.getNode(4)->addProperty("type", string("Company"));
    graph.getNode(5)->addProperty("type", string("Location"));

    cout << "Nodes added with properties.\n";

    // Add edges with weights
    graph.addEdge(1, 1, 2, 2.0);     // 1 -> 2
    graph.addEdge(2, 1, 3, 1.0);     // 1 -> 3
    graph.addEdge(3, 2, 3, 3.0);     // 2 -> 3
    graph.addEdge(4, 2, 4, 1.0);     // 2 -> 4
    graph.addEdge(5, 3, 4, 1.0);     // 3 -> 4
    graph.addEdge(6, 4, 5, 1.0);     // 4 -> 5

    // Add properties to edges
    graph.getEdge(1)->addProperty("relation", string("works_at"));
    graph.getEdge(2)->addProperty("relation", string("owns"));
    graph.getEdge(3)->addProperty("relation", string("partner"));
    graph.getEdge(4)->addProperty("relation", string("connects"));
    graph.getEdge(5)->addProperty("relation", string("linked"));
    graph.getEdge(6)->addProperty("relation", string("located"));

    cout << "Edges added with properties and weights.\n";

    // -------------------------
    cout << "\n=== SHORTEST PATH (DIJKSTRA) ===\n";

    unordered_map<int, vector<int>> paths;
    vector<double> distances = graph.dijkstra(1, paths);  // from node 1

    for (auto& pair : paths) {
        cout << "Shortest path to node " << pair.first << ": ";
        for (int id : pair.second) {
            cout << id << " ";
        }
        cout << "(Distance: " << distances[pair.first - 1] << ")\n"; // assuming node IDs start from 1
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