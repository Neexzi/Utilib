#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <type_traits>

#include "improvements/abstraction/print/print.h"
#include "improvements/utils/text/Color/color.h"
#include "improvements/utils/Additions/Time/ETimer.h"
#include "improvements/utils/Additions/Types/value.h"
#include "improvements/utils/Additions/random/random.h"

class console {
public:
    static void error(const Value& textLog) {
        setColor(12); // красный
        print("Error: ", textLog, " ", getCurrentDateTime());
        resetTextColor();
    }

    static void warn(const Value& textLog) {
        setColor(14); // жёлтый
        print("Warn: ", textLog, " ", getCurrentDateTime());
        resetTextColor();
    }

    static void log(const Value& textLog) {
        setColor(10); // зелёный
        print("Log: ", textLog, " ", getCurrentDateTime());
        resetTextColor();
    }

    static void info(const Value& textLog) {
        setColor(9); // синий
        print("Info: ", textLog, " ", getCurrentDateTime());
        resetTextColor();
    }

    template<typename T>
    static void table(const std::vector<std::vector<T>>& data) {
        if (data.empty()) return;

        size_t num_columns = data[0].size();
        std::vector<size_t> column_widths(num_columns, 0);

        // Вычисляем ширину колонок
        for (const auto& row : data) {
            for (size_t i = 0; i < num_columns; ++i) {
                column_widths[i] = std::max(column_widths[i], getWidth(row[i]));
            }
        }

        // Печать таблицы
        for (const auto& row : data) {
            for (size_t i = 0; i < num_columns; ++i) {
                std::cout << std::setw(column_widths[i] + 2) << toString(row[i]);
            }
            std::cout << "\n";
        }
    }

private:
    static void resetTextColor() {
        setColor(15);
    }

    template<typename T>
    static std::string toString(const T& val) {
        if constexpr (std::is_same<T, std::string>::value)
            return val;
        else if constexpr (std::is_convertible<T, std::string>::value)
            return static_cast<std::string>(val);
        else
            return std::to_string(val);
    }

    template<typename T>
    static size_t getWidth(const T& val) {
        return toString(val).length();
    }
};
