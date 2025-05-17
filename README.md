# NoSQL Graph DataBase

A lightweight NoSQL graph database built from scratch in C++ as a DSA project. 

## Structure
- include/: Header files
- src/: Source/implementation files
- main.cpp: Entry point
- demo.cpp: For the purpose of demonstrating querying

---

##  Features Implemented

### 1.  Node and Edge Management
- Add and store nodes and edges dynamically
- Each node/edge can have multiple **typed properties**: `int`, `float`, or `string`

### 2.  Graph Traversals (DFS & BFS)
- Depth-First Search (DFS)
- Breadth-First Search (BFS)
- Visit all reachable nodes from a given start node

### 3.  Shortest Path (Dijkstra's Algorithm)
- Compute shortest distance from a start node to all others using **Dijkstra’s algorithm**
- Supports **weighted edges**

### 4.  Property-based Indexing
- Fast lookup using **hash map indexing** on node and edge properties
- Retrieve all nodes or edges matching a specific key-value pair

### 5.  Property System
- Custom `PropertyValue` struct to handle multiple data types
- Works even without `std::variant` (GCC 6.3 compatible)
---
