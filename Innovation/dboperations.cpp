#include "graphdb.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

// Simple tokenizer for natural language triplets
vector<string> tokenize(const string &input)
{
    stringstream ss(input);
    string word;
    vector<string> tokens;
    while (ss >> word)
        tokens.push_back(word);
    return tokens;
}

void loadFromJson(GraphDB &db, const string &path)
{
    ifstream in(path);
    if (!in.is_open())
    {
        cerr << "Failed to open db.json\n";
        return;
    }
    json j;
    in >> j;
    unordered_map<string, int> nameToId;
    int nodeId = 1, edgeId = 1;

    for (const auto &item : j)
    {
        string source = item["source"], target = item["target"], relation = item["relation"];
        if (!nameToId.count(source))
        {
            db.addNode(nodeId);
            db.setNodeProperty(nodeId, "name", source);
            nameToId[source] = nodeId++;
        }
        if (!nameToId.count(target))
        {
            db.addNode(nodeId);
            db.setNodeProperty(nodeId, "name", target);
            nameToId[target] = nodeId++;
        }
        db.addEdge(edgeId, nameToId[source], nameToId[target], 1.0);
        db.setEdgeProperty(edgeId, "relation", relation);
        edgeId++;
    }
}

void printNode(const shared_ptr<Node> &node)
{
    cout << "Node " << node->getId() << ": ";
    for (const auto &p : node->getProperties())
    {
        cout << p.first << "=" << p.second.toString() << " ";
    }
    cout << "\n";
}

int findNodeIdByName(GraphDB &db, const string &name)
{
    auto ids = db.findNodesByProperty("name", name);
    return ids.empty() ? -1 : ids[0];
}

int main()
{
    GraphDB db;
    loadFromJson(db, "db.json");
    string cmd;

    while (true)
    {
        cout << "\nCommand (search/delete/add/exit): ";
        getline(cin, cmd);

        if (cmd == "exit")
            break;
        else if (cmd == "search")
        {
            cout << "Enter name to search: ";
            string name;
            getline(cin, name);
            int id = findNodeIdByName(db, name);
            if (id == -1)
                cout << "Not found.\n";
            else
                printNode(db.getNode(id));
        }
        else if (cmd == "delete")
        {
            cout << "Enter name to delete: ";
            string name;
            getline(cin, name);
            int id = findNodeIdByName(db, name);
            if (id == -1)
                cout << "Node not found.\n";
            else
            {
                db.deleteNode(id);
                cout << "Deleted node and its edges.\n";
            }
        }
        else if (cmd == "add")
        {
            cout << "Enter natural language like 'Elon Musk founded SpaceX': ";
            string line;
            getline(cin, line);
            auto words = tokenize(line);
            if (words.size() < 3)
            {
                cout << "Invalid format. Must be 'source relation target'.\n";
                continue;
            }

            string source = words[0];
            for (int i = 1; i < words.size() - 2; ++i)
                source += " " + words[i];
            string relation = words[words.size() - 2];
            string target = words[words.size() - 1];

            int sId = findNodeIdByName(db, source);
            int tId = findNodeIdByName(db, target);

            int nid = db.getMaxNodeId() + 1;
            int eid = db.getMaxEdgeId() + 1;

            if (sId == -1)
            {
                db.addNode(nid);
                db.setNodeProperty(nid, "name", source);
                sId = nid++;
            }
            if (tId == -1)
            {
                db.addNode(nid);
                db.setNodeProperty(nid, "name", target);
                tId = nid++;
            }

            db.addEdge(eid, sId, tId, 1.0);
            db.setEdgeProperty(eid, "relation", relation);

            cout << "✅ Added edge: " << source << " --[" << relation << "]--> " << target << "\n";
        }
        else
            cout << "Unknown command. Try again.\n";
    }

    return 0;
}