#pragma once
#include <vector>
#include <random>
#include <variant>

struct Range {
    int from;
    int to;
    Range(int f, int t) : from(f), to(t) {}
};

inline int random_impl(std::initializer_list<Range> ranges) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<int> values;

    for (const auto& range : ranges) {
        for (int i = range.from; i <= range.to; ++i)
            values.push_back(i);
    }

    if (values.empty()) return 0;

    std::uniform_int_distribution<> dis(0, values.size() - 1);
    return values[dis(gen)];
}

#define random(...) random_impl({__VA_ARGS__})
