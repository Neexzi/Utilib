#include "console.h"
#include <cstdlib>  // system
#include <iostream>


void cls() {
#if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
#else
        std::system("clear");
#endif
    }
