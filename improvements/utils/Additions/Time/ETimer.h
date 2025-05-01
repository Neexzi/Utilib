#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ctime>
#include <thread>
#include <string>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running;

public:
    Timer() : running(false) {}

    void startTime() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stopTime() {
        if (running) {
            end_time = std::chrono::high_resolution_clock::now();
            running = false;
        }
    }

    double getDuration() const {
        if (running) {
            auto current_time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double>(current_time - start_time).count();
        } else {
            return std::chrono::duration<double>(end_time - start_time).count();
        }
    }

    double print() {
        return getDuration();
    }

    static void sleep(double seconds) {
        std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
    }
};

std::string getCurrentDateTime();  // реализация должна быть отдельно

#endif // TIMER_H
