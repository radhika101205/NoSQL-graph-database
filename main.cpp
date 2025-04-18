#include "graphdb.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    GraphDB graph;

    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5);

    graph.addEdge(1, 1, 2, 2.0);
    graph.addEdge(2, 1, 3, 2.0);
    graph.addEdge(3, 2, 3, 3.0);
    graph.addEdge(4, 2, 4, 1.0);
    graph.addEdge(5, 3, 4, 1.0);
    graph.addEdge(6, 4, 5, 1.0);

    std::unordered_map<int, std::vector<int>> paths;
    std::vector<double> distances = graph.dijkstra(1, paths);

    std::cout << "Distances from node 1:\n";
    int i = 0;
    for (const auto& pair : graph.getAllNodes()) {
        std::cout << "Node " << pair.first << ": " << distances[i++] << std::endl;
    }

    std::cout << "\nPaths from node 1:\n";
    for (const auto& pair : paths) {
        std::cout << "Path to Node " << pair.first << ": ";
        for (int node : pair.second) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
