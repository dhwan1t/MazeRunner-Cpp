#ifndef AI_PATHFINDER_H
#define AI_PATHFINDER_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include "MazeGenerator.h"

/**
 * AIPathfinder class
 * Implements BFS and Dijkstra's algorithm for finding shortest path
 * Uses Queue for BFS and Priority Queue (Min Heap) for Dijkstra
 * Member: 3 - AI bot pathfinding
 */
class AIPathfinder {
private:
    // Structure for pathfinding nodes
    struct Node {
        int x, y;
        int cost;          // Cost to reach this node (for Dijkstra)
        int distance;      // Distance from start (for BFS)
        
        Node(int x, int y, int cost = 0, int distance = 0) 
            : x(x), y(y), cost(cost), distance(distance) {}
    };
    
    // Comparator for priority queue (Min Heap) in Dijkstra
    struct NodeComparator {
        bool operator()(const Node& a, const Node& b) {
            return a.cost > b.cost;  // Min heap
        }
    };
    
    // Helper function to get neighbors
    std::vector<Node> getNeighbors(int x, int y, const MazeGenerator& maze);
    
    // Helper function to reconstruct path from visited nodes
    std::vector<std::pair<int, int>> reconstructPath(
        const std::unordered_map<std::string, std::pair<int, int>>& cameFrom,
        int endX, int endY);
    
    // Helper to create key for hash map
    std::string createKey(int x, int y);
    
public:
    // Find shortest path using BFS (Breadth First Search)
    // Returns vector of coordinates representing the path
    std::vector<std::pair<int, int>> findPathBFS(
        int startX, int startY, 
        int endX, int endY, 
        const MazeGenerator& maze);
    
    // Find shortest path using Dijkstra's algorithm
    // Returns vector of coordinates representing the path
    std::vector<std::pair<int, int>> findPathDijkstra(
        int startX, int startY, 
        int endX, int endY, 
        const MazeGenerator& maze);
    
    // Get path length
    int getPathLength(const std::vector<std::pair<int, int>>& path);
};

#endif // AI_PATHFINDER_H

