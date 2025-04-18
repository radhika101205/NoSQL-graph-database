#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

int main()
{
    ifstream edgesIn("edges.json");
    if (!edgesIn.is_open())
    {
        cerr << "❌ edges.json not found!" << endl;
        return 1;
    }

    json edgesData;
    try
    {
        edgesIn >> edgesData;
    }
    catch (const json::parse_error &e)
    {
        cerr << "❌ edges.json is not a valid JSON file.\n";
        return 1;
    }
    edgesIn.close();

    // Load or initialize db.json
    json dbData;
    ifstream dbIn("db.json");
    if (dbIn.is_open())
    {
        try
        {
            dbIn >> dbData;
        }
        catch (...)
        {
            cerr << "⚠️ db.json is corrupted. Starting fresh.\n";
            dbData = json::array();
        }
        dbIn.close();
    }
    else
    {
        dbData = json::array();
    }

    // Append new edges
    for (const auto &edge : edgesData)
    {
        dbData.push_back(edge);
    }

    // Write back to db.json
    ofstream dbOut("db.json");
    dbOut << dbData.dump(2);
    dbOut.close();

    cout << "✅ Appended " << edgesData.size() << " entries to db.json.\n";
    return 0;
}
