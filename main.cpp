#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include "MazeGenerator.h"
#include "Player.h"
#include "AIPathfinder.h"
#include "GameManager.h"
#include "Utilities.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// Forward declarations
void displayMenu();
void playGame();
void watchAISolve();
void viewHighScores();
void displayMaze(const MazeGenerator& maze, const Player& player, 
                 const std::vector<std::pair<int, int>>& aiPath = {}, 
                 int currentAIStep = -1);
char getInput();
void displayInstructions();

/**
 * Get character input (non-blocking)
 */
char getInput() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return 0;
#else
    char ch = 0;
    struct termios oldt, newt;
    int oldf;
    
    // Save terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    // Try to read a character
    int result = read(STDIN_FILENO, &ch, 1);
    
    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    // Return character if read, otherwise 0
    return (result > 0) ? ch : 0;
#endif
}

/**
 * Display the main menu
 */
void displayMenu() {
    Utilities::clearScreen();
    Utilities::printSeparator(60);
    Utilities::printCentered("ASCII MAZE RUNNER", 60);
    Utilities::printSeparator(60);
    std::cout << std::endl;
    
    Utilities::setColor("cyan");
    std::cout << "  1. Play Game" << std::endl;
    std::cout << "  2. Watch AI Solve" << std::endl;
    std::cout << "  3. View High Scores" << std::endl;
    std::cout << "  4. Instructions" << std::endl;
    std::cout << "  5. Exit" << std::endl;
    Utilities::resetColor();
    
    std::cout << std::endl;
    Utilities::printSeparator(60);
    std::cout << "Enter your choice (1-5): ";
}

/**
 * Display instructions
 */
void displayInstructions() {
    Utilities::clearScreen();
    Utilities::printSeparator(60);
    Utilities::printCentered("INSTRUCTIONS", 60);
    Utilities::printSeparator(60);
    std::cout << std::endl;
    
    std::cout << "  Controls:" << std::endl;
    std::cout << "    W - Move Up" << std::endl;
    std::cout << "    S - Move Down" << std::endl;
    std::cout << "    A - Move Left" << std::endl;
    std::cout << "    D - Move Right" << std::endl;
    std::cout << "    Q - Quit Game" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  Symbols:" << std::endl;
    std::cout << "    # - Wall" << std::endl;
    std::cout << "    (space) - Path" << std::endl;
    std::cout << "    P - Player" << std::endl;
    std::cout << "    S - Start" << std::endl;
    std::cout << "    E - Exit" << std::endl;
    std::cout << "    * - AI Bot (when watching AI solve)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  Objective:" << std::endl;
    std::cout << "    Navigate from S (Start) to E (Exit)" << std::endl;
    std::cout << "    Try to complete in as few moves as possible!" << std::endl;
    std::cout << std::endl;
    
    Utilities::printSeparator(60);
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

/**
 * Display maze with player and optional AI path
 */
void displayMaze(const MazeGenerator& maze, const Player& player, 
                 const std::vector<std::pair<int, int>>& aiPath, 
                 int currentAIStep) {
    auto mazeGrid = maze.getMaze();
    int width = maze.getWidth();
    int height = maze.getHeight();
    
    // Create a copy for display
    std::vector<std::vector<char>> display = mazeGrid;
    
    // Draw AI path if provided
    if (!aiPath.empty() && currentAIStep >= 0) {
        for (int i = 0; i <= currentAIStep && i < (int)aiPath.size(); i++) {
            int x = aiPath[i].first;
            int y = aiPath[i].second;
            if (display[y][x] != 'S' && display[y][x] != 'E') {
                display[y][x] = '*';
            }
        }
    }
    
    // Draw player
    int px = player.getX();
    int py = player.getY();
    if (px >= 0 && px < width && py >= 0 && py < height) {
        if (display[py][px] != 'E') {
            display[py][px] = 'P';
        }
    }
    
    // Print maze
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char cell = display[i][j];
            
            // Color coding
            if (cell == 'P') {
                Utilities::setColor("green");
                std::cout << cell;
                Utilities::resetColor();
            } else if (cell == 'E') {
                Utilities::setColor("red");
                std::cout << cell;
                Utilities::resetColor();
            } else if (cell == 'S') {
                Utilities::setColor("blue");
                std::cout << cell;
                Utilities::resetColor();
            } else if (cell == '*') {
                Utilities::setColor("yellow");
                std::cout << cell;
                Utilities::resetColor();
            } else {
                std::cout << cell;
            }
        }
        std::cout << std::endl;
    }
}

/**
 * Main game loop - Player plays
 */
void playGame() {
    Utilities::clearScreen();
    
    // Ask for maze size
    int size = 21;
    std::cout << "Enter maze size (15, 21, or 25): ";
    std::cin >> size;
    if (size < 15) size = 15;
    if (size > 25) size = 25;
    
    // Generate maze
    MazeGenerator maze(size, size);
    maze.generateMaze();
    
    // Find start and exit positions
    int startX = 1, startY = 1;
    int exitX = maze.getWidth() - 2;
    int exitY = maze.getHeight() - 2;
    
    // Initialize player
    Player player;
    player.initialize(startX, startY);
    
    // Initialize game manager
    GameManager gameManager;
    gameManager.startGame(1);
    
    // Find shortest path for efficiency calculation
    AIPathfinder pathfinder;
    auto shortestPath = pathfinder.findPathBFS(startX, startY, exitX, exitY, maze);
    int shortestPathLength = pathfinder.getPathLength(shortestPath);
    
    // Game loop
    bool gameRunning = true;
    bool gameWon = false;
    
    std::cout << "Game starting! Use W/A/S/D to move, Q to quit." << std::endl;
    std::cout << "Press Enter to begin...";
    std::cin.ignore();
    std::cin.get();
    
    while (gameRunning && !gameWon) {
        Utilities::clearScreen();
        
        // Display maze
        displayMaze(maze, player);
        
        // Display stats
        std::cout << std::endl;
        Utilities::setColor("cyan");
        std::cout << "Moves: " << player.getMoves() 
                  << " | Time: " << gameManager.getElapsedTime() << "s"
                  << " | Shortest Path: " << shortestPathLength << " moves" << std::endl;
        Utilities::resetColor();
        
        std::cout << "Controls: W/A/S/D to move, Q to quit" << std::endl;
        std::cout << "Enter move: ";
        
        // Get input (simpler blocking input)
        std::string inputStr;
        std::cin >> inputStr;
        
        if (inputStr.empty()) continue;
        
        char input = inputStr[0];
        
        if (input == 'q' || input == 'Q') {
            gameRunning = false;
        } else if (input == 'w' || input == 'W' || 
                   input == 'a' || input == 'A' || 
                   input == 's' || input == 'S' || 
                   input == 'd' || input == 'D') {
            player.move(input, maze);
            
            // Check if player reached exit
            if (player.hasReachedExit(maze)) {
                gameWon = true;
            }
        }
    }
    
    if (gameWon) {
        Utilities::clearScreen();
        displayMaze(maze, player);
        
        int finalScore = gameManager.endGame(player.getMoves(), 
                                            gameManager.getElapsedTime(), 
                                            shortestPathLength);
        
        std::cout << std::endl;
        Utilities::setColor("green");
        std::cout << "Congratulations! You reached the exit!" << std::endl;
        Utilities::resetColor();
        
        std::cout << "Moves: " << player.getMoves() << std::endl;
        std::cout << "Time: " << gameManager.getElapsedTime() << " seconds" << std::endl;
        std::cout << "Shortest Path: " << shortestPathLength << " moves" << std::endl;
        std::cout << "Efficiency: " << gameManager.calculateEfficiency(player.getMoves(), shortestPathLength) << "%" << std::endl;
        std::cout << "Score: " << finalScore << std::endl;
        
        // Ask for player name
        std::string playerName;
        std::cout << std::endl << "Enter your name for the leaderboard: ";
        std::cin.ignore();
        std::getline(std::cin, playerName);
        
        if (!playerName.empty()) {
            gameManager.addScore(playerName, finalScore);
        }
        
        std::cout << std::endl << "Press Enter to continue...";
        std::cin.get();
    }
}

/**
 * Watch AI solve the maze
 */
void watchAISolve() {
    Utilities::clearScreen();
    
    // Ask for maze size
    int size = 21;
    std::cout << "Enter maze size (15, 21, or 25): ";
    std::cin >> size;
    if (size < 15) size = 15;
    if (size > 25) size = 25;
    
    // Ask for algorithm
    std::cout << "Choose algorithm (1=BFS, 2=Dijkstra): ";
    int algoChoice;
    std::cin >> algoChoice;
    
    // Generate maze
    MazeGenerator maze(size, size);
    maze.generateMaze();
    
    // Find start and exit positions
    int startX = 1, startY = 1;
    int exitX = maze.getWidth() - 2;
    int exitY = maze.getHeight() - 2;
    
    // Find path using chosen algorithm
    AIPathfinder pathfinder;
    std::vector<std::pair<int, int>> path;
    
    if (algoChoice == 2) {
        path = pathfinder.findPathDijkstra(startX, startY, exitX, exitY, maze);
    } else {
        path = pathfinder.findPathBFS(startX, startY, exitX, exitY, maze);
    }
    
    if (path.empty()) {
        std::cout << "No path found!" << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        return;
    }
    
    // Animate AI solving
    std::cout << "AI Solving Maze..." << std::endl;
    std::cout << "Path length: " << pathfinder.getPathLength(path) << " moves" << std::endl;
    std::cout << "Press Enter to start animation...";
    std::cin.ignore();
    std::cin.get();
    
    // Create a dummy player for display
    Player dummyPlayer;
    dummyPlayer.initialize(startX, startY);
    
    // Animate path
    for (int i = 0; i < (int)path.size(); i++) {
        Utilities::clearScreen();
        
        // Update dummy player position
        dummyPlayer.initialize(path[i].first, path[i].second);
        
        // Display maze with AI path
        displayMaze(maze, dummyPlayer, path, i);
        
        std::cout << std::endl;
        Utilities::setColor("yellow");
        std::cout << "AI Step: " << (i + 1) << " / " << path.size() << std::endl;
        Utilities::resetColor();
        
        Utilities::sleep(200);  // Animation speed
    }
    
    Utilities::clearScreen();
    displayMaze(maze, dummyPlayer, path, path.size() - 1);
    std::cout << std::endl;
    Utilities::setColor("green");
    std::cout << "AI solved the maze in " << path.size() << " moves!" << std::endl;
    Utilities::resetColor();
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

/**
 * View high scores
 */
void viewHighScores() {
    Utilities::clearScreen();
    GameManager gameManager;
    auto topScores = gameManager.getTopScores(10);
    
    Utilities::printSeparator(60);
    Utilities::printCentered("HIGH SCORES", 60);
    Utilities::printSeparator(60);
    std::cout << std::endl;
    
    if (topScores.empty()) {
        std::cout << "  No scores recorded yet!" << std::endl;
    } else {
        Utilities::setColor("yellow");
        std::cout << std::setw(5) << "Rank" << std::setw(25) << "Player" 
                  << std::setw(15) << "Score" << std::endl;
        Utilities::resetColor();
        Utilities::printSeparator(60);
        
        for (size_t i = 0; i < topScores.size(); i++) {
            std::cout << std::setw(5) << (i + 1) 
                      << std::setw(25) << topScores[i].first 
                      << std::setw(15) << topScores[i].second << std::endl;
        }
    }
    
    std::cout << std::endl;
    Utilities::printSeparator(60);
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}

/**
 * Main function
 */
int main() {
    bool running = true;
    
    while (running) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                watchAISolve();
                break;
            case 3:
                viewHighScores();
                break;
            case 4:
                displayInstructions();
                break;
            case 5:
                running = false;
                std::cout << "Thanks for playing!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                Utilities::sleep(1000);
                break;
        }
    }
    
    return 0;
}

