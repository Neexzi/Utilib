#pragma once
#include <variant>
#include <string>
#include <iostream>

class Value {
public:
    using Variant = std::variant<int, double, std::string, bool>;

    Value() = default;
    Value(const char* str) : data(std::string(str)) {}
    Value(const std::string& str) : data(str) {}
    Value(int i) : data(i) {}
    Value(double d) : data(d) {}
    Value(bool b) : data(b) {}

    Value& operator=(const char* str) {
        data = std::string(str);
        return *this;
    }

    template <typename T>
    Value& operator=(T&& val) {
        data = std::forward<T>(val);
        return *this;
    }

    const Variant& get() const {
        return data;
    }

    friend std::ostream& operator<<(std::ostream& os, const Value& val) {
        std::visit([&os](auto&& arg) {
            os << arg;
        }, val.data);
        return os;
    }

private:
    Variant data;
};
