#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <sstream>
#include <string>

class StringBuilder {
private:
    std::ostringstream stream;

public:
    StringBuilder() = default;


    void clear() {
        stream.str("");
        stream.clear();
    }


    template<typename T>
    StringBuilder& append(const T& value) {
        stream << value;
        return *this;
    }


    StringBuilder& insert(size_t pos, const std::string& value) {
        std::string current = stream.str();
        if (pos > current.size()) pos = current.size();
        current.insert(pos, value);
        stream.str("");
        stream.clear();
        stream << current;
        return *this;
    }


    std::string toString() const {
        return stream.str();
    }


    operator std::string() const {
        return toString();
    }


    size_t length() const {
        return stream.str().length();
    }


    bool empty() const {
        return stream.str().empty();
    }
};

#endif // STRING_BUILDER_H
