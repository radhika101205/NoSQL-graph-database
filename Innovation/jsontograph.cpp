// jsontograph.cpp
#include "graphdb.hpp" // Ensure graphdb.hpp is in the same directory or adjust the path accordingly
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

GraphDB loadGraphFromJson(const std::string &filename)
{
    GraphDB db;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return db;
    }

    json data;
    file >> data;
    int nodeId = 0, edgeId = 0;
    std::unordered_map<std::string, int> nodeMap; // Maps entity name to nodeId

    for (const auto &entry : data)
    {
        std::string source = entry["source"];
        std::string target = entry["target"];
        std::string relation = entry["relation"];

        // Assign or reuse node ids
        if (nodeMap.find(source) == nodeMap.end())
        {
            nodeMap[source] = nodeId;
            db.addNode(nodeId);
            db.setNodeProperty(nodeId, "name", source);
            nodeId++;
        }
        if (nodeMap.find(target) == nodeMap.end())
        {
            nodeMap[target] = nodeId;
            db.addNode(nodeId);
            db.setNodeProperty(nodeId, "name", target);
            nodeId++;
        }

        // Add edge
        db.addEdge(edgeId, nodeMap[source], nodeMap[target], 1.0);
        db.setEdgeProperty(edgeId, "relation", relation);
        edgeId++;
    }

    return db;
}

// Example entry point
int main()
{
    GraphDB db = loadGraphFromJson("db.json");
    std::cout << "✅ Graph loaded from db.json with " << db.getNodeCount() << " nodes and " << db.getEdgeCount() << " edges.\n";
    return 0;
}
