#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Player.h"
#include "MazeGenerator.h"
#include "AIPathfinder.h"

/**
 * GameManager class
 * Controls scoring, level progression, and state management
 * Uses Queue for move history and HashMap for score records
 * Member: 4 - Scoring system, Member: 5 - Game management
 */
class GameManager {
private:
    // Queue for move history tracking
    struct GameEvent {
        std::string eventType;  // "MOVE", "START", "FINISH", etc.
        int x, y;
        long long timestamp;
        
        GameEvent(const std::string& type, int x = 0, int y = 0) 
            : eventType(type), x(x), y(y) {
            timestamp = 0;  // Will be set when added
        }
    };
    
    std::queue<GameEvent> eventQueue;
    
    // HashMap for score records (player name -> score)
    std::unordered_map<std::string, int> scoreMap;
    
    // Current game state
    int currentLevel;
    int score;
    long long gameStartTime;
    long long gameEndTime;
    
    // File for saving high scores
    const std::string SCORE_FILE = "highscores.txt";
    
    // Helper functions
    void loadScores();
    void saveScores();
    void addEvent(const std::string& eventType, int x = 0, int y = 0);
    
public:
    // Constructor
    GameManager();
    
    // Start new game
    void startGame(int level = 1);
    
    // End game and calculate final score
    int endGame(int moves, long long timeTaken, int shortestPathLength);
    
    // Update score
    void updateScore(int points);
    
    // Get current score
    int getScore() const;
    
    // Get current level
    int getLevel() const;
    
    // Process events from queue
    void processEvents();
    
    // Add player score to leaderboard
    void addScore(const std::string& playerName, int score);
    
    // Get top scores (sorted)
    std::vector<std::pair<std::string, int>> getTopScores(int count = 10);
    
    // Calculate efficiency score
    // Returns percentage: (shortestPath / playerMoves) * 100
    int calculateEfficiency(int playerMoves, int shortestPathLength);
    
    // Get game time elapsed
    long long getElapsedTime() const;
};

#endif // GAME_MANAGER_H

