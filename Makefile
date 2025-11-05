# Makefile for ASCII Maze Runner Game
# Compiles all source files into executable

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = MazeRunner
SOURCES = main.cpp MazeGenerator.cpp Player.cpp AIPathfinder.cpp GameManager.cpp Utilities.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

