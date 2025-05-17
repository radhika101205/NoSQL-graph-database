#include "../include/graphdb.hpp"
#include <iostream>
#include <limits>  
#include <algorithm>

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

//Set node and edge properties
void GraphDB::setNodeProperty(int nodeId, const std::string& key, const std::string& value) {
    if (nodes.count(nodeId)) {
        nodes[nodeId]->addProperty(key, value);
        nodeIndex[key][value].insert(nodeId); // Update index
    }
}

void GraphDB::setEdgeProperty(int edgeId, const std::string& key, const std::string& value) {
    if (edges.count(edgeId)) {
        edges[edgeId]->addProperty(key, value);
        edgeIndex[key][value].insert(edgeId); // Update index
    }
}


// Update node property
void GraphDB::updateNodeProperty(int nodeId, const string& key, const PropertyValue& value) {
    if (!hasNode(nodeId)) {
        cout << "Node " << nodeId << " not found.\n";
        return;
    }
    nodes[nodeId]->setProperty(key, value);
    if (value.type == PropertyType::STRING) {
        updateNodeIndex(nodeId, key, value.stringValue);
    }
}

// Add an edge
void GraphDB::addEdge(int edgeId, int sourceId, int targetId, double weight) {
    if (edges.find(edgeId) != edges.end()) {
        cout << "Edge " << edgeId << " already exists.\n";
        return;
    }

    if (!hasNode(sourceId) || !hasNode(targetId)) {
        cout << "Source or target node does not exist.\n";
        return;
    }

    auto edge = make_shared<Edge>(edgeId, sourceId, targetId);
    edge->setProperty("weight", PropertyValue(weight));
    edges[edgeId] = edge;
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
    if (value.type == PropertyType::STRING) {
        updateEdgeIndex(edgeId, key, value.stringValue);
    }
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

//Updating indexes

void GraphDB::updateNodeIndex(int nodeId, const string& key, const string& value) {
    nodeIndex[key][value].insert(nodeId);
}

void GraphDB::updateEdgeIndex(int edgeId, const string& key, const string& value) {
    edgeIndex[key][value].insert(edgeId);
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

//Property based lookup methods

vector<int> GraphDB::findNodesByProperty(const string& key, const string& value) {
    vector<int> result;
    if (nodeIndex.count(key) && nodeIndex[key].count(value)) {
        for (int id : nodeIndex[key][value]) {
            result.push_back(id);
        }
    }
    return result;
}

vector<int> GraphDB::findEdgesByProperty(const string& key, const string& value) {
    vector<int> result;
    if (edgeIndex.count(key) && edgeIndex[key].count(value)) {
        for (int id : edgeIndex[key][value]) {
            result.push_back(id);
        }
    }
    return result;
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

//Dijkstra's algo

unordered_map<int, double> GraphDB::dijkstra(int startNodeId, unordered_map<int, vector<int>>& paths) {
    // Step 1: Initialize distances and priority queue
    unordered_map<int, double> dist; // Node ID -> Distance
    unordered_map<int, int> previous; // Node ID -> Previous node
    
    // Initialize distances to infinity (except for the start node)
    for (auto& node : nodes) {
        dist[node.first] = numeric_limits<double>::infinity();  // Initially set to infinity
        previous[node.first] = -1;  // No previous node
    }

    dist[startNodeId] = 0; // Start node distance is 0

    // Priority queue to select node with smallest distance
    auto compare = [&dist](int left, int right) {
        return dist[left] > dist[right];
    };

    priority_queue<int, vector<int>, decltype(compare)> pq(compare);
    pq.push(startNodeId);

    while (!pq.empty()) {
        int currentNode = pq.top();
        pq.pop();

        // Process neighbors
        for (const auto& edgePair : edges) {
            auto edge = edgePair.second;
            if (edge->getSource() != currentNode) continue;

            int neighbor = edge->getTarget();
            double weight = edge->getProperty("weight").getFloat();
            double newDist = dist[currentNode] + weight;

            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                previous[neighbor] = currentNode;
                pq.push(neighbor);
            }
        }
    }

    // Reconstruct paths from startNode to every other node
    // for (const auto& pair : nodes) {
    //     int target = pair.first;
    //     vector<int> path;
    //     int current = target;

    //     // Skip unreachable nodes
    //     if (dist[current] == numeric_limits<double>::infinity()) continue;

    //     while (current != -1) {
    //         path.push_back(current);
    //         current = previous[current];
    //     }

    //     reverse(path.begin(), path.end());
    //     paths[target] = path;  // ✅ Store the correct path
    // }

    // // Return distances in order of node ids (optional, adjust if needed)
    // vector<double> result;
    // for (const auto& pair : nodes) {
    //     result.push_back(dist[pair.first]);
    // }

    // return result;
    for (const auto& pair : dist) {
        int nodeId = pair.first;
        vector<int> path;
        for (int at = nodeId; at != -1; at = previous[at]) {
            path.insert(path.begin(), at);
        }
        if (!path.empty() && path[0] == startNodeId)
            paths[nodeId] = path;
    }
    
    return dist;
}
