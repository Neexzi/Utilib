#include "print_utils.h"
#include <windows.h>

void initUTF8Console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}