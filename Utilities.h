#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <chrono>

/**
 * Utility class for console operations, screen clearing, and formatting
 * Member: 6 - Utility + Display
 */
class Utilities {
public:
    // Clear screen function (cross-platform)
    static void clearScreen();
    
    // Set cursor position (for better rendering)
    static void setCursorPosition(int x, int y);
    
    // Color codes for terminal output
    static void setColor(const std::string& color);
    static void resetColor();
    
    // Sleep for specified milliseconds
    static void sleep(int milliseconds);
    
    // Get current time in seconds (for timing)
    static long long getCurrentTime();
    
    // Print separator line
    static void printSeparator(int length = 50);
    
    // Center text
    static void printCentered(const std::string& text, int width = 50);
};

#endif // UTILITIES_H

