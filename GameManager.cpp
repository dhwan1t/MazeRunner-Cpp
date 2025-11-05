#include "GameManager.h"
#include "Utilities.h"
#include <sstream>
#include <iomanip>

/**
 * Constructor: Initialize game manager
 */
GameManager::GameManager() : currentLevel(1), score(0), gameStartTime(0), gameEndTime(0) {
    loadScores();
}

/**
 * Start new game
 */
void GameManager::startGame(int level) {
    currentLevel = level;
    score = 0;
    gameStartTime = Utilities::getCurrentTime();
    gameEndTime = 0;
    
    // Clear event queue
    while (!eventQueue.empty()) {
        eventQueue.pop();
    }
    
    addEvent("START", 0, 0);
}

/**
 * End game and calculate final score
 */
int GameManager::endGame(int moves, long long timeTaken, int shortestPathLength) {
    gameEndTime = Utilities::getCurrentTime();
    
    // Calculate score based on moves, time, and efficiency
    int efficiency = calculateEfficiency(moves, shortestPathLength);
    int timeBonus = std::max(0, 1000 - (int)timeTaken);  // Bonus for faster completion
    int moveBonus = std::max(0, 500 - moves);  // Bonus for fewer moves
    
    score = efficiency * 10 + timeBonus + moveBonus;
    
    addEvent("FINISH", 0, 0);
    processEvents();
    
    return score;
}

/**
 * Update score
 */
void GameManager::updateScore(int points) {
    score += points;
}

/**
 * Get current score
 */
int GameManager::getScore() const {
    return score;
}

/**
 * Get current level
 */
int GameManager::getLevel() const {
    return currentLevel;
}

/**
 * Add event to queue
 */
void GameManager::addEvent(const std::string& eventType, int x, int y) {
    GameEvent event(eventType, x, y);
    event.timestamp = Utilities::getCurrentTime();
    eventQueue.push(event);
}

/**
 * Process events from queue
 */
void GameManager::processEvents() {
    // Process events (can be used for logging or replay)
    while (!eventQueue.empty()) {
        GameEvent event = eventQueue.front();
        eventQueue.pop();
        // Events can be logged or processed here
    }
}

/**
 * Add player score to leaderboard
 */
void GameManager::addScore(const std::string& playerName, int score) {
    // Update score if player exists and new score is higher
    if (scoreMap.find(playerName) != scoreMap.end()) {
        if (score > scoreMap[playerName]) {
            scoreMap[playerName] = score;
        }
    } else {
        scoreMap[playerName] = score;
    }
    
    saveScores();
}

/**
 * Get top scores (sorted)
 * Uses sorting algorithm to return top N scores
 */
std::vector<std::pair<std::string, int>> GameManager::getTopScores(int count) {
    std::vector<std::pair<std::string, int>> scores;
    
    // Copy from HashMap to vector
    for (const auto& pair : scoreMap) {
        scores.push_back(pair);
    }
    
    // Sort by score (descending order)
    std::sort(scores.begin(), scores.end(), 
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });
    
    // Return top N scores
    if (scores.size() > count) {
        scores.resize(count);
    }
    
    return scores;
}

/**
 * Calculate efficiency score
 * Returns percentage: (shortestPath / playerMoves) * 100
 */
int GameManager::calculateEfficiency(int playerMoves, int shortestPathLength) {
    if (playerMoves == 0 || shortestPathLength == 0) {
        return 0;
    }
    
    // Efficiency as percentage: how close to optimal
    int efficiency = (shortestPathLength * 100) / playerMoves;
    return std::min(100, efficiency);  // Cap at 100%
}

/**
 * Get game time elapsed
 */
long long GameManager::getElapsedTime() const {
    if (gameEndTime > 0) {
        return gameEndTime - gameStartTime;
    }
    return Utilities::getCurrentTime() - gameStartTime;
}

/**
 * Load scores from file
 */
void GameManager::loadScores() {
    std::ifstream file(SCORE_FILE);
    if (!file.is_open()) {
        return;  // File doesn't exist yet, that's okay
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        
        if (iss >> name >> score) {
            scoreMap[name] = score;
        }
    }
    
    file.close();
}

/**
 * Save scores to file
 */
void GameManager::saveScores() {
    std::ofstream file(SCORE_FILE);
    if (!file.is_open()) {
        return;  // Couldn't open file for writing
    }
    
    for (const auto& pair : scoreMap) {
        file << pair.first << " " << pair.second << std::endl;
    }
    
    file.close();
}

