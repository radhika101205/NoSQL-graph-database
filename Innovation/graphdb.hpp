#ifndef GRAPHDB_HPP
#define GRAPHDB_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <queue>
#include <stack>
#include <limits>
#include <set>

enum class PropertyType
{
    INT,
    FLOAT,
    STRING
};

struct PropertyValue
{
    PropertyType type;
    int intValue;
    double floatValue;
    std::string stringValue;

    PropertyValue() = default;

    PropertyValue(int v) : type(PropertyType::INT), intValue(v) {}
    PropertyValue(double v) : type(PropertyType::FLOAT), floatValue(v) {}
    PropertyValue(const std::string &v) : type(PropertyType::STRING), stringValue(v) {}

    std::string toString() const
    {
        switch (type)
        {
        case PropertyType::INT:
            return std::to_string(intValue);
        case PropertyType::FLOAT:
            return std::to_string(floatValue);
        case PropertyType::STRING:
            return stringValue;
        default:
            return "";
        }
    }

    double getFloat() const
    {
        return type == PropertyType::FLOAT ? floatValue : 1.0;
    }
};

class Node
{
    int id;
    std::unordered_map<std::string, PropertyValue> properties;

public:
    Node(int id);
    void setProperty(const std::string &key, const PropertyValue &value);
    void addProperty(const std::string &key, const PropertyValue &value);
    PropertyValue getProperty(const std::string &key) const;
    const std::unordered_map<std::string, PropertyValue> &getProperties() const;
    int getId() const { return id; }
};

class Edge
{
    int id;
    int sourceId;
    int targetId;
    std::unordered_map<std::string, PropertyValue> properties;

public:
    Edge(int edgeId, int from, int to);
    void setProperty(const std::string &key, const PropertyValue &value);
    void addProperty(const std::string &key, const PropertyValue &value);
    PropertyValue getProperty(const std::string &key) const;
    const std::unordered_map<std::string, PropertyValue> &getProperties() const;
    int getSource() const { return sourceId; }
    int getTarget() const { return targetId; }
    int getId() const { return id; }
};

class GraphDB
{
    std::unordered_map<int, std::shared_ptr<Node>> nodes;
    std::unordered_map<int, std::shared_ptr<Edge>> edges;

    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<int>>> nodeIndex;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<int>>> edgeIndex;

public:
    void addNode(int nodeId);
    void deleteNode(int nodeId);
    void updateNodeProperty(int nodeId, const std::string &key, const PropertyValue &value);
    std::shared_ptr<Node> getNode(int nodeId);

    void addEdge(int edgeId, int from, int to, double weight);
    void deleteEdge(int edgeId);
    void updateEdgeProperty(int edgeId, const std::string &key, const PropertyValue &value);
    std::shared_ptr<Edge> getEdge(int edgeId);

    void setNodeProperty(int nodeId, const std::string &key, const std::string &value);
    void setEdgeProperty(int edgeId, const std::string &key, const std::string &value);

    bool hasNode(int nodeId) const;
    bool hasEdge(int edgeId) const;

    std::vector<int> getAdjacentNodes(int nodeId) const;

    void updateNodeIndex(int nodeId, const std::string &key, const std::string &value);
    void updateEdgeIndex(int edgeId, const std::string &key, const std::string &value);

    std::vector<int> findNodesByProperty(const std::string &key, const std::string &value);
    std::vector<int> findEdgesByProperty(const std::string &key, const std::string &value);

    void bfs(int startNodeId, std::function<void(int)> visit);
    void dfs(int startNodeId, std::function<void(int)> visit);
    std::vector<double> dijkstra(int startNodeId, std::unordered_map<int, std::vector<int>> &paths);
    int getNodeCount() const
    {
        return nodes.size();
    }

    int getEdgeCount() const
    {
        return edges.size();
    }

    int getMaxNodeId() const
    {
        if (nodes.empty())
            return 0;
        int maxId = 0;
        for (const auto &[id, _] : nodes)
        {
            maxId = std::max(maxId, id);
        }
        return maxId;
    }

    int getMaxEdgeId() const
    {
        if (edges.empty())
            return 0;
        int maxId = 0;
        for (const auto &[id, _] : edges)
        {
            maxId = std::max(maxId, id);
        }
        return maxId;
    }
};

#endif // GRAPHDB_HPP
