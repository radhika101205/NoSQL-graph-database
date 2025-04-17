#include "include/graphdb.hpp"
#include <iostream>

int main() {
    GraphDB graph;

    // Adding nodes
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);
    graph.addNode(5);

    // Adding edges
    graph.addEdge(101, 1, 2);
    graph.addEdge(102, 1, 3);
    graph.addEdge(103, 2, 4);
    graph.addEdge(104, 3, 5);
    graph.addEdge(105, 4, 5);

    std::cout << "BFS starting from node 1:\n";
    graph.bfs(1, [](int nodeId) {
        std::cout << "Visited (BFS): " << nodeId << std::endl;
    });

    std::cout << "\nDFS starting from node 1:\n";
    graph.dfs(1, [](int nodeId) {
        std::cout << "Visited (DFS): " << nodeId << std::endl;
    });

    return 0;
}
