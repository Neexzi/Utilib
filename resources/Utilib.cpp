#include "Utilib.h"
#include <iostream>
#include <cstdlib>

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
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

void initUTF8Console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void cls() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::system("clear");
#endif
}

void setColor(int color) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
#else
    switch(color) {
        case 12: std::cout << RED;     break;
        case 10: std::cout << GREEN;   break;
        case 14: std::cout << YELLOW;  break;
        case 9:  std::cout << BLUE;    break;
        case 13: std::cout << MAGENTA; break;
        case 11: std::cout << CYAN;    break;
        case 15: std::cout << WHITE;   break;
        case 7:  std::cout << RESET;   break;
        default: std::cout << RESET;   break;
    }
#endif
}


std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm* now_tm = std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}


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

Cipher::Cipher(const std::string& key) {
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty");
    }
    key_ = Cipher::stringToBytes(key);
}

std::vector<uint8_t> hexToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    std::string cleanHex;
    for (char c : hex) {
        if (isxdigit(c)) {
            cleanHex += c;
        }
    }
    if (cleanHex.length() % 2 != 0) {
        cleanHex = "0" + cleanHex;
    }
    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        std::string byteString = cleanHex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<uint8_t> Cipher::generateKeyStream(size_t length) {
    std::vector<uint8_t> stream;
    stream.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        const uint8_t key_byte = key_[i % key_.size()];
        stream.push_back((key_byte << 3) | (key_byte >> 5));
    }

    return stream;
}

std::string Cipher::encrypt(const std::string& text) {
    const auto data = Cipher::stringToBytes(text);
    auto keystream = generateKeyStream(data.size());

    std::vector<uint8_t> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ keystream[i];
    }

    return Cipher::bytesToString(result);
}

std::string Cipher::decrypt(const std::string& ciphertext) {
    return encrypt(ciphertext); // XOR обратим
}

std::vector<uint8_t> Cipher::stringToBytes(const std::string& str) {
    return {str.begin(), str.end()};
}

std::string Cipher::bytesToString(const std::vector<uint8_t>& bytes) {
    return {bytes.begin(), bytes.end()};
}

void initialization(bool logs)
{
    if (logs == true)
    {
        Ascii font = Ascii(small);
        setColor(10);
        font.print("This");
        font.print("is");
        font.print("initialization");
        setColor(15);
        Timer timer;
        timer.startTime();
        initUTF8Console();
        std::cout << "✓ Кодировка UTF-8 установлена\n";
        setColor(10);
        std::cout << "✓ ANSI-цвета подключены\n";
        setColor(11);
        std::cout << "✓ Кроссплатформенность настроена\n";
        setColor(10);
        print("✓ Макросы проверяны");
        std::cout << "✓ Утилиты подключены\n";
        setColor(15);
        timer.stopTime();
        print("Время подключения: ", timer.print());
    }
    else
    {
        initUTF8Console();
    }
}