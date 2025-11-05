#!/bin/bash
# Compilation script for ASCII Maze Runner Game

echo "Compiling ASCII Maze Runner Game..."
echo ""

g++ -std=c++17 main.cpp MazeGenerator.cpp Player.cpp AIPathfinder.cpp GameManager.cpp Utilities.cpp -o MazeRunner

if [ $? -eq 0 ]; then
    echo ""
    echo "Compilation successful! Run ./MazeRunner to play."
else
    echo ""
    echo "Compilation failed. Please check for errors."
    exit 1
fi

