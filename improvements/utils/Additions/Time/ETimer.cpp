#include <chrono>
#include <iomanip>
#include <ctime>

std::string getCurrentDateTime() {
    // Получаем текущее время
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Преобразуем время в строку
    std::tm* now_tm = std::localtime(&now_time_t);

    // Форматируем дату и время в строку
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}