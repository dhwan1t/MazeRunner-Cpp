# ASCII Maze Runner Game

A console-based maze game built with C++ for Advanced Data Structures and Algorithms (ADSA) project.

## 🎯 Project Overview

This project simulates a maze-based game where:
- The maze is generated algorithmically using **DFS (Depth First Search)**
- The player navigates from start to exit point
- An AI bot finds the shortest path using **BFS** or **Dijkstra's algorithm**
- Everything is displayed in ASCII format in the console

## 🧩 Technical Specifications

- **Language:** C++17
- **Interface:** ASCII-based console
- **Build:** g++ or any standard C++ compiler
- **Platform:** Cross-platform (Windows/Linux/Mac)

## 📁 Project Structure

```
MazeRunner/
├── main.cpp              # Game loop, main menu, and control flow
├── MazeGenerator.cpp/.h  # Maze generation using DFS (Graph + Stack)
├── Player.cpp/.h         # Player movement and collision detection
├── AIPathfinder.cpp/.h   # BFS/Dijkstra pathfinding algorithms
├── GameManager.cpp/.h    # Scoring, level progression, state management
├── Utilities.cpp/.h      # Helper functions, color codes, screen clearing
└── README.md             # This file
```

## 🧱 Data Structures Implemented

1. **Graph (Adjacency List)** - Maze layout and connections
2. **Stack** - DFS for maze generation (backtracking)
3. **Queue** - BFS for pathfinding and move tracking
4. **Priority Queue (Min Heap)** - Dijkstra's path cost calculation
5. **HashMap (unordered_map)** - Scoreboard and visited nodes tracking
6. **Linked List** - Move history tracking

## 🧮 Algorithms Implemented

- **DFS (Depth First Search)** - Maze generation
- **BFS (Breadth First Search)** - Shortest pathfinding
- **Dijkstra's Algorithm** - Optimal pathfinding with cost
- **Sorting Algorithm** - Leaderboard sorting
- **Graph Traversal** - Path exploration
- **File I/O** - Saving and loading high scores

## 🚀 How to Compile and Run

### Compilation

```bash
g++ main.cpp MazeGenerator.cpp Player.cpp AIPathfinder.cpp GameManager.cpp Utilities.cpp -o MazeRunner
```

### Running

**Linux/Mac:**
```bash
./MazeRunner
```

**Windows:**
```bash
MazeRunner.exe
```

## 🎮 How to Play

1. **Main Menu Options:**
   - Play Game - Navigate the maze yourself
   - Watch AI Solve - Watch AI solve the maze automatically
   - View High Scores - See top scores
   - Instructions - View game controls
   - Exit - Quit the game

2. **Controls:**
   - `W` - Move Up
   - `S` - Move Down
   - `A` - Move Left
   - `D` - Move Right
   - `Q` - Quit Game

3. **Symbols:**
   - `#` - Wall
   - ` ` (space) - Path
   - `P` - Player
   - `S` - Start position
   - `E` - Exit position
   - `*` - AI Bot (when watching AI solve)

4. **Objective:**
   - Navigate from `S` (Start) to `E` (Exit)
   - Try to complete in as few moves as possible
   - Your efficiency is calculated based on optimal path length

## 👥 Team Division

| Member | Responsibility | Data Structure | Algorithm |
|--------|----------------|----------------|-----------|
| 1 | Maze generation | Graph + Stack | DFS |
| 2 | Player control | Linked List | Movement logic |
| 3 | AI bot pathfinding | Queue / Min Heap | BFS / Dijkstra |
| 4 | Scoring system | HashMap | Sorting leaderboard |
| 5 | Game management | Queue | Event handling |
| 6 | Utility + Display | None | Rendering + I/O |

## 📊 Scoring System

The score is calculated based on:
- **Efficiency**: How close your path is to the optimal path
- **Time Bonus**: Faster completion gives more points
- **Move Bonus**: Fewer moves give more points

Final Score = (Efficiency × 10) + Time Bonus + Move Bonus

## 🎨 Features

- ✅ Procedural maze generation using DFS
- ✅ Player movement with collision detection
- ✅ AI pathfinding with BFS and Dijkstra algorithms
- ✅ Real-time score tracking
- ✅ High score leaderboard with file persistence
- ✅ Color-coded ASCII display
- ✅ Cross-platform compatibility
- ✅ Animated AI solving mode

## 🔧 Code Organization

- **Object-Oriented Design**: Each module is a separate class
- **Modular Structure**: Easy for team members to work on different modules
- **Well Documented**: Comments explain data structures and algorithms
- **Standard C++ Libraries**: Only uses standard library (no external dependencies)

## 📝 Notes

- Maze dimensions must be odd numbers for proper structure
- High scores are saved to `highscores.txt` file
- The game supports different maze sizes (15x15, 21x21, 25x25)
- AI can use either BFS or Dijkstra algorithm for pathfinding

## 🐛 Troubleshooting

If you encounter issues:

1. **Compilation errors**: Ensure you're using C++17 or later
2. **Input issues**: On Unix/Linux, terminal input may need configuration
3. **Color display**: Some terminals may not support ANSI color codes

## 📄 License

This project is created for educational purposes as part of an ADSA course project.

---

**Enjoy playing the Maze Runner Game!** 🎮
