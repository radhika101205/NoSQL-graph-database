#pragma once

#include "node.hpp"
#include "edge.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <set>
#include <queue>
#include <stack>

using namespace std;

class GraphDB {
private:
    unordered_map<int, shared_ptr<Node>> nodes;
    unordered_map<int, shared_ptr<Edge>> edges;

public:
    // Node operations
    void addNode(int nodeId);
    shared_ptr<Node> getNode(int nodeId);
    void deleteNode(int nodeId);
    void updateNodeProperty(int nodeId, const string& key, const PropertyValue& value);

    // Edge operations
    void addEdge(int edgeId, int sourceId, int targetId, double weight = 1.0);
    shared_ptr<Edge> getEdge(int edgeId);
    void deleteEdge(int edgeId);
    void updateEdgeProperty(int edgeId, const string& key, const PropertyValue& value);

    // Utility
    bool hasNode(int nodeId) const;
    bool hasEdge(int edgeId) const;

    // Traversal
    void bfs(int startNodeId, function<void(int)> visit);
    void dfs(int startNodeId, function<void(int)> visit);
    vector<int> getAdjacentNodes(int nodeId) const;

    //Dijstra's algorithm
    vector<int> dijkstra(int startNodeId,unordered_map<int, vector<int>>& paths);
};
