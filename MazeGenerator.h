#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <vector>
#include <stack>
#include <random>
#include <algorithm>
#include <utility>

/**
 * MazeGenerator class
 * Generates a maze using Depth First Search (DFS) algorithm
 * Uses Graph (Adjacency List) and Stack data structures
 * Member: 1 - Maze generation
 */
class MazeGenerator {
private:
    int width;      // Maze width (must be odd)
    int height;     // Maze height (must be odd)
    
    // Graph representation: each cell is a node, connections are edges
    // Using 2D grid where each cell can connect to neighbors
    std::vector<std::vector<bool>> visited;  // Track visited cells in DFS
    std::vector<std::vector<char>> maze;     // Final maze representation
    
    // Stack for DFS traversal
    struct Cell {
        int x, y;
        Cell(int x = 0, int y = 0) : x(x), y(y) {}
    };
    
    // Helper functions for maze generation
    bool isValidCell(int x, int y) const;
    std::vector<Cell> getUnvisitedNeighbors(int x, int y);
    void removeWall(int x1, int y1, int x2, int y2);
    
public:
    // Constructor
    MazeGenerator(int width = 21, int height = 21);
    
    // Generate maze using DFS algorithm
    void generateMaze();
    
    // Get maze representation
    std::vector<std::vector<char>> getMaze() const;
    
    // Get dimensions
    int getWidth() const;
    int getHeight() const;
    
    // Set start and exit positions
    void setStartPosition(int x, int y);
    void setExitPosition(int x, int y);
    
    // Check if a position is a valid path (not a wall)
    bool isValidPath(int x, int y) const;
    
    // Get start and exit positions
    std::pair<int, int> getStartPosition() const;
    std::pair<int, int> getExitPosition() const;
    
    // Print maze (for debugging)
    void printMaze() const;
};

#endif // MAZE_GENERATOR_H

