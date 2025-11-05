#include "Utilities.h"
#include <thread>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

/**
 * Clear the console screen (cross-platform)
 */
void Utilities::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * Set cursor position (for better rendering)
 */
void Utilities::setCursorPosition(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    std::cout << "\033[" << y << ";" << x << "H";
#endif
}

/**
 * Set terminal color (ANSI codes for Unix/Linux/Mac, Windows API for Windows)
 */
void Utilities::setColor(const std::string& color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == "red") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    else if (color == "green") SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else if (color == "blue") SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if (color == "yellow") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else if (color == "cyan") SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if (color == "magenta") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    if (color == "red") std::cout << "\033[31m";
    else if (color == "green") std::cout << "\033[32m";
    else if (color == "blue") std::cout << "\033[34m";
    else if (color == "yellow") std::cout << "\033[33m";
    else if (color == "cyan") std::cout << "\033[36m";
    else if (color == "magenta") std::cout << "\033[35m";
    else std::cout << "\033[0m";
#endif
}

/**
 * Reset terminal color to default
 */
void Utilities::resetColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    std::cout << "\033[0m";
#endif
}

/**
 * Sleep for specified milliseconds
 */
void Utilities::sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/**
 * Get current time in seconds since epoch
 */
long long Utilities::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

/**
 * Print a separator line
 */
void Utilities::printSeparator(int length) {
    for (int i = 0; i < length; i++) {
        std::cout << "=";
    }
    std::cout << std::endl;
}

/**
 * Print centered text
 */
void Utilities::printCentered(const std::string& text, int width) {
    int padding = (width - text.length()) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; i++) {
        std::cout << " ";
    }
    std::cout << text << std::endl;
}

