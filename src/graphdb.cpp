#include "../include/graphdb.hpp"
#include <iostream>

using namespace std;

// Add a node
void GraphDB::addNode(int nodeId) {
    if (nodes.find(nodeId) != nodes.end()) {
        cout << "Node " << nodeId << " already exists.\n";
        return;
    }
    nodes[nodeId] = make_shared<Node>(nodeId);
}

// Get a node
shared_ptr<Node> GraphDB::getNode(int nodeId) {
    if (nodes.find(nodeId) != nodes.end()) {
        return nodes[nodeId];
    }
    return nullptr;
}

// Delete a node and its connected edges
void GraphDB::deleteNode(int nodeId) {
    if (nodes.find(nodeId) == nodes.end()) return;

    for (auto it = edges.begin(); it != edges.end();) {
        if (it->second->getSource() == nodeId || it->second->getTarget() == nodeId) {
            it = edges.erase(it);
        } else {
            ++it;
        }
    }

    nodes.erase(nodeId);
}

// Update node property
void GraphDB::updateNodeProperty(int nodeId, const string& key, const PropertyValue& value) {
    if (!hasNode(nodeId)) {
        cout << "Node " << nodeId << " not found.\n";
        return;
    }
    nodes[nodeId]->setProperty(key, value);
}

// Add an edge
void GraphDB::addEdge(int edgeId, int sourceId, int targetId) {
    if (edges.find(edgeId) != edges.end()) {
        cout << "Edge " << edgeId << " already exists.\n";
        return;
    }

    if (!hasNode(sourceId) || !hasNode(targetId)) {
        cout << "Source or target node does not exist.\n";
        return;
    }

    edges[edgeId] = make_shared<Edge>(edgeId, sourceId, targetId);
}

// Get an edge
shared_ptr<Edge> GraphDB::getEdge(int edgeId) {
    if (edges.find(edgeId) != edges.end()) {
        return edges[edgeId];
    }
    return nullptr;
}

// Delete an edge
void GraphDB::deleteEdge(int edgeId) {
    edges.erase(edgeId);
}

// Update edge property
void GraphDB::updateEdgeProperty(int edgeId, const string& key, const PropertyValue& value) {
    if (!hasEdge(edgeId)) {
        cout << "Edge " << edgeId << " not found.\n";
        return;
    }
    edges[edgeId]->setProperty(key, value);
}

// Utility
bool GraphDB::hasNode(int nodeId) const {
    return nodes.find(nodeId) != nodes.end();
}

bool GraphDB::hasEdge(int edgeId) const {
    return edges.find(edgeId) != edges.end();
}

// Get adjacent nodes
vector<int> GraphDB::getAdjacentNodes(int nodeId) const {
    vector<int> neighbors;
    for (const auto& pair : edges) {
        if (pair.second->getSource() == nodeId) {
            neighbors.push_back(pair.second->getTarget());
        }
    }
    return neighbors;
}

// BFS
void GraphDB::bfs(int startNodeId, function<void(int)> visit) {
    set<int> visited;
    queue<int> q;

    if (!hasNode(startNodeId)) return;

    q.push(startNodeId);
    visited.insert(startNodeId);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        visit(current);

        for (int neighbor : getAdjacentNodes(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
}

// DFS
void GraphDB::dfs(int startNodeId, function<void(int)> visit) {
    set<int> visited;
    stack<int> stack;

    if (!hasNode(startNodeId)) return;

    stack.push(startNodeId);

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {
            visit(current);
            visited.insert(current);

            // Reverse order for DFS to maintain logical order
            vector<int> neighbors = getAdjacentNodes(current);
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    stack.push(*it);
                }
            }
        }
    }
}
