#include "Player.h"
#include <iostream>

/**
 * Constructor: Initialize player
 */
Player::Player() : x(0), y(0), startX(0), startY(0), moves(0), 
                   moveHistoryHead(nullptr), moveHistoryTail(nullptr) {
}

/**
 * Destructor: Clean up move history linked list
 */
Player::~Player() {
    clearHistory();
}

/**
 * Initialize player at start position
 */
void Player::initialize(int startX, int startY) {
    this->x = startX;
    this->y = startY;
    this->startX = startX;
    this->startY = startY;
    this->moves = 0;
    clearHistory();
    addToHistory(startX, startY);
}

/**
 * Add move to history (Linked List implementation)
 */
void Player::addToHistory(int x, int y) {
    MoveNode* newNode = new MoveNode(x, y);
    
    if (moveHistoryHead == nullptr) {
        moveHistoryHead = moveHistoryTail = newNode;
    } else {
        moveHistoryTail->next = newNode;
        moveHistoryTail = newNode;
    }
}

/**
 * Clear move history
 */
void Player::clearHistory() {
    while (moveHistoryHead != nullptr) {
        MoveNode* temp = moveHistoryHead;
        moveHistoryHead = moveHistoryHead->next;
        delete temp;
    }
    moveHistoryTail = nullptr;
}

/**
 * Move player in specified direction
 * Direction: 'W' (up), 'S' (down), 'A' (left), 'D' (right)
 * Returns true if move was successful, false if blocked
 */
bool Player::move(char direction, const MazeGenerator& maze) {
    int newX = x;
    int newY = y;
    
    // Calculate new position based on direction
    switch (direction) {
        case 'W':
        case 'w':
            newY--;
            break;
        case 'S':
        case 's':
            newY++;
            break;
        case 'A':
        case 'a':
            newX--;
            break;
        case 'D':
        case 'd':
            newX++;
            break;
        default:
            return false;
    }
    
    // Check if move is valid (collision detection)
    if (isValidMove(newX, newY, maze)) {
        x = newX;
        y = newY;
        moves++;
        addToHistory(x, y);
        return true;
    }
    
    return false;
}

/**
 * Check if move is valid (collision detection)
 */
bool Player::isValidMove(int newX, int newY, const MazeGenerator& maze) const {
    // Check boundaries
    if (newX < 0 || newX >= maze.getWidth() || newY < 0 || newY >= maze.getHeight()) {
        return false;
    }
    
    // Check if it's a valid path (not a wall)
    return maze.isValidPath(newX, newY);
}

/**
 * Get current X position
 */
int Player::getX() const {
    return x;
}

/**
 * Get current Y position
 */
int Player::getY() const {
    return y;
}

/**
 * Get number of moves
 */
int Player::getMoves() const {
    return moves;
}

/**
 * Check if player reached exit
 */
bool Player::hasReachedExit(const MazeGenerator& maze) const {
    auto mazeGrid = maze.getMaze();
    if (y >= 0 && y < maze.getHeight() && x >= 0 && x < maze.getWidth()) {
        char cell = mazeGrid[y][x];
        // Check if current position is exit (E) or if we're at the exit coordinates
        if (cell == 'E') {
            return true;
        }
        // Also check if we're at the exit position (bottom-right)
        int exitX = maze.getWidth() - 2;
        int exitY = maze.getHeight() - 2;
        return (x == exitX && y == exitY);
    }
    return false;
}

/**
 * Reset player to start position
 */
void Player::reset() {
    x = startX;
    y = startY;
    moves = 0;
    clearHistory();
    addToHistory(x, y);
}

/**
 * Get move history as a list
 */
std::list<std::pair<int, int>> Player::getMoveHistory() const {
    std::list<std::pair<int, int>> history;
    MoveNode* current = moveHistoryHead;
    
    while (current != nullptr) {
        history.push_back(std::make_pair(current->x, current->y));
        current = current->next;
    }
    
    return history;
}

