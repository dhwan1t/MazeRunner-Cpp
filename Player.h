#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <list>
#include "MazeGenerator.h"

/**
 * Player class
 * Handles player movement, collision detection, and position tracking
 * Uses Linked List for move history
 * Member: 2 - Player control
 */
class Player {
private:
    int x;              // Current X position
    int y;              // Current Y position
    int startX;         // Starting X position
    int startY;         // Starting Y position
    int moves;          // Number of moves made
    
    // Linked List for move history
    struct MoveNode {
        int x, y;
        MoveNode* next;
        MoveNode(int x, int y) : x(x), y(y), next(nullptr) {}
    };
    
    MoveNode* moveHistoryHead;  // Head of move history linked list
    MoveNode* moveHistoryTail;  // Tail of move history linked list
    
    // Helper function to add move to history
    void addToHistory(int x, int y);
    
    // Clear move history
    void clearHistory();
    
public:
    // Constructor
    Player();
    
    // Destructor
    ~Player();
    
    // Initialize player at start position
    void initialize(int startX, int startY);
    
    // Move player in specified direction
    // Returns true if move was successful, false if blocked
    bool move(char direction, const MazeGenerator& maze);
    
    // Get current position
    int getX() const;
    int getY() const;
    
    // Get number of moves
    int getMoves() const;
    
    // Check if player reached exit
    bool hasReachedExit(const MazeGenerator& maze) const;
    
    // Reset player to start position
    void reset();
    
    // Get move history (for analysis)
    std::list<std::pair<int, int>> getMoveHistory() const;
    
    // Check if move is valid (collision detection)
    bool isValidMove(int newX, int newY, const MazeGenerator& maze) const;
};

#endif // PLAYER_H

