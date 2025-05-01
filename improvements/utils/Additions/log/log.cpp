#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "../../../abstraction/print/print.h"
#include "../../text/color/color.h"
#include "../Types/value.h"
#include "../Time/ETimer.h"

void console_error(Value TextLog) {
    setColor(12);
    print("Error: ",TextLog," ",getCurrentDateTime());
    setColor(15);
}
void console_warn(Value TextLog) {
    setColor(14);
    print("Warn: ",TextLog," ",getCurrentDateTime());
    setColor(15);
}
void console_log(Value TextLog) {
    setColor(10);
    print("log: ",TextLog," ",getCurrentDateTime());
    setColor(15);
}
void console_info(Value TextLog) {
    setColor(9);
    print("info: ",TextLog," ",getCurrentDateTime());
    setColor(15);
}

template<typename T>
void console_table(const std::vector<std::vector<T>>& table) {
    if (table.empty()) return;

    // Определяем ширину столбцов
    size_t num_columns = table[0].size();
    std::vector<size_t> column_widths(num_columns, 0);

    // Находим максимальную ширину для каждого столбца
    for (const auto& row : table) {
        for (size_t i = 0; i < num_columns; ++i) {
            column_widths[i] = std::max(column_widths[i], std::to_string(row[i]).length());
        }
    }

    // Выводим таблицу
    for (const auto& row : table) {
        for (size_t i = 0; i < num_columns; ++i) {
            std::cout << std::setw(column_widths[i] + 2) << row[i];
        }
        std::cout << "\n";
    }
}
/*
case 12: std::cout << RED;    break;
case 10: std::cout << GREEN;  break;
case 14: std::cout << YELLOW; break;
case 9:  std::cout << BLUE;   break;
case 13: std::cout << MAGENTA;break;
case 11: std::cout << CYAN;   break;
case 15: std::cout << WHITE;  break;
default: std::cout << RESET;  break;
*/