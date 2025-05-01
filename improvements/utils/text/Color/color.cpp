#include <iostream>
#if defined(_WIN32)
#include <windows.h>
#else
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#endif

void setColor(int color) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    switch(color) {
        case 12: std::cout << RED;    break;
        case 10: std::cout << GREEN;  break;
        case 14: std::cout << YELLOW; break;
        case 9:  std::cout << BLUE;   break;
        case 13: std::cout << MAGENTA;break;
        case 11: std::cout << CYAN;   break;
        case 15: std::cout << WHITE;  break;
        default: std::cout << RESET;  break;
    }
#endif
}
// 15