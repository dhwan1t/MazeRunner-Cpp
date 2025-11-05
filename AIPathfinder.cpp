#include "AIPathfinder.h"
#include <queue>
#include <sstream>
#include <limits>

/**
 * Create a unique key for a position (for hash map)
 */
std::string AIPathfinder::createKey(int x, int y) {
    std::ostringstream oss;
    oss << x << "," << y;
    return oss.str();
}

/**
 * Get valid neighbors of a cell
 */
std::vector<AIPathfinder::Node> AIPathfinder::getNeighbors(int x, int y, const MazeGenerator& maze) {
    std::vector<Node> neighbors;
    
    // Directions: up, down, left, right
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // Check if neighbor is valid and is a path
        if (nx >= 0 && nx < maze.getWidth() && 
            ny >= 0 && ny < maze.getHeight() && 
            maze.isValidPath(nx, ny)) {
            neighbors.push_back(Node(nx, ny));
        }
    }
    
    return neighbors;
}

/**
 * Reconstruct path from visited nodes map
 */
std::vector<std::pair<int, int>> AIPathfinder::reconstructPath(
    const std::unordered_map<std::string, std::pair<int, int>>& cameFrom,
    int endX, int endY) {
    
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> current(endX, endY);
    std::string currentKey = createKey(endX, endY);
    
    // Reconstruct path backwards
    while (cameFrom.find(currentKey) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(currentKey);
        currentKey = createKey(current.first, current.second);
    }
    
    // Add start position
    path.push_back(current);
    
    // Reverse to get path from start to end
    std::reverse(path.begin(), path.end());
    
    return path;
}

/**
 * Find shortest path using BFS (Breadth First Search)
 * Uses Queue data structure
 */
std::vector<std::pair<int, int>> AIPathfinder::findPathBFS(
    int startX, int startY, 
    int endX, int endY, 
    const MazeGenerator& maze) {
    
    // Queue for BFS traversal
    std::queue<Node> queue;
    
    // HashMap to track visited nodes and their parents
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, std::pair<int, int>> cameFrom;
    
    // Initialize
    queue.push(Node(startX, startY, 0, 0));
    visited[createKey(startX, startY)] = true;
    
    // BFS algorithm
    while (!queue.empty()) {
        Node current = queue.front();
        queue.pop();
        
        // Check if reached destination
        if (current.x == endX && current.y == endY) {
            return reconstructPath(cameFrom, endX, endY);
        }
        
        // Explore neighbors
        std::vector<Node> neighbors = getNeighbors(current.x, current.y, maze);
        
        for (const Node& neighbor : neighbors) {
            std::string neighborKey = createKey(neighbor.x, neighbor.y);
            
            if (visited.find(neighborKey) == visited.end()) {
                visited[neighborKey] = true;
                cameFrom[neighborKey] = std::make_pair(current.x, current.y);
                queue.push(Node(neighbor.x, neighbor.y, 0, current.distance + 1));
            }
        }
    }
    
    // No path found
    return std::vector<std::pair<int, int>>();
}

/**
 * Find shortest path using Dijkstra's algorithm
 * Uses Priority Queue (Min Heap) data structure
 */
std::vector<std::pair<int, int>> AIPathfinder::findPathDijkstra(
    int startX, int startY, 
    int endX, int endY, 
    const MazeGenerator& maze) {
    
    // Priority Queue (Min Heap) for Dijkstra
    std::priority_queue<Node, std::vector<Node>, NodeComparator> pq;
    
    // HashMap to track distances and parents
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::pair<int, int>> cameFrom;
    std::unordered_map<std::string, bool> visited;
    
    // Initialize distances to infinity
    for (int y = 0; y < maze.getHeight(); y++) {
        for (int x = 0; x < maze.getWidth(); x++) {
            if (maze.isValidPath(x, y)) {
                distances[createKey(x, y)] = std::numeric_limits<int>::max();
            }
        }
    }
    
    // Set start distance to 0
    std::string startKey = createKey(startX, startY);
    distances[startKey] = 0;
    pq.push(Node(startX, startY, 0));
    
    // Dijkstra's algorithm
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        std::string currentKey = createKey(current.x, current.y);
        
        // Skip if already processed
        if (visited.find(currentKey) != visited.end()) {
            continue;
        }
        
        visited[currentKey] = true;
        
        // Check if reached destination
        if (current.x == endX && current.y == endY) {
            return reconstructPath(cameFrom, endX, endY);
        }
        
        // Explore neighbors
        std::vector<Node> neighbors = getNeighbors(current.x, current.y, maze);
        
        for (const Node& neighbor : neighbors) {
            std::string neighborKey = createKey(neighbor.x, neighbor.y);
            
            if (visited.find(neighborKey) == visited.end()) {
                int newCost = distances[currentKey] + 1;  // Each step costs 1
                
                if (newCost < distances[neighborKey]) {
                    distances[neighborKey] = newCost;
                    cameFrom[neighborKey] = std::make_pair(current.x, current.y);
                    pq.push(Node(neighbor.x, neighbor.y, newCost));
                }
            }
        }
    }
    
    // No path found
    return std::vector<std::pair<int, int>>();
}

/**
 * Get path length
 */
int AIPathfinder::getPathLength(const std::vector<std::pair<int, int>>& path) {
    return path.empty() ? 0 : path.size() - 1;
}

