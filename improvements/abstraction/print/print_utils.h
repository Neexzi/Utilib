#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H
#include <iostream>

void initUTF8Console();

inline void print() {
    std::cout << std::endl;
}

template<typename First, typename... Rest>
void print(const First& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
    }
    print(rest...);
}

#endif