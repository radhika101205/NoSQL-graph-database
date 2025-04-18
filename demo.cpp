#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <sstream>

class LiteGraphNoSQL {
private:
    struct Node {
        std::string key;
        std::string value;
        std::vector<std::string> edges;
        Node(const std::string& k, const std::string& v) : key(k), value(v) {}
    };

    // Map of user graphs (userID -> graph)
    std::unordered_map<std::string, std::unordered_map<std::string, Node>> userGraphs;
    // Map of user value indexes (userID -> valueIndex)
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> userValueIndexes;
    // Map of user credentials (username -> hashed password)
    std::unordered_map<std::string, std::string> users;
    std::string currentUser; // Track logged-in user
    const std::string logFile = "litegraphnosql_log.txt";

    // Simple hash function for passwords (for demo purposes)
    std::string hashPassword(const std::string& password) {
        return std::to_string(std::hash<std::string>{}(password));
    }

    void logOperation(const std::string& operation, const std::string& user, const std::string& key, const std::string& value = "") {
        std::ofstream log(logFile, std::ios::app);
        if (log.is_open()) {
            log << user << " " << operation << " " << key << " " << value << "\n";
            log.close();
        }
    }

    void updateValueIndex(const std::string& user, const std::string& key, const std::string& oldValue, const std::string& newValue) {
        if (!oldValue.empty() && oldValue != newValue) {
            std::vector<std::string>& keys = userValueIndexes[user][oldValue];
            keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
            if (keys.empty()) userValueIndexes[user].erase(oldValue);
        }
        if (!newValue.empty()) {
            userValueIndexes[user][newValue].push_back(key);
        }
    }

    void loadFromLog() {
        std::ifstream log(logFile);
        std::string user, operation, key, value;
        if (log.is_open()) {
            while (log >> user >> operation >> key) {
                std::getline(log, value);
                // Remove leading space from getline
                value = value.empty() ? "" : value.substr(value.find_first_not_of(" "));
                std::cout << "Parsing: user=" << user << ", op=" << operation << ", key=" << key << ", value='" << value << "'\n"; // Debug output
                if (operation == "INSERT" || operation == "UPDATE") {
                    if (userGraphs.find(user) == userGraphs.end()) {
                        userGraphs[user] = std::unordered_map<std::string, Node>();
                        userValueIndexes[user] = std::unordered_map<std::string, std::vector<std::string>>();
                    }
                    if (!value.empty() && value != " ") { // Ensure value is valid
                        if (operation == "INSERT") {
                            userGraphs[user].emplace(key, Node(key, value));
                            userValueIndexes[user][value].push_back(key);
                        } else {
                            auto it = userGraphs[user].find(key);
                            if (it != userGraphs[user].end()) {
                                std::string oldValue = it->second.value;
                                it->second.value = value;
                                updateValueIndex(user, key, oldValue, value);
                            }
                        }
                    } else {
                        std::cout << "Skipping invalid value for " << operation << " operation\n";
                    }
                } else if (operation == "DELETE") {
                    auto it = userGraphs[user].find(key);
                    if (it != userGraphs[user].end()) {
                        updateValueIndex(user, key, it->second.value, "");
                        userGraphs[user].erase(key);
                    }
                } else if (operation == "EDGE") {
                    std::string target;
                    std::istringstream iss(value);
                    if (iss >> target) { // Check if target can be extracted
                        auto fromIt = userGraphs[user].find(key);
                        auto toIt = userGraphs[user].find(target);
                        if (fromIt != userGraphs[user].end() && toIt != userGraphs[user].end()) {
                            fromIt->second.edges.push_back(target);
                        }
                    } else {
                        std::cout << "Skipping invalid EDGE operation, no target found\n";
                    }
                }
            }
            log.close();
        } else {
            std::cout << "Log file not found or cannot be opened\n";
        }
    }

public:
    LiteGraphNoSQL() {
        loadFromLog();
    }

    // Register a new user with a password
    bool registerUser(const std::string& username, const std::string& password) {
        if (users.find(username) != users.end()) {
            std::cout << "Username already exists\n";
            return false;
        }
        users[username] = hashPassword(password);
        userGraphs[username] = std::unordered_map<std::string, Node>();
        userValueIndexes[username] = std::unordered_map<std::string, std::vector<std::string>>();
        std::cout << "User " << username << " registered successfully\n";
        return true;
    }

    // Log in a user
    bool login(const std::string& username, const std::string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second == hashPassword(password)) {
            currentUser = username;
            std::cout << "Logged in as " << username << "\n";
            return true;
        }
        std::cout << "Invalid username or password\n";
        return false;
    }

    // Log out
    void logout() {
        if (!currentUser.empty()) {
            std::cout << "Logged out from " << currentUser << "\n";
            currentUser.clear();
        }
    }

    void insert(const std::string& key, const std::string& value) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        if (userGraphs[currentUser].find(key) == userGraphs[currentUser].end()) {
            userGraphs[currentUser].emplace(key, Node(key, value));
            userValueIndexes[currentUser][value].push_back(key);
            logOperation("INSERT", currentUser, key, value);
            std::cout << "Node inserted: " << key << " -> " << value << "\n";
        } else {
            std::cout << "Key already exists: " << key << "\n";
        }
    }

    void update(const std::string& key, const std::string& value) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        auto it = userGraphs[currentUser].find(key);
        if (it != userGraphs[currentUser].end()) {
            std::string oldValue = it->second.value;
            it->second.value = value;
            updateValueIndex(currentUser, key, oldValue, value);
            logOperation("UPDATE", currentUser, key, value);
            std::cout << "Node updated: " << key << " -> " << value << "\n";
        } else {
            std::cout << "Node not found: " << key << "\n";
        }
    }

    void deleteNode(const std::string& key) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        auto it = userGraphs[currentUser].find(key);
        if (it != userGraphs[currentUser].end()) {
            std::string value = it->second.value;
            for (auto iter = userGraphs[currentUser].begin(); iter != userGraphs[currentUser].end(); ++iter) {
                std::vector<std::string>& edges = iter->second.edges;
                edges.erase(std::remove(edges.begin(), edges.end(), key), edges.end());
            }
            updateValueIndex(currentUser, key, value, "");
            userGraphs[currentUser].erase(key);
            logOperation("DELETE", currentUser, key);
            std::cout << "Node deleted: " << key << "\n";
        } else {
            std::cout << "Node not found: " << key << "\n";
        }
    }

    std::string get(const std::string& key) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return "Not logged in";
        }
        auto it = userGraphs[currentUser].find(key);
        if (it != userGraphs[currentUser].end()) {
            return it->second.value;
        }
        return "Node not found";
    }

    void addEdge(const std::string& fromKey, const std::string& toKey) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        auto fromIt = userGraphs[currentUser].find(fromKey);
        auto toIt = userGraphs[currentUser].find(toKey);
        if (fromIt != userGraphs[currentUser].end() && toIt != userGraphs[currentUser].end()) {
            fromIt->second.edges.push_back(toKey);
            logOperation("EDGE", currentUser, fromKey, toKey);
            std::cout << "Edge added: " << fromKey << " -> " << toKey << "\n";
        } else {
            std::cout << "One or both nodes not found\n";
        }
    }

    void traverse() {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        if (userGraphs[currentUser].empty()) {
            std::cout << "Graph is empty for " << currentUser << "\n";
            return;
        }
        for (auto it = userGraphs[currentUser].begin(); it != userGraphs[currentUser].end(); ++it) {
            std::cout << "Node " << it->second.key << ": " << it->second.value;
            if (!it->second.edges.empty()) {
                std::cout << " | Edges: ";
                for (size_t i = 0; i < it->second.edges.size(); ++i) {
                    std::cout << it->second.edges[i] << " ";
                }
            }
            std::cout << "\n";
        }
    }

    void neighbors(const std::string& key) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return;
        }
        auto it = userGraphs[currentUser].find(key);
        if (it != userGraphs[currentUser].end()) {
            const std::vector<std::string>& edges = it->second.edges;
            if (edges.empty()) {
                std::cout << "No neighbors for " << key << "\n";
            } else {
                std::cout << "Neighbors of " << key << ": ";
                for (size_t i = 0; i < edges.size(); ++i) {
                    auto neighborIt = userGraphs[currentUser].find(edges[i]);
                    if (neighborIt != userGraphs[currentUser].end()) {
                        std::cout << edges[i] << " (" << neighborIt->second.value << ") ";
                    }
                }
                std::cout << "\n";
            }
        } else {
            std::cout << "Node not found: " << key << "\n";
        }
    }

    std::vector<std::string> shortestPath(const std::string& start, const std::string& end) {
        if (currentUser.empty()) {
            std::cout << "Please log in first\n";
            return {};
        }
        if (userGraphs[currentUser].find(start) == userGraphs[currentUser].end() || 
            userGraphs[currentUser].find(end) == userGraphs[currentUser].end()) {
            std::cout << "Start or end node not found\n";
            return {};
        }

        std::unordered_map<std::string, std::string> parent;
        std::queue<std::string> q;
        std::unordered_set<std::string> visited;

        q.push(start);
        visited.insert(start);
        parent[start] = "";

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();

            if (current == end) break;

            auto it = userGraphs[currentUser].find(current);
            if (it != userGraphs[currentUser].end()) {
                for (const auto& neighbor : it->second.edges) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        parent[neighbor] = current;
                        q.push(neighbor);
                    }
                }
            }
        }

        std::vector<std::string> path;
        for (std::string at = end; !at.empty(); at = parent[at]) {
            path.push_back(at);
            if (at == start) break;
        }
        if (path.empty() || path.back() != start) {
            std::cout << "No path found between " << start << " and " << end << "\n";
            return {};
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// CLI to test the graph database with user authentication
int main() {
    LiteGraphNoSQL db;
    std::string command, username, password, key, value, target, start, end;

    std::cout << "LiteGraphNoSQL - Commands: REGISTER, LOGIN, LOGOUT, INSERT, UPDATE, DELETE, GET, EDGE, NEIGHBORS, TRAVERSE, SHORTEST, EXIT\n";

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command == "EXIT") break;

        if (command == "REGISTER") {
            std::cin >> username >> password;
            db.registerUser(username, password);
        }
        else if (command == "LOGIN") {
            std::cin >> username >> password;
            db.login(username, password);
        }
        else if (command == "LOGOUT") {
            db.logout();
        }
        else if (command == "INSERT" || command == "UPDATE") {
            std::cin >> key;
            std::getline(std::cin, value);
            value = value.substr(1); // Adjusted to handle empty input
            if (command == "INSERT") db.insert(key, value);
            else db.update(key, value);
        }
        else if (command == "DELETE" || command == "GET" || command == "NEIGHBORS") {
            std::cin >> key;
            if (command == "DELETE") db.deleteNode(key);
            else if (command == "GET") std::cout << db.get(key) << "\n";
            else db.neighbors(key);
        }
        else if (command == "EDGE") {
            std::cin >> key >> target;
            db.addEdge(key, target);
        }
        else if (command == "TRAVERSE") {
            db.traverse();
        }
        else if (command == "SHORTEST") {
            std::cin >> start >> end;
            std::vector<std::string> path = db.shortestPath(start, end);
            if (!path.empty()) {
                std::cout << "Shortest path from " << start << " to " << end << ": ";
                for (const auto& node : path) {
                    std::cout << node << " ";
                }
                std::cout << "\n";
            }
        }
        else {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    return 0;
}