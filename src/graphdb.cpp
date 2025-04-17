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
