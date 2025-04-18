#include <iostream>
#include "graphdb.hpp"

using namespace std;

int main() {
    // Create a GraphDB object
    GraphDB graph;

    // Add nodes
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5);

    // Add edges with weights
    graph.addEdge(1, 1, 2, 2.0); // Edge 1-2 with weight 2.0
    graph.addEdge(2, 1, 3, 1.0); // Edge 1-3 with weight 1.0
    graph.addEdge(3, 2, 3, 3.0); // Edge 2-3 with weight 3.0
    graph.addEdge(4, 2, 4, 1.0); // Edge 2-4 with weight 1.0
    graph.addEdge(5, 3, 4, 1.0); // Edge 3-4 with weight 1.0
    graph.addEdge(6, 4, 5, 1.0); // Edge 4-5 with weight 1.0

    // Call Dijkstra's algorithm
    unordered_map<int, vector<int>> paths;
    vector<int> distances = graph.dijkstra(1, paths);

    // Print distances and paths
    cout << "Distances from node 1:\n";
    for (int i = 1; i <= 5; ++i) {
        cout << "Node " << i << ": " << distances[i - 1] << endl;
    }

    cout << "\nPaths from node 1:\n";
    for (const auto& pathEntry : paths) {
        cout << "Path to Node " << pathEntry.first << ": ";
        for (int node : pathEntry.second) {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}
