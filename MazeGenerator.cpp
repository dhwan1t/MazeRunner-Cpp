#include "MazeGenerator.h"
#include <iostream>
#include <random>

/**
 * Constructor: Initialize maze with given dimensions
 * Ensures dimensions are odd numbers for proper maze structure
 */
MazeGenerator::MazeGenerator(int width, int height) {
    // Ensure dimensions are odd
    this->width = (width % 2 == 0) ? width + 1 : width;
    this->height = (height % 2 == 0) ? height + 1 : height;
    
    // Initialize maze with all walls
    maze.resize(this->height, std::vector<char>(this->width, '#'));
    visited.resize(this->height, std::vector<bool>(this->width, false));
}

/**
 * Check if a cell is within maze boundaries
 */
bool MazeGenerator::isValidCell(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * Get unvisited neighbors of a cell (for DFS)
 * Returns cells that are 2 steps away (for proper maze structure)
 */
std::vector<MazeGenerator::Cell> MazeGenerator::getUnvisitedNeighbors(int x, int y) {
    std::vector<Cell> neighbors;
    
    // Directions: up, down, left, right (2 steps each)
    int dx[] = {0, 0, -2, 2};
    int dy[] = {-2, 2, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValidCell(nx, ny) && !visited[ny][nx]) {
            neighbors.push_back(Cell(nx, ny));
        }
    }
    
    return neighbors;
}

/**
 * Remove wall between two adjacent cells
 */
void MazeGenerator::removeWall(int x1, int y1, int x2, int y2) {
    // Make both cells paths
    maze[y1][x1] = ' ';
    maze[y2][x2] = ' ';
    
    // Make the cell between them a path too
    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;
    maze[midY][midX] = ' ';
}

/**
 * Generate maze using Depth First Search (DFS) algorithm
 * Uses Stack data structure for backtracking
 */
void MazeGenerator::generateMaze() {
    // Clear previous maze
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze[i][j] = '#';
            visited[i][j] = false;
        }
    }
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Stack for DFS backtracking
    std::stack<Cell> cellStack;
    
    // Start from (1, 1) - ensure it's a path
    int startX = 1;
    int startY = 1;
    
    visited[startY][startX] = true;
    maze[startY][startX] = ' ';
    cellStack.push(Cell(startX, startY));
    
    // DFS algorithm
    while (!cellStack.empty()) {
        Cell current = cellStack.top();
        cellStack.pop();
        
        // Get unvisited neighbors
        std::vector<Cell> neighbors = getUnvisitedNeighbors(current.x, current.y);
        
        if (!neighbors.empty()) {
            // Push current cell back to stack
            cellStack.push(current);
            
            // Randomly choose a neighbor
            std::uniform_int_distribution<> dis(0, neighbors.size() - 1);
            Cell chosen = neighbors[dis(gen)];
            
            // Mark as visited
            visited[chosen.y][chosen.x] = true;
            
            // Remove wall between current and chosen
            removeWall(current.x, current.y, chosen.x, chosen.y);
            
            // Push chosen cell to stack
            cellStack.push(chosen);
        }
    }
    
    // Set start and exit positions
    setStartPosition(1, 1);
    setExitPosition(width - 2, height - 2);
}

/**
 * Set start position in the maze
 */
void MazeGenerator::setStartPosition(int x, int y) {
    if (isValidCell(x, y) && maze[y][x] == ' ') {
        maze[y][x] = 'S';
    }
}

/**
 * Set exit position in the maze
 */
void MazeGenerator::setExitPosition(int x, int y) {
    if (isValidCell(x, y) && maze[y][x] == ' ') {
        maze[y][x] = 'E';
    }
}

/**
 * Check if a position is a valid path (not a wall)
 */
bool MazeGenerator::isValidPath(int x, int y) const {
    if (!isValidCell(x, y)) return false;
    char cell = maze[y][x];
    return cell == ' ' || cell == 'S' || cell == 'E' || cell == 'P' || cell == '*' || cell == 'B';
}

/**
 * Get maze representation
 */
std::vector<std::vector<char>> MazeGenerator::getMaze() const {
    return maze;
}

/**
 * Get maze width
 */
int MazeGenerator::getWidth() const {
    return width;
}

/**
 * Get maze height
 */
int MazeGenerator::getHeight() const {
    return height;
}

/**
 * Get start position
 */
std::pair<int, int> MazeGenerator::getStartPosition() const {
    return std::make_pair(1, 1);
}

/**
 * Get exit position
 */
std::pair<int, int> MazeGenerator::getExitPosition() const {
    return std::make_pair(width - 2, height - 2);
}

/**
 * Print maze (for debugging)
 */
void MazeGenerator::printMaze() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
}

