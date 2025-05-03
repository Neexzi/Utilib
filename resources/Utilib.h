#ifndef UTILIB_H
#define UTILIB_H

#include <locale>
#include <codecvt>
#include <algorithm>
#include <numeric>
#include <sys/stat.h>
#include <stdio.h>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include <random>
#include <variant>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
#include <thread>
#include <memory>
#include <sstream>
#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <format>
#include <fstream>
#include <future>
#include <iterator>
#include <list>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <stdexcept>
#include <optional>
#include <variant>


//=======Важно
void initialization(bool logs);
//UTF8
void initUTF8Console();
//cls
void cls();
//Колор
void setColor(int color);

//=======Макросы и синтаксис
//For изи
#define efor(VAR, START, COUNT) \
for (int VAR = (START), _end_##VAR = (START) + (COUNT); VAR < _end_##VAR; ++VAR)

//Улучшеный Print
inline void print() {
    std::cout << std::endl;
}

template<typename First, typename... Rest>
void print(const First& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
    }
    print(rest...);
}


// input
#ifndef INPUT_HPP
#define INPUT_HPP

namespace input {

template <typename T>
inline T get_input(const std::string& prompt = "") {
    T value;
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Пожалуйста, введите корректное значение.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

template <>
inline std::string get_input<std::string>(const std::string& prompt) {
    std::string value;
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    std::getline(std::cin, value);
    return value;
}

template <typename T, typename Validator>
inline T get_input_with_validation(const std::string& prompt, Validator validator) {
    while (true) {
        try {
            T value = get_input<T>(prompt);
            if (validator(value)) {
                return value;
            } else {
                std::cout << "Введенное значение не удовлетворяет условиям. Попробуйте снова.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\nПожалуйста, попробуйте снова.\n";
        }
    }
}

template <typename T>
inline T get_input_in_range(const std::string& prompt, T min, T max) {
    return get_input_with_validation<T>(prompt,
        [min, max](T value) { return value >= min && value <= max; });
}

template <typename T>
inline T get_input_from_list(const std::string& prompt, const std::vector<T>& options) {
    return get_input_with_validation<T>(prompt,
        [&options](T value) {
            return std::find(options.begin(), options.end(), value) != options.end();
        });
}

inline bool get_yes_no_input(const std::string& prompt, char yes_char = 'y', char no_char = 'n') {
    std::string options = std::string(" (") + yes_char + "/" + no_char + "): ";
    char response = get_input_with_validation<char>(
        prompt + options,
        [yes_char, no_char](char c) {
            return std::tolower(c) == std::tolower(yes_char) ||
                   std::tolower(c) == std::tolower(no_char);
        });
    return std::tolower(response) == std::tolower(yes_char);
}

} // namespace input

#endif // INPUT_HPP
using namespace input;

//=======Мини утилиты

// Рандом
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

// Время либ

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

std::string getCurrentDateTime();

// Динамический тип
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

// Логирование

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


// Фишки дополнения
#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

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
#endif


// Шифрования
#ifndef SIMPLE_CIPHER_H
#define SIMPLE_CIPHER_H

std::vector<uint8_t> hexToBytes(const std::string& hex);

class Cipher {
public:
    explicit Cipher(const std::string& key);
    std::string encrypt(const std::string& text);
    std::string decrypt(const std::string& ciphertext);
private:
    std::vector<uint8_t> key_;
    std::vector<uint8_t> generateKeyStream(size_t length);
    static std::vector<uint8_t> stringToBytes(const std::string& str);
    static std::string bytesToString(const std::vector<uint8_t>& bytes);
};

#endif













/*
				     _    _
 __ _    ___   __   (_)  (_)
/ _` |  (_-<  / _|  | |  | |
\__,_|  /__/  \__|  |_|  |_|
*/
#ifndef FONTS_H
#define FONTS_H
#define DEBUG false
//#include <string>
//#include <memory>

//#include <vector>
//#include <string>
//#include <iostream>

using vs = std::vector<std::string>;


class Fonts
{
    unsigned int def_rows;
    unsigned int def_cols;
    unsigned int char_rows;
    unsigned int char_cols;
    unsigned int curr_col;
    vs letters;

protected:
    auto getCharGrid(unsigned int rows = 0, unsigned int cols = 0)
    {
        this->char_rows = rows ? rows : def_rows;
        this->char_cols = cols ? cols : def_cols;

        return vs (char_rows, std::string(char_cols, ' '));
    }

public:
    Fonts(int def_rows, int def_cols)
    {
        this->def_rows = def_rows;
        this->def_cols = def_cols;

        letters.reserve(def_rows);
        for (int i = 0; i < def_rows; i++)
        {
            letters.emplace_back(100, ' '); // Create rows with 100 spaces each
        }
        curr_col = 0;
    }

    void pushChar(vs character)
    {
        if(character.empty()) return;

        // Add empty rows, all with the same lenght
        while(letters.size() < char_rows)
            letters.emplace_back(std::string(letters[0].size(), ' '));

        // Space needed to add the new character (if it is negative there is no need)
        int needed = 2 + curr_col + character[0].size() - letters[0].size();

        if(needed > 0){
            for(auto& row : letters)
                row.resize(needed + row.size());
        }

        for (unsigned int i = 0; i < char_rows; i++)
        {
            for (unsigned int j = 0; j < char_cols; j++)
            {
                letters[i][j + curr_col] = character[i][j];
            }
        }
        curr_col += (char_cols + 2);
    }

    vs getletters()
    {
        return letters;
    }

    void printvector()
    {
        for (unsigned int i = 0; i < letters.size(); i++)
        {
          std::cout << letters[i] << std::endl;
        }
    }

    void resetMatrix()
    {
        letters.clear();
        letters.reserve(def_rows);
        for(unsigned int i = 0; i < def_rows; i++) {
            letters.emplace_back(100, ' ');
        }
        curr_col = 0;
    }

    /********************************adding virtual functions********************************/
    // Virtual functions for space
    virtual vs space()
    {
        std::cout << "space not overridden?" << std::endl;
        return vs();
    }

    // Virtual functions for lowercase letters
    virtual vs a()
    {
        std::cout << "a not overridden?" << std::endl;
        return vs();
    }
    virtual vs b()
    {
        std::cout << "b not overridden?" << std::endl;
        return vs();
    }
    virtual vs c()
    {
        std::cout << "c not overridden?" << std::endl;
        return vs();
    }
    virtual vs d()
    {
        std::cout << "d not overridden?" << std::endl;
        return vs();
    }
    virtual vs e()
    {
        std::cout << "e not overridden?" << std::endl;
        return vs();
    }
    virtual vs f()
    {
        std::cout << "f not overridden?" << std::endl;
        return vs();
    }
    virtual vs g()
    {
        std::cout << "g not overridden?" << std::endl;
        return vs();
    }
    virtual vs h()
    {
        std::cout << "h not overridden?" << std::endl;
        return vs();
    }
    virtual vs i()
    {
        std::cout << "i not overridden?" << std::endl;
        return vs();
    }
    virtual vs j()
    {
        std::cout << "j not overridden?" << std::endl;
        return vs();
    }
    virtual vs k()
    {
        std::cout << "k not overridden?" << std::endl;
        return vs();
    }
    virtual vs l()
    {
        std::cout << "l not overridden?" << std::endl;
        return vs();
    }
    virtual vs m()
    {
        std::cout << "m not overridden?" << std::endl;
        return vs();
    }
    virtual vs n()
    {
        std::cout << "n not overridden?" << std::endl;
        return vs();
    }
    virtual vs o()
    {
        std::cout << "o not overridden?" << std::endl;
        return vs();
    }
    virtual vs p()
    {
        std::cout << "p not overridden?" << std::endl;
        return vs();
    }
    virtual vs q()
    {
        std::cout << "q not overridden?" << std::endl;
        return vs();
    }
    virtual vs r()
    {
        std::cout << "r not overridden?" << std::endl;
        return vs();
    }
    virtual vs s()
    {
        std::cout << "s not overridden?" << std::endl;
        return vs();
    }
    virtual vs t()
    {
        std::cout << "t not overridden?" << std::endl;
        return vs();
    }
    virtual vs u()
    {
        std::cout << "u not overridden?" << std::endl;
        return vs();
    }
    virtual vs v()
    {
        std::cout << "v not overridden?" << std::endl;
        return vs();
    }
    virtual vs w()
    {
        std::cout << "w not overridden?" << std::endl;
        return vs();
    }
    virtual vs x()
    {
        std::cout << "x not overridden?" << std::endl;
        return vs();
    }
    virtual vs y()
    {
        std::cout << "y not overridden?" << std::endl;
        return vs();
    }
    virtual vs z()
    {
        std::cout << "z not overridden?" << std::endl;
        return vs();
    }

    // Virtual functions for uppercase letters
    virtual vs A()
    {
        std::cout << "A not overridden?" << std::endl;
        return vs();
    }
    virtual vs B()
    {
        std::cout << "B not overridden?" << std::endl;
        return vs();
    }
    virtual vs C()
    {
        std::cout << "C not overridden?" << std::endl;
        return vs();
    }
    virtual vs D()
    {
        std::cout << "D not overridden?" << std::endl;
        return vs();
    }
    virtual vs E()
    {
        std::cout << "E not overridden?" << std::endl;
        return vs();
    }
    virtual vs F()
    {
        std::cout << "F not overridden?" << std::endl;
        return vs();
    }
    virtual vs G()
    {
        std::cout << "G not overridden?" << std::endl;
        return vs();
    }
    virtual vs H()
    {
        std::cout << "H not overridden?" << std::endl;
        return vs();
    }
    virtual vs I()
    {
        std::cout << "I not overridden?" << std::endl;
        return vs();
    }
    virtual vs J()
    {
        std::cout << "J not overridden?" << std::endl;
        return vs();
    }
    virtual vs K()
    {
        std::cout << "K not overridden?" << std::endl;
        return vs();
    }
    virtual vs L()
    {
        std::cout << "L not overridden?" << std::endl;
        return vs();
    }
    virtual vs M()
    {
        std::cout << "M not overridden?" << std::endl;
        return vs();
    }
    virtual vs N()
    {
        std::cout << "N not overridden?" << std::endl;
        return vs();
    }
    virtual vs O()
    {
        std::cout << "O not overridden?" << std::endl;
        return vs();
    }
    virtual vs P()
    {
        std::cout << "P not overridden?" << std::endl;
        return vs();
    }
    virtual vs Q()
    {
        std::cout << "Q not overridden?" << std::endl;
        return vs();
    }
    virtual vs R()
    {
        std::cout << "R not overridden?" << std::endl;
        return vs();
    }
    virtual vs S()
    {
        std::cout << "S not overridden?" << std::endl;
        return vs();
    }
    virtual vs T()
    {
        std::cout << "T not overridden?" << std::endl;
        return vs();
    }
    virtual vs U()
    {
        std::cout << "U not overridden?" << std::endl;
        return vs();
    }
    virtual vs V()
    {
        std::cout << "V not overridden?" << std::endl;
        return vs();
    }
    virtual vs W()
    {
        std::cout << "W not overridden?" << std::endl;
        return vs();
    }
    virtual vs X()
    {
        std::cout << "X not overridden?" << std::endl;
        return vs();
    }
    virtual vs Y()
    {
        std::cout << "Y not overridden?" << std::endl;
        return vs();
    }
    virtual vs Z()
    {
        std::cout << "Z not overridden?" << std::endl;
        return vs();
    }
    virtual vs zero()
    {
        std::cout << "Zero not overridden?" << std::endl;
        return vs();
    }
    virtual vs one()
    {
        std::cout << "One not overridden?" << std::endl;
        return vs();
    }
    virtual vs two()
    {
        std::cout << "Two not overridden?" << std::endl;
        return vs();
    }
    virtual vs three()
    {
        std::cout << "Three not overridden?" << std::endl;
        return vs();
    }
    virtual vs four()
    {
        std::cout << "Four not overridden?" << std::endl;
        return vs();
    }
    virtual vs five()
    {
        std::cout << "Five not overridden?" << std::endl;
        return vs();
    }
    virtual vs six()
    {
        std::cout << "Six not overridden?" << std::endl;
        return vs();
    }
    virtual vs seven()
    {
        std::cout << "Seven not overridden?" << std::endl;
        return vs();
    }
    virtual vs eight()
    {
        std::cout << "Eight not overridden?" << std::endl;
        return vs();
    }
    virtual vs nine()
    {
        std::cout << "Nine not overridden?" << std::endl;
        return vs();
    }
    /********************************done adding virtual functions********************************/
};
#endif

#ifndef SEVENSTAR_H
#define SEVENSTAR_H

class SevenStar : public Fonts
{
	static const int ROWS = 7;
	static const int COLS = 7;

public:
	SevenStar() : Fonts(ROWS, COLS) {}

	vs A()
	{
		vs character = getCharGrid();

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 1)
				{
					if (j % 3 == 0)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 3 || i == 5 || i == 6)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				if (i == 4)
				{

					character[i][j] = '%';
				}
			}
		}

		return character;
	}
	vs B()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 1 && i <= 5 && i != 3)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}
	vs C()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				// first aur last line
				if (i == 0 || i == 6)
				{
					if ((j % 7) % 3 != 0)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				// second nd second last line

				if (i == 1 || i == 5)
				{
					if (((j) % 7) < 2 || ((j) % 7) > 4)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				// rest

				if (i == 2 || i == 4 || i == 3)
				{
					if (((j) % 7) < 2)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}
	vs D()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (((j) % 7) != 0 && ((j) % 7) % 3 == 0)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (((j) % 7) == 1 || ((j) % 7) == 2 || ((j) % 7) == 5 || ((j) % 7) == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs E()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 2 || i == 4)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs F()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					character[i][j] = '%';
				}

				if (i == 2 || i == 5 || i == 6 || i == 1 || i == 4)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs G()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5 || i == 4)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 2 || j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs H()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i >= 0 && i <= 6)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j > 1 && j < 5)
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs I()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					character[i][j] = '%';
				}
				else
				{
					if (j == 2 || j == 3 || j == 4)
					{
						character[i][j] = '%';
					}
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs J()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j < 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2 || i == 3)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 4 || i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 6)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs K()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 4 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 4)
				{
					if (j == 2 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs L()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i < 6)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				else
					character[i][j] = '%';
			}
		}
		return character;
	}

	vs M()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1)
				{
					if (j == 3)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 4 && i <= 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs N()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 1)
				{
					if (j == 3 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
					{
						character[i][j] = '%';
					}
				}

				if (i == 4 || i == 5)
				{
					if (j == 2 || j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}
	vs O()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 6 || i == 0)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			//	cout<<endl;
		}
		return character;
	}

	vs P()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2)
				{
					if (j == 2 || j == 3 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i >= 4 && i <= 6)
				{
					if (j == 0 || j == 1)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}

	vs Q()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j % 3 == 0)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 1 && i <= 4)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				if (i == 6)
				{
					if (j == 0 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs R()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2 || i == 5 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
			// cout<<endl;
		}
		return character;
	}

	vs S()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 3 || j == 2 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					if (j == 4 || j == 5 || j == 6)
					{
						character[i][j] = ' ';
					}
					else
					{
						character[i][j] = '%';
					}
				}

				if (i == 3)
				{

					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					if (j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs T()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					character[i][j] = '%';
				}

				if (i >= 1 && i <= 6)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs U()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i <= 5)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (j == 0 || j == 6)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs V()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 5)
				{
					if (j % 3 == 0)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}
				if (i < 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}

	vs W()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 1 || i == 2 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					character[i][j] = '%';
				}

				if (i == 5)
				{
					if (j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs X()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 1 || i == 5 || i == 6)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 4)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs Y()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{

			for (int j = 0; j < 7; j++)
			{

				if (i == 0 || i == 1 || i == 2)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i > 3)
				{
					if (j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs Z()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j > 0)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 1)
				{
					if (j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 2)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 5)
				{
					if (j == 1 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}

			// cout<<endl;
		}
		return character;
	}
	// small letter
	vs a()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 1)
				{
					if (j % 3 == 0)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 3 || i == 5 || i == 6)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				if (i == 4)
				{

					character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs b()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 1 && i <= 5 && i != 3)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}
	vs c()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				// first aur last line
				if (i == 0 || i == 6)
				{
					if ((j % 7) % 3 != 0)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				// second nd second last line

				if (i == 1 || i == 5)
				{
					if (((j) % 7) < 2 || ((j) % 7) > 4)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}

				// rest

				if (i == 2 || i == 4 || i == 3)
				{
					if (((j) % 7) < 2)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}
	vs d()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (((j) % 7) != 0 && ((j) % 7) % 3 == 0)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (((j) % 7) == 1 || ((j) % 7) == 2 || ((j) % 7) == 5 || ((j) % 7) == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs e()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 2 || i == 4)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs f()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					character[i][j] = '%';
				}

				if (i == 2 || i == 5 || i == 6 || i == 1 || i == 4)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs g()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5 || i == 4)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j == 2 || j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs h()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i >= 0 && i <= 6)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 3)
				{
					if (j > 1 && j < 5)
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs i()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					character[i][j] = '%';
				}
				else
				{
					if (j == 2 || j == 3 || j == 4)
					{
						character[i][j] = '%';
					}
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs j()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j < 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2 || i == 3)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 4 || i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 6)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs k()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 4 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 4)
				{
					if (j == 2 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs l()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i < 6)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				else
					character[i][j] = '%';
			}
		}
		return character;
	}

	vs m()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1)
				{
					if (j == 3)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 4 && i <= 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs n()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 1)
				{
					if (j == 3 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
					{
						character[i][j] = '%';
					}
				}

				if (i == 4 || i == 5)
				{
					if (j == 2 || j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}
	vs o()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 6 || i == 0)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			//	cout<<endl;
		}
		return character;
	}

	vs p()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2)
				{
					if (j == 2 || j == 3 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i >= 4 && i <= 6)
				{
					if (j == 0 || j == 1)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}

	vs q()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j % 3 == 0)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i >= 1 && i <= 4)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 5)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				if (i == 6)
				{
					if (j == 0 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs r()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 3)
				{
					if (j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 2 || i == 5 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}
			// cout<<endl;
		}
		return character;
	}

	vs s()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 1 || i == 5)
				{
					if (j == 3 || j == 2 || j == 4)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}

				if (i == 2)
				{
					if (j == 4 || j == 5 || j == 6)
					{
						character[i][j] = ' ';
					}
					else
					{
						character[i][j] = '%';
					}
				}

				if (i == 3)
				{

					if (j == 0 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					if (j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs t()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					character[i][j] = '%';
				}

				if (i >= 1 && i <= 6)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs u()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i <= 5)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
				else
				{
					if (j == 0 || j == 6)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}
			}
		}
		return character;
	}

	vs v()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 5)
				{
					if (j % 3 == 0)
					{
						character[i][j] = ' ';
					}
					else
						character[i][j] = '%';
				}
				if (i < 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
					{
						character[i][j] = '%';
					}
					else
					{
						character[i][j] = ' ';
					}
				}
			}
		}
		return character;
	}

	vs w()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 1 || i == 2 || i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 2 || j == 4)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 4)
				{
					character[i][j] = '%';
				}

				if (i == 5)
				{
					if (j == 3)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	vs x()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 1 || i == 5 || i == 6)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 2 || i == 4)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs y()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{

			for (int j = 0; j < 7; j++)
			{

				if (i == 0 || i == 1 || i == 2)
				{
					if (j == 2 || j == 3 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i == 3)
				{
					if (j == 0 || j == 5 || j == 6)
						character[i][j] = ' ';
					else
						character[i][j] = '%';
				}

				if (i > 3)
				{
					if (j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs z()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j > 0)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 1)
				{
					if (j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}

				if (i == 2)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 3 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 5)
				{
					if (j == 1 || j == 2)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}

			// cout<<endl;
		}
		return character;
	}

	// digits
	vs zero()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 0 || j == 1 || j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 0 || j == 1 || j == 3 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 0 || j == 1 || j == 2 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs one()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 2 || i == 3 || i == 4 || i == 5)
				{
					if (j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 1 || j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 6)
				{
					if (j == 1 || j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}
	vs two()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 0 || j == 1 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 0 || j == 1 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 5)
				{
					if (j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 6)
				{
					if (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs three()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2 || i == 4)
				{
					if (j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 3 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs four()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 5 || i == 6)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 0 || j == 1 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 0 || j == 1 || j == 2 || j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs five()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 0 || j == 1 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4 || i == 5)
				{
					if (j == 0 || j == 1 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 6)
				{
					if (j == 2 || j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs six()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1 || i == 4 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 0 || j == 1)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 0 || j == 1 || j == 3 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 6)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs seven()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0)
				{
					if (j == 0 || j == 1 || j == 2 || j == 3 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 0 || j == 1 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 3 || j == 4)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3 || i == 4 || i == 5 || i == 6)
				{
					if (j == 2 || j == 3)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs eight()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 3 || i == 6)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1 || i == 2 || i == 4 || i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	vs nine()
	{
		vs character = getCharGrid();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 6)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 1)
				{
					if (j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 2)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 3)
				{
					if (j == 1 || j == 2 || j == 4 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 4)
				{
					if (j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
				if (i == 5)
				{
					if (j == 0 || j == 1 || j == 5 || j == 6)
						character[i][j] = '%';
					else
						character[i][j] = ' ';
				}
			}
		}
		return character;
	}

	 // for space
     vs space()
    {
        vs character = getCharGrid(1,1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef BOOMER_H
#define BOOMER_H

class Boomer : public Fonts
{
    static const int rows = 7;
    static const int cols = 7;

public:
    Boomer() : Fonts(rows, cols) {}

    // Uppercase

    vs A()
    {

        vs character = getCharGrid(6,7);
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][3] = character[3][3] = character[5][1] = character[5][5] = '_';
        character[1][1] = character[2][0] = character[2][2] = character[5][6] = '/';
        character[1][5] = character[2][4] = character[2][6] = character[5][0] = '\\';
        character[3][0] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][6] = character[5][2] = character[5][4] = '|';
        return character;
    }

    vs B()
    {
        vs character = getCharGrid(6,7);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][3] = character[1][4] = character[2][3] = character[3][2] = character[3][3] = character[3][4] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[4][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[2][2] = character[4][2] = '|';
        character[1][6] = character[3][6] = character[5][0] = '\\';
        character[2][4] = character[2][6] = character[4][6] = character[5][5] = character[4][4] = '/';
        return character;
    }
    vs C()
    {
        vs character = getCharGrid(7,7);
        character[0][0] = character[0][1] = character[1][1] = character[1][2] = character[1][5] = character[2][1] = character[2][3] = character[2][4] = character[3][1] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = character[4][1] = character[5][1] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = ' ';
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][3] = character[1][4] = character[4][3] = character[4][4] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[2][0] = character[3][0] = character[3][2] = character[4][0] = '|';
        character[1][0] = character[2][2] = character[2][6] = character[4][5] = character[5][6] = '/';

        character[1][6] = character[2][5] = character[4][2] = character[4][6] = character[5][1] = '\\';
        return character;
    }
    vs D()
    {

        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[5][1] = character[1][3] = character[5][2] = character[5][3] = '_';
        character[4][3] = character[4][5] = character[5][4] = '/';
        character[1][6] = '\\';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[2][2] = character[3][2] = character[4][2] = character[2][4] = character[3][4] = character[2][6] = character[3][6] = '|';
        return character;
    }

    vs E()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[0][5] = character[1][1] = character[1][2] = character[2][1] = character[2][2] = character[3][1] = character[3][2] = character[4][1] = character[4][2] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[2][3] = character[2][4] = character[3][3] = character[3][4] = character[4][3] = character[4][4] = character[4][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[1][0] = character[1][6] = character[2][0] = character[2][2] = character[3][0] = character[3][5] = character[4][0] = character[4][2] = '|';
        character[5][0] = '\\';
        character[5][5] = '/';
        return character;
    }

    vs F()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[1][1] = character[1][2] = character[2][1] = character[2][2] = character[3][1] = character[3][2] = character[4][1] = character[4][2] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[1][5] = character[2][3] = character[3][3] = character[5][1] = '_';
        character[1][0] = character[1][5] = character[2][0] = character[2][2] = character[3][0] = character[3][4] = character[4][0] = character[4][2] = character[5][2] = '|';
        character[5][0] = '\\';
        return character;
    }
    vs G()
    {

        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[3][4] = character[3][5] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[2][2] = character[3][2] = character[4][2] = '|';
        character[1][6] = character[2][5] = character[4][4] = character[4][6] = character[5][1] = '\\';
        character[2][6] = character[5][6] = '/';
        return character;
    }

    vs H()
    {

        vs character = getCharGrid(6,7);
        character[0][1] = character[0][5] = character[3][3] = character[2][3] = character[5][1] = character[5][5] = '_';
        character[1][0] = character[1][2] = character[1][4] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][6] = character[5][2] = character[5][4] = '|';
        character[5][0] = '\\';
        character[5][6] = '/';
        character[5][0] = '\\';
        character[5][6] = '/';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid(6,7);
        character[0][0] = character[0][6] = character[1][2] = character[1][3] = character[1][4] = character[2][0] = character[2][1] = character[2][3] = character[2][5] = character[2][6] = character[3][0] = character[3][1] = character[3][3] = character[3][5] = character[3][6] = character[4][0] = character[4][1] = character[4][3] = character[4][6] = character[5][0] = character[5][6] = ' ';
        character[1][0] = character[1][6] = character[2][2] = character[2][4] = character[3][2] = character[3][4] = character[4][2] = character[4][4] = '|';
        character[5][5] = '/';
        character[5][1] = '\\';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][1] = character[1][5] = character[4][1] = character[4][5] = character[5][2] = character[5][3] = character[5][4] = '_';

        return character;
    }

    vs J()
    {
        vs character = getCharGrid(6,7);
        character[0][0] = character[0][1] = character[0][2] = character[0][6] = character[1][0] = character[1][1] = character[1][4] = character[1][5] = character[2][0] = character[2][1] = character[2][2] = character[2][3] = character[2][5] = character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][5] = character[4][5] = character[5][6] = ' ';
        character[1][2] = character[1][6] = character[2][4] = character[2][6] = character[3][4] = character[3][6] = '|';
        character[4][0] = character[4][4] = character[4][6] = character[5][5] = '/';
        character[4][1] = character[5][0] = '\\';
        character[4][2] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = '_';

        return character;
    }

    vs K()
    {

        vs character = getCharGrid(6,7);
        character[0][6] = character[0][1] = character[0][5] = character[5][1] = character[5][5] = '_';
        character[1][4] = character[1][6] = character[2][5] = character[2][3] = character[5][6] = '/';
        character[3][5] = character[4][6] = character[4][3] = character[5][4] = character[5][0] = '\\';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[1][2] = character[2][2] = character[4][2] = character[5][2] = '|';
        return character;
    }

    vs L()
    {
        vs character = getCharGrid(6,7);

        character[0][1] = character[4][3] = character[4][4] = character[4][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][2] = character[4][0] = character[4][2] = '|';
        character[5][0] = '\\';
        character[5][5] = '/';

        return character;
    }

    vs M()
    {
        vs character = getCharGrid(6, 8);
        character[0][0] = character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[0][7] = character[5][1] = character[5][6] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[3][2] = character[4][2] = character[5][2] = character[3][5] = character[4][5] = character[5][5] = character[1][7] = character[2][7] = character[3][7] = character[4][7] = '|';
        character[2][2] = character[2][5] = '.';
        character[1][3] = character[3][3] = character[5][0] = '\\';
        character[1][4] = character[3][4] = character[5][7] = '/';

        return character;
    }

    vs N()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][5] = character[5][1] = character[5][5] = '_';
        character[1][2] = character[2][3] = character[4][3] = character[5][0] = character[5][4] = '\\';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[4][2] = character[5][2] = character[1][4] = character[2][4] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = '|';
        character[5][6] = '/';
        character[3][2] = '.';
        character[3][4] = '`';
        return character;
    }

    vs O()
    {
        vs character = getCharGrid(6,7);
        character[1][0] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][6] = '|';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[4][0] = character[4][2] = character[5][1] = '\\';
        character[4][4] = character[4][6] = character[5][5] = '/';
        return character;
    }

    vs P()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][3] = character[1][4] = character[2][3] = character[3][3] = character[3][4] = character[5][1] = '_';
        character[2][4] = character[2][6] = character[3][5] = '/';
        character[1][6] = character[5][0] = '\\';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[2][2] = character[4][2] = character[5][2] = '|';
        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(6,7);
        character[0][0] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[2][1] = character[2][3] = character[2][5] = character[3][1] = character[3][3] = character[3][5] = character[4][1] = character[4][5] = character[5][0] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[5][2] = character[5][5] = '_';
        character[4][0] = character[4][2] = character[5][1] = character[5][4] = character[5][6] = '\\';
        character[1][0] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][6] = '|';
        character[4][4] = '\'';
        character[4][3] = character[4][6] = character[5][3] = '/';
        return character;
    }

    vs R()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][3] = character[1][4] = character[5][1] = character[5][5] = character[2][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[2][2] = character[4][2] = character[5][2] = character[5][6] = '|';
        character[1][6] = character[4][5] = character[4][3] = character[5][0] = character[5][4] = '\\';
        character[2][4] = character[2][6] = character[3][5] = '/';

        return character;
    }
    vs S()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[1][5] = character[4][2] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[2][0] = character[3][6] = character[4][1] = character[5][0] = '\\';
        character[1][0] = character[4][0] = character[4][4] = character[4][6] = character[5][5] = '/';
        character[1][6] = '|';
        character[2][2] = character[3][1] = '`';
        character[2][5] = character[3][4] = '.';
        character[2][3] = character[2][4] = character[3][2] = character[3][3] = '-';
        return character;
    }
    vs T()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[0][6] = character[1][2] = character[1][3] = character[1][4] = character[2][0] = character[2][1] = character[2][3] = character[2][5] = character[2][6] = character[3][0] = character[3][1] = character[3][3] = character[3][5] = character[3][6] = character[4][0] = character[4][1] = character[4][3] = character[4][5] = character[4][6] = character[5][0] = character[5][1] = character[5][3] = character[5][5] = character[5][6] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][1] = character[1][5] = character[5][3] = '_';
        character[1][0] = character[1][6] = character[2][2] = character[2][4] = character[3][2] = character[3][4] = character[4][2] = character[4][4] = '|';
        character[5][4] = '/';
        character[5][2] = '\\';
        return character;
    }

    vs U()
    {
        vs character = getCharGrid(6,7);
        character[1][0] = character[1][2] = character[1][4] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][6] = '|';
        character[0][1] = character[0][5] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[5][1] = '\\';
        character[5][5] = '/';
        return character;
    }

    vs V()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][5] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][0] = character[1][2] = character[1][4] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][6] = '|';
        character[4][0] = character[4][2] = character[5][1] = '\\';
        character[4][4] = character[4][6] = character[5][5] = '/';
        return character;
    }

    vs W()
    {
        vs character = getCharGrid(6, 8);
        character[0][1] = character[0][6] = '_';
        character[1][0] = character[1][2] = character[1][5] = character[1][7] = character[2][0] = character[2][2] = character[2][5] = character[2][7] = character[3][0] = character[3][2] = character[3][5] = character[3][7] = '|';
        character[3][4] = character[4][4] = character[4][0] = character[5][1] = character[5][5] = '\\';
        character[3][3] = character[4][3] = character[4][7] = character[5][2] = character[5][6] = '/';
        return character;
    }

    vs X()
    {
        vs character = getCharGrid(6,7);

        character[0][0] = character[0][1] = character[0][5] = character[0][6] = '_';
        character[1][0] = character[1][2] = character[2][1] = character[3][5] = character[4][4] = character[4][6] = character[5][5] = character[5][0] = '\\';
        character[1][4] = character[1][6] = character[2][5] = character[3][1] = character[4][0] = character[4][2] = character[5][1] = character[5][6] = '/';
        character[2][3] = 'v';
        character[4][3] = '^';
        return character;
    }

    vs Y()
    {

        vs character = getCharGrid(6,7);
        character[0][0] = character[0][1] = character[0][5] = character[0][6] = character[5][3] = '_';
        character[1][4] = character[1][6] = character[2][5] = character[5][4] = character[3][4] = '/';
        character[1][0] = character[1][2] = character[3][2] = character[5][2] = character[2][1] = '\\';
        character[2][3] = 'v';
        character[4][2] = character[4][4] = '|';
        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][1] = character[1][2] = character[1][3] = character[4][5] = character[4][6] = character[4][4] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[2][3] = character[2][5] = character[1][6] = character[3][2] = character[3][4] = character[4][1] = character[4][3] = character[5][6] = '/';
        character[5][0] = '\\';
        character[1][0] = '|';
        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(6,7);

        character[2][2] = character[2][3] = character[2][5] = '_';
        character[3][1] = '/';
        character[3][3] = '_';
        character[3][4] = '`';
        character[3][6] = '|';
        character[4][0] = '|';
        character[4][2] = '(';
        character[4][3] = '_';
        character[4][4] = '|';
        character[4][6] = '|';
        character[5][1] = '\\';
        character[5][2] = '_';
        character[5][3] = '_';
        character[5][4] = ',';
        character[5][5] = '_';
        character[5][6] = '|';
        return character;
    }

    vs b()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[5][1] = character[5][3] = character[5][4] = character[2][3] = character[2][4] = character[3][3] = character[4][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[1][2] = character[2][2] = character[4][2] = character[4][6] = '|';
        character[4][4] = ')';
        character[5][2] = '.';
        character[3][2] = '\'';
        character[3][5] = '\\';
        character[5][5] = '/';

        return character;
    }

    vs c()
    {
        vs character = getCharGrid(6,6);
        character[2][2] = character[2][3] = character[2][4] = character[5][2] = character[5][3] = character[5][4] = character[3][3] = character[3][4] = character[4][3] = character[4][4] = '_';
        character[3][1] = '/';
        character[4][0] = '|';
        character[4][2] = '(';
        character[5][1] = '\\';
        character[3][5] = character[5][5] = '|';
        return character;
    }

    vs d()
    {
        vs character = getCharGrid(6,7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][6] = character[1][0] = character[1][1] = character[1][2] = character[1][3] = character[1][5] = character[2][0] = character[2][1] = character[2][2] = character[2][3] = character[2][5] = ' ';
        character[0][5] = character[2][2] = character[2][3] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = character[5][5] = '_';
        character[1][4] = character[1][6] = character[2][4] = character[2][6] = character[3][6] = character[4][0] = character[4][4] = character[4][6] = character[5][6] = '|';
        character[4][2] = '(';
        character[5][4] = ',';
        character[3][4] = '`';
        character[5][1] = '\\';
        character[3][1] = '/';
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][6] = character[1][0] = character[1][1] = character[1][2] = character[1][3] = character[1][5] = character[2][0] = character[2][1] = character[2][2] = character[2][3] = character[2][5] = ' ';
        character[0][5] = character[2][2] = character[2][3] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = character[5][5] = '_';
        character[1][4] = character[1][6] = character[2][4] = character[2][6] = character[3][6] = character[4][0] = character[4][4] = character[4][6] = character[5][6] = '|';
        character[4][2] = '(';
        character[5][4] = ',';
        character[3][4] = '`';
        character[5][1] = '\\';
        character[3][1] = '/';

        return character;
    }

    vs e()
    {
        vs character = getCharGrid(6,6);
        character[2][2] = character[2][3] = character[2][4] = character[5][2] = character[5][3] = character[5][4] = character[4][3] = character[4][4] = character[3][3] = '_';
        character[3][5] = character[5][1] = '\\';
        character[4][0] = character[5][5] = '|';
        character[3][1] = character[4][5] = '/';

        return character;
    }

    vs f()
    {
        vs character = getCharGrid(6,5);
        character[0][3] = character[0][4] = character[1][3] = character[2][3] = character[3][3] = character[5][1] = '_';
        character[1][4] = character[2][0] = character[2][2] = character[3][0] = character[3][4] =
            character[4][0] = character[4][2] = character[5][0] = character[5][2] = '|';
        character[1][1] = '/';

        return character;
    }

    vs g()
    {
        vs character = getCharGrid(8, 7);

        character[2][0] = character[2][1] = character[2][4] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][5] = character[4][1] = character[4][5] = character[5][0] = character[5][5] = character[6][0] = character[6][1] = character[6][5] = character[7][0] = character[7][6] = ' ';
        character[2][2] = character[2][3] = character[2][5] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = character[6][2] = character[6][3] = character[7][2] = character[7][3] = character[7][4] = '_';
        character[3][6] = character[4][0] = character[4][4] = character[4][6] = character[5][6] = character[6][6] = character[7][1] = '|';
        character[3][1] = character[6][4] = character[7][5] = '/';
        character[5][1] = '\\';
        character[3][4] = '`';
        character[5][4] = ',';
        character[4][2] = '(';

        return character;
    }

    vs h()
    {

        vs character = getCharGrid(6,7);
        character[0][1] = character[5][1] = character[5][5] = character[2][3] = character[2][4] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[1][2] = character[2][2] = character[4][2] = character[5][2] = character[4][4] = character[5][4] = character[4][6] = character[5][6] = '|';
        character[3][5] = '\\';
        return character;
    }

    vs i()
    {

        vs character = getCharGrid(6,3);
        character[0][1] = character[1][1] = character[2][1] = character[5][1] = '_';
        character[3][2] = character[4][2] = character[5][2] = character[3][0] = character[4][0] = character[5][0] = '|';
        character[1][0] = '(';
        character[1][2] = ')';
        return character;
    }

    vs j()
    {
        vs character = getCharGrid(8, 5);
        character[0][0] = character[0][1] = character[0][2] = character[1][0] = character[1][1] = character[2][0] = character[2][1] = character[2][2] = character[3][0] = character[3][1] = character[3][3] = character[4][0] = character[4][1] = character[4][3] = character[5][0] = character[5][3] = ' ';
        character[0][3] = character[1][3] = character[2][3] = character[6][1] = character[7][1] = character[7][2] = '_';
        character[3][2] = character[3][4] = character[4][2] = character[4][4] = character[5][2] = character[5][4] = character[6][4] = character[7][0] = '|';
        character[6][2] = character[7][3] = '/';
        character[1][2] = '(';
        character[1][4] = ')';
        return character;
    }

    vs k()
    {
        vs character = getCharGrid(6,6);
        character[0][1] = character[2][4] = character[2][5] = character[5][1] = character[5][4] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][2] = character[4][0] = character[5][0] = character[5][2] = '|';
        character[3][3] = character[3][5] = '/';
        character[4][4] = '<';
        character[5][3] = character[5][5] = '\\';
        return character;
    }

    vs l()
    {
        vs character = getCharGrid(6, 3);

        character[0][1] = character[5][1] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[1][2] = character[2][2] = character[3][2] = character[4][2] = character[5][0] = character[5][2] = '|';
        return character;
    }

    vs m()
    {
        vs character = getCharGrid(7, 11);

        character[2][1] = character[2][3] = character[2][4] = character[2][6] = character[2][7] = character[2][8] = character[3][3] = character[3][7] = character[5][1] = character[5][5] = character[5][9] = '_';
        character[2][0] = character[2][2] = character[2][5] = character[2][9] = character[3][1] = character[4][1] = character[4][3] = character[4][5] = character[4][7] = character[4][9] = character[5][3] = character[5][7] = ' ';
        character[3][0] = character[4][0] = character[5][0] = character[4][2] = character[5][2] = character[4][4] = character[5][4] = character[4][6] = character[5][6] = character[4][8] = character[5][8] = character[4][10] = character[5][10] = '|';
        character[3][2] = '\'';
        character[3][9] = '\\';
        character[3][5] = '`';
        return character;
    }

    vs n()
    {
        vs character = getCharGrid(6,7);

        character[2][1] = character[2][3] = character[2][4] = character[4][3] = character[4][5] = character[4][4] = character[3][3] = character[5][5] = character[5][1] = '_';
        character[2][0] = character[2][2] = character[2][5] = character[2][6] = character[3][1] = character[4][1] = character[4][3] = character[4][5] = ' ';
        character[3][0] = character[4][0] = character[4][2] = character[4][4] = character[4][6] = character[5][0] = character[5][2] = character[5][4] = character[5][6] = '|';
        character[3][2] = '\'';
        character[3][5] = '\\';
        return character;
    }

    vs o()
    {
        vs character = getCharGrid(6,7);
        character[2][2] = character[2][3] = character[2][4] = '_';
        character[3][3] = '_';
        character[3][1] = '/';
        character[3][5] = '\\';
        character[4][0] = character[4][6] = '|';
        character[4][2] = '(';
        character[4][4] = ')';
        character[4][3] = '_';
        character[5][1] = '\\';
        character[5][5] = '/';
        character[5][2] = character[5][3] = character[5][4] = '_';
        return character;
    }

    vs p()
    {
        vs character = getCharGrid(8,7);

        character[2][1] = character[2][4] = character[2][3]= character[7][1] = '_';
        character[3][3] = character[4][3] = character[5][3] = character[5][4] = '_';
        character[3][0] = character[4][0] = character[5][0] = character[6][0] = '|';
        character[7][0] = character[4][2] = character[6][2] = character[7][2] = character[4][6] = '|';
        character[3][2] = '\'';
        character[5][2] = '.';
        character[4][4] = ')';
        character[3][5] = '\\';
        character[5][5] = '/';

        return character;
    }

        vs q()
    {
        vs character = getCharGrid(8, 7);

        character[2][2] = character[2][5] = character[2][3] = character[7][5] = '_';
        character[3][3] = character[4][3] = character[5][3] = character[5][2] = '_';
        character[4][0] = character[4][4] = character[6][4] = character[7][6] = character[7][4]='|';
        character[3][6] = character[4][6] = character[5][6] = character[6][6] = '|';
        character[4][2] = '(';
        character[5][1] = '\\';
        character[3][1] = '/';
        character[5][4] = ',';
        character[3][4] = '`';

        return character;
    }



   vs r()
{
    vs character = getCharGrid(8, 5);


     character[2][3]=character[2][1] = character[5][1] =  character[3][3] = '_';
    character[3][0]=character[4][0] = character[5][0] =character[4][2] = character[5][2] = character[3][4] = '|';
  character[3][2]='\'';



    return character;
}

    vs s()
    {
        vs character = getCharGrid(6,7);

        character[2][3] = character[2][4] = character[2][5] = '_';
        character[3][2] = '/';
        character[3][4] = character[3][5] = '_';
        character[3][6] = '|';
        character[4][2] = '\\';
        character[4][3] = character[4][4] = '_';
        character[4][6] = '\\';
        character[5][2] = '|';
        character[5][3] = character[5][4] = character[5][5] = '_';
        character[5][6] = '/';
        return character;
    }

    vs t()
    {
        vs character = getCharGrid(6,5);
        character[0][1] = character[2][3] = character[3][2] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][4] = character[4][0] = character[4][2] = character[5][4] = '|';
        character[5][1] = '\\';
        return character;
    }

    vs u()
    {
        vs character = getCharGrid(7,7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = ' ';
        character[1][0] = character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = ' ';
        character[2][0] = character[2][2] = character[2][3] = character[2][6] = character[5][0] = ' ';
        character[3][1] = character[3][3] = character[3][5] = character[4][1] = character[4][5] = ' ';
        character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = ' ';
        character[3][0] = character[4][0] = character[3][2] = character[4][2] = character[3][4] = character[4][4] = character[3][6] = character[4][6] = character[5][6] = '|';
        character[2][1] = character[2][5] = character[4][3] = character[5][2] = character[5][3] = character[5][5] = '_';
        character[5][1] = '\\';
        character[5][4] = ',';
        return character;
    }

    vs v()
    {

        vs character = getCharGrid(6,7);
        character[2][0] = character[2][1] = character[2][5] = character[2][6] = character[5][3] = '_';
        character[3][0] = character[3][2] = character[4][1] = character[5][2] = '\\';
        character[3][4] = character[3][6] = character[4][5] = character[5][4] = '/';
        character[4][3] = 'V';
        return character;
    }


        vs w()
        {
            vs character = getCharGrid(6,10);

            //Enter the character grid in ROWS X COLS
            character[2][0] = character[2][1] = character[2][8] = character[2][9] = character[5][3] = character[5][6] = '_';
            character[3][0] = character[3][2] = character[3][5] = character[4][1] = character[5][2] = character[5][5] ='\\';
            character[3][4] = character[3][7] = character[3][9] = character[4][8] = character[5][4] = character[5][7] = '/';
            character[4][3] = character[4][6] = 'V';
            return character;
        }



    vs x()
    {
        vs character = getCharGrid(6,7);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = ' ';
        character[1][0] = character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = ' ';
        character[2][0] = character[2][1] = character[2][4] = character[2][5] = character[5][1] = character[5][4] = '_';
        character[3][1] = character[3][4] = character[4][0] = character[4][2] = character[4][3] = character[4][5] = ' ';
        character[3][0] = character[3][2] = character[5][3] = character[5][5] = '\\';
        character[3][3] = character[3][5] = character[5][0] = character[5][2] = '/';
        character[4][1] = '>';
        character[4][4] = '<';

        return character;
    }


        vs y()
        {
            vs character = getCharGrid(6,7);

        character[1][0]=character[1][2]=character[1][4]=character[1][6]=character[2][0]=character[2][2]=character[2][4]=character[2][6]=character[3][6]=character[4][6]=character[5][1]='|';
        character[3][1]='\\';
        character[0][1]=character[0][5]=character[2][3]=character[3][2]=character[3][3]=character[4][3]=character[4][2]=character[5][4]=character[5][3]=character[5][2]='_';
        character[4][4]=character[5][5]='/';
        character[3][4]=',';

            return character;
        }



    vs z()
    {
        vs character = getCharGrid(6,5);

        character[2][0] = character[3][2] = character[3][3] = character[4][0] = character[4][2] = ' ';
        character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[3][1] = character[5][1] = character[5][2] = character[5][3] = '_';
        character[3][0] = character[5][4] = '|';
        character[3][4] = character[4][1] = character[4][3] = character[5][0] = '/';
        return character;
    }

    // Numbers

    vs zero()
    {
        vs character = getCharGrid(6,7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][0] = character[1][6] = character[2][0] = character[2][2] = character[2][6] = character[3][0] = character[3][4] = character[3][6] = character[4][2] = '|';
        character[2][4] = '\'';
        character[4][0] = character[5][1] = '\\';
        character[2][3] = character[3][3] = character[4][4] = character[4][6] = character[5][5] = '/';

        return character;
    }

        vs one()
        {
            vs character = getCharGrid(6,7);

            character[0][1] = character[0][2] = character[0][3] =character[4][0]=character[4][5]=character[5][1]=character[5][2]=character[5][3]=character[5][4]='_';
            character[1][0]=character[5][5]='/';
            character[2][0]='`';
            character[2][1]=character[3][1]=character[4][1]='|';
            character[1][4]=character[2][4]=character[3][4]=character[4][4]='|';
            character[5][0]='\\';

            return character;
        }



    vs two()
    {
        vs character = getCharGrid(6,7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][3] = character[4][4] = character[4][5] = character[4][6] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[1][0] = character[2][3] = character[2][5] = character[3][2] = character[3][4] = character[4][1] = character[4][3] = character[5][6] = '/';
        character[1][6] = character[5][0] = '\\';
        character[4][0] = '.';
        character[2][0] = '`';
        character[2][1] = character[2][6] = '\'';

        return character;
    }

   vs three()
        {
            vs character = getCharGrid(6,7);

            character[0][0]=character[0][6]=character[1][5]=character[2][0]=character[2][1]=character[2][2]=character[2][3]=character[2][5]=character[3][0]=character[3][1]=character[3][2]=character[3][3]=character[3][5]=' ';
            character[0][1]=character[0][2]=character[0][3]=character[0][4]=character[0][5]=character[1][1]=character[1][2]=character[1][3]=character[1][4]=character[4][1]=character[4][2]=character[4][3]=character[5][1]=character[5][2]=character[5][3]=character[5][4]='_';
            character[1][0]=character[1][6]='|';
            character[2][4]=character[2][6]=character[4][4]=character[4][6]=character[5][5]='/';
            character[3][4]=character[3][6]=character[5][0]='\\';
            character[4][0]='.';
            return character;
        }


    vs four()
    {
        vs character = getCharGrid(6,7);
        character[0][3] = character[0][4] = character[0][5] = character[3][3] = character[4][1] = character[4][2] = character[4][3] = character[5][5] = '_';
        character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[2][4] = character[3][4] = character[5][4] = '|';
        character[1][2] = character[2][1] = character[2][3] = character[3][2] = character[3][0] = character[5][6] = '/';
        character[4][0] = '\\';
        return character;
    }

    vs five()
        {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3]
        = character[1][4] = character[1][5] = character[2][1] = character[2][2] = character[2][3] = character[4][2]
        = character[4][3] = character[5][1] = character[5][3] = character[5][4] = '_';
        character[1][0] = character[1][6] = character[2][0]  = '|';
        character[2][5] = character[3][4] = character[3][6] = character[4][1] = character[5][0] = '\\';
        character[4][0] = character[4][4] = character[4][6] = character[5][5] = '/';
        return character;
        }




    vs six()
    {
        vs character = getCharGrid(6, 7);
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[1][5] = character[2][3] = character[2][4] = character[2][5] = character[3][3] = character[3][4] = character[3][2] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[4][3] = '_';
        character[5][0] = '\\';
        character[5][6] = '/';
        character[4][2] = '\\';
        character[4][4] = '/';
        character[3][6] = '\\';
        character[2][0] = character[2][2] = '/';
        character[1][1] = '/';
        character[1][6] = character[3][0] = character[4][0] = character[4][6] = '|';

        return character;
    }

    vs seven()
    {
        vs character = getCharGrid(7,7);
        character[0][0] = character[1][4] = character[1][5] = character[2][6] = character[2][4] = character[2][2] = character[2][1] = character[2][0] = character[3][0] = character[3][1] = character[3][3] = character[3][5] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][5] = character[4][6] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[6][0] = character[6][1] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = ' ';

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][1] = character[1][2] = character[1][3] = character[5][1] = '_';

        character[1][0] = '|';
        character[4][0] = '.';
        character[5][0] = '\\';

        character[1][6] = character[2][3] = character[2][5] = character[3][2] = character[3][4] = character[4][1] = character[4][3] = character[5][2] = '/';
        character[1][0] = '|';
        character[4][0] = '.';
        character[5][0] = '\\';

        character[1][6] = character[2][3] = character[2][5] = character[3][2] = character[3][4] = character[4][1] = character[4][3] = character[5][2] = '/';

        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(6, 7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[3][3] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = '_';
        character[1][0] = character[1][6] = character[4][0] = character[4][2] = character[4][4] = character[4][6] = '|';
        character[2][1] = character[3][5] = character[5][0] = '\\';
        character[2][5] = character[3][1] = character[5][6] = '/';
        character[2][3] = 'V';

        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(6,7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3]
        = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[4][1]
        = character[4][2] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = character[5][4]
        = character[4][6] = '_';

        character[1][0] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][6] = '|';

        character[4][0] = '.';
        character[4][4] = character[4][6] = character[5][5] = '/';
        character[3][0] = character[5][0] = '\\';
        return character;
    }

     // for space
     vs space()
    {
        vs character = getCharGrid(1,1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef STRAIGHT_H
#define STRAIGHT_H

class Straight : public Fonts
{
    static const int ROWS = 3;
    static const int COLS = 4;

public:
    Straight() : Fonts(ROWS, COLS) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[1][1] = character[2][0] = '/';
        character[1][2] = character[2][3] = '\\';
        character[2][1] = character[2][2] = '-';

        return character;
    }

    vs B()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][3] = ' ';
        character[2][1] = character[2][2] = character[1][1] = character[1][2] = character[0][1] = character[0][2] = '_';
        character[2][0] = character[1][0] = '|';
        character[2][3] = character[1][3] = ')';

        return character;
    }

    vs C()
    {
        vs character = getCharGrid(3, 4);
        character[0][0] = character[1][1] = character[1][2] = character[1][3] = character[0][3] = character[2][3] = ' ';
        character[0][1] = character[0][2] = '_';
        character[1][0] = '/';
        character[2][0] = '\\';
        character[2][1] = character[2][2] = '_';
        return character;
    }

    vs D()
    {
        vs character = getCharGrid(3, 4);

        character[0][0] = character[0][3] = character[1][1] = character[1][2] = ' ';
        character[0][1] = character[0][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = character[2][0] = '|';
        character[1][3] = '\\';
        character[2][3] = '/';

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = '_';
        character[1][0] = character[2][0] = '|';
        character[1][1] = character[2][1] = character[2][2] = '_';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[2][0] = '|';
        character[0][1] = character[0][2] = character[1][1] = '_';

        return character;
    }

    vs G()
    {
        vs character = getCharGrid(3, 4);

        character[0][1] = character[0][2] = character[1][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = '/';
        character[2][0] = '\\';
        character[2][3] = ')';

        return character;
    }

    vs H()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[1][3] = '|';
        character[2][0] = character[2][3] = '|';
        character[1][1] = character[1][2] = '_';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid(3, 1);

        character[1][0] = character[2][0] = '|';

        return character;
    }

    vs J()
    {

        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[1][1] = ' ';
        character[2][0] = character[2][1] = '_';
        character[2][2] = ')';
        character[1][2] = '|';

        return character;
    }

    vs K()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS

        character[1][0] = character[2][0] = '|';
        character[1][1] = '_';
        character[1][2] = '/';
        character[2][2] = '\\';
        character[2][1] = ' ';

        return character;
    }

    vs L()
    {
        vs character = getCharGrid(3, 3);

        /*// Enter the character grid in ROWS X COLS*/
        character[1][0] = character[2][0] = '|';
        character[2][1] = character[2][2] = '_';

        return character;
    }

    vs M()
    {
        vs character = getCharGrid(2, 4);

        character[0][0] = character[1][0] = character[0][3] = character[1][3] = '|';
        character[0][1] = '\\';
        character[0][2] = '/';

        return character;
    }

    vs N()
    {
        vs character = getCharGrid(3, 4);

        character[1][0] = '|';
        character[1][1] = '\\';
        character[1][2] = ' ';
        character[1][3] = '|';
        character[2][0] = '|';
        character[2][1] = ' ';
        character[2][2] = '\\';
        character[2][3] = '|';

        return character;
    }

    vs O()
    {
        vs character = getCharGrid(3, 4);
        character[0][1] = character[0][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = character[2][3] = '/';
        character[2][0] = character[1][3] = '\\';
        return character;
    }

    vs P()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][0] = ' ', character[0][1] = character[0][2] = '_';
        character[1][0] = '|', character[1][1] = character[1][2] = '_', character[1][3] = ')';
        character[2][0] = '|', character[2][1] = character[2][2] = ' ';

        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(3, 4);

        character[0][1] = character[0][2] = character[2][1] = '_';
        character[1][0] = character[2][3] = '/';
        character[2][0] = character[1][3] = character[2][2] = '\\';

        return character;
    }

    vs R()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[1][1] = character[1][2] = '_';
        character[1][0] = character[2][0] = '|';
        character[2][2] = '\\';
        character[1][3] = ')';

        return character;
    }

    vs S()
    {
        vs character = getCharGrid(3, 3);
        character[1][1] = character[0][1] = character[0][2] = character[2][0] = character[2][1] = '_';
        character[1][0] = '(';
        character[2][2] = ')';

        return character;
    }

    vs T()
    {
        vs character = getCharGrid(3, 3);
        character[0][0] = character[0][1] = character[0][2] = '_';
        character[1][1] = character[2][1] = '|';

        return character;
    }

    vs U()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[2][3] = '/';
        character[1][3] = character[2][0] = '\\';
        character[2][1] = character[2][2] = '_';

        return character;
    }

    vs V()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[2][1] = '\\';
        character[1][3] = character[2][2] = '/';

        return character;
    }

    vs W()
    {
        vs character = getCharGrid(3, 4);

        character[1][0] = character[1][3] = character[2][0] = character[2][3] = '|';
        character[1][1] = character[1][2] = ' ';
        character[2][1] = '/';
        character[2][2] = '\\';

        return character;
    }

    vs X()
    {
        vs character = getCharGrid(3, 3);
        character[0][0] = character[2][2] = '\\';
        character[0][2] = character[2][0] = '/';
        character[1][1] = '-';
        return character;
    }

    vs Y()
    {

        vs character = getCharGrid(3, 3);
        character[0][0] = '\\';
        character[0][2] = '/';
        character[1][1] = '-';
        character[2][1] = '|';

        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(3, 3);

        character[0][0] = '_';
        character[0][1] = '_';
        character[0][2] = '_';
        character[1][2] = '/';
        character[1][1] = '_';
        character[1][0] = ' ';
        character[2][0] = '/';
        character[2][1] = '_';
        character[2][2] = '_';
        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(4, 4);

        // Enter the character grid in ROWS X COLS
        character[1][1] = '_';
        character[2][0] = '(';
        character[2][1] = '_';
        character[2][2] = '|';

        return character;
    }

    vs b()
    {
        vs character = getCharGrid(3, 3);

        character[1][0] = character[2][0] = '|';
        character[1][1] = character[2][1] = '_';
        character[2][2] = ')';

        return character;
    }

    vs c()
    {
        vs character = getCharGrid(3, 2);

        // Enter the character grid in ROWS X COLS
        character[1][0] = ' ';
        character[2][0] = '(';
        character[1][1] = character[2][1] = '_';
        return character;
    }

    vs d()
    {
        vs character = getCharGrid(4, 4);

        // Enter the character grid in ROWS X COLS
        character[1][2] = '|';
        character[1][1] = '_';
        character[2][0] = '(';
        character[2][2] = '|';
        character[2][1] = '_';

        return character;
    }

    vs e()
    {
        vs character = getCharGrid(ROWS, COLS);

        // Enter the character grid in ROWS X COLS
        character[1][1] = '_';
        character[2][0] = '(';
        character[2][1] = '-';
        return character;
    }

    vs f()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[0][1] = '_';
        character[1][0] = '(';
        character[1][1] = '_';
        character[2][0] = '|';

        return character;
    }

    vs g()
    {
        vs character = getCharGrid(4, 3);

        // Enter the character grid in ROWS X COLS
        character[1][1] = character[2][1] = character[3][0] = '_';
        character[2][2] = ')';
        character[2][0] = '(';
        character[3][1]='/';
        return character;

    }

    vs h()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[1][0] = '|';
        character[1][1] = '_';
        character[2][0] = '|';
        character[2][2] = ')';

        return character;
    }

    vs i()
    {
        vs character = getCharGrid(3, 1);

        character[1][0] = '.';
        character[2][0] = '|';

        return character;
    }

    vs j()
    {
        vs character = getCharGrid(4, 2);

        character[1][1] = '.';
        character[2][1] = '|';
        character[3][1] = '/';

        return character;
    }

    vs k()
    {
        vs character = getCharGrid(3, 2);

        character[1][0] = character[2][0] = '|';
        character[2][1] = '(';

        return character;
    }

    vs l()
    {
        vs character = getCharGrid(3, 1);

        // Enter the character grid in ROWS X COLS
        character[1][0] = '|';
        character[2][0] = '|';

        return character;
    }


        vs m()
        {
            vs character = getCharGrid(3,4);

            //Enter the character grid in ROWS X COLS
            character[1][1] = '_';
            character[2][0] = '|';
            character[2][2] = '|';
            character[1][2] = '_';
            character[2][3] = ')';

            return character;
        }



    vs n()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[1][1] = '_';
        character[2][0] = '|';
        character[2][2] = ')';

        return character;
    }

    vs o()
    {
        vs character = getCharGrid(3, 3);

        character[1][1] = character[2][1] = '_';
        character[2][0] = '(';
        character[2][2] = ')';

        return character;
    }

    vs p()
    {
        vs character = getCharGrid(3, 3);

        character[1][0] = character[2][0] = '|';
        character[0][1] = character[1][1] = '_';
        character[1][2] = ')';
        return character;
    }

    vs q()
    {
        vs character = getCharGrid(4, 4);
        character[1][2] = '_';
        character[2][1] = '(';
        character[2][2] = '_';
        character[2][3] = character[3][3] = '|';

        return character;
    }

    vs r()
    {
        // Enter the character grid in ROWS X COLS
        vs character = getCharGrid(2, 2);

        character[0][1] = '_';
        character[1][0] = '|';

        return character;
    }

    vs s()
    {
        vs character = getCharGrid(2, 2);

        character[0][1] = '_';
        character[1][0] = '_';
        character[1][1] = ')';

        return character;
    }

    vs t()
    {
        vs character = getCharGrid(3, 2);

        character[1][0] = character[2][0] = '|';
        character[1][1] = character[2][1] = '_';

        return character;
    }

    vs u()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[2][0] = '|';
        character[2][1] = '_';
        character[2][2] = '|';

        return character;
    }

    vs v()
    {
        vs character = getCharGrid(3, 2);

        // Enter the character grid in ROWS X COLS

        character[2][0] = '\\';
        character[2][1] = '/';
        return character;
    }

    vs w()
    {
        vs character = getCharGrid(3, 3);

        // Enter the character grid in ROWS X COLS
        character[2][0] = '\\';
        character[2][1] = ')';
        character[2][2] = '/';

        return character;
    }

    vs x()
    {
        vs character = getCharGrid(3, 2);

        // Enter the character grid in ROWS X COLS
        character[2][0] = ')';
        character[2][1] = '(';

        return character;
    }

    vs y()
    {
        vs character = getCharGrid(3, 2);

        // Enter the character grid in ROWS X COLS

        character[1][0] = '\\';
        character[1][1] = '/';
        character[2][0] = '/';

        return character;
    }

    vs z()
    {
        vs character = getCharGrid(4, 4);

        character[1][1] = '_';
        character[2][1] = '/';
        character[2][2] = '_';
        character[3][3] = ' ';
        return character;
    }

    // Numbers

    vs zero()
    {
        vs character = getCharGrid(3, 4);
        character[0][0] = character[0][3] = character[1][1] = character[1][2] = ' ';
        character[0][1] = character[0][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = character[2][3] = character[1][3] = character[2][0] = '|';

        return character;
    }

    vs one()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[1][2] = '/';
        character[1][3] = character[2][3] = '|';

        return character;
    }

    vs two()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[2][2] = character[2][3] = character[1][2] = '_';
        character[1][3] = ')';
        character[2][1] = '/';

        return character;
    }

    vs three()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[2][1] = character[2][2] = character[1][2] = '_';
        character[1][3] = character[2][3] = ')';
        // character[2][1] = '/';

        return character;
    }

    vs four()
    {
        vs character = getCharGrid(3, 4);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = ' ';
        character[1][1] = character[1][2] = '_';
        character[1][0] = character[1][3] = character[2][3] = '|';

        return character;
    }

    vs five()
    {
        vs character = getCharGrid(4, 4);

        // Enter the character grid in ROWS X COLS
        character[0][1] = '_';
        character[0][2] = '_';
        character[1][0] = '|';
        character[1][1] = '_';
        character[2][2] = ')';
        character[2][0] = character[2][1] = '_';

        return character;
    }

    vs six()
    {
        vs character = getCharGrid(3, 4);

        character[0][1] = character[0][2] = character[1][1] = character[1][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = '/';
        character[2][0] = '\\';
        character[2][3] = ')';

        return character;
    }

    vs seven()
    {
        vs character = getCharGrid(3, 4);

        // Enter the character grid in ROWS X COLS
        character[0][0] = '_';
        character[0][1] = '_';
        character[0][2] = '_';
        character[1][2] = '/';
        character[2][1] = '/';

        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(3, 4);

        character[0][1] = character[0][2] = character[1][1] = character[1][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = character[2][0] = '(';
        character[1][3] = character[2][3] = ')';

        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(3, 4);

        character[0][1] = character[0][2] = character[1][1] = character[1][2] = character[2][1] = character[2][2] = '_';
        character[1][0] = '(';
        character[2][3] = '/';
        character[1][3] = '\\';

        return character;
    }

    // for space
    vs space()
    {
        vs character = getCharGrid(1, 1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef STARWAR_H
#define STARWAR_H

class Starwar : public Fonts
{
    static const int rows = 6;
    static const int cols = 13;

public:
    Starwar() : Fonts(rows, cols) {}
    // Uppercase

    vs A()
    {
        vs character = getCharGrid(6,13);
        character[5][0] = character[4][1] = character[3][2] = character[2][3] = character[1][4] = character[5][3] = character[3][5] = '/';
        character[1][8] = character[2][9] = character[3][10] = character[4][11] = character[5][12] = character[3][7] = character[5][9] = '\\';
        character[0][5] = character[0][6] = character[0][7] = character[3][6] = character[4][4] = character[4][5] = character[4][6] = character[4][7] = character[4][8] = character[5][1] = character[5][2] = character[5][10] = character[5][11] = '_';
        character[2][6] = '^';
        return character;
    }

    vs B()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[0][0] = '.';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[1][4] = character[2][4] = character[3][4] = character[4][4] = '_';
        character[1][0] = character[5][0] = character[4][0] = character[3][0] = character[2][0] = character[2][3] = character[4][3] = character[2][8] = character[4][8] = '|';
        character[1][7] = '\\';
        character[5][7] = '/';
        character[3][7] = '<';
        character[2][5] = character[4][5] = ')';
        return character;
    }

    vs C()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[5][0] = character[0][0] = character[0][1] = ' ';
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[1][8] = character[2][0] = character[3][0] = character[4][0] = character[5][8] = character[3][3] = '|';
        character[1][1] = '/';
        character[5][1] = '\\';
        character[2][3] = ',';
        character[4][4] = character[4][5] = character[4][6] = character[4][7] = character[2][4] = character[2][5] = character[2][6] = character[2][7] = '-';
        character[4][8] = '.';
        character[4][3] = '`';
        character[2][8] = '\'';

        return character;
    }

    vs D()
    {
        vs character = getCharGrid(6,10);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[2][9] = character[3][9] = character[4][9] = character[3][3] = character[3][6] = '|';
        character[2][4] = character[2][5] = character[4][4] = character[4][5] = '-';
        character[2][3] = character[2][6] = '.';
        character[4][3] = character[4][6] = '\'';
        character[1][8] = '\\';
        character[5][8] = '/';

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(6, 9);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[1][4] = character[1][5] = character[1][6] = character[1][7] = '_';
        character[4][4] = character[4][5] = character[4][6] = character[4][7] = '_';
        character[2][4] = character[2][5] = '_';
        character[3][4] = character[3][5] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = '|';
        character[1][8] = character[5][8] =  '|';
        character[2][3] = character[4][3] =   '|';
        character[3][6] =   '|';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[2][4] = character[2][5] = character[3][4] = character[3][5] = character[5][1] = character[5][2] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[2][3] = character[4][3] = character[5][3] = character[1][8] = character[3][6] = '|';
        return character;
    }

    vs G()
    {
        vs character = getCharGrid(6, 10);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[2][6] = character[2][7] = character[3][6] = character[4][4] = character[4][5] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[1][1] = '/';
        character[5][1] = '\\';
        character[1][9] = character[2][0] = character[2][3] = character[3][0] = character[3][3] = character[3][5] = character[3][8] = character[4][0] = character[4][3] = character[4][6] = character[4][8] = character[5][8] = '|';

        return character;
    }

    vs H()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[1][3] = character[1][5] = character[1][8] = character[2][0] = character[2][3] = character[2][5] = character[2][8] = character[3][0] = character[3][8] = character[4][0] = character[4][3] = character[4][5] = character[4][8] = character[5][0] = character[5][3] = character[5][5] = character[5][8] = '|';
        character[0][1] = character[0][2] = character[0][6] = character[0][7] = character[2][4] = character[3][4] = character[5][1] = character[5][2] = character[5][6] = character[5][7] = '_';
        character[0][0] = character[0][4] = character[0][8] = character[1][1] = character[1][2] = character[1][6] = character[1][7] = character[2][1] = character[2][2] = character[2][6] = character[2][7] = character[3][1] = character[3][2] = character[3][6] = character[3][7] = character[4][1] = character[4][2] = character[4][6] = character[4][7] = character[1][4] = character[4][4] = character[5][4] = ' ';
        return character;
    }
    vs I()
    {
        vs character = getCharGrid(6,4);
        character[0][1] = character[5][1] = character[0][2] = character[5][2] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[1][3] = character[2][3] = character[3][3] = character[4][3] = character[5][3] = '|';
        return character;
    }

    vs J()
    {
        vs character = getCharGrid(6,10);

        // Enter the character grid in ROWS X COLS
        character[4][0] = character[3][6] = character[2][6] = character[1][6] = character[1][9] = character[2][9] = character[3][9] = character[4][9] = '|';
        character[0][7] = character[0][8] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[3][1] = character[3][2] = character[4][4] = character[4][5] = '-';
        character[3][0] = character[3][3] = '.';
        character[5][1] = '\\';
        character[5][8] = '/';
        character[4][3] = '`';
        character[4][6] = '\'';
        return character;
    }

    vs K()
    {
        vs character = getCharGrid(6, 8);

        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[1][3] = character[5][3] = '|';
        character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[0][7] = character[5][1] = character[5][2] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[2][3] = '\'';
        character[4][3] = '.';
        character[3][5] = '<';
        character[1][4] = character[1][7] = character[2][6] = '/';
        character[5][4] = character[5][7] = character[4][6] = '\\';

        return character;
    }

    vs L()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[2][1] = character[2][2] = character[2][4] = character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[3][1] = character[3][2] = character[3][4] = character[3][5] = character[3][6] = character[3][7] = character[3][8] = character[4][1] = character[4][2] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = character[5][8] = ' ';
        character[1][0] = character[1][3] = character[2][0] = character[2][3] = character[3][0] = character[3][3] = character[4][0] = character[5][0] = character[5][8] = '|';
        character[4][3] = '`';
        character[4][8] = '.';
        character[4][4] = character[4][5] = character[4][6] = character[4][7] = '-';
        character[0][1] = character[0][2] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';

        return character;
    }

    vs M()
    {
        vs character = getCharGrid(6,10);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[3][3] = character[4][3] = character[5][3] = character[1][9] = character[2][9] = character[3][9] = character[4][9] = character[5][9] = character[3][6] = character[4][6] = character[5][6] = '|';
        character[0][1] = character[0][2] = character[0][3] = character[0][6] = character[0][7] = character[0][8] = character[5][1] = character[5][2] = character[5][7] = character[5][8] = '_';
        character[1][4] = character[2][3] = character[3][4] = '\\';
        character[1][5] = character[3][5] = character[2][6] = '/';
        character[0][0] = character[0][9] = '.';

        return character;
    }

    vs N()
    {
        // Enter the character grid in ROWS X COLS
        vs character = getCharGrid(6,9);
        character[0][1] = character[0][2] = character[0][6] = character[0][7] = character[5][1] = character[5][2] = character[5][6] = character[5][7] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[4][3] = character[5][3] = character[1][5] = character[2][5] = character[1][8] = character[2][8] = character[3][8] = character[4][8] = character[5][8] = '|';
        character[1][3] = character[2][4] = character[4][4] = character[5][5] = '\\';
        character[0][0] = character[0][8] = character[3][3] = '.';
        character[3][5] = '`';

        return character;
    }

    vs O()
    {
        vs character = getCharGrid(6,10);

        // Enter the character grid in ROWS X COLS
        character[1][4] = character[1][5] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[2][0] = character[3][0] = character[4][0] = character[2][9] = character[3][9] = character[4][9] = character[2][3] = character[2][6] = character[3][3] = character[3][6] = '|';
        character[1][8] = character[5][1] = '\\';
        character[1][1] = character[5][8] = '/';
        character[4][4] = character[4][5] = '-';
        character[4][3] = '`';
        character[4][6] = '\'';

        return character;
    }

    vs P()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[2][4] = character[1][4] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[5][2] = character[3][5] = character[3][4] = character[3][6] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[2][8] = character[2][3] = character[4][3] = character[5][3] = '|';
        character[1][7] = '\\';
        character[0][0] = '.';
        character[3][7] = '/';
        character[2][5] = ')';

        return character;
    }


        vs Q()
        {
            vs character = getCharGrid(6,14);

            //Enter the character grid in ROWS X COLS
            character[1][4] = character[1][5] =  character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = character[5][8] = character[5][9] = character[5][10] = character[5][11] = character[5][12] =  '_';
            character[2][0] = character[3][0] = character[4][0] = character[2][3] = character[3][3] = character[2][6] = character[3][6] = character[2][9] = character[3][9] = '|';
            character[4][4] = character[4][5] = character[4][10] = character[4][11] = '-';
            character[1][8] = character[5][1] = character[5][7] = '\\';
            character[4][6] = character[4][9] = '\'';
            character[5][13] = '\\';
            character[4][12] = '.';
            character[4][3] = '`';
            character[1][1] = '/';

            return character;
        }




    vs R()
    {
        vs character = getCharGrid(6,13);

        // Enter the character grid in ROWS X COLS
        character[1][4] = character[2][4] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[5][2] = character[5][7] = character[5][8] = character[5][9] = character[5][10] = character[5][11] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[2][8] = character[2][3] = character[5][12] = character[5][3] = '|';
        character[4][8] = character[4][9] = character[4][10] = character[4][11] = '-';
        character[1][7] = character[4][4] = character[4][7] = '\\';
        character[0][0] = character[5][6] = character[4][12] = '.';
        character[3][7] = '/';
        character[2][5] = ')';
        character[5][5] = '`';

        return character;
    }


    vs S()
    {
        vs character = getCharGrid(6,13);

        //Enter the character grid in ROWS X COLS
        character[0][12] = character[4][0] = '.';
        character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[1][4] = character[5][8] = '/';
        character[1][12] = character[2][3] = character[4][9] = character[5][0] = '|';
        character[2][7] = '(';
        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = '-';
        character[2][12] = '`';
        character[3][4] = character[3][8] = '\\';
        character[4][5] = ')';



        return character;
    }


    vs T()
    {
        vs character = getCharGrid(6,13);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[2][1] = character[2][2] = character[2][3] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[5][5] = character[5][6] = '-';
        character[0][0] = character[0][12] = '.';
        character[2][0] = character[2][12] = '`';
        character[1][0] = character[1][12] = character[2][4] = character[2][7] = character[3][4] = character[3][7] = character[4][4] = character[4][7] = character[5][4] = character[5][7] = '|';
        return character;
    }

    vs U()
    {
        vs character = getCharGrid(6,10);

        // Enter the character grid in ROWS X COLS
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[1][3] = character[2][3] = character[3][3] = character[1][6] = character[2][6] = character[3][6] = character[1][9] = character[2][9] = character[3][9] = character[4][9] = '|';
        character[0][1] = character[0][2] = character[0][7] = character[0][8] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[5][7] = '_';
        character[4][4] = character[4][5] = '-';
        character[4][3] = '`';
        character[4][6] = '\'';
        character[5][1] = '\\';
        character[5][8] = '/';
        return character;
    }

    vs V()
    {
        vs character = getCharGrid(6,12);

        // Enter the character grid in ROWS X COLS

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[5][5] = character[5][6] = '_';
        character[1][0] = character[2][1] = character[3][2] = character[4][3] = character[5][4] = character[1][4] = character[2][5] = '\\';
        character[1][11] = character[2][10] = character[3][9] = character[4][8] = character[5][7] = character[1][7] = character[2][6] = '/';

        return character;
    }

     vs W()
        {
            vs character = getCharGrid(6,18);

            //Enter the character grid in ROWS X COLS
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][8] = character[0][9] = character[0][14] = character[0][15] = character[0][16] = character[0][17]= character[5][5]= character[5][6]= character[5][11]= character[5][12] = '_';
        character[1][0] = character[2][1] = character[3][2] = character[4][3] = character[5][4] = character[1][4] = character[2][5]= character[2][11]= character[1][10]= character[4][9]= character[5][10]= '\\';
        character[1][13] = character[2][12] = character[3][15] = character[4][14] = character[5][13] = character[1][7] = character[2][6]= character[4][8]= character[5][7]= character[1][17]= character[2][16] = '/';

            return character;
        }


    vs X()
    {
        vs character = getCharGrid(6,9);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][1] = character[0][2] = character[0][6] = character[0][7] = character[0][8] = character[5][1] = character[5][2] = character[5][6] = character[5][7] = '_';
        character[1][0] = character[1][3] = character[2][1] = character[5][5] = character[5][8] = character[4][7] = '\\';
        character[4][1] = character[5][0] = character[5][3] = character[1][5] = character[1][8] = character[2][7] = '/';
        character[2][4] = 'v';
        character[3][2] = '>';
        character[3][6] = '<';
        character[4][4] = '.';
        return character;
    }

    vs Y()
    {
        vs character = getCharGrid(6, 12);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][0] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[3][3] = character[3][8] = character[5][5] = character[5][6] = '_';
        character[1][0] = character[2][1] = character[3][2] = character[1][4] = character[2][5] = '\\';
        character[1][7] = character[2][6] = character[1][11] = character[2][10] = character[3][9] = '/';
        character[4][4] = character[5][4] = character[4][7] = character[5][7] = '|';

        return character;
    }

    vs Z()
        {
            vs character = getCharGrid(6,11);

            //Enter the character grid in ROWS X COLS
            character[0][4] = character[0][1] = character[0][2] = character[0][3] = character[0][8] = character[0][5] = character[0][6] = character[0][7] = character[5][6]= character[5][7]= character[5][8]= character[5][9] = character[5][2]= character[5][3]= character[5][4]= character[5][5] = '_';
        character[1][8] = character[2][7] = character[3][6] = character[4][5] = character[5][1] = character[4][2] = character[2][4]= character[3][3]='/';
        character[2][0] = '`';
        character[5][10] = character[1][0] ='|';
        character[4][10] = '.';
        character[2][1] = character[2][2] = character[2][3] = character[4][6] = character[4][7] = character[4][8] = character[4][9] = '-';

        return character;
        }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(6, 13);

        character[5][0] = character[4][1] = character[3][2] = character[2][3] = character[1][4] = character[5][3] = character[3][5] = '/';
        character[1][8] = character[2][9] = character[3][10] = character[4][11] = character[5][12] = character[3][7] = character[5][9] = '\\';
        character[0][5] = character[0][6] = character[0][7] = character[3][6] = character[4][4] = character[4][5] = character[4][6] = character[4][7] = character[4][8] = character[5][1] = character[5][2] = character[5][10] = character[5][11] = '_';
        character[2][6] = '^';

        return character;
    }

    /*
        vs b()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs c()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs d()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs e()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs f()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs g()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs h()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }
    */

    /*
        vs i()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs j()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs k()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs l()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs m()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs n()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs o()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs p()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs q()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs r()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs s()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs t()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs u()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs v()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs w()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs x()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs y()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    /*
        vs z()
        {
            vs character = getCharGrid(ROWS,COLS);

            //Enter the character grid in ROWS X COLS

            return character;
        }

    */

    // Numbers

    vs zero()
    {
        vs character = getCharGrid(6,7);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][1] = character[5][5] = '/';
        character[1][5] = character[5][1] = '\\';
        character[2][0] = character[3][0] = character[4][0] = character[2][6] = character[3][6] = character[4][6] = character[2][2] = character[3][2] = character[4][2] = character[2][4] = character[3][4] = character[4][4] = '|';

        return character;
    }

    vs one()
    {
        vs character = getCharGrid(6,4);
        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[5][2] = '_';
        character[1][0] = '/';
        character[1][1] = character[5][2] = '_';
        character[2][1] = character[3][1] = character[4][1] = character[5][1] = character[1][3] = character[2][3] = character[3][3] = character[4][3] = character[5][3] = '|';

        return character;
    }

    vs two()
    {
        vs character = getCharGrid(6,6);
        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[4][4] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][4] = '\\';
        character[3][2] = character[3][4] = character[4][1] = character[4][3] = '/';
        character[2][3] = ')';
        character[1][0] = character[2][5] = character[5][0] = character[5][5] = '|';
        return character;
    }


        vs three()
        {
            vs character = getCharGrid(6,7);

            //Enter the character grid in ROWS X COLS
            character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][2] = character[1][3] = character[2][2] = character[2][3] = character[3][2] = character[3][3] = character[4][1] = character[4][2] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = '_';
            character[1][0] = character[3][1] = character[5][0] = character[2][6] = character[4][6] = '|';
            character[2][4] = character[4][4] = ')';
            character[3][5] = '<';
            character[1][5] = '\\';
            character[5][5] = '/';

            return character;
        }



    vs four()
    {
        vs character = getCharGrid(6,8);

        character[0][1] = character[0][4] = character[3][1] = character[3][2] = character[3][6] = character[5][4] = character[2][6] = '_';
        character[1][0] = character[1][2] = character[1][3] = character[1][5] = character[2][0] = character[2][2] = character[2][3] = character[2][5] = character[3][0] = character[3][7] = character[4][3] = character[4][5] = character[5][3] = character[5][5] = '|';

        return character;
    }

    vs five()
    {
        vs character = getCharGrid(6,7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[4][1] = character[4][2] = character[4][3] = character[5][1] = character[5][2] = character[5][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[5][0] = character[1][6] = character[2][2] = character[4][6] = '|';
        character[4][4] = ')';
        character[3][5] = '\\';
        character[5][5] = '/';
        return character;
    }

    vs six()
    {
        vs character = getCharGrid(6, 7);

        // Enter the character grid in ROWS X COLS
        character[0][3] = character[0][4] = character[2][4] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][2] = character[1][4] = character[2][1] = character[2][3] = character[5][5] = '/';
        character[3][0] = character[4][0] = character[4][6] = '|';
        character[3][5] = character[5][1] = '\\';
        character[4][2] = '(';
        character[4][4] = ')';
        character[3][2] = '.';
        return character;
    }

    vs seven()
    {
        vs character = getCharGrid(6, 8);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[5][2] = '_';
        character[1][0] = character[1][7] = '|';
        character[2][4] = character[2][6] = character[3][3] = character[3][5] = character[4][2] = character[4][4] = character[5][1] = character[5][3] = '/';

        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(6,7);
        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][3] = character[3][3] = character[4][3] = character[5][2] = character[5][3] = character[5][4] = '_';
        character[1][1] = character[5][5] = '/';
        character[5][1] = character[1][5] = '\\';
        character[2][2] = character[4][2] = '(';
        character[2][4] = character[4][4] = ')';
        character[3][1] = '>';
        character[3][5] = '<';
        character[2][0] = character[2][6] = character[4][0] = character[4][6] = '|';
        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(6,7);
        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][3] = character[3][2] = character[3][3] = character[5][3] = '_';
        character[1][1] = character[4][3] = character[4][5] = character[5][2] = character[5][4] = '/';
        character[1][5] = character[3][1] = '\\';
        character[2][2] = '(';
        character[2][4] = ')';
        character[3][4] = ',';
        character[2][0] = character[2][6] = character[3][6] = '|';
        return character;
    }

     // for space
     vs space()
    {
        vs character = getCharGrid(1,1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef CARLOS_H
#define CARLOS_H

class Carlos : public Fonts
{
    static const int rows = 7;
    static const int cols = 14;

public:
    Carlos() : Fonts(rows, cols) {}

    vs A()
    {
        vs character = getCharGrid(7, 10);

        character[0][3] = character[0][4] = character[0][5] = character[0][6] = '_';
        character[1][2] = '(';
        character[1][7] = ')';
        character[2][0] = character[2][1] = character[2][3] = character[2][6] = character[2][8] = character[2][9] = ' ';
        character[2][2] = character[2][4] = '/';
        character[2][5] = character[2][7] = '\\';
        character[3][1] = character[3][3] = '(';
        character[3][4] = character[3][5] = '_';
        character[3][6] = character[3][8] = ')';
        character[4][2] = ')';
        character[4][7] = '(';
        character[5][1] = character[5][4] = '/';
        character[5][5] = character[5][8] = '\\';
        character[6][0] = '/';
        character[6][1] = character[6][2] = character[6][7] = character[6][8] = '_';
        character[6][3] = '(';
        character[6][6] = ')';
        character[6][9] = '\\';

        return character;
    }

    vs B()
    {
        vs character = getCharGrid(7, 9);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[2][5] = character[4][5] = character[5][5] = '_';
        character[0][5] = character[0][6] = character[1][1] = character[1][5] = character[3][6] = character[5][1] = '_';
        character[1][0] = character[2][4] = character[5][4] = character[6][0] = '(';
        character[1][7] = character[3][2] = character[4][7] = '\\';
        character[2][2] = character[2][6] = character[2][8] = character[5][2] = character[5][6] = character[5][8] = ')';
        character[3][7] = character[4][2] = character[6][7] = '/';
        character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        return character;
    }

    vs C()
    {
        vs character = getCharGrid(7, 8);

        character[0][0] = character[0][1] = character[0][2] = character[1][0] = character[1][1] = character[1][3] = character[2][0] = character[2][2] = character[3][1] = character[4][1] = character[5][0] = character[5][2] = character[6][0] = character[6][1] = ' ';
        character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][4] = character[1][5] = character[1][6] = character[5][4] = character[5][5] = character[5][6] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        character[1][2] = character[2][1] = character[2][3] = '/';
        character[5][1] = character[5][3] = character[6][2] = '\\';
        character[3][0] = character[3][2] = character[4][0] = character[4][2] = '(';
        character[1][7] = character[6][7] = ')';

        return character;
    }

    vs D()
    {
        vs character = getCharGrid(7, 10);

        character[0][1] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';

        character[1][0] = '(';
        character[1][1] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][2] = ')';
        character[2][4] = ')';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][1] = '(';
        character[3][3] = '(';
        character[3][7] = ')';
        character[3][9] = ')';

        character[4][2] = ')';
        character[4][4] = ')';
        character[4][7] = ')';
        character[4][9] = ')';

        character[5][1] = '/';
        character[5][3] = '/';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';

        character[6][0] = '(';
        character[6][1] = '_';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(7, 8);
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';

        character[1][1] = '/';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '/';

        character[2][0] = '(';
        character[2][2] = '(';
        character[2][3] = '_';
        character[2][4] = '_';

        character[3][1] = ')';
        character[3][3] = '_';
        character[3][4] = '_';
        character[3][5] = ')';

        character[4][0] = '(';
        character[4][2] = '(';

        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '_';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '\\';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(7, 11);
        character[0][1] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';
        character[0][8] = '_';
        character[0][9] = '_';

        character[1][0] = '(';
        character[1][1] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][7] = '_';
        character[1][8] = '_';
        character[1][9] = '_';
        character[1][10] = ')';

        character[2][2] = ')';
        character[2][4] = '(';
        character[2][5] = '_';
        character[2][6] = '_';
        character[2][7] = '_';

        character[3][1] = '(';
        character[3][5] = '_';
        character[3][6] = '_';
        character[3][7] = '_';
        character[3][8] = ')';

        character[4][2] = ')';
        character[4][4] = '(';

        character[5][1] = '(';
        character[5][5] = ')';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '/';

        return character;
    }

    vs G()
    {
        vs character = getCharGrid(7, 10);
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';

        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][8] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][7] = '\\';
        character[2][8] = '_';
        character[2][9] = ')';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][5] = '_';
        character[3][6] = '_';
        character[3][7] = '_';
        character[3][8] = '_';

        character[4][0] = '(';
        character[4][2] = '(';
        character[4][4] = '(';
        character[4][5] = '_';
        character[4][6] = '_';
        character[4][9] = ')';

        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';
        return character;
    }

    vs H()
    {
        vs character = getCharGrid(7, 10);

        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][8] = character[2][0] = character[2][2] = character[2][7] = character[3][0] = character[3][1] = character[3][3] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][5] = character[4][7] = character[5][0] = character[5][1] = character[5][3] = character[5][6] = character[6][0] = character[6][4] = character[6][5] = ' ';
        character[0][1] = character[0][2] = character[0][7] = character[0][8] = character[2][4] = character[2][5] = character[3][4] = character[3][5] = character[6][2] = character[6][7] = '_';
        character[1][0] = character[2][3] = character[3][7] = character[4][1] = character[4][3] = character[5][5] = character[5][7] = '(';
        character[1][3] = character[2][1] = character[6][6] = character[6][8] = '\\';
        character[1][6] = character[2][8] = character[6][1] = character[6][3] = '/';
        character[1][9] = character[2][6] = character[3][2] = character[4][6] = character[4][8] = character[5][2] = character[5][4] = ')';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid(7, 8);

        // Enter the character grid in ROWS X COL
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][2] = character[1][6] = character[5][2] = character[5][6] = character[5][7] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        character[2][3] = character[2][5] = character[3][3] = character[3][5] = character[4][3] = character[4][5] = character[5][3] = character[5][5] = '|';
        character[1][1] = character[6][7] = '(';
        character[1][7] = ')';
        character[6][1] = '/';

        return character;
    }

    vs J()
    {
        vs character = getCharGrid(7, 11);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[1][2] = character[1][3] = character[1][4] = character[1][7] = character[1][8] = character[1][9] = character[4][1] = character[4][2] = character[5][3] = character[6][2] = character[6][3] = character[6][4] = '_';
        character[1][1] = character[3][4] = character[3][6] = character[5][0] = character[5][2] = '(';
        character[1][10] = character[2][5] = character[2][7] = character[4][5] = character[4][7] = ')';
        character[5][4] = character[5][6] = character[6][5] = '/';
        character[6][1] = '\\';

        return character;
    }

    vs K()
    {
        vs character = getCharGrid(7, 10);

        character[0][1] = character[0][2] = character[0][6] = character[0][7] = character[0][8] = character[1][7] = character[1][8] = character[2][3] = character[6][2] = character[6][7] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[2][2] = character[3][5] = character[5][2] = '(';
        character[1][1] = character[3][1] = character[4][1] = character[6][1] = character[6][3] = character[1][3] = character[1][9] = ')';
        character[4][3] = character[2][4] = character[1][5] = character[2][6] = '/';
        character[4][4] = character[5][5] = character[6][6] = character[4][6] = character[5][7] = character[6][8] = '\\';

        return character;
    }

    vs L()
    {
        vs character = getCharGrid(7, 11);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][1] = character[1][5] = character[5][0] = character[5][1] = character[5][5] = character[5][6] = character[5][7] = character[4][8] = character[4][9] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = character[6][7] = character[6][8] = '_';
        character[6][0] = '\\';
        character[6][9] = '/';
        character[2][2] = character[2][4] = character[3][2] = character[3][4] = character[4][2] = character[4][4] = character[5][2] = character[5][4] = '|';
        character[1][0] = '(';
        character[1][6] = character[5][8] = character[5][10] = ')';
        return character;
    }


        vs M()
        {
            vs character = getCharGrid(7,14);

            //Enter the character grid in ROWS X COL
            character[6][1] = character[5][1] = character[4][2] = character[3][3] = character[5][3] = character[4][4] = character[2][7] = character[4][7] = character[1][8] = character[1][10] = '/';
            character[1][3] = character[1][5] = character[2][6] = character[4][6] = character[3][10] = character[4][9] = character[5][10] = character[4][11] = character[5][12] = character[6][12] = '\\';
            character[0][3] = character[0][4] = character[0][9] = character[0][10] = character[3][5] = character[3][8] = character[5][2] = character[5][11] = '_';
            character[2][4] = character[2][10] = character[6][13] = ')';
            character[2][3] = character[2][9] = character[6][0] = '(';
            return character;
        }


    vs N()
    {
        vs character = getCharGrid(7, 14);

        // Enter the character grid in ROWS X COL
        character[3][2] = character[3][4] = character[3][6] = character[3][8] = character[3][10] = character[3][12] = character[1][13] = ')';
        character[6][0] = character[4][1] = character[4][3] = character[4][5] = character[4][7] = character[4][9] = character[4][11] = '(';
        character[1][3] = character[2][2] = character[2][4] = character[5][1] = character[5][3] = character[6][2] = character[1][11] = character[2][10] = character[2][12] = character[5][9] = character[5][11] = character[6][10] = '/';
        character[1][6] = character[2][5] = character[2][7] = character[5][6] = character[5][8] = character[6][7] = '\\';
        character[0][4] = character[0][5] = character[0][12] = character[6][1] = character[6][8] = character[6][9] = '_';

        return character;
    }

    vs O()
    {
        vs character = getCharGrid(8, 11);

        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][3] = ')';
        character[3][6] = '(';
        character[3][7] = ')';
        character[3][9] = ')';
        character[4][0] = '(';
        character[4][2] = '(';
        character[4][3] = ')';
        character[4][6] = '(';
        character[4][7] = ')';
        character[4][9] = ')';
        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';
        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';

        return character;
    }


        vs P()
        {
            vs character = getCharGrid(9,9);

           character[0][1]=character[0][2]=character[0][3]=character[0][4]=character[0][5]=character[1][3]=character[1][4]=character[2][4]=character[3][4]=character[3][5]=character[6][1]=character[6][2]='_';
           character[1][0]=character[3][0]=character[5][0]=character[5][2]='(';
           character[1][6]=character[6][3]='\\';
           character[2][1]=character[4][1]=character[4][3]=character[2][3]=character[2][5]=character[2][7]=')';
           character[3][6]=character[6][0]='/';
            return character;
        }


    vs Q()
    {
        vs character = getCharGrid(8, 11);

        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][7] = ')';
        character[3][9] = ')';
        character[4][0] = '(';
        character[4][2] = '(';
        character[4][5] = '/';
        character[4][6] = '\\';
        character[4][7] = ')';
        character[4][9] = ')';
        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '\\';
        character[5][7] = '\\';
        character[5][8] = '/';
        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '\\';
        character[6][8] = '\\';
        character[6][9] = '_';

        character[7][10] = ')';
        character[7][9] = character[7][8] = '_';
        character[7][7] = '\\';

        return character;
    }

    vs R()
    {
        vs character = getCharGrid(7, 10);

        character[1][0] = character[3][0] = character[5][0] = character[2][3] = character[5][2] = '(';
        character[2][1] = character[4][1] = character[6][1] = character[6][3] = character[5][8] = character[5][9] = character[2][6] = character[2][8] = ')';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = '_';
        character[1][4] = character[1][5] = character[2][4] = character[2][5] = character[3][5] = character[3][6] = character[4][8] = character[5][7] = character[6][2] = character[6][6] = character[6][7] = '_';
        character[1][7] = character[4][3] = character[5][4] = character[6][5] = character[4][5] = character[5][6] = '\\';
        character[3][7] = character[6][8] = '/';

        return character;
    }

    vs S()
    {
        vs character = getCharGrid(7, 9);

        // Enter the character grid in ROWS X COL
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = ' ';
        character[1][1] = '/';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][7] = '\\';
        character[2][0] = '(';
        character[2][1] = ' ';
        character[2][2] = '(';
        character[2][3] = '_';
        character[2][4] = '_';
        character[2][5] = '_';
        character[3][1] = '\\';
        character[3][2] = '_';
        character[3][3] = '_';
        character[3][4] = '_';
        character[3][5] = '_';
        character[3][7] = '\\';
        character[4][6] = ')';
        character[4][8] = ')';
        character[5][4] = '_';
        character[5][3] = '_';
        character[5][2] = '_';
        character[5][5] = '/';
        character[5][7] = '/';
        character[6][1] = '/';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '/';

        return character;
    }


        vs T()
        {
            vs character = getCharGrid(7,10);

            //Enter the character grid in ROWS X COL
            character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[1][3] = character[0][4] = character[0][5] = character[6][4] = character[6][5] = character[0][6] = character[0][7] = character[0][8] = character[1][6] = character[1][7] = character[1][8] = '_';
            character[1][0] = character[3][3] = character[5][3] = character[3][5] = character[5][5] = '(';
            character[1][9] = character[2][4] = character[4][4] = character[2][6] = character[4][6] = ')';
            character[6][3] = '/';
            character[6][6] = '\\';

            return character;
        }


     vs U()
    {
        vs character = getCharGrid(7, 11);
        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][9] = character[0][10] = character[1][0] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][9] = character[1][10] = character[2][1] = character[2][3] = character[2][4] = character[2][5] = character[2][6] = character[2][8] = character[2][10] = character[3][0] = character[3][2] = character[3][4] = character[3][5] = character[3][7] = character[3][9] = character[3][10] = character[4][1] = character[4][3] = character[4][4] = character[4][5] = character[4][6] = character[4][8] = character[4][10] = character[5][0] = character[5][2] = character[5][7] = character[5][9] = character[5][10] = character[6][0] = character[6][9] = character[6][10] = ' ';
character[0][1] = character[0][2] = character[0][7] = character[0][8] = character[5][4] = character[5][5] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = character[6][7] = '_';
character[1][1] = character[1][3] = character[2][7] = character[2][9] = character[3][1] = character[3][3] = character[4][7] = character[4][9] = character[5][1] = ')';
character[1][6] = character[1][8] = character[2][0] = character[2][2] = character[3][6] = character[3][8] = character[4][0] = character[4][2] = character[5][8] = '(';
character[5][3] = character[6][1] = '\\';
character[5][6] = character[6][8] = '/';
        return character;
    }


        vs V()
        {
            vs character = getCharGrid(7,10);
                character[3][1] = character[3][3] = character[4][2] = character[4][4] = character[5][3] = character[6][4] ='\\';
                character[3][8] = character[3][6] = character[4][7] = character[4][5] = character[5][6] = character[6][5] ='/';
                character[0][1] = character[0][2] = character[0][7] = character[0][8] = '_';
                character[2][0] = character[2][2] = character[1][8] = character[1][6] = '(';
                character[1][1] = character[1][3] = character[2][7] = character[2][9] = ')';
                character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][6] = character[6][7] =' ';
                character[6][8] = character[6][9] = character[5][0] = character[5][1] = character[5][2] = character[5][4] =' ';
                character[5][5] = character[5][7] = character[5][8] = character[5][9] = character[4][0] = character[4][1] =' ';
                character[4][3] = character[4][6] = character[4][8] = character[4][9] = character[3][0] = character[3][2] =' ';
                character[3][4] = character[3][5] = character[3][7] = character[3][9] = character[2][1] = character[2][3] =' ';
                character[2][4] = character[2][5] = character[2][6] = character[2][8] = character[1][0] = character[1][2] =' ';
                character[1][4] = character[1][5] = character[1][7] = character[1][9] = character[0][0] = character[0][3] =' ';
                character[0][4] = character[0][5] = character[0][6] = character[0][9] = ' ';
            return character;
        }


    vs W()
    {
        vs character = getCharGrid(7, 15);
        character[0][1] = character[0][2] = character[0][3] = character[0][11] = character[0][12] = character[0][13] = character[6][5] = character[6][9] = character[4][7] = character[2][7] = '_';
        character[4][11] = character[1][0] = character[1][3] = character[5][6] = '(';
        character[1][11] = character[1][14] = character[5][8] = character[4][3] = ')';
        character[2][1] = character[2][4] = character[3][2] = character[3][5] = character[5][3] = character[6][4] = character[3][8] = character[6][8] = '\\';
        character[2][10] = character[2][13] = character[3][9] = character[3][12] = character[5][11] = character[6][10] = character[3][6] = character[6][6] = '/';

        return character;
    }

    vs X()
    {
        vs character = getCharGrid(7, 11);

        // Enter the character grid in ROWS X COL
        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][8] = character[2][0] = character[2][2] = character[2][7] = character[3][0] = character[3][1] = character[3][3] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][5] = character[4][7] = character[5][0] = character[5][1] = character[5][3] = character[5][6] = character[6][0] = character[6][4] = character[6][5] = ' ';
        character[0][1] = character[0][2] = character[0][9] = character[0][8] = character[1][1] = character[1][9] = character[2][5] = character[4][5] = character[5][1] = character[5][9] = character[6][1] = character[6][2] = character[6][8] = character[6][9] = '_';
        character[1][0] = character[6][0] = '(';
        character[1][3] = character[2][2] = character[2][4] = character[3][3] = character[4][7] = character[5][6] = character[5][8] = character[6][7] = '\\';
        character[1][7] = character[2][8] = character[2][6] = character[3][7] = character[4][3] = character[5][2] = character[5][4] = character[6][3] = '/';
        character[1][10] = character[6][10] = ')';

        return character;
    }

    vs Y()
    {
        vs character = getCharGrid(7, 10);

        character[0][0] = character[0][1] = character[0][9] = character[0][8] = character[6][4] = character[6][5] = '_';
        character[1][9] = character[5][5] = '(';
        character[1][0] = character[5][4] = ')';
        character[1][2] = character[2][1] = character[2][3] = character[3][2] = character[3][4] = character[4][3] = character[6][6] = '\\';
        character[1][7] = character[2][8] = character[2][6] = character[3][7] = character[3][5] = character[4][6] = character[6][3] = '/';

        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(7, 10);
        character[0][8] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';

        character[1][1] = '(';
        character[1][2] = '_';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        // character[1][6] = '_';
        character[1][7] = ' ';
        character[1][8] = ' ';
        // character[3][9] = ' ';
        character[1][9] = ')';

        character[2][5] = '/';
        character[2][8] = '/';

        character[3][1] = '_';
        character[3][2] = '_';
        character[3][3] = '_';
        character[3][4] = '/';
        character[3][5] = ' ';
        character[3][6] = ' ';
        character[3][7] = '/';
        character[3][8] = '_';

        character[4][0] = '/';
        character[4][1] = '_';
        character[4][2] = '_';
        character[4][3] = ' ';
        character[4][4] = ' ';
        character[4][5] = ' ';
        character[4][6] = '_';
        character[4][7] = '_';
        character[4][8] = '_';
        character[4][9] = ')';

        character[5][2] = '/';
        character[5][4] = '/';
        character[5][5] = '_';
        character[5][6] = '_';
        character[5][7] = '_';
        character[5][8] = '_';
        character[3][9] = ' ';

        character[6][1] = '(';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '_';
        character[6][8] = '_';
        character[6][9] = ')';

        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(7, 10);

        character[0][3] = character[0][4] = character[0][5] = character[0][6] = '_';
        character[1][2] = '(';
        character[1][7] = ')';
        character[2][0] = character[2][1] = character[2][3] = character[2][6] = character[2][8] = character[2][9] = ' ';
        character[2][2] = character[2][4] = '/';
        character[2][5] = character[2][7] = '\\';
        character[3][1] = character[3][3] = '(';
        character[3][4] = character[3][5] = '_';
        character[3][6] = character[3][8] = ')';
        character[4][2] = ')';
        character[4][7] = '(';
        character[5][1] = character[5][4] = '/';
        character[5][5] = character[5][8] = '\\';
        character[6][0] = '/';
        character[6][1] = character[6][2] = character[6][7] = character[6][8] = '_';
        character[6][3] = '(';
        character[6][6] = ')';
        character[6][9] = '\\';

        return character;
    }

     vs b()
    {
        vs character = getCharGrid(7, 9);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[2][5] = character[4][5] = character[5][5] = '_';
        character[0][5] = character[0][6] = character[1][1] = character[1][5] = character[3][6] = character[5][1] = '_';
        character[1][0] = character[2][4] = character[5][4] = character[6][0] = '(';
        character[1][7] = character[3][2] = character[4][7] = '\\';
        character[2][2] = character[2][6] = character[2][8] = character[5][2] = character[5][6] = character[5][8] = ')';
        character[3][7] = character[4][2] = character[6][7] = '/';
        character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        return character;
    }

    vs c()
    {
        vs character = getCharGrid(7, 8);

        character[0][0] = character[0][1] = character[0][2] = character[1][0] = character[1][1] = character[1][3] = character[2][0] = character[2][2] = character[3][1] = character[4][1] = character[5][0] = character[5][2] = character[6][0] = character[6][1] = ' ';
        character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][4] = character[1][5] = character[1][6] = character[5][4] = character[5][5] = character[5][6] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        character[1][2] = character[2][1] = character[2][3] = '/';
        character[5][1] = character[5][3] = character[6][2] = '\\';
        character[3][0] = character[3][2] = character[4][0] = character[4][2] = '(';
        character[1][7] = character[6][7] = ')';

        return character;
    }

    vs d()
    {
        vs character = getCharGrid(7, 10);

        character[0][1] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';

        character[1][0] = '(';
        character[1][1] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][2] = ')';
        character[2][4] = ')';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][1] = '(';
        character[3][3] = '(';
        character[3][7] = ')';
        character[3][9] = ')';

        character[4][2] = ')';
        character[4][4] = ')';
        character[4][7] = ')';
        character[4][9] = ')';

        character[5][1] = '/';
        character[5][3] = '/';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';

        character[6][0] = '(';
        character[6][1] = '_';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';

        return character;
    }

    vs e()
    {
        vs character = getCharGrid(7, 8);
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';

        character[1][1] = '/';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '/';

        character[2][0] = '(';
        character[2][2] = '(';
        character[2][3] = '_';
        character[2][4] = '_';

        character[3][1] = ')';
        character[3][3] = '_';
        character[3][4] = '_';
        character[3][5] = ')';

        character[4][0] = '(';
        character[4][2] = '(';

        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '_';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '\\';

        return character;
    }

    vs f()
    {
        vs character = getCharGrid(7, 11);
        character[0][1] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';
        character[0][8] = '_';
        character[0][9] = '_';

        character[1][0] = '(';
        character[1][1] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][7] = '_';
        character[1][8] = '_';
        character[1][9] = '_';
        character[1][10] = ')';

        character[2][2] = ')';
        character[2][4] = '(';
        character[2][5] = '_';
        character[2][6] = '_';
        character[2][7] = '_';

        character[3][1] = '(';
        character[3][5] = '_';
        character[3][6] = '_';
        character[3][7] = '_';
        character[3][8] = ')';

        character[4][2] = ')';
        character[4][4] = '(';

        character[5][1] = '(';
        character[5][5] = ')';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '/';

        return character;
    }

    vs g()
    {
        vs character = getCharGrid(7, 10);
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';

        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][8] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][7] = '\\';
        character[2][8] = '_';
        character[2][9] = ')';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][5] = '_';
        character[3][6] = '_';
        character[3][7] = '_';
        character[3][8] = '_';

        character[4][0] = '(';
        character[4][2] = '(';
        character[4][4] = '(';
        character[4][5] = '_';
        character[4][6] = '_';
        character[4][9] = ')';

        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';

        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';
        return character;
    }

    vs h()
    {
        vs character = getCharGrid(7, 10);

        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][8] = character[2][0] = character[2][2] = character[2][7] = character[3][0] = character[3][1] = character[3][3] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][5] = character[4][7] = character[5][0] = character[5][1] = character[5][3] = character[5][6] = character[6][0] = character[6][4] = character[6][5] = ' ';
        character[0][1] = character[0][2] = character[0][7] = character[0][8] = character[2][4] = character[2][5] = character[3][4] = character[3][5] = character[6][2] = character[6][7] = '_';
        character[1][0] = character[2][3] = character[3][7] = character[4][1] = character[4][3] = character[5][5] = character[5][7] = '(';
        character[1][3] = character[2][1] = character[6][6] = character[6][8] = '\\';
        character[1][6] = character[2][8] = character[6][1] = character[6][3] = '/';
        character[1][9] = character[2][6] = character[3][2] = character[4][6] = character[4][8] = character[5][2] = character[5][4] = ')';

        return character;
    }

    vs i()
    {
        vs character = getCharGrid(7, 8);

        // Enter the character grid in ROWS X COL
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][2] = character[1][6] = character[5][2] = character[5][6] = character[5][7] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '_';
        character[2][3] = character[2][5] = character[3][3] = character[3][5] = character[4][3] = character[4][5] = character[5][3] = character[5][5] = '|';
        character[1][1] = character[6][7] = '(';
        character[1][7] = ')';
        character[6][1] = '/';

        return character;
    }

    vs j()
    {
        vs character = getCharGrid(7, 11);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[1][2] = character[1][3] = character[1][4] = character[1][7] = character[1][8] = character[1][9] = character[4][1] = character[4][2] = character[5][3] = character[6][2] = character[6][3] = character[6][4] = '_';
        character[1][1] = character[3][4] = character[3][6] = character[5][0] = character[5][2] = '(';
        character[1][10] = character[2][5] = character[2][7] = character[4][5] = character[4][7] = ')';
        character[5][4] = character[5][6] = character[6][5] = '/';
        character[6][1] = '\\';

        return character;
    }

    vs k()
    {
        vs character = getCharGrid(7, 10);

        character[0][1] = character[0][2] = character[0][6] = character[0][7] = character[0][8] = character[1][7] = character[1][8] = character[2][3] = character[6][2] = character[6][7] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[2][2] = character[3][5] = character[5][2] = '(';
        character[1][1] = character[3][1] = character[4][1] = character[6][1] = character[6][3] = character[1][3] = character[1][9] = ')';
        character[4][3] = character[2][4] = character[1][5] = character[2][6] = '/';
        character[4][4] = character[5][5] = character[6][6] = character[4][6] = character[5][7] = character[6][8] = '\\';

        return character;
    }

    vs l()
    {
        vs character = getCharGrid(7, 11);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][1] = character[1][5] = character[5][0] = character[5][1] = character[5][5] = character[5][6] = character[5][7] = character[4][8] = character[4][9] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = character[6][7] = character[6][8] = '_';
        character[6][0] = '\\';
        character[6][9] = '/';
        character[2][2] = character[2][4] = character[3][2] = character[3][4] = character[4][2] = character[4][4] = character[5][2] = character[5][4] = '|';
        character[1][0] = '(';
        character[1][6] = character[5][8] = character[5][10] = ')';
        return character;
    }


        vs m()
        {
            vs character = getCharGrid(7,14);

            //Enter the character grid in ROWS X COL
            character[6][1] = character[5][1] = character[4][2] = character[3][3] = character[5][3] = character[4][4] = character[2][7] = character[4][7] = character[1][8] = character[1][10] = '/';
            character[1][3] = character[1][5] = character[2][6] = character[4][6] = character[3][10] = character[4][9] = character[5][10] = character[4][11] = character[5][12] = character[6][12] = '\\';
            character[0][3] = character[0][4] = character[0][9] = character[0][10] = character[3][5] = character[3][8] = character[5][2] = character[5][11] = '_';
            character[2][4] = character[2][10] = character[6][13] = ')';
            character[2][3] = character[2][9] = character[6][0] = '(';
            return character;
        }


    vs n()
    {
        vs character = getCharGrid(7, 14);

        // Enter the character grid in ROWS X COL
        character[3][2] = character[3][4] = character[3][6] = character[3][8] = character[3][10] = character[3][12] = character[1][13] = ')';
        character[6][0] = character[4][1] = character[4][3] = character[4][5] = character[4][7] = character[4][9] = character[4][11] = '(';
        character[1][3] = character[2][2] = character[2][4] = character[5][1] = character[5][3] = character[6][2] = character[1][11] = character[2][10] = character[2][12] = character[5][9] = character[5][11] = character[6][10] = '/';
        character[1][6] = character[2][5] = character[2][7] = character[5][6] = character[5][8] = character[6][7] = '\\';
        character[0][4] = character[0][5] = character[0][12] = character[6][1] = character[6][8] = character[6][9] = '_';

        return character;
    }

    vs o()
    {
        vs character = getCharGrid(8, 11);

        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][3] = ')';
        character[3][6] = '(';
        character[3][7] = ')';
        character[3][9] = ')';
        character[4][0] = '(';
        character[4][2] = '(';
        character[4][3] = ')';
        character[4][6] = '(';
        character[4][7] = ')';
        character[4][9] = ')';
        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '_';
        character[5][6] = '/';
        character[5][8] = '/';
        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '/';

        return character;
    }


        vs p()
        {
            vs character = getCharGrid(9,9);

           character[0][1]=character[0][2]=character[0][3]=character[0][4]=character[0][5]=character[1][3]=character[1][4]=character[2][4]=character[3][4]=character[3][5]=character[6][1]=character[6][2]='_';
           character[1][0]=character[3][0]=character[5][0]=character[5][2]='(';
           character[1][6]=character[6][3]='\\';
           character[2][1]=character[4][1]=character[4][3]=character[2][3]=character[2][5]=character[2][7]=')';
           character[3][6]=character[6][0]='/';
            return character;
        }


    vs q()
    {
        vs character = getCharGrid(8, 11);

        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[1][2] = '/';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][7] = '\\';

        character[2][1] = '/';
        character[2][3] = '/';
        character[2][6] = '\\';
        character[2][8] = '\\';

        character[3][0] = '(';
        character[3][2] = '(';
        character[3][7] = ')';
        character[3][9] = ')';
        character[4][0] = '(';
        character[4][2] = '(';
        character[4][5] = '/';
        character[4][6] = '\\';
        character[4][7] = ')';
        character[4][9] = ')';
        character[5][1] = '\\';
        character[5][3] = '\\';
        character[5][4] = '_';
        character[5][5] = '\\';
        character[5][7] = '\\';
        character[5][8] = '/';
        character[6][2] = '\\';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '\\';
        character[6][8] = '\\';
        character[6][9] = '_';

        character[7][10] = ')';
        character[7][9] = character[7][8] = '_';
        character[7][7] = '\\';

        return character;
    }

    vs r()
    {
        vs character = getCharGrid(7, 10);

        character[1][0] = character[3][0] = character[5][0] = character[2][3] = character[5][2] = '(';
        character[2][1] = character[4][1] = character[6][1] = character[6][3] = character[5][8] = character[5][9] = character[2][6] = character[2][8] = ')';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = '_';
        character[1][4] = character[1][5] = character[2][4] = character[2][5] = character[3][5] = character[3][6] = character[4][8] = character[5][7] = character[6][2] = character[6][6] = character[6][7] = '_';
        character[1][7] = character[4][3] = character[5][4] = character[6][5] = character[4][5] = character[5][6] = character[1][7] = '\\';
        character[3][7] = character[6][8] = '/';

        return character;
    }

    vs s()
    {
        vs character = getCharGrid(7, 9);

        // Enter the character grid in ROWS X COL
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = ' ';
        character[1][1] = '/';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        character[1][6] = '_';
        character[1][7] = '\\';
        character[2][0] = '(';
        character[2][1] = ' ';
        character[2][2] = '(';
        character[2][3] = '_';
        character[2][4] = '_';
        character[2][5] = '_';
        character[3][1] = '\\';
        character[3][2] = '_';
        character[3][3] = '_';
        character[3][4] = '_';
        character[3][5] = '_';
        character[3][7] = '\\';
        character[4][6] = ')';
        character[4][8] = ')';
        character[5][4] = '_';
        character[5][3] = '_';
        character[5][2] = '_';
        character[5][5] = '/';
        character[5][7] = '/';
        character[6][1] = '/';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '/';

        return character;
    }


        vs t()
        {
            vs character = getCharGrid(7,10);

            //Enter the character grid in ROWS X COL
            character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[1][3] = character[0][4] = character[0][5] = character[6][4] = character[6][5] = character[0][6] = character[0][7] = character[0][8] = character[1][6] = character[1][7] = character[1][8] = '_';
            character[1][0] = character[3][3] = character[5][3] = character[3][5] = character[5][5] = '(';
            character[1][9] = character[2][4] = character[4][4] = character[2][6] = character[4][6] = ')';
            character[6][3] = '/';
            character[6][6] = '\\';

            return character;
        }


     vs u()
    {
        vs character = getCharGrid(7, 11);
        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][9] = character[0][10] = character[1][0] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][9] = character[1][10] = character[2][1] = character[2][3] = character[2][4] = character[2][5] = character[2][6] = character[2][8] = character[2][10] = character[3][0] = character[3][2] = character[3][4] = character[3][5] = character[3][7] = character[3][9] = character[3][10] = character[4][1] = character[4][3] = character[4][4] = character[4][5] = character[4][6] = character[4][8] = character[4][10] = character[5][0] = character[5][2] = character[5][7] = character[5][9] = character[5][10] = character[6][0] = character[6][9] = character[6][10] = ' ';
character[0][1] = character[0][2] = character[0][7] = character[0][8] = character[5][4] = character[5][5] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = character[6][7] = '_';
character[1][1] = character[1][3] = character[2][7] = character[2][9] = character[3][1] = character[3][3] = character[4][7] = character[4][9] = character[5][1] = ')';
character[1][6] = character[1][8] = character[2][0] = character[2][2] = character[3][6] = character[3][8] = character[4][0] = character[4][2] = character[5][8] = '(';
character[5][3] = character[6][1] = '\\';
character[5][6] = character[6][8] = '/';
        return character;
    }


        vs v()
        {
            vs character = getCharGrid(7,10);
                character[3][1] = character[3][3] = character[4][2] = character[4][4] = character[5][3] = character[6][4] ='\\';
                character[3][8] = character[3][6] = character[4][7] = character[4][5] = character[5][6] = character[6][5] ='/';
                character[0][1] = character[0][2] = character[0][7] = character[0][8] = '_';
                character[2][0] = character[2][2] = character[1][8] = character[1][6] = '(';
                character[1][1] = character[1][3] = character[2][7] = character[2][9] = ')';
                character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][6] = character[6][7] =' ';
                character[6][8] = character[6][9] = character[5][0] = character[5][1] = character[5][2] = character[5][4] =' ';
                character[5][5] = character[5][7] = character[5][8] = character[5][9] = character[4][0] = character[4][1] =' ';
                character[4][3] = character[4][6] = character[4][8] = character[4][9] = character[3][0] = character[3][2] =' ';
                character[3][4] = character[3][5] = character[3][7] = character[3][9] = character[2][1] = character[2][3] =' ';
                character[2][4] = character[2][5] = character[2][6] = character[2][8] = character[1][0] = character[1][2] =' ';
                character[1][4] = character[1][5] = character[1][7] = character[1][9] = character[0][0] = character[0][3] =' ';
                character[0][4] = character[0][5] = character[0][6] = character[0][9] = ' ';
            return character;
        }


    vs w()
    {
        vs character = getCharGrid(7, 15);
        character[0][1] = character[0][2] = character[0][3] = character[0][11] = character[0][12] = character[0][13] = character[6][5] = character[6][9] = character[4][7] = character[2][7] = '_';
        character[4][11] = character[1][0] = character[1][3] = character[5][6] = '(';
        character[1][11] = character[1][14] = character[5][8] = character[4][3] = ')';
        character[2][1] = character[2][4] = character[3][2] = character[3][5] = character[5][3] = character[6][4] = character[3][8] = character[6][8] = '\\';
        character[2][10] = character[2][13] = character[3][9] = character[3][12] = character[5][11] = character[6][10] = character[3][6] = character[6][6] = '/';

        return character;
    }

    vs x()
    {
        vs character = getCharGrid(7, 11);

        // Enter the character grid in ROWS X COL
        character[0][0] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[1][2] = character[1][4] = character[1][5] = character[1][7] = character[1][8] = character[2][0] = character[2][2] = character[2][7] = character[3][0] = character[3][1] = character[3][3] = character[3][6] = character[4][0] = character[4][2] = character[4][4] = character[4][5] = character[4][7] = character[5][0] = character[5][1] = character[5][3] = character[5][6] = character[6][0] = character[6][4] = character[6][5] = ' ';
        character[0][1] = character[0][2] = character[0][9] = character[0][8] = character[1][1] = character[1][9] = character[2][5] = character[4][5] = character[5][1] = character[5][9] = character[6][1] = character[6][2] = character[6][8] = character[6][9] = '_';
        character[1][0] = character[6][0] = '(';
        character[1][3] = character[2][2] = character[2][4] = character[3][3] = character[4][7] = character[5][6] = character[5][8] = character[6][7] = '\\';
        character[1][7] = character[2][8] = character[2][6] = character[3][7] = character[4][3] = character[5][2] = character[5][4] = character[6][3] = '/';
        character[1][10] = character[6][10] = ')';

        return character;
    }

    vs y()
    {
        vs character = getCharGrid(7, 10);

        character[0][0] = character[0][1] = character[0][9] = character[0][8] = character[6][4] = character[6][5] = '_';
        character[1][9] = character[5][5] = '(';
        character[1][0] = character[5][4] = ')';
        character[1][2] = character[2][1] = character[2][3] = character[3][2] = character[3][4] = character[4][3] = character[6][6] = '\\';
        character[1][7] = character[2][8] = character[2][6] = character[3][7] = character[3][5] = character[4][6] = character[6][3] = '/';

        return character;
    }

    vs z()
    {
        vs character = getCharGrid(7, 10);
        character[0][8] = '_';
        character[0][2] = '_';
        character[0][3] = '_';
        character[0][4] = '_';
        character[0][5] = '_';
        character[0][6] = '_';
        character[0][7] = '_';

        character[1][1] = '(';
        character[1][2] = '_';
        character[1][3] = '_';
        character[1][4] = '_';
        character[1][5] = '_';
        // character[1][6] = '_';
        character[1][7] = ' ';
        character[1][8] = ' ';
        // character[3][9] = ' ';
        character[1][9] = ')';

        character[2][5] = '/';
        character[2][8] = '/';

        character[3][1] = '_';
        character[3][2] = '_';
        character[3][3] = '_';
        character[3][4] = '/';
        character[3][5] = ' ';
        character[3][6] = ' ';
        character[3][7] = '/';
        character[3][8] = '_';

        character[4][0] = '/';
        character[4][1] = '_';
        character[4][2] = '_';
        character[4][3] = ' ';
        character[4][4] = ' ';
        character[4][5] = ' ';
        character[4][6] = '_';
        character[4][7] = '_';
        character[4][8] = '_';
        character[4][9] = ')';

        character[5][2] = '/';
        character[5][4] = '/';
        character[5][5] = '_';
        character[5][6] = '_';
        character[5][7] = '_';
        character[5][8] = '_';
        character[3][9] = ' ';

        character[6][1] = '(';
        character[6][2] = '_';
        character[6][3] = '_';
        character[6][4] = '_';
        character[6][5] = '_';
        character[6][6] = '_';
        character[6][7] = '_';
        character[6][8] = '_';
        character[6][9] = ')';

        return character;
    }



    // Numbers
    vs zero()
    {
        vs character = getCharGrid(7, 8);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[1][4] = character[5][3] = character[5][4] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '_';
        character[2][0] = character[2][2] = character[3][0] = character[3][2] = character[4][0] = character[4][2] = character[5][0] = character[5][2] = '(';
        character[2][5] = character[2][7] = character[3][5] = character[3][7] = character[4][5] = character[4][7] = character[5][5] = character[5][7] = ')';
        character[1][1] = character[6][6] = '/';
        character[1][6] = character[6][1] = '\\';
        return character;
    }

    vs one()
    {
        vs character = getCharGrid(7, 7);

        character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[3][1] = character[6][4] = character[6][5] = '_';
        character[1][2] = character[2][1] = character[3][0] = character[1][6] = character[2][3] = character[3][2] = character[6][3] = '/';
        character[6][6] = '\\';
        character[2][4] = character[4][4] = character[2][6] = character[4][6] = ')';
        character[3][3] = character[5][3] = character[3][5] = character[5][5] = '(';
        return character;
    }


        vs two()
        {
            vs character = getCharGrid(7,11);

            //Enter the character grid in ROWS X COL
            character[0][3]= '_'; character[0][4]= '_'; character[0][5]= '_';
            character[0][6]= '_'; character[0][7]= '_'; character[0][8]= '_';
            character[1][2]= '('; character[1][3]= '_'; character[1][4]= '_';
            character[1][5]= '_'; character[1][6]= '_'; character[1][9]= '\\';

            character[2][7]= ')'; character[2][9]= '/';
            character[3][2]= '_'; character[3][3]= '_'; character[3][6]= '/'; character[3][8]= '/';
            character[4][1]= '/'; character[4][4]= '\\';
            character[4][5]= '/'; character[4][7]= '/'; character[4][9]= '_'; character[4][10]= '_';
            character[5][0]= '('; character[5][2]= '('; character[5][3]= ')'; character[5][5]= '\\';
            character[5][6]= '_'; character[5][7]= '_'; character[5][8]= '/'; character[5][10]= '/';
            character[6][1]= '\\';
            character[6][2]= '_'; character[6][3]= '_'; character[6][4]= '\\';
            character[6][5]= '_'; character[6][6]= '_'; character[6][7]= '_'; character[6][8]= '_'; character[6][9]= '(';



            return character;
        }



     vs three()
        {
            vs character = getCharGrid(8,9);

            //Enter the character grid in ROWS X COL
            character[0][0]=  character[0][1]=  character[0][2]= character[0][3]= character[0][4]= '_';
            character[1][1]=  character[1][2]= '_';
            character[2][1]=  character[2][2]= '_';
            character[3][1]=  character[3][2]= '_';
            character[5][0]=  character[5][1]=  character[5][2]= '_';
            character[6][1]=  character[6][2]=  character[6][3]= character[6][4]= '_';

            character[1][5]=  character[4][3]=  character[4][5]= '\\';

            character[2][5]=  character[5][3]=  character[6][5]='/';

            character[3][0]= character[3][4]= '(';

            character[1][0]= character[2][3]=   character[5][6]= character[6][0]=')';



            return character;
        }

   vs four()
        {
            vs character = getCharGrid(7,11);

            character[0][2] = character[0][6] = character[2][3] = character[2][4] = character[2][8] = character[2][9] = character[2][10] = character[3][2] = character[3][3] = character[3][4] = character[3][8] = character[3][9] = character[6][5] = character[6][6] = character[6][7] = '_';
            character[1][1] = character[1][3] = character[1][5] = character[6][4] = '/';
            character[1][7] = character[3][1] = character[6][8] = '\\';
            character[2][0] = character[2][2] = character[2][7] = character[3][10] = character[4][7] = character[5][7] = '(';
            character[2][5] = character[4][5] = character[5][5] = ')';

            return character;
        }


        vs five()
        {
            vs character = getCharGrid(7,8);

            character[0][1]='_',character[0][2]='_',character[0][3]='_',character[0][4]='_',character[0][5]='_',character[0][6]='_';
            character[1][0]='|',character[1][3]='_',character[1][4]='_',character[1][5]='_',character[1][6]='(';
            character[2][0]='|',character[2][2]='|',character[2][3]='_',character[2][4]='_';
            character[3][0]='|',character[3][1]='_',character[3][2]='_',character[3][3]='_',character[3][5]='\\';
            character[4][4]='\\',character[4][6]='\\';
            character[5][0]='_',character[5][1]='_',character[5][2]='_',character[5][3]='_',character[5][4]='_',character[5][5]=')',character[5][7]=')';
            character[6][0]=')',character[6][1]='_',character[6][2]='_',character[6][3]='_',character[6][4]='_',character[6][5]='_',character[6][6]='/';



            return character;
        }




        vs six()
        {
            vs character = getCharGrid(7,10);

            //Enter the character grid in ROWS X COL
            character[0][0] = character[0][1] = character[0][2] =' ', character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] ='_', character[0][9] =' ' ;
            character[1][0] = character[1][1] =' ', character[1][2] ='/', character[1][3] =' ', character[1][4] = character[1][5] = character[1][6] = character[1][7] ='_', character[1][8] ='(', character[1][9] = ' ';
            character[2][0] =' ', character[2][1] ='/', character[2][2] =' ', character[2][3] ='/', character[2][4] = character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = ' ';\
            character[3][0] ='(', character[3][1] =' ', character[3][2] ='(', character[3][3] = character[3][4] = character[3][5] = character[3][6] = character[3][7] ='_', character[3][8] = character[3][9] =' ' ;
            character[4][0] ='(', character[4][1] = character[4][2] = character[4][3] =' ', character[4][4] = character[4][5] = character[4][6] ='_', character[4][7] =' ', character[4][8] ='\\', character[4][9] =' ' ;
            character[5][0] =' ', character[5][1] ='\\', character[5][2] =' ', character[5][3] ='(', character[5][4] = character[5][5] = character[5][6] ='_', character[5][7] =')', character[5][8] =' ', character[5][9] =')' ;
            character[6][0] = character[6][1] =' ', character[6][2] ='\\', character[6][3] = character[6][4] = character[6][5] = character[6][6] ='_', character[6][7] ='_', character[6][8] ='/', character[6][9] =' ' ;

            return character;
        }




        vs seven()
        {
            vs character = getCharGrid(7,10);

            //Enter the character grid in ROWS X COL
            character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] ='_', character[0][9] =' ' ;
            character[1][0] =')', character[1][1] = character[1][2] = character[1][3] = character[1][4] ='_', character[1][5] = character[1][6] = character[1][7] =' ', character[1][8] ='/', character[1][9] = ' ';
            character[2][0] = character[2][1] =' ', character[2][2] = character[2][3] = character[2][4] ='_', character[2][5] =')', character[2][6] =' ', character[2][7] ='/', character[2][8] = character[2][9] = ' ';
            character[3][0] =' ', character[3][1] =')', character[3][2] = character[3][3] = character[3][4] ='_', character[3][5] =' ', character[3][6] ='(', character[3][7] = character[3][8] = character[3][9] =' ' ;
            character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] =' ', character[4][5] =')', character[4][6] =' ', character[4][7] =')', character[4][8] = character[4][9] =' ' ;
            character[5][0] = character[5][1] = character[5][2] = character[5][3] =' ', character[5][4] ='(', character[5][5] =' ', character[5][6] ='(', character[5][7] = character[5][8] = character[5][9] =' ' ;
            character[6][0] = character[6][1] = character[6][2] = character[6][3] =' ', character[6][4] ='/', character[6][5] = character[6][6] ='_', character[6][7] ='\\', character[6][8] = character[6][9] =' ' ;


            return character;
        }




        vs eight()
        {
            vs character = getCharGrid(7,9);


            character[0][2]= '_'; character[0][3]= '_'; character[0][4]= '_'; character[0][5]= '_'; character[0][6]= '_';
            character[1][1]= '/'; character[1][4]= '_'; character[1][7]= '\\';

            character[2][0]= '('; character[2][3]= '('; character[2][4]= '_'; character[2][5]= ')'; character[2][8]= ')';
            character[3][1]= '\\';
            character[3][4]= '_'; character[3][7]= '/';
            character[4][1]= '/'; character[4][3]= '/'; character[4][5]= '\\';
            character[4][7]= '\\';

            character[5][0]= '('; character[5][2]= '('; character[5][3]= '_'; character[5][4]= '_';
            character[5][5]= '_'; character[5][6]= ')'; character[5][8]= ')';
            character[6][1]= '\\';
            character[6][2]= '_'; character[6][3]= '_'; character[6][4]= '_'; character[6][5]= '_';
            character[6][6]= '_'; character[6][7]= '/';



            return character;
        }




        vs nine()
        {
            vs character = getCharGrid(7, 11);

          character[0][2]= '_'; character[0][3]= '_';
          character[0][4]= '_'; character[0][5]= '_'; character[0][6]= '_';
          character[1][1]= '/'; character[1][3]= '_';
          character[1][4]= '_'; character[1][7]= '\\';

           character[2][0]= '('; character[2][2]= '(';
           character[2][3]= '_'; character[2][4]= '_'; character[2][5]= ')'; character[2][8]= '\\';

          character[3][1]= '\\';
          character[3][2]= '_'; character[3][3]= '_'; character[3][4]= '_'; character[3][5]= '_'; character[3][6]= '_'; character[3][9]= ')';
          character[4][7]= ')'; character[4][9]= ')';
          character[5][2]= '_'; character[5][3]= '_'; character[5][4]= '_'; character[5][5]= '_'; character[5][6]= '/'; character[5][8]= '/';
          character[6][1]= '('; character[6][2]= '_'; character[6][3]= '_'; character[6][4]= '_'; character[6][5]= '_'; character[6][6]= '_'; character[6][7]= '/';



            return character;
        }



    // for space
    vs space()
    {
        vs character = getCharGrid(1, 1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef BANNER_H
#define BANNER_H

class Banner : public Fonts
{
    static const int rows = 7;
    static const int cols = 7;

public:
    Banner() : Fonts(rows, cols) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(7, 7);
        character[0][0] = character[0][1] = character[0][2] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[1][1] = character[1][3] = character[1][5] = character[1][6] = character[2][0] = character[2][2] = character[2][3] = character[2][4] = character[2][6] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = ' ';
        character[0][3] = character[1][2] = character[1][4] = character[2][1] = character[2][5] = character[3][0] = character[3][6] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[4][6] = character[5][0] = character[5][6] = character[6][0] = character[6][6] = '#';
        return character;
    }

    vs B()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][5] = character[4][0] = character[4][6] = character[5][0] = character[5][6] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '#';
        character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] =
            character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] =
                character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] =
                    character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = ' ';

        return character;
    }

    vs C()
    {
        vs character = getCharGrid(7, 7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[5][6] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';
        return character;
    }

    vs D()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][0] = character[3][6] = character[4][0] = character[4][6] = character[5][0] = character[5][6] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = '#';
        character[1][0] = '#';
        character[2][0] = '#';
        character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '#';
        character[4][0] = '#';
        character[5][0] = '#';
        character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '#';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[2][0] = character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[4][0] = character[5][0] = character[6][0] = '#';

        return character;
    }

    vs G()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = ' ';
        character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = ' ';
        character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = character[2][6] = ' ';
        character[3][1] = character[3][2] = ' ';
        character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = ' ';
        character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = ' ';
        character[6][0] = character[6][6] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = '#';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[3][6] = character[4][6] = character[5][6] = '#';
        character[6][1] = character[6][3] = character[6][4] = character[6][5] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = '#';

        return character;
    }

    vs H()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = character[0][6] = character[1][6] = character[2][6] = character[4][6] = character[5][6] = character[6][6] = '#';
        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = character[0][6] = character[1][6] = character[2][6] = character[4][6] = character[5][6] = character[6][6] = '#';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid(7, 7);
        character[0][2] = character[0][3] = character[6][2] = character[0][4] = character[6][3] = character[6][4] = character[2][3] = character[1][3] = character[3][3] = character[4][3] = character[5][3] = '#';
        return character;
    }

    vs J()
    {
        vs character = getCharGrid(7, 7);
        character[0][6] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[5][6] = character[6][5] = character[6][4] = character[6][3] = character[6][2] = character[6][1] = character[5][0] = character[4][0] = '#';

        return character;
    }

    vs K()
    {
        vs character = getCharGrid(7, 7);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][5] = character[1][0] = character[1][4] = character[2][0] = character[2][3] = character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[4][3] = character[5][0] = character[5][4] = character[6][0] = character[6][5] = '#';

        return character;
    }

    vs L()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '#';

        return character;
    }

    vs M()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[1][1] = character[1][5] = character[2][2] = character[2][4] = character[3][3] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[5][6] = character[6][6] = character[0][6] = '#';

        return character;
    }

    vs N()
    {
        vs character = getCharGrid(7, 7);
        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[0][6] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[5][6] = character[6][6] = character[1][1] = character[2][2] = character[3][3] = character[4][4] = character[5][5] = '#';

        return character;
    }

    vs O()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][0] = character[3][6] = character[4][0] = character[4][6] = character[5][0] = character[5][6] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '#';

        return character;
    }

    vs P()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][6] = character[2][6] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = '#';

        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(7, 7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][0] = character[3][6] = character[4][0] = character[4][4] = character[4][6] = character[5][0] = character[5][5] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][6] = '#';

        return character;
    }

    vs R()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][6] = character[2][6] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[4][4] = character[5][5] = character[6][6] = '#';

        return character;
    }

    vs S()
    {
        vs character = getCharGrid(7, 7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = '#';
        character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = '#';
        character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';
        character[1][0] = character[2][0] = '#';
        character[1][6] = '#';
        character[4][6] = character[5][6] = '#';
        character[5][0] = '#';

        return character;
    }

    vs T()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = '#';
        character[0][4] = character[0][5] = character[0][6] = '#';
        character[1][3] = character[2][3] = character[3][3] = character[4][3] = '#';
        character[5][3] = character[6][3] = '#';

        return character;
    }

    vs U()
    {
        vs character = getCharGrid(7, 7);
        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = '#';
        character[0][6] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[5][6] = '#';
        character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';

        return character;
    }

    vs V()
    {
        vs character = getCharGrid(7, 7);
        character[0][0] = character[0][6] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][0] = character[3][6] = '#';
        character[4][1] = character[4][5] = character[5][2] = character[5][4] = character[6][3] = '#';

        return character;
    }

    vs W()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[0][6] = character[1][6] = character[2][6] = character[3][6] = character[4][6] = character[5][6] = character[1][3] = character[2][3] = character[3][3] = character[4][3] = character[5][3] = character[6][1] = character[6][2] = character[6][4] = character[6][5] = '#';

        return character;
    }

    vs X()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][6] = character[1][1] = character[1][5] = character[2][2] = character[2][4] = character[3][3] = character[4][2] = character[4][4] = character[5][1] = character[5][5] = character[6][0] = character[6][6] = '#';

        return character;
    }

    vs Y()
    {
        vs character = getCharGrid(7, 7);

        character[3][3] = character[4][3] = character[5][3] = character[6][3] = character[0][0] = character[0][6] = character[1][1] = character[1][5] = character[2][2] = character[2][4] = '#';

        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(7, 7);

        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[6][0] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = character[1][5] = character[2][4] = character[3][3] = character[4][2] = character[5][1] = '#';

        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(7, 4);

        character[3][1] = character[3][2] = character[3][3] = character[4][0] = character[4][3] = character[5][0] = character[5][2] = character[5][3] = character[6][1] = character[6][3] = '#';

        return character;
    }

    vs b()
    {
        vs character = getCharGrid(7, 4);

        character[1][0] = character[2][0] = character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[4][3] = character[5][0] = character[5][3] = character[6][0] = character[6][1] = character[6][2] = '#';

        return character;
    }

    vs c()
    {
        vs character = getCharGrid(7, 3);

        character[3][1] = character[3][2] = character[4][0] = character[5][0] = character[6][2] = character[6][1] = '#';

        return character;
    }

    vs d()
    {
        vs character = getCharGrid(7, 4);

        character[1][3] = character[2][3] = character[3][3] = character[4][3] = character[5][3] = character[6][3] = '#';
        character[3][1] = character[3][2] = character[4][0] = character[5][0] = character[6][1] = character[6][2] = '#';

        return character;
    }

    vs e()
    {

        vs character = getCharGrid(7, 4);
        character[3][1] = character[3][2] = character[4][0] = character[4][2] = character[4][3] = character[5][0] = character[5][1] = character[6][1] = character[6][2] = '#';

        return character;
    }

    vs f()
    {
        vs character = getCharGrid(7, 4);

        character[1][2] = character[2][1] = character[2][3] = character[3][1] = character[4][0] = character[4][1] = character[4][2] = character[5][1] = character[6][1] = '#';

        return character;
    }

        vs g()
        {
            vs character = getCharGrid(8,4);
         character[3][1] = character[3][2] = character[3][3] = character[4][0] = character[4][3] = character[5][1] = character[5][2] = character[6][0] = character[7][1] = character[7][2] = character[7][3] = '#';

            return character;
        }



        vs h()
        {
            vs character = getCharGrid(7,4);
            character[1][0] = character[2][0] = character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[4][3] = character[5][0] = character[5][3] = character[6][0] = character[6][3] = '#';
            return character;
        }


    vs i()
    {
        vs character = getCharGrid(7, 3);

        character[1][1] = character[3][1] = character[3][0] = character[4][1] = character[5][1] = character[6][0] = character[6][1] = character[6][2] = '#';

        return character;
    }

    vs j()
    {
        vs character = getCharGrid(8, 3);

        // Enter the character grid in ROWS X COLS
        character[1][2] = character[3][2] = character[4][2] = character[5][2] = character[6][0] = character[6][2] = character[7][1] = '#';

        return character;
    }

    vs k()
    {
        vs character = getCharGrid(7, 5);

        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] =
            character[6][0] = character[3][2] = character[4][1] = character[5][2] = character[6][3] = '#';

        return character;
    }


        vs l()
        {
            vs character = getCharGrid(7,3);

            //Enter the character grid in ROWS X COLS
            character[1][0] = character[1][1] = character[2][1] = character[3][1] =
            character[4][1] = character[5][1] = character[6][0] = character[6][1] = character[6][2] = '#';

            return character;
        }



    vs m()
    {
        vs character = getCharGrid(7, 6);

        character[4][0] = character[5][0] = character[6][0] = character[3][1] = character[3][4] = character[4][2] = character[4][3] = character[4][5] = character[6][5] = character[5][5] = '#';

        return character;
    }

    vs n()
    {
        vs character = getCharGrid(7, 4);

        character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[4][3] = character[5][0] = character[5][3] = character[6][0] = character[6][3] = '#';

        return character;
    }

    vs o()
    {
        vs character = getCharGrid(7, 5);

        // Enter the character grid in ROWS X COLS
        character[3][2] = character[3][3] = character[4][1] = character[4][4] = character[5][1] = character[5][4] = character[6][2] = character[6][3] = '#';

        return character;
    }


       vs p()
        {
            vs character = getCharGrid(8,5);

            // Enter the character grid in ROWS X COLS
            character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[5][0] = character[6][0] = character[6][1] = character[6][2] =character[7][0] = character[4][3] = character[5][3] = '#';

            return character;
        }



   //@$
        vs q()
        {
            vs character = getCharGrid(8,7);

            //Enter the character grid in ROWS X COLS
            character[2][4] = character[2][5] =  character[2][6] = character[3][3] = character[3][6] = character[4][2] = character[4][6] = character[5][3] = character[5][4] = character[5][5] =  character[5][6] = character[6][6] = '#';
            return character;
        }


        vs r()
        {
            vs character = getCharGrid(7,4);
            character[3][0] = character[3][1] = character[3][2] = character[4][0] = character[5][0] = character[6][0] = character[4][3] = '#';
            return character;
        }


    vs s()
        {
            vs character = getCharGrid(7,4);

            // Enter the character grid in ROWS X COLS
            character[3][1] = character[3][2] = character[3][3] = character[4][0] = character[4][1] = character[5][2] = character[5][3] = character[6][0] =character[6][1] = character[6][2]  = '#';
            character[0][0]=character[0][1]=character[0][2]=character[0][3]=character[1][0]=character[1][1]= character[1][2]=character[1][3]=character[2][0]=character[2][1]=character[2][2]=character[2][3]= character[3][0]=character[4][2]=character[4][3]=character[5][0]=character[5][1]=character[6][3] =' ';
            return character;
        }

        vs t()
        {
            vs character = getCharGrid(7,4);

            //Enter the character grid in ROWS X COLS
           character[1][1] = character[2][1] = character[3][0] = character[3][1] = character[3][2] = character[4][1] = character[5][1] = character[6][2] = character[6][3] = '#';

            return character;
        }



        vs u()
        {
            vs character = getCharGrid(7,4);

            //Enter the character grid in ROWS X COLS
             character[3][0] = character[3][3] = character[4][0] = character[4][3] = character[5][0] = character[5][3] = character[6][1] = character[6][2] = character[6][3] = '#';
            return character;
        }



    vs v()
    {
        vs character = getCharGrid(7, 5);

        // Enter the character grid in ROWS X COLS
        character[2][0] = character[3][0] = character[4][0] = character[5][1] = character[6][2] = character[5][3] = character[4][4] = character[3][4] = character[2][4] = '#';

        return character;
    }

    vs w()
    {
        vs character = getCharGrid();

        character[1][0] = character[1][4] = character[2][0] = character[2][4] = character[3][0] =
            character[3][2] = character[3][4] = character[4][0] = character[4][1] = character[4][3] = character[4][4] = '#';

        return character;
    }

        vs x()
        {
            vs character = getCharGrid(7,4);

            character[3][0] = character[3][3] = character[4][1] = character[4][2] = character[5][1] = character[5][2] = character[6][0] = character[6][3] = '#';

            return character;
        }


      vs y()
        {
            vs character = getCharGrid(8, 4);
character[3][0] = character[3][3] = character[4][0] = character[4][3] = character[5][1] = character[5][3] = character[6][2] = character[7][1] = '#';
            return character;
        }

        vs z()
        {
            vs character = getCharGrid(7,4);
            character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[4][2] = character[5][1] = character[6][0] = character[6][1] = character[6][2] = character[6][3] ='#';

            return character;
        }


    // Numbers

    vs zero()
    {
        vs character = getCharGrid(7, 8);
        character[0][0] = character[0][1] = character[0][5] = character[0][6] = character[0][7] = character[1][0] = character[1][2] = character[1][3] = character[1][4] = character[1][6] = character[1][7] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = character[2][7] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][7] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[4][7] = character[5][0] = character[5][2] = character[5][3] = character[5][4] = character[5][6] = character[5][7] = character[6][0] = character[6][1] = character[6][5] = ' ';
character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][5] = character[2][0] = character[2][6] = character[3][0] = character[3][6] = character[4][0] = character[4][6] = character[5][1] = character[5][5] = character[6][2] = character[6][3] = character[6][4] = '#';
        return character;
    }


    vs one()
    {
        vs character = getCharGrid();

        character[0][2] = character[1][2] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = '#';
        character[6][0] = character[6][1] = character[6][3] = character[6][4] = '#';
        character[2][0] = character[1][1] = '#';
        return character;
    }


        vs two()
        {
            vs character = getCharGrid(7,7);

            character[0][1]=character[0][2]=character[0][3]=character[0][4]=character[0][5]=character[1][0]=character[1][6]=character[2][6]=character[3][5]=character[3][4]=character[3][3]='#';
            character[3][2]=character[3][1]='#';
            character[4][0]=character[5][0]=character[6][0]=character[6][1]=character[6][2]=character[6][3]='#';
            character[6][4]=character[6][5]=character[6][6]='#';

            return character;
        }



    vs three()
    {
        vs character = getCharGrid(7, 7);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = '#';
        character[2][6] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[4][6] = '#';
        character[5][0] = character[6][1] = character[5][6] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';

        return character;
    }

    vs four()
    {
        vs character = getCharGrid();
        character[0][0] = character[1][0] = character[2][0] = character[3][0] = '#';
        character[1][5] = character[2][5] = character[3][5] = '#';
        character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[4][6] = '#';
        character[5][5] = character[6][5] = '#';
        return character;
    }

    vs five()
    {
        vs character = getCharGrid(7, 7);

        // Enter the character grid in ROWS X COLS
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[2][0] = character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[4][6] = character[5][0] = character[5][6] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';
        character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = character[2][6] = character[3][6] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[6][0] = character[6][6] = ' ';

        return character;
    }


        vs six()
        {
            vs character = getCharGrid(7,7);

            //Enter the character grid in ROWS X COLS
            character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = character[2][0] = character[3][0] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[4][0] = character[4][6] = character[5][0] = character[5][6] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';
            character[0][0] = character[0][6] = character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = character[2][6] = character[3][6] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[6][0] = character[6][6] = ' ';


            return character;
        }



    vs seven()
    {
        vs character = getCharGrid(7, 7);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[1][0] = character[1][5] = character[2][4] = character[3][3] = character[4][2] = character[5][2] = character[6][2] = '#';
        character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[2][0] = character[2][1] = character[2][2] = character[2][3] = character[2][5] = character[2][6] = character[3][0] = character[3][1] = character[3][2] = character[3][4] = character[3][5] = character[3][6] = character[4][0] = character[4][1] = character[4][3] = character[4][4] = character[4][5] = character[4][6] = character[5][0] = character[5][1] = character[5][3] = character[5][4] = character[5][5] = character[5][6] = character[6][0] = character[6][1] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = ' ';
        return character;
    }

    vs eight()
    {
        vs character = getCharGrid();
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = '#';
        character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = '#';
        character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = '#';
        character[1][0] = character[2][0] = character[4][0] = character[5][0] = '#';
        character[1][6] = character[2][6] = character[4][6] = character[5][6] = '#';

        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(7, 7);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][0] = character[1][6] = character[2][0] = character[2][6] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = character[4][6] = character[5][0] = character[5][6] = character[6][1] = character[6][2] = character[6][3] = character[6][4] = character[6][5] = character[6][6] = '#';

        character[0][0] = character[0][6] = character[1][1] = character[1][2] = character[1][3] = character[1][4] = character[1][5] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = character[3][0] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = character[4][5] = character[5][1] = character[5][2] = character[5][3] = character[5][4] = character[5][5] = character[6][0] = character[6][5] = ' ';

        return character;
    }

    // for space
    vs space()
    {
        vs character = getCharGrid(1, 1);

        character[0][0] = ' ';

        return character;
    }
};
#endif

#ifndef BLOCK_H
#define BLOCK_H


class Block : public Fonts
{
    static const int rows = 11;
    static const int cols = 20;

public:
    Block() : Fonts(rows, cols) {}
    // Uppercase

    vs A()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';
        character[2][9] = character[2][10] = character[7][4] = character[7][5] = character[7][6] = character[7][7] = character[6][4] = character[6][15] = character[7][12] = character[7][13] = character[7][14] = character[7][15] = character[5][8] = character[5][9] = character[5][10] = character[5][11] = '_';
        character[3][8] = character[4][7] = character[5][6] = character[6][5] = character[4][9] = character[6][7] = '/';
        character[3][11] = character[4][12] = character[5][13] = character[6][14] = character[6][12] = character[4][10] = '\\';
        character[7][3] = character[7][8] = character[7][11] = character[7][16] = '|';

        return character;
    }

    vs B()
    {
        vs character = getCharGrid(11, 20);

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[5][13] = '.';
        character[1][0] = character[1][19] = character[2][0] = character[2][2] = character[2][17] = character[2][19] = character[3][0] = character[3][2] = character[3][5] = character[3][17] = character[3][19] = character[4][0] = character[4][2] = character[4][7] = character[4][9] = character[4][13] = character[4][17] = character[4][19] = character[5][0] = character[5][2] = character[5][7] = character[5][17] = character[5][19] = character[6][0] = character[6][2] = character[6][7] = character[6][9] = character[6][14] = character[6][17] = character[6][19] = character[7][0] = character[7][2] = character[7][5] = character[7][17] = character[7][19] = character[8][0] = character[8][2] = character[8][17] = character[8][19] = character[9][0] = character[9][19] = '|';
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][8] = character[9][9] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = '-';
        character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[3][6] = character[3][10] = character[4][10] = character[5][10] = character[5][11] = character[6][6] = character[6][10] = character[6][11] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[7][13] = '/';
        character[3][12] = '\\';
        character[5][12] = character[9][2] = character[9][17] = character[10][1] = character[10][18] = '\'';
        character[4][11] = character[6][12] = ')';

        return character;
    }

    vs C()
    {
        vs character = getCharGrid();

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[3][9] = character[3][10] = character[3][11] = character[4][13] = character[6][9] = character[6][10] = character[6][11] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = '_';
        character[3][14] = character[4][14] = character[5][5] = character[5][7] = '|';
        character[4][12] = character[6][5] = character[6][14] = '\\';
        character[4][5] = '/';
        character[3][6] = character[4][7] = character[6][8] = character[6][12] = character[7][7] = character[7][13] = '.';
        character[3][7] = character[4][8] = character[6][13] = character[7][14] = '\'';
        character[6][7] = character[7][6] = '`';

        return character;
    }

    vs D()
    {
        vs character = getCharGrid();

        // Enter the character grid in ROWS X COLS

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][5] = character[3][9] = character[3][10] = character[3][11] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[6][5] = character[6][9] = character[6][10] = character[6][11] = '_';
        character[3][4] = character[4][6] = character[4][8] = character[5][6] = character[5][8] = character[5][13] = character[5][15] = character[6][6] = character[6][8] = character[7][4] = '|';
        character[3][14] = character[4][13] = character[6][12] = character[7][13] = '.';
        character[4][15] = '\\';
        character[6][15] = '/';
        character[6][13] = character[7][14] = '\'';
        character[3][13] = character[4][12] = '`';

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = character[3][5] = character[3][9] = character[3][10] = character[3][11] = character[4][9] = character[4][13] = character[5][9] = character[5][13] = character[6][5] = character[6][9] = character[6][10] = character[6][11] = '_';
        character[3][4] = character[3][14] = character[4][6] = character[4][8] = character[4][14] = character[5][6] = character[5][10] = character[6][6] = character[6][8] = character[6][14] = character[7][4] = character[7][14] = '|';
        character[4][12] = '\\';
        character[6][12] = '/';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[3][5] = character[3][9] = character[3][10] = character[3][11] = character[4][9] = character[4][13] = character[5][9] = character[6][5] = character[6][9] = '_';
        character[3][4] = character[3][14] = character[4][6] = character[4][8] = character[4][14] = character[5][6] = character[5][10] = character[6][6] = character[6][8] = character[7][4] = character[7][10] = '|';
        character[4][12] = '\\';

        return character;
    }

    vs G()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[5][11] = character[5][12] = character[5][13] = character[5][14] = character[6][14] = character[6][8] = character[6][9] = character[6][10] = character[4][12] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][8] = character[3][9] = character[3][10] = '_';
        character[3][13] = character[4][13] = character[5][4] = character[5][6] = character[6][15] = '|';
        character[3][6] = character[4][7] = character[7][13] = '\'';
        character[4][4] = '/';
        character[6][4] = character[4][11] = '\\';
        character[3][5] = character[4][6] = character[6][7] = character[7][6] = character[7][12] = '.';
        character[7][5] = character[6][6] = '`';
        character[6][11] = ']';
        return character;
    }

    vs H()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][11] = character[2][12] = character[2][13] = character[2][14] = character[3][5] = character[3][14] = character[4][9] = character[4][10] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = character[6][5] = character[6][14] = character[5][9] = character[5][10] = '_';
        character[3][4] = character[3][9] = character[3][10] = character[3][15] = character[4][6] = character[4][8] = character[4][11] = character[4][13] = character[5][13] = character[5][6] = character[6][6] = character[6][8] = character[6][11] = character[6][13] = character[7][4] = character[7][9] = character[7][10] = character[7][15] = '|';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid();

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][8] = character[3][12] = character[6][8] = character[6][12] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[3][7] = character[3][13] = character[4][9] = character[4][11] = character[5][9] = character[5][11] = character[6][9] = character[6][11] = character[7][7] = character[7][13] = '|';

        return character;
    }

    vs J()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][8] = character[3][12] = character[5][6] = character[6][8] = character[7][7] = character[7][8] = character[7][9] = '_';
        character[3][7] = character[3][13] = character[4][9] = character[4][11] = character[5][9] = character[5][11] = character[6][5] = character[6][7] = character[6][11] = '|';
        character[6][9] = character[7][11] = '\'';
        character[7][5] = '`';
        character[7][6] = character[7][10] = '.';

        return character;
    }

    vs K()
    {
        vs character = getCharGrid();

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][8] = character[9][9] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = '-';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[5][12] = '.';
        character[9][2] = character[9][17] = character[10][1] = character[10][18] = character[5][11] = '\'';
        character[1][0] = character[1][19] = character[2][0] = character[2][19] = character[3][0] = character[3][19] = character[4][0] = character[4][19] = character[5][0] = character[5][19] = character[6][0] = character[6][19] = character[7][0] = character[7][19] = character[8][0] = character[8][19] = character[9][0] = character[9][19] = character[2][2] = character[2][17] = character[3][2] = character[3][17] = character[4][2] = character[4][17] = character[5][2] = character[5][17] = character[6][2] = character[6][17] = character[7][2] = character[7][17] = character[8][2] = character[8][17] = '|';
        character[3][4] = character[3][8] = character[3][9] = character[3][14] = character[4][6] = character[4][8] = character[5][6] = character[6][6] = character[6][8] = character[7][4] = character[7][9] = character[7][10] = character[7][15] = '|';
        character[2][5] = character[2][6] = character[2][7] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[3][5] = character[3][10] = character[3][13] = character[4][9] = character[5][9] = character[5][10] = character[6][5] = character[6][14] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = '_';
        character[4][10] = character[4][12] = '/';
        character[6][11] = character[6][13] = '\\';
        return character;
    }

    vs L()
    {
        vs character = getCharGrid();

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';
        character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[3][6] = character[3][10] = character[6][6] = character[6][10] = character[6][11] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = character[5][13] = '_';
        character[3][5] = character[3][11] = character[4][7] = character[5][7] = character[6][7] = character[4][9] = character[5][9] = character[6][9] = character[7][5] = character[7][14] = character[6][14] = '|';
        character[6][12] = '/';

        return character;
    }

    vs M()
    {
        vs character = getCharGrid();

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][4] = character[2][5] = character[2][6] = character[2][7] = character[2][12] = character[2][13] = character[2][14] = character[2][15] = character[7][4] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = character[7][15] = character[6][4] = character[6][8] = character[6][11] = character[6][15] = character[3][4] = character[3][15] = '_';
        character[3][3] = character[3][16] = character[4][5] = character[4][14] = character[5][5] = character[5][14] = character[6][5] = character[6][14] = character[7][3] = character[7][9] = character[7][10] = character[7][16] = character[5][7] = character[5][12] = character[6][7] = character[6][12] = '|';
        character[3][8] = character[4][9] = character[5][8] = character[6][9] = '\\';
        character[3][11] = character[4][10] = character[5][11] = character[6][10] = '/';
        return character;
    }

    vs N()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';
        character[2][4] = character[2][5] = character[2][6] = character[2][7] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[2][14] = character[3][4] = character[3][10] = character[3][14] = character[6][4] = character[6][8] = character[6][14] = character[7][4] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = '_';
        character[3][3] = character[3][9] = character[3][15] = character[4][5] = character[4][11] = character[4][13] = character[5][5] = character[5][11] = character[5][13] = character[6][5] = character[6][7] = character[6][13] = character[7][3] = character[7][9] = character[7][15] = '|';
        character[3][8] = character[4][9] = character[5][8] = character[5][10] = character[6][9] = character[7][10] = '\\';
        return character;
    }


    vs O()
        {
        vs character = getCharGrid(11, 20);
        character[0] = " .----------------. ";
        character[1] = "| .--------------. |";
        character[2] = "| |     ____     | |";
        character[3] = "| |   .'    `.   | |";
        character[4] = "| |  /  .--.  \\  | |";
        character[5] = "| |  | |    | |  | |";
        character[6] = "| |  \\  `--'  /  | |";
        character[7] = "| |   '.____.'   | |";
        character[8] = "| |              | |";
        character[9] = "| '--------------' |";
        character[10] ="'----------------' ";

        return character;

        }


    vs P()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[3][6] = character[3][10] = character[3][11] = character[4][10] = character[4][11] = character[5][10] = character[5][11] = character[5][12] = character[6][6] = character[6][10] = '_';
        character[3][5] = character[4][7] = character[4][9] = character[4][14] = character[5][7] = character[6][7] = character[6][9] = character[7][5] = character[7][11] = '|';
        character[3][14] = '\\';
        character[4][12] = ')';
        character[5][13] = '/';

        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][7] = character[2][8] = character[2][9] = character[6][13] = character[7][7] = character[7][8] = character[7][9] = character[7][12] = character[7][13] = '_';
        character[5][4] = character[5][6] = character[5][10] = character[5][12] = character[7][14] = '|';
        character[3][5] = character[3][11] = character[4][7] = character[4][9] = character[7][6] = character[7][10] = '.';
        character[4][12] = character[6][4] = character[6][12] = character[7][11] = '\\';
        character[4][4] = '/';
        character[6][7] = character[7][5] = '`';
        character[4][8] = character[6][8] = '-';
        character[3][6] = character[3][10] = character[6][9] = '\'';

        return character;
    }

    vs R()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';
        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[3][5] = character[3][9] = character[3][10] = character[4][10] = character[4][9] = character[5][9] = character[5][10] = character[6][5] = character[7][5] = character[7][6] = character[7][7] = character[6][14] = character[7][12] = character[7][13] = character[7][14] = '_';
        character[3][4] = character[4][6] = character[4][8] = character[4][13] = character[5][6] = character[6][6] = character[6][8] = character[7][4] = character[7][9] = character[7][11] = character[7][15] = '|';
        character[3][12] = character[6][11] = character[6][13] = '\\';
        character[5][12] = '/';
        character[4][11] = ')';
        return character;
    }

    vs S()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = '_';
        character[3][9] = character[3][10] = character[3][11] = '_';
        character[4][9] = character[4][10] = character[4][13] = '_';
        character[5][8] = character[5][9] = character[5][10] = '_';
        character[6][8] = character[6][9] = character[6][10] = character[6][11] = '_';
        character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[4][5] = character[6][5] = character[7][5] = character[3][14] = character[4][14] = character[6][14] = '|';
        character[5][12] = '-';
        character[5][7] = character[5][12] = character[7][13] = '.';
        character[5][6] = character[7][14] = '\'';
        character[5][11] = character[6][6] = '`';
        character[3][6] = '/';
        character[4][12] = character[6][7] = '\\';
        character[4][8] = '(';
        character[6][12] = ')';

        return character;
    }

    vs T()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[6][7] = character[6][11] = character[3][7] = character[3][11] = character[4][5] = character[4][13] = character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = '_';
        character[3][4] = character[3][14] = character[4][4] = character[4][14] = character[4][8] = character[4][10] = character[5][8] = character[5][10] = character[6][8] = character[6][10] = character[7][6] = character[7][12] = '|';
        character[4][6] = '/';
        character[4][12] = '\\';

        return character;
    }

    vs U()
    {
        vs character = getCharGrid(11, 20);

        character[0] = " .----------------. ";
        character[1] = "| .--------------. |";
        character[2] = "| | _____  _____ | |";
        character[3] = "| ||_   _||_   _|| |";
        character[4] = "| |  | |    | |  | |";
        character[5] = "| |  | '    ' |  | |";
        character[6] = "| |   \\ `--' /   | |";
        character[7] = "| |    `.__.'    | |";
        character[8] = "| |              | |";
        character[9] = "| '--------------' |";
        character[10] = " '----------------' ";

        return character;
    }

    vs V()
    {
        vs character = getCharGrid(11, 20);

        character[0] = " .----------------. ";
        character[1] = "| .--------------. |";
        character[2] = "| |  ____  ____  | |";
        character[3] = "| | |_  _||_  _| | |";
        character[4] = "| |   \\ \\  / /   | |";
        character[5] = "| |    \\ \\/ /    | |";
        character[6] = "| |     \\  /     | |";
        character[7] = "| |     |__|     | |";
        character[8] = "| |              | |";
        character[9] = "| '--------------' |";
        character[10] = " '----------------' ";

        return character;
    }

    vs W()
    {
        vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][4] = character[2][5] = character[2][6] = character[2][7] = character[2][12] = character[2][13] = character[2][14] = character[2][15] = character[7][6] = character[7][7] = character[7][12] = character[7][13] = character[3][4] = character[3][15] = character[3][11] = character[3][8] = character[2][11] = character[2][8] = '_';
        character[3][3] = character[3][16] = character[4][5] = character[4][14] = character[5][5] = character[5][14] = character[6][5] = character[6][14] = character[7][5] = character[3][9] = character[3][10] = character[7][14] = character[5][7] = character[5][12] = character[4][7] = character[4][12] = '|';
        character[5][8] = character[4][9] = character[7][8] = character[6][9] = '/';
        character[5][11] = character[4][10] = character[7][11] = character[6][10] = '\\';

        return character;
    }

    vs X()
    {
        vs character = getCharGrid();
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][12] = character[2][13] = character[2][14] = character[7][4] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = character[7][15] = character[3][5] = character[3][14] = character[3][11] = character[3][8] = character[6][5] = character[6][8] = character[6][11] = character[6][14] = character[2][11] = character[2][8] = '_';
        character[3][4] = character[3][15] = character[7][4] = character[7][9] = character[7][10] = character[3][9] = character[3][10] = character[7][15] = '|';
        character[4][6] = character[4][8] = character[6][11] = character[6][13] = '\\';
        character[4][11] = character[4][13] = character[6][6] = character[6][8] = '/';
        character[5][7] = '>';
        character[5][12] = '<';
        character[5][9] = character[6][10] = '`';
        character[5][10] = character[6][9] = '\'';
        return character;
    }

vs Y()
{
   vs character = getCharGrid(11, 20);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] ='.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] =  '\'';
        character[2][5]= character[2][6]= character[2][7]= character[2][8]= character[2][11]= character[2][12]= character[2][13]= character[2][14]= character[6][7]= character[6][12]= character[3][5]= character[3][8]=character[3][11]=character[3][14]='_';
for(int i=7;i<13;i++){
    character[7][i]='_';
}
character[3][4]=character[3][9]=character[3][10]=character[3][15]=character[6][8]=character[6][11]=character[7][6]=character[7][13]='|';
character[4][6]=character[4][8]=character[5][7]=character[5][9]='\\';
character[4][11]=character[4][13]=character[5][10]=character[5][12]='/';

    return character;
}


    vs Z()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[9][2] = character[9][17] = '.';
        character[10][1] = character[10][18] = '\'';

        character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[4][6] = character[3][8] = character[3][9] = character[3][13] = character[6][6] = character[6][10] = character[6][11] = character[5][13] = '_';
        character[6][14] = character[7][5] = character[3][5] = character[4][5] = character[3][14] = character[7][14] = '|';
        character[4][7] = character[4][10] = character[4][12] = character[5][8] = character[6][7] = character[6][9] = character[6][12] = '/';
        character[5][9] = character[5][11] = '\'';
        character[5][8] = character[5][10] = '.';
        character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = '_';

        return character;
    }

    // Lowercase
        vs a()
        {
        vs character = getCharGrid(11, 20);

        character[0] =  " .----------------. ";
        character[1] =  "| .--------------. |";
        character[2] =  "| |      __      | |";
        character[3] =  "| |     /  \\     | |";
        character[4] =  "| |    / /\\ \\    | |";
        character[5] =  "| |   / /__\\ \\   | |";
        character[6] =  "| | _/ /    \\ \\_ | |";
        character[7] =  "| ||____|  |____|| |";
        character[8] =  "| |              | |";
        character[9] =  "| '--------------' |";
        character[10] = " '----------------' ";

            return character;
        }



    vs b()
    {
       vs character = getCharGrid(11, 20);

       character[0] =  " .----------------. ";
       character[1] =  "| .--------------. |";
       character[2] =  "| |   ______     | |";
       character[3] =  "| |  |_   _ \\    | |";
       character[4] =  "| |    | |_) |   | |";
       character[5] =  "| |    |  __'.   | |";
       character[6] =  "| |   _| |__) |  | |";
       character[7] =  "| |  |_______/   | |";
       character[8] =  "| |              | |";
       character[9] =  "| '--------------' |";
       character[10] = " '----------------' ";

    return character;
    };

    vs c()
    {
        // vs character = getCharGrid();

        // Enter the character grid in ROWS X COLS
        vs character = getCharGrid();

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[3][9] = character[3][10] = character[3][11] = character[4][13] = character[6][9] = character[6][10] = character[6][11] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = '_';
        character[3][14] = character[4][14] = character[5][5] = character[5][7] = '|';
        character[4][12] = character[6][5] = character[6][14] = '\\';
        character[4][5] = '/';
        character[3][6] = character[4][7] = character[6][8] = character[6][12] = character[7][7] = character[7][13] = '.';
        character[3][7] = character[4][8] = character[6][13] = character[7][14] = '\'';
        character[6][7] = character[7][6] = '`';

        return character;

        return character;
    }


    vs d()
    {
        vs character = getCharGrid(11, 20);

      character[0] = " .----------------. ";
      character[1] = "| .--------------. |";
      character[2] = "| |  ________    | |";
      character[3] = "| | |_   ___ `.  | |";
      character[4] = "| |   | |   `. ) | |";
      character[5] = "| |   | |    } ) | |";
      character[6] = "| |  _| |___,' ) | |";
      character[7] = "| | |________,'  | |";
      character[8] = "| |              | |";
      character[9] = "| '..............' |";
      character[10] ="'..................' ";

        return character;
    };


    // It is same as E in Ascii-Generator
    vs e()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = character[3][5] = character[3][9] = character[3][10] = character[3][11] = character[4][9] = character[4][13] = character[5][9] = character[5][13] = character[6][5] = character[6][9] = character[6][10] = character[6][11] = '_';
        character[3][4] = character[3][14] = character[4][6] = character[4][8] = character[4][14] = character[5][6] = character[5][10] = character[6][6] = character[6][8] = character[6][14] = character[7][4] = character[7][14] = '|';
        character[4][12] = '\\';
        character[6][12] = '/';

        return character;
    }


    vs f()
    {
        vs character = getCharGrid(11, 20);

        character[0] = " .----------------. ";
        character[1] = "| .--------------. |";
        character[2] = "| |  _________   | |";
        character[3] = "| | |_   ___  |  | |";
        character[4] = "| |   | |__ `_|  | |";
        character[5] = "| |   |  __|     | |";
        character[6] = "| |  _| |_       | |";
        character[7] = "| | |_____|      | |";
        character[8] = "| |              | |";
        character[9] = "| '..............' |";
        character[10] ="'..................' ";

            return character;
        }


    vs g()
    {
        vs character = getCharGrid(11, 20);

        character[0] =  " .----------------. ";
        character[1] =  "| .--------------. |";
        character[2] =  "| |    ______    | |";
        character[3] =  "| |  .' ___  |   | |";
        character[4] =  "| | / .'   \\_|   | |";
        character[5] =  "| | | |    ____  | |";
        character[6] =  "| | \\ `.___]  _| | |";
        character[7] =  "| |  `._____.'   | |";
        character[8] =  "| |              | |";
        character[9] =  "| '--------------' |";
        character[10] = " '----------------' ";

        return character;
    }

    vs h()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][11] = character[2][12] = character[2][13] = character[2][14] = character[3][5] = character[3][14] = character[4][9] = character[4][10] = character[7][5] = character[7][6] = character[7][7] = character[7][8] = character[7][11] = character[7][12] = character[7][13] = character[7][14] = character[6][5] = character[6][14] = character[5][9] = character[5][10] = '_';
        character[3][4] = character[3][9] = character[3][10] = character[3][15] = character[4][6] = character[4][8] = character[4][11] = character[4][13] = character[5][13] = character[5][6] = character[6][6] = character[6][8] = character[6][11] = character[6][13] = character[7][4] = character[7][9] = character[7][10] = character[7][15] = '|';

        return character;
    }

    vs i()
    {
        vs character = getCharGrid();

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][8] = character[3][12] = character[6][8] = character[6][12] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[3][7] = character[3][13] = character[4][9] = character[4][11] = character[5][9] = character[5][11] = character[6][9] = character[6][11] = character[7][7] = character[7][13] = '|';

        return character;
    }

    vs j()
    {
        vs character = getCharGrid(11, 20);

        character[0] =  " .----------------. ";
        character[1] =  "| .--------------. |";
        character[2] =  "| |     _____    | |";
        character[3] =  "| |    |_   _|   | |";
        character[4] =  "| |      | |     | |";
        character[5] =  "| |   _  | |     | |";
        character[6] =  "| |  | |_' |     | |";
        character[7] =  "| |  `.___.'     | |";
        character[8] =  "| |              | |";
        character[9] =  "| '--------------' |";
        character[10] = " '----------------' ";

        return character;
    }


        vs k()
        {
            vs character = getCharGrid(11, 20);

           character[0] = " .----------------. ";
           character[1] = "| .--------------. |";
           character[2] = "| |  ___  ____   | |";
           character[3] = "| | |_  ||_  _|  | |";
           character[4] = "| |   | |_/ /    | |";
           character[5] = "| |   |  __'.    | |";
           character[6] = "| |  _| |  \\ \\_  | |";
           character[7] = "| | |____||____| | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";

            return character;
        }




        vs l()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |   _____      | |";
            character[3] = "| |  |_   _|     | |";
            character[4] = "| |    | |       | |";
            character[5] = "| |    | |   _   | |";
            character[6] = "| |   _| |__/ |  | |";
            character[7] = "| |  |________|  | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";


            return character;
        }




        vs m()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| | ____    ____ | |";
            character[3] = "| ||_   \\  /   _|| |";
            character[4] = "| |  |   \\/   |  | |";
            character[5] = "| |  | |\\  /| |  | |";
            character[6] = "| | _| |_\\/_| |_ | |";
            character[7] = "| ||_____||_____|| |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }




        vs n()
        {
            vs character = getCharGrid(11, 20);

           character[0] = " .----------------. ";
           character[1] = "| .--------------. |";
           character[2] = "| | ____  _____  | |";
           character[3] = "| ||_   \\|_   _| | |";
           character[4] = "| |  |   \\ | |   | |";
           character[5] = "| |  | |\\ \\| |   | |";
           character[6] = "| | _| |_\\   |_  | |";
           character[7] = "| ||_____|\\____| | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";


            return character;
        }




        vs o()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |     ____     | |";
            character[3] = "| |   .'    `.   | |";
            character[4] = "| |  /  .--.  \\  | |";
            character[5] = "| |  | |    | |  | |";
            character[6] = "| |  \\  `--'  /  | |";
            character[7] = "| |   `.____.'   | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }




        vs p()
        {
            vs character = getCharGrid(11, 20);

           character[0] = " .----------------. ";
           character[1] = "| .--------------. |";
           character[2] = "| |   ______     | |";
           character[3] = "| |  |_   __ \\   | |";
           character[4] = "| |    | |__) |  | |";
           character[5] = "| |    |  ___/   | |";
           character[6] = "| |   _| |_      | |";
           character[7] = "| |  |_____|     | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";


            return character;
        }




        vs q()
        {
            vs character = getCharGrid(11 ,20);

           character[0] = " .----------------. ";
           character[1] = "| .--------------. |";
           character[2] = "| |    ___       | |";
           character[3] = "| |  .'   '.     | |";
           character[4] = "| | /  .-.  \\    | |";
           character[5] = "| | | |   | |    | |";
           character[6] = "| | \\  `-'  \\_   | |";
           character[7] = "| |  `.___.\\__|  | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";

            return character;
        }




        vs r()
        {
            vs character = getCharGrid(11 ,20);

           character[0] = " .----------------.";
           character[1] = "| .--------------. |";
           character[2] = "| |  _______     | |";
           character[3] = "| | |_   __ \\    | |";
           character[4] = "| |   | |__) |   | |";
           character[5] = "| |   |  __ /    | |";
           character[6] = "| |  _| |  \\ \\_  | |";
           character[7] = "| | |____| |___| | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";

            return character;
        }




        vs s()
        {
            vs character = getCharGrid(11 ,20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |    _______   | |";
            character[3] = "| |   /  ___  |  | |";
            character[4] = "| |  |  (__ \\_|  | |";
            character[5] = "| |   '.___`-.   | |";
            character[6] = "| |  |`\\____) |  | |";
            character[7] = "| |  |_______.'  | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }



    vs t()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[6][7] = character[6][11] = character[3][7] = character[3][11] = character[4][5] = character[4][13] = character[2][5] = character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[2][13] = '_';
        character[3][4] = character[3][14] = character[4][4] = character[4][14] = character[4][8] = character[4][10] = character[5][8] = character[5][10] = character[6][8] = character[6][10] = character[7][6] = character[7][12] = '|';
        character[4][6] = '/';
        character[4][12] = '\\';

        return character;
    }


        vs u()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| | _____  _____ | |";
            character[3] = "| ||_   _||_   _|| |";
            character[4] = "| |  | |    | |  | |";
            character[5] = "| |  | '    ' |  | |";
            character[6] = "| |   \\ `--' /   | |";
            character[7] = "| |    `.__.'    | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }



        vs v()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |  ____  ____  | |";
            character[3] = "| | |_  _||_  _| | |";
            character[4] = "| |   \\ \\  / /   | |";
            character[5] = "| |    \\ \\/ /    | |";
            character[6] = "| |     \\  /     | |";
            character[7] = "| |     |__|     | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10] = " '----------------' ";

            return character;
        }


        vs w()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| | _____  _____ | |";
            character[3] = "| ||_   _||_   _|| |";
            character[4] = "| |  | | /\\ | |  | |";
            character[5] = "| |  | |/  \\| |  | |";
            character[6] = "| |  |   /\\   |  | |";
            character[7] = "| |  |__/  \\__|  | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
           character[10] = " '----------------' ";

            return character;
        }




        vs x()
        {
            vs character = getCharGrid(11, 20);

           character[0] = " .----------------. ";
           character[1] = "| .--------------. |";
           character[2] = "| |  ____  ____  | |";
           character[3] = "| | |_  _||_  _| | |";
           character[4] = "| |   \\ \\  / /   | |";
           character[5] = "| |    > `' <    | |";
           character[6] = "| |  _/ /'`\\ \\_  | |";
           character[7] = "| | |____||____| | |";
           character[8] = "| |              | |";
           character[9] = "| '--------------' |";
           character[10]= " '----------------' ";

            return character;
        }




        vs y()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |  ____  ____  | |";
            character[3] = "| | |_  _||_  _| | |";
            character[4] = "| |   \\ \\  / /   | |";
            character[5] = "| |    \\ \\/ /    | |";
            character[6] = "| |    _|  |_    | |";
            character[7] = "| |   |______|   | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }




        vs z()
        {
            vs character = getCharGrid(11, 20);

            character[0] = " .----------------. ";
            character[1] = "| .--------------. |";
            character[2] = "| |   ________   | |";
            character[3] = "| |  |  __   _|  | |";
            character[4] = "| |  |_/  / /    | |";
            character[5] = "| |     .'.' _   | |";
            character[6] = "| |   _/ /__/ |  | |";
            character[7] = "| |  |________|  | |";
            character[8] = "| |              | |";
            character[9] = "| '--------------' |";
            character[10]= " '----------------' ";

            return character;
        }



    // Numbers

    vs zero()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = '_';
        character[4][5] = character[4][14] = character[5][5] = character[5][7] = character[5][12] = character[5][14] = character[6][5] = character[6][14] = '|';
        character[4][9] = character[4][10] = character[6][9] = character[6][10] = '-';
        character[3][6] = character[3][13] = character[4][8] = character[4][11] = character[7][7] = character[7][12] = '.';
        character[3][7] = character[3][12] = character[6][11] = character[7][6] = character[7][13] = '\'';
        character[6][8] = '`';

        return character;
    }

    vs one()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[9][2] = character[9][17] = '.';
        character[10][1] = character[10][18] = '\'';
        character[2][8] = character[2][9] = character[6][7] = character[6][11] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = '_';
        character[3][10] = character[4][8] = character[4][10] = character[5][8] = character[5][10] = character[6][8] = character[6][10] = character[7][6] = character[7][12] = '|';
        character[3][7] = '/';
        character[4][7] = '`';

        return character;
    }

    vs two()
    {
        vs character = getCharGrid(11, 20);

        character[0] = " .----------------. ";
        character[1] = "| .--------------. |";
        character[2] = "| |    _____     | |";
        character[3] = "| |   / ___ `.   | |";
        character[4] = "| |  |_/___) |   | |";
        character[5] = "| |   .'____.'   | |";
        character[6] = "| |  / /____     | |";
        character[7] = "| |  |_______|   | |";
        character[8] = "| |              | |";
        character[9] = "| '--------------' |";
        character[10] = " '----------------' ";

        return character;
    }

    vs three()
    {
        vs character = getCharGrid(11, 20);

        character[0][1] = '.';
        character[0][2] = '-';
        character[0][3] = '-';
        character[0][4] = '-';
        character[0][5] = '-';
        character[0][6] = '-';
        character[0][7] = '-';
        character[0][8] = '-';
        character[0][9] = '-';
        character[0][10] = '-';
        character[0][11] = '-';
        character[0][12] = '-';
        character[0][13] = '-';
        character[0][14] = '-';
        character[0][15] = '-';
        character[0][16] = '-';
        character[0][17] = '-';
        character[0][18] = '.';

        character[1][0] = '|';
        character[1][2] = '.';
        character[1][3] = '-';
        character[1][4] = '-';
        character[1][5] = '-';
        character[1][6] = '-';
        character[1][7] = '-';
        character[1][8] = '-';
        character[1][9] = '-';
        character[1][10] = '-';
        character[1][11] = '-';
        character[1][12] = '-';
        character[1][13] = '-';
        character[1][14] = '-';
        character[1][15] = '-';
        character[1][16] = '-';
        character[1][17] = '.';
        character[1][19] = '|';

        character[2][0] = '|';
        character[2][2] = '|';
        character[2][7] = '_';
        character[2][8] = '_';
        character[2][9] = '_';
        character[2][10] = '_';
        character[2][11] = '_';
        character[2][12] = '_';
        character[2][17] = '|';
        character[2][19] = '|';

        character[3][0] = '|';
        character[3][2] = '|';
        character[3][6] = '/';
        character[3][8] = '_';
        character[3][9] = '_';
        character[3][10] = '_';
        character[3][11] = '_';
        character[3][13] = '\'';
        character[3][14] = '.';
        character[3][17] = '|';
        character[3][19] = '|';

        character[4][0] = '|';
        character[4][2] = '|';
        character[4][6] = '\'';
        character[4][7] = '\'';
        character[4][10] = '_';
        character[4][11] = '_';
        character[4][12] = ')';
        character[4][14] = '|';
        character[4][17] = '|';
        character[4][19] = '|';

        character[5][0] = '|';
        character[5][2] = '|';
        character[5][6] = '_';
        character[5][9] = '|';
        character[5][10] = '_';
        character[5][11] = '_';
        character[5][13] = '\'';
        character[5][14] = '.';
        character[5][17] = '|';
        character[5][19] = '|';

        character[6][0] = '|';
        character[6][2] = '|';
        character[6][5] = '|';
        character[6][7] = '\\';
        character[6][8] = '_';
        character[6][9] = '_';
        character[6][10] = '_';
        character[6][11] = '_';
        character[6][12] = ')';
        character[6][14] = '|';
        character[6][17] = '|';
        character[6][19] = '|';

        character[7][0] = '|';
        character[7][2] = '|';
        character[7][6] = '\\';
        character[7][7] = '_';
        character[7][8] = '_';
        character[7][9] = '_';
        character[7][10] = '_';
        character[7][11] = '_';
        character[7][12] = '_';
        character[7][13] = '.';
        character[7][14] = '\'';
        character[7][17] = '|';
        character[7][19] = '|';

        character[8][0] = '|';
        character[8][2] = '|';
        character[8][17] = '|';
        character[8][19] = '|';

        character[9][0] = '|';
        character[9][2] = '|';
        character[9][3] = '-';
        character[9][4] = '-';
        character[9][5] = '-';
        character[9][6] = '-';
        character[9][7] = '-';
        character[9][8] = '-';
        character[9][9] = '-';
        character[9][10] = '-';
        character[9][11] = '-';
        character[9][12] = '-';
        character[9][13] = '-';
        character[9][14] = '-';
        character[9][15] = '-';
        character[9][16] = '-';
        character[9][17] = '\'';
        character[9][19] = '|';

        character[10][1] = '\'';
        character[10][2] = '-';
        character[10][3] = '-';
        character[10][4] = '-';
        character[10][5] = '-';
        character[10][6] = '-';
        character[10][7] = '-';
        character[10][8] = '-';
        character[10][9] = '-';
        character[10][10] = '-';
        character[10][11] = '-';
        character[10][12] = '-';
        character[10][13] = '-';
        character[10][14] = '-';
        character[10][15] = '-';
        character[10][16] = '-';
        character[10][17] = '-';
        character[10][18] = '\'';

        return character;
    }

    vs four()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][6] = character[2][11] = character[4][8] = character[4][9] = character[4][13] = character[5][6] = character[5][7] = character[5][8] = character[5][9] = character[5][13] = character[6][9] = character[6][13] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = character[7][13] = '_';
        character[3][5] = character[3][7] = character[3][10] = character[3][12] = character[4][5] = character[4][7] = character[4][10] = character[4][12] = character[5][5] = character[5][14] = character[6][10] = character[6][12] = character[7][8] = character[7][14] = '|';

        return character;
    }

    vs five()
    {
        vs character = getCharGrid(11, 20);

        character[0][1] = '.';
        character[0][2] = '-';
        character[0][3] = '-';
        character[0][4] = '-';
        character[0][5] = '-';
        character[0][6] = '-';
        character[0][7] = '-';
        character[0][8] = '-';
        character[0][9] = '-';
        character[0][10] = '-';
        character[0][11] = '-';
        character[0][12] = '-';
        character[0][13] = '-';
        character[0][14] = '-';
        character[0][15] = '-';
        character[0][16] = '-';
        character[0][17] = '-';
        character[0][18] = '.';
        character[1][0] = '|';
        character[1][2] = '.';
        character[1][3] = '-';
        character[1][4] = '-';
        character[1][5] = '-';
        character[1][6] = '-';
        character[1][7] = '-';
        character[1][8] = '-';
        character[1][9] = '-';
        character[1][10] = '-';
        character[1][11] = '-';
        character[1][12] = '-';
        character[1][13] = '-';
        character[1][14] = '-';
        character[1][15] = '-';
        character[1][16] = '-';
        character[1][17] = '.';
        character[1][19] = '|';
        character[2][0] = '|';
        character[2][2] = '|';
        character[2][6] = '_';
        character[2][7] = '_';
        character[2][8] = '_';
        character[2][9] = '_';
        character[2][10] = '_';
        character[2][11] = '_';
        character[2][12] = '_';
        character[2][17] = '|';
        character[2][19] = '|';
        character[3][0] = '|';
        character[3][2] = '|';
        character[3][5] = '|';
        character[3][8] = '_';
        character[3][9] = '_';
        character[3][10] = '_';
        character[3][11] = '_';
        character[3][12] = '_';
        character[3][13] = '|';
        character[3][17] = '|';
        character[3][19] = '|';
        character[4][0] = '|';
        character[4][2] = '|';
        character[4][5] = '|';
        character[4][7] = '|';
        character[4][8] = '_';
        character[4][9] = '_';
        character[4][10] = '_';
        character[4][11] = '_';
        character[4][17] = '|';
        character[4][19] = '|';
        character[5][0] = '|';
        character[5][2] = '|';
        character[5][5] = '\'';
        character[5][6] = '_';
        character[5][7] = '.';
        character[5][8] = '_';
        character[5][9] = '_';
        character[5][10] = '_';
        character[5][11] = '_';
        character[5][12] = '\'';
        character[5][13] = '\'';
        character[5][14] = '.';
        character[5][17] = '|';
        character[5][19] = '|';
        character[6][0] = '|';
        character[6][2] = '|';
        character[6][5] = '|';
        character[6][7] = '\\';
        character[6][8] = '_';
        character[6][9] = '_';
        character[6][10] = '_';
        character[6][11] = '_';
        character[6][12] = ')';
        character[6][14] = '|';
        character[6][17] = '|';
        character[6][19] = '|';
        character[7][0] = '|';
        character[7][2] = '|';
        character[7][6] = '\\';
        character[7][7] = '_';
        character[7][8] = '_';
        character[7][9] = '_';
        character[7][10] = '_';
        character[7][11] = '_';
        character[7][12] = '_';
        character[7][13] = '.';
        character[7][14] = '\'';
        character[7][17] = '|';
        character[7][19] = '|';
        character[8][0] = '|';
        character[8][2] = '|';
        character[8][17] = '|';
        character[8][19] = '|';
        character[9][0] = '|';
        character[9][2] = '\'';
        character[9][3] = '-';
        character[9][4] = '-';
        character[9][5] = '-';
        character[9][6] = '-';
        character[9][7] = '-';
        character[9][8] = '-';
        character[9][9] = '-';
        character[9][10] = '-';
        character[9][11] = '-';
        character[9][12] = '-';
        character[9][13] = '-';
        character[9][14] = '-';
        character[9][15] = '-';
        character[9][16] = '-';
        character[9][17] = '\'';
        character[9][19] = '|';
        character[10][1] = '\'';
        character[10][2] = '-';
        character[10][3] = '-';
        character[10][4] = '-';
        character[10][5] = '-';
        character[10][6] = '-';
        character[10][7] = '-';
        character[10][8] = '-';
        character[10][9] = '-';
        character[10][10] = '-';
        character[10][11] = '-';
        character[10][12] = '-';
        character[10][13] = '-';
        character[10][14] = '-';
        character[10][15] = '-';
        character[10][16] = '-';
        character[10][17] = '-';
        character[10][18] = '\'';

        return character;
    }

    vs six()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';
        character[0][1] = character[0][18] = character[1][2] = character[1][17] = '.';
        character[10][1] = character[10][18] = character[9][2] = character[9][17] = '\'';

        character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[3][8] = character[3][9] = character[3][10] = character[3][11] = character[4][8] = character[4][9] = character[4][10] = character[4][11] = character[4][13] = character[5][8] = character[5][9] = character[5][10] = character[5][11] = character[6][8] = character[6][9] = character[6][10] = character[6][11] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[3][6] = character[5][7] = character[5][13] = character[7][5] = character[7][14] = '\'';
        character[4][7] = character[4][5] = character[5][5] = character[6][5] = character[4][14] = character[6][14] = '|';
        character[3][13] = character[4][12] = '\\';
        character[6][7] = '(';
        character[6][12] = ')';
        character[3][5] = character[7][6] = character[5][14] = character[7][13] = '.';
        character[5][12] = '`';
        return character;
    }

    vs seven()
    {
        vs character = getCharGrid(11, 20);

        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[10][1] = character[10][18] = '.';
        character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';
        character[3][5] = character[3][13] = character[4][5] = '|';
        character[2][6] = character[2][7] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[2][12] = character[4][6] = character[3][8] = character[3][9] = character[3][10] = character[7][8] = '_';
        character[4][7] = character[4][10] = character[5][9] = character[6][8] = character[7][7] = character[7][9] = character[6][10] = character[5][11] = character[4][12] = '/';

        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(11, 20);
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[0][6] = character[0][7] = character[0][8] = character[0][9] = character[0][10] = character[0][11] = character[0][12] = character[0][13] = character[0][14] = character[0][15] = character[0][16] = character[0][17] = character[1][3] = character[1][4] = character[1][5] = character[1][6] = character[1][7] = character[1][8] = character[1][9] = character[1][10] = character[1][11] = character[1][12] = character[1][13] = character[1][14] = character[1][15] = character[1][16] = '-';
        character[9][3] = character[9][4] = character[9][5] = character[9][6] = character[9][7] = character[9][10] = character[9][11] = character[9][12] = character[9][13] = character[9][14] = character[9][15] = character[9][16] = character[10][2] = character[10][3] = character[10][4] = character[10][5] = character[10][6] = character[10][7] = character[10][8] = character[10][9] = character[10][10] = character[10][11] = character[10][12] = character[10][13] = character[10][14] = character[10][15] = character[10][16] = character[10][17] = character[9][8] = character[9][9] = '-';
        character[1][0] = character[2][0] = character[3][0] = character[4][0] = character[5][0] = character[6][0] = character[7][0] = character[8][0] = character[9][0] = character[2][2] = character[3][2] = character[4][2] = character[5][2] = character[6][2] = character[7][2] = character[6][5] = character[4][6] = character[4][13] = character[8][2] = '|';
        character[1][19] = character[2][19] = character[3][19] = character[4][19] = character[5][19] = character[6][19] = character[7][19] = character[8][19] = character[9][19] = character[2][17] = character[3][17] = character[4][17] = character[5][17] = character[6][17] = character[7][17] = character[6][14] = character[8][17] = '|';

        character[0][1] = character[0][18] = character[1][2] = character[1][17] = character[3][6] = character[3][13] = character[10][1] = character[7][6] = character[7][13] = character[5][13] = character[5][6] = character[10][18] = '.';
        character[5][7] = character[7][5] = '`';
        character[4][9] = character[4][10] = character[3][9] = character[3][10] = character[2][8] = character[2][9] = character[2][10] = character[2][11] = character[5][8] = character[5][9] = character[5][10] = character[5][11] = character[6][8] = character[6][9] = character[6][10] = character[6][11] = character[7][7] = character[7][8] = character[7][9] = character[7][10] = character[7][11] = character[7][12] = '_';
        character[4][8] = character[6][7] = '(';
        character[4][11] = character[6][12] = ')';
        character[7][14] = character[5][12] = character[3][7] = character[3][12] = character[10][1] = character[9][2] = character[9][17] = character[10][18] = '\'';

        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(11, 20);

        character[0][1] = '.';
        character[0][2] = '-';
        character[0][3] = '-';
        character[0][4] = '-';
        character[0][5] = '-';
        character[0][6] = '-';
        character[0][7] = '-';
        character[0][8] = '-';
        character[0][9] = '-';
        character[0][10] = '-';
        character[0][11] = '-';
        character[0][12] = '-';
        character[0][13] = '-';
        character[0][14] = '-';
        character[0][15] = '-';
        character[0][16] = '-';
        character[0][17] = '-';
        character[0][18] = '.';
        character[1][0] = '|';
        character[1][2] = '.';
        character[1][3] = '-';
        character[1][4] = '-';
        character[1][5] = '-';
        character[1][6] = '-';
        character[1][7] = '-';
        character[1][8] = '-';
        character[1][9] = '-';
        character[1][10] = '-';
        character[1][11] = '-';
        character[1][12] = '-';
        character[1][13] = '-';
        character[1][14] = '-';
        character[1][15] = '-';
        character[1][16] = '-';
        character[1][17] = '.';
        character[1][19] = '|';
        character[2][0] = '|';
        character[2][2] = '|';
        character[2][7] = '_';
        character[2][8] = '_';
        character[2][9] = '_';
        character[2][10] = '_';
        character[2][11] = '_';
        character[2][12] = '_';
        character[2][17] = '|';
        character[2][19] = '|';
        character[3][0] = '|';
        character[3][2] = '|';
        character[3][5] = '.';
        character[3][6] = '\'';
        character[3][8] = '_';
        character[3][9] = '_';
        character[3][10] = '_';
        character[3][11] = '_';
        character[3][13] = '\'';
        character[3][14] = '.';
        character[3][17] = '|';
        character[3][19] = '|';
        character[4][0] = '|';
        character[4][2] = '|';
        character[4][5] = '|';
        character[4][7] = '(';
        character[4][8] = '_';
        character[4][9] = '_';
        character[4][10] = '_';
        character[4][11] = '_';
        character[4][12] = ')';
        character[4][14] = '|';
        character[4][17] = '|';
        character[4][19] = '|';
        character[5][0] = '|';
        character[5][2] = '|';
        character[5][5] = '\'';
        character[5][6] = '_';
        character[5][7] = '.';
        character[5][8] = '_';
        character[5][9] = '_';
        character[5][10] = '_';
        character[5][11] = '_';
        character[5][12] = '.';
        character[5][14] = '|';
        character[5][17] = '|';
        character[5][19] = '|';
        character[6][0] = '|';
        character[6][2] = '|';
        character[6][5] = '|';
        character[6][7] = '\\';
        character[6][8] = '_';
        character[6][9] = '_';
        character[6][10] = '_';
        character[6][11] = '_';
        character[6][12] = '|';
        character[6][14] = '|';
        character[6][17] = '|';
        character[6][19] = '|';
        character[7][0] = '|';
        character[7][2] = '|';
        character[7][6] = '\\';
        character[7][7] = '_';
        character[7][8] = '_';
        character[7][9] = '_';
        character[7][10] = '_';
        character[7][11] = '_';
        character[7][12] = '_';
        character[7][13] = ',';
        character[7][14] = '\'';
        character[7][17] = '|';
        character[7][19] = '|';
        character[8][0] = '|';
        character[8][2] = '|';
        character[8][17] = '|';
        character[8][19] = '|';
        character[9][0] = '|';
        character[9][2] = '\'';
        character[9][3] = '-';
        character[9][4] = '-';
        character[9][5] = '-';
        character[9][6] = '-';
        character[9][7] = '-';
        character[9][8] = '-';
        character[9][9] = '-';
        character[9][10] = '-';
        character[9][11] = '-';
        character[9][12] = '-';
        character[9][13] = '-';
        character[9][14] = '-';
        character[9][15] = '-';
        character[9][16] = '-';
        character[9][17] = '\'';
        character[9][19] = '|';
        character[10][1] = '\'';
        character[10][2] = '-';
        character[10][3] = '-';
        character[10][4] = '-';
        character[10][5] = '-';
        character[10][6] = '-';
        character[10][7] = '-';
        character[10][8] = '-';
        character[10][9] = '-';
        character[10][10] = '-';
        character[10][11] = '-';
        character[10][12] = '-';
        character[10][13] = '-';
        character[10][14] = '-';
        character[10][15] = '-';
        character[10][16] = '-';
        character[10][17] = '-';
        character[10][18] = '\'';

        return character;
    }
};
#endif

#ifndef AMONGUS_H
#define AMONGUS_H


class Amongus : public Fonts
{
    static const int ROWS = 4;
    static const int COLS = 9;

public:
    Amongus() : Fonts(ROWS, COLS) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(4, 8);

        character[0][3] = character[0][4] =
            character[1][3] = character[1][4] =
                character[2][3] = character[2][4] =
                    character[3][1] = character[3][2] = character[3][5] = character[3][6] = '_';
        character[1][2] = character[2][1] = '/';
        character[1][5] = character[2][6] = '\\';
        character[3][0] = character[2][2] = character[3][4] = '(';
        character[2][5] = character[3][3] = character[3][7] = ')';

        return character;
    }

    vs B()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][3] =
            character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = ')';
        character[1][5] = '\\';
        character[2][5] = '<';
        character[3][5] = '/';

        return character;
    }

    vs C()
    {
        vs character = getCharGrid(4, 6);

        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[1][4] =
            character[2][3] = character[2][4] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][5] = character[3][5] = ')';
        character[2][0] = character[2][2] = '(';
        character[1][1] = '/';
        character[3][1] = '\\';

        return character;
    }

    vs D()
    {
        vs character = getCharGrid(4, 7);
        character[0][1] = character[0][4] = character[0][2] = character[0][3] = character[1][3] = character[2][3] =
            character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = character[2][2] = '(';
        character[2][1] = character[2][4] = character[2][6] = ')';
        character[1][5] = '\\';
        character[3][5] = '/';

        // Enter the character grid in ROWS X COLS

        return character;
    }

    vs E()
    {
        vs character = getCharGrid(4, 6);
        character[1][0] = character[3][0] = '(';
        character[1][5] = character[2][1] = character[2][4] = character[3][5] = ')';
        character[1][2] = character[1][3] = character[1][4] = character[2][2] = character[2][3] = character[3][2] = character[3][3] = character[3][4] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = '_';

        return character;
    }

    vs F()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][2] = character[1][3] = character[1][4] = character[2][2] = character[2][3] = character[3][1] = character[3][2] = '_';
        character[1][0] = character[3][0] = '(';
        character[1][5] = character[2][1] = character[2][4] = character[3][3] = ')';
        return character;
    }

    vs G()
    {
        vs character = getCharGrid(4, 6);
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[1][4] = character[2][3] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[2][0] = character[2][2] = '(';
        character[1][5] = ')';
        character[1][1] = character[3][5] = '/';
        character[3][1] = '\\';
        character[2][4] = '-';
        character[2][5] = '.';

        // Enter the character grid in ROWS X COLS

        return character;
    }

    vs H()
    {
        vs character = getCharGrid(4, 7);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][5] = character[1][3] = character[2][3] = character[3][1] = character[3][5] = '_';
        character[1][0] = character[1][4] = character[2][5] = character[3][0] = character[3][4] = '(';
        character[1][2] = character[1][6] = character[2][1] = character[3][2] = character[3][6] = ')';
        return character;
    }

    vs I()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][4] = character[2][1] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = character[2][3] = '(';
        character[1][5] = character[3][5] = character[2][2] = ')';

        return character;
    }

    vs J()
    {
        vs character = getCharGrid(4, 7);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][5] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][1] = character[2][4] = '(';
        character[1][6] = character[2][3] = character[3][5] = ')';
        character[2][0] = '.';
        character[2][1] = '-';
        character[3][0] = '\\';
        return character;
    }

    vs K()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[2][4] = character[3][0] = '(';
        character[1][2] = character[1][5] = character[2][1] = character[3][2] = character[3][5] = ')';
        character[1][3] = '/';
        character[3][3] = '\\';
        // Enter the character grid in ROWS X COLS

        return character;
    }

    vs L()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[2][2] = character[3][0] = '(';
        character[1][3] = character[2][1] = character[3][5] = ')';
        return character;
    }

    vs M()
    {
        vs character = getCharGrid(4, 8);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[3][1] = character[3][6] = '_';
        character[1][0] = character[2][6] = character[3][0] = '(';
        character[1][7] = character[2][1] = character[3][7] = ')';
        character[1][3] = character[3][3] = character[3][5] = '\\';
        character[1][4] = character[3][2] = character[3][4] = '/';
        return character;
    }

    vs N()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[1][3] = character[2][4] = character[3][0] = '(';
        character[1][5] = character[2][1] = character[3][2] = character[3][5] = ')';
        character[1][2] = character[3][3] = '\\';

        // Enter the character grid in ROWS X COLS

        return character;
    }

    vs O()
    {
        vs character = getCharGrid(4, 7);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] =
            character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] =
                character[1][3] = character[2][3] = '_';
        character[1][0] = character[3][0] = character[2][2] = character[2][5] = '(';
        character[1][6] = character[2][1] = character[2][4] = character[3][6] = ')';
        return character;
    }

    vs P()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][2] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[3][3] = ')';
        character[1][5] = '\\';
        character[2][5] = '/';

        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(4, 7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] =
            character[3][1] = character[3][2] = character[3][3] = character[1][3] = character[2][3] = '_';
        character[1][0] = character[3][0] = character[2][2] = character[2][5] = '(';
        character[1][6] = character[2][1] = character[2][4] = ')';
        character[3][4] = '/';
        character[3][5] = character[3][6] = '\\';
        return character;
    }

    vs R()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[3][2] = character[3][5] = ')';
        character[3][3] = character[1][5] = '\\';
        character[2][5] = '/';
        return character;
    }

    vs S()
    {
        vs character = getCharGrid(4, 5);

        character[0][1] = character[0][2] = character[0][3] =
            character[1][2] = character[1][3] =
                character[2][1] = character[2][2] =
                    character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][4] = '/';
        character[1][1] = character[2][3] = ' ';
        character[2][0] = character[2][4] = '\\';
        character[1][4] = ')';
        character[3][0] = '(';

        return character;
    }

    vs T()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] =
            character[1][1] = character[1][4] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][3] = character[3][1] = '(';
        character[1][5] = character[2][2] = character[3][4] = ')';
        return character;
    }

    vs U()
    {
        vs character = getCharGrid(4, 8);
        character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = '_';
        character[1][0] = character[1][4] = character[2][2] = character[2][6] = character[3][0] = '(';
        character[1][3] = character[1][7] = character[2][1] = character[2][5] = character[3][7] = ')';

        // Enter the character grid in ROWS X COLS

        return character;
    }

    vs V()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][4] = '_';
        character[1][2] = character[2][1] = character[3][2] = '\\';
        character[1][0] = '(';
        character[1][3] = character[2][4] = character[3][3] = '/';
        character[1][5] = ')';
        return character;
    }

    vs W()
    {
        vs character = getCharGrid(4, 8);
        character[0][1] = character[0][6] = character[3][1] = character[3][2] = character[3][5] = character[3][6] = '_';
        character[1][0] = character[3][0] = '(';
        character[1][7] = character[3][7] = ')';
        character[1][3] = character[1][5] = character[3][3] = '/';
        character[1][2] = character[1][4] = character[3][4] = '\\';
        character[2][1] = ')';
        character[2][6] = '(';
        return character;
    }

    vs X()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[2][4] = character[3][0] = '(';
        character[1][2] = character[3][3] = '\\';
        character[1][3] = character[3][2] = '/';
        character[1][5] = character[2][1] = character[3][5] = ')';
        return character;
    }

    vs Y()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS

        character[0][1] = character[0][4] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][1] = '(';
        character[1][5] = character[3][4] = ')';
        character[1][2] = character[2][1] = '\\';
        character[1][3] = character[2][4] = '/';
        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(5, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[1][5] = character[3][5] = ')';
        character[2][1] = character[2][3] = '/';

        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(4, 8);

        character[0][3] = character[0][4] =
            character[1][3] = character[1][4] =
                character[2][3] = character[2][4] =
                    character[3][1] = character[3][2] = character[3][5] = character[3][6] = '_';
        character[1][2] = character[2][1] = '/';
        character[1][5] = character[2][6] = '\\';
        character[3][0] = character[2][2] = character[3][4] = '(';
        character[2][5] = character[3][3] = character[3][7] = ')';

        return character;
    }

    vs b()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = ')';
        character[1][5] = '\\';
        character[3][5] = '/';
        character[2][5] = '<';
        return character;
    }

    vs c()
    {
        vs character = getCharGrid(4, 6);
        character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[1][4] = character[2][3] = character[2][4] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][1] = '/';
        character[3][1] = '\\';
        character[2][0] = character[2][2] = '(';
        character[1][5] = character[3][5] = ')';
        return character;
    }

    vs d()
    {
        vs character = getCharGrid(4, 7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[3][4] =
            character[3][1] = character[3][2] = character[3][3] = character[1][3] = character[2][3] = '_';
        character[1][0] = character[3][0] = character[2][2] = '(';
        character[2][1] = character[2][4] = character[2][6] = ')';
        character[3][5] = '/';
        character[1][5] = '\\';
        return character;
    }

    vs e()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][2] = character[1][3] = character[1][4] = character[2][2] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[1][5] = character[2][4] = character[3][5] = ')';
        return character;
    }
    vs f()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][2] = character[1][3] = character[1][4] = character[2][2] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[1][5] = character[2][4] = character[3][5] = ')';
        return character;
    }


        vs g()
        {
            vs character = getCharGrid(4,7);

            character[0][2]= character[0][3] = character[0][4]= character[1][3] = character[1][4] = character[2][3] = character[3][2] = character[3][3] =character[3][4] ='_';
            character[1][1] =character[3][5] = '/';
            character[1][5] = ')';
            character[2][0] = character[2][2] = '(';
            character[2][4] = '-';
            character[2][5] = '.';
            character[3][1] = '\\';

            return character;
        }



    vs h()
    {
        vs character = getCharGrid(4,7);

        //Enter the character grid in ROWS X COLS

        character[0][1] = character[0][5]= character[1][3]= character[2][3]=character[3][5] = character[3][1]='_';
        character[1][0] = character[1][4] =character[3][0] = character[3][4] = character[2][5]='(';
        character[1][6] = character[1][2] = character[3][2] = character[3][6] = character[2][1] = ')';

        return character;
    }

        vs i()
        {
            vs character = getCharGrid(4,6);
            character[0][1]= character[0][2]= character[0][3]= character[0][4]= character[1][1]= character[1][4]= character[2][1]= character[2][4]= character[3][1]= character[3][2]= character[3][3]= character[3][4]='_';
            character[1][0]= character[2][3]= character[3][0]='(';
            character[1][5]= character[2][2]= character[3][5]=')';
            return character;
        }




    vs j()
    {
        vs character = getCharGrid(4, 7);
        character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][2] = character[1][5] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][1] = character[2][4] = '(';
        character[1][6] = character[2][3] = character[3][5] = ')';
        character[2][0] = '.';
        character[3][0] = '\\';
        return character;
    }

    vs k()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[2][4] = character[3][0] = '(';
        character[1][2] = character[1][5] = character[2][1] = character[3][2] = character[3][5] = ')';
        character[1][3] = '/';
        character[3][3] = '\\';
        return character;
    }

    vs l()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[2][2] = character[3][0] = '(';
        character[1][3] = character[2][1] = character[3][5] = ')';
        return character;
    }

    vs m()
    {
        vs character = getCharGrid(4, 8);
        character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[3][1] = character[3][6] = '_';
        character[1][0] = character[2][6] = character[3][0] = '(';
        character[1][7] = character[2][1] = character[3][7] = ')';
        character[1][4] = character[3][2] = character[3][4] = '/';
        character[1][3] = character[3][3] = character[3][5] = '\\';
        return character;
    }

    vs n()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[1][3] = character[3][0] = character[2][4] = '(';
        character[1][5] = character[2][1] = character[3][2] = character[3][5] = ')';
        character[1][2] = character[3][3] = '\\';
        // character[0][0] = character[0][2] = character[0][3] = character[0][5] = character[1][1] = character[1][4] = character[2][0] =character[2][2] =character[2][3] = character[2][5]  = ' ';

        return character;
    }

    vs o()
    {
        vs character = getCharGrid(4, 7);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = '_';
        character[1][0] = character[2][2] = character[2][5] = character[3][0] = '(';
        character[1][6] = character[2][1] = character[2][4] = character[3][6] = ')';
        return character;
    }

    vs p()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[2][2] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = '_';
        character[1][5] = '\\';
        character[3][3] = character[2][1] = ')';
        character[1][0] = character[3][0] = '(';
        character[2][5] = '/';

        return character;
    }

    vs q()
    {
        vs character = getCharGrid(4, 7);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[0][5] = character[1][3] = character[3][3] = character[2][3] = character[3][1] = character[3][2] = '_';
        character[3][5] = character[3][6] = '\\';
        character[1][6] = character[2][1] = character[2][4] = ')';
        character[1][0] = character[2][2] = character[2][5] = character[3][0] = '(';
        character[3][4] = '/';

        return character;
    }

    vs r()
    {
        vs character = getCharGrid(4, 6);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][3] = character[3][1] = character[3][4] = '_';
        character[1][5] = character[3][3] = '\\';
        character[3][5] = character[2][1] = character[3][2] = ')';
        character[1][0] = character[3][0] = '(';
        character[2][5] = '/';

        return character;
    }

    vs s()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[1][3] = character[2][1] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][4] = '/';
        character[2][0] = character[2][4] = '\\';
        character[1][4] = ')';
        character[3][0] = '(';
        return character;
    }

    vs t()
    {
        vs character = getCharGrid(4,6);

        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][4] = character[3][2] = character[3][3] ='_';
        character[1][0] = character[2][3] = character[3][1] = '(';
        character[1][5] = character[2][2] = character[3][4] = ')';


        return character;
    }

    vs u()
        {
            vs character = getCharGrid(4,8);

            character[0][1] = character[0][2] = character[0][5] = character[0][6] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = character[3][5] = character[3][6] = '_';
            character[1][0] = character[1][4] = character[2][2] = character[2][6] = character[3][0] = '(';
            character[1][3] = character[1][7] = character[2][1] = character[2][5] = character[3][7] = ')';
            return character;
        }

    vs v()
        {
            vs character = getCharGrid(4,6);

            character[0][1] = character[0][4] = '_';
            character[1][0] = '(';
            character[1][5] = ')';
            character[1][2] = character[2][1] = character[3][2] = '\\';
            character[1][3] = character[2][4] = character[3][3] = '/';
            return character;
        }


  vs w()
        {
            vs character = getCharGrid(4,8);

            character[0][1] = character[0][6] = character[3][1] = character[3][2] = character[3][5] = character[3][6] = '_';
            character[1][0] = character[2][6] = character[3][0] = '(';
            character[1][7] = character[2][1] = character[3][7] = ')';
            character[1][2] = character[1][4] = character[3][4] = '\\';
            character[1][3] = character[1][5] = character[3][3] = '/';
            return character;
        }
    vs x()
    {
        vs character = getCharGrid(4, 6);

        character[0][1] = character[0][4] = character[3][1] = character[3][4] = '_';
        character[1][0] = character[3][0] = character[2][4] = '(';
        character[1][5] = character[3][5] = character[2][1] = ')';
        character[1][2] = character[3][3] = '\\';
        character[1][3] = character[3][2] = '/';

        return character;
    }


    vs y()
    {
        vs character = getCharGrid(4,6);

        character[0][1] = character[0][4] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][1] = '(';
        character[1][5] = character[3][4] = ')';
        character[1][2] = character[2][1] = '\\';
        character[1][3] = character[2][4] = '/';

        return character;
    }



    vs z()
    {
        vs character = getCharGrid(ROWS, COLS);
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[2][3] = '/';
        character[1][5] = character[3][5] = ')';
        return character;
    }

    // Numbers

    vs zero()
    {
        vs character = getCharGrid(4, 7);
        character[0][2] = character[0][3] = character[0][4] = character[3][2] = character[3][3] =
            character[3][4] = character[1][3] = character[2][3] = '_';
        character[1][1] = character[3][5] = '/';
        character[1][5] = character[3][1] = '\\';
        character[2][0] = character[2][2] = '(';
        character[2][4] = character[2][6] = ')';

        return character;
    }
    vs one()
    {
        vs character = getCharGrid(4, 4);

        character[0][1] = character[0][2] = character[3][1] = character[3][2] = '_';
        character[1][0] = '/';
        character[1][3] = character[2][1] = character[3][3] = ')';
        character[2][2] = character[3][0] = '(';
        return character;
    }

    vs two()
    {
        vs character = getCharGrid(4, 6);
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = character[3][4] = '_';
        character[1][0] = character[3][0] = '(';
        character[2][1] = character[2][4] = '/';
        character[1][4] = '\\';
        character[3][5] = ')';

        return character;
    }

    vs three()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][0] = character[2][1] = '(';
        character[3][4] = '/';
        character[2][4] = '\\';
        character[1][4] = ')';
        return character;
    }

    vs four()
    {
        vs character = getCharGrid(4, 6);
        character[0][2] = character[0][3] = character[2][4] = character[2][1] = character[3][3] = '_';
        character[2][0] = character[3][2] = '(';
        character[1][1] = '/';
        character[1][4] = '|';
        character[1][2] = '.';
        character[2][5] = character[3][4] = ')';
        return character;

        return character;
    }

    vs five()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[1][3] = character[2][1] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][0] = '|';
        character[1][4] = ')';
        character[3][0] = '(';
        character[2][4] = '\\';
        character[3][4] = '/';
        return character;
    }

    vs six()
    {
        vs character = getCharGrid(ROWS, COLS);

        // Enter the character grid in ROWS X COLS
        character[0][2] = character[2][2] = '_';
        character[1][1] = character[2][0] = character[3][4] = '/';
        character[3][0] = character[2][4] = '\\';
        character[1][3] = ')';
        character[3][1] = character[3][2] = character[3][3] = '_';

        return character;
    }

    vs seven()
    {
        vs character = getCharGrid(4, 5);

        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][2] = character[3][1] = '_';
        character[2][1] = character[2][3] = character[3][2] = '/';
        character[1][0] = character[3][0] = '(';
        character[1][4] = ')';
        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[2][2] =
            character[3][1] = character[3][2] = character[3][3] = '_';
        character[2][0] = character[3][4] = '/';
        character[2][4] = character[3][0] = '\\';
        character[1][0] = '(';
        character[1][4] = ')';

        return character;
    }


        vs nine()
        {
            vs character = getCharGrid(4,5);
            character[0][1] = character[0][2] = character[0][3] =  character[1][2] =  character[2][1] =  character[3][2] = '_';
            character[1][0] = character[2][4] =  character[3][3] = '/';
             character[1][4] =  character[2][0] = '\\';
              character[3][1] = '(';


            return character;
        }


};
#endif

#ifndef DRPEPPER_H
#define DRPEPPER_H


class Drpepper : public Fonts
{
    static const int rows = 4;
    static const int cols = 7;

public:
    Drpepper() : Fonts(rows, cols) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(4, 5);

        character[1][0] = character[1][4] = character[2][0] = character[2][4] = character[3][0] = character[3][2] = character[3][4] = '|';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][3] = '_';
        character[1][2] = '.';
        return character;
    }
    vs B()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][3] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = '|';
        character[1][2] = character[2][2] = '.';
        character[1][4] = '>';
        character[2][4] = '\\';
        character[3][4] = '/';
        return character;
    }

    vs C()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][2] = character[2][1] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[1][3] = character[2][3] = character[2][4] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][0] = '|';
        character[1][4] = '>';
        character[2][2] = '<';
        character[3][0] = '`';
        character[3][4] = '/';
        return character;
    }

    vs D()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][3] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[2][2] = character[2][4] = character[3][0] = '|';
        character[1][2] = '.';
        character[1][4] = '\\';
        character[3][4] = '/';
        return character;
    }

    vs E()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[2][1] = character[2][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[1][3] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = '|';
        character[1][4] = character[2][3] = character[3][4] = '>';
        return character;
    }

    vs F()
    {
        vs character = getCharGrid(4, 5);

        character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[1][3] = character[2][2] = character[3][1] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[3][2] = '|';
        character[1][4] = character[2][3] = '>';

        return character;
    }

    vs G()
    {
        vs character = getCharGrid(4, 6);

        character[0] = " ___  ";
        character[1] = "/  _> ";
        character[2] = "| <_/\\";
        character[3] = "`____/";
        return character;
    }

    vs H()
    {
        vs character = getCharGrid(4, 5);

        character[1][0] = character[1][2] = character[1][4] = character[2][0] = character[2][4] = character[3][0] = character[3][2] = character[3][4] = '|';
        character[0][1] = character[0][3] = character[3][1] = character[3][3] = '_';

        return character;
    }

    vs I()
    {
        vs character = getCharGrid(5, 3);
        character[0][0] = character[0][2] = character[1][1] = character[2][1] = character[4][0] = character[4][1] = character[4][2] = ' ';
        character[0][1] = character[3][1] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][2] = '|';
        return character;
    }

    vs J()
    {
        vs character = getCharGrid(4, 4);
        character[1][1] = character[1][3] = character[2][1] = character[2][3] = '|';
        character[2][0] = character[3][1] = character[3][2] = character[0][2] = '_';
        character[3][0] = '\\';
        character[3][3] = '/';
        return character;
    }

    vs K()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][3] = character[0][4] = character[3][1] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = '|';
        character[1][2] = character[1][4] = '/';
        character[2][3] = character[3][2] = character[3][4] = '\\';
        return character;
    }

    vs L()
    {
        vs character = getCharGrid(5, 5);
        character[0][1] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][4] = '|';
        return character;
    }

    vs M()
    {
        vs character = getCharGrid(4, 7);
        character[0][0] = character[0][3] = character[0][6] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = ' ';
        character[0][1] = character[0][2] = character[0][4] = character[0][5] = character[3][1] = character[3][3] = character[3][5] = '_';
        character[1][0] = character[2][0] = character[2][6] = character[3][0] = character[3][2] = character[3][4] = character[3][6] = '|';
        character[1][3] = character[1][6] = '\\';
        return character;
    }


        vs N()
        {
            vs character = getCharGrid(4,7);

            //Enter the character grid in ROWS X COLS
            character[0][0] = character[0][3] = character[0][6] = character[1][1] = character[1][2] = character[1][4] = character[1][5] = character[2][1] = character[2][2] = character[2][3] = character[2][4] = character[2][5] = ' ';
        character[0][1]  = character[0][3] = character[3][1] = character[3][3]  = '_';
        character[1][0] = character[2][0] = character[3][0] =character[1][4]=character[2][4]=character[3][4] = '|';
        character[1][2] = character[3][2] = '\\';
            return character;
        }

    vs O()
    {
        vs character = getCharGrid(4, 6);

        character[0] = " ___ ";
        character[1] = "| . |";
        character[2] = "| | |";
        character[3] = "`___'";

        return character;
    }

    vs P()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][2] = character[3][3] = character[3][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] =  character[2][3] = character[3][1] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[3][2] = '|';
        character[1][2] = '.';
        character[1][4] = '\\';
        character[2][4] = '/';

        return character;
    }

    vs Q()
    {
        vs character = getCharGrid(5, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][3] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[1][4] = character[2][0] = character[2][2] = character[2][4] = '|';
        character[1][2] = '.';
        character[3][0] = '`';
        character[3][4] = '\\';
        return character;
    }

    vs R()
    {
        vs character = getCharGrid(5, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][2] = character[2][3] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][3] = '_';
        character[1][0] = character[2][0] = character[3][0] = '|';
        character[1][2] = '.';
        character[1][4] = character[3][2] = character[3][4] = '\\';
        character[2][4] = '/';
        return character;
    }

    vs S()
    {

        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[1][2] = character[1][1] = character[3][2] = character[3][1] = character[2][1] = character[0][3] = character[1][3] = character[3][3] = character[2][2] = '_';
        character[3][4] = character[1][0] = '/';
        character[1][4] = '>';
        character[3][0] = '<';
        character[2][0] = character[2][4] = char(92);
        return character;
    }

    vs T()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][2] = character[2][0] = character[2][2] = character[2][4] = character[3][0] = character[3][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[1][3] = character[3][2] = '_';
        character[1][0] = character[1][4] = character[2][1] = character[2][3] = character[3][1] = character[3][3] = '|';
        return character;
    }

    vs U()
        {
            vs character = getCharGrid(rows,cols);

            character[0][1] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
            character[1][0] = character[1][4] = character[1][2] = character[2][0] = character[2][4] = '|';
            character[2][2] = character[3][4] = '\'';
            character[3][0] = '`';

            return character;
        }

    vs V()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][2] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][3] = character[3][4] = ' ';
        character[0][1] = character[0][3] = character[3][1] = character[3][2] = '_';
        character[1][0] = character[1][2] = character[1][4] = character[2][0] = character[2][4] = character[3][0] = '|';
        character[2][2] = '\'';
        character[3][3] = '/';
        return character;
    }

    vs W()
    {
        vs character = getCharGrid(4, 7);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][3] = character[0][5] = character[3][1] = character[3][2] = character[3][4] = '_';
        character[1][0] = character[1][2] = character[1][4] = character[1][6] = character[2][0] = character[2][2] = character[2][4] = character[2][6] = character[3][0] = '|';
        character[3][3] = character[3][5] = '/';

        return character;
    }

    vs X()
    {

        vs character = getCharGrid(4, 5);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[3][0] = character[0][4] = character[3][3] = '_';
        character[1][0] = character[1][2] = character[2][1] = character[2][3] = character[3][2] = character[3][4] = '\\';
        character[1][3] = character[3][1] = '/';

        return character;
    }

    vs Y()
    {
        vs character = getCharGrid(4, 5);
        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][3] = character[3][2] = '_';
        character[1][0] = character[1][2] = character[1][4] = character[3][1] = character[3][3] = '|';
        character[2][4] = '/';
        character[2][0] = '\\';

        return character;
    }

    vs Z()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[1][2] = character[1][3] = character[2][0] = character[2][2] = character[2][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][4] = '|';
        character[1][4] = character[2][1] = character[2][3] = character[3][0] = '/';
        return character;
    }

    // Lowercase

    vs a()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][0] = character[1][4] = character[2][3] = ' ';
        character[1][1] = character[1][2] = character[1][3] = character[2][1] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[2][0] = character[3][0] = '<';
        character[2][2] = '>';
        character[2][4] = character[3][4] = '|';
        return character;
    }

    vs b()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][4] = character[2][1] = character[2][3] = ' ';
        character[0][1] = character[1][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[1][2] = character[2][0] = character[3][0] = '|';
        character[2][2] = '.';
        character[2][4] = '\\';
        character[3][4] = '/';
        return character;
    }

    vs c()
    {
        vs character = getCharGrid(4, 5);
        character[1][1] = character[1][2] = character[1][3] = character[3][1] = character[3][3] = '_';
        character[2][2] = character[3][2] = '|';
        character[3][0] = '\\';
        character[3][4] = '.';
        character[2][4] = '\'';
        character[2][0] = '/';
        return character;
    }

    vs d()
    {
        vs character = getCharGrid(4, 5);

        // Enter the character grid in ROWS X COLS
        character[0][3] = character[3][1] = character[3][2] = character[1][1] = character[3][3] = '_';
        character[1][2] = character[3][4] = character[2][4] = character[1][4] = '|';
        character[2][2] = '.';
        character[2][0] = '/';
        character[3][0] = '\\';
        return character;
    }

    vs e()
    {
        vs character = getCharGrid(3, 6);

        character[0] = " ___ ";
        character[1] = "/ ._>";
        character[2] = "\\___.";

        return character;
    }

 vs f()
    {
        vs character = getCharGrid(4, 5);
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[1][2] = character[2][2] = character[3][2] = '|';
        character[2][3] = '-';
        character[1][4] = '\'';
        return character;
    }


    vs g()
    {
        vs character = getCharGrid(5, 5);

        // Enter the character grid in ROWS X COLS
        character[1][1] = character[1][2] = character[1][3] = character[4][1] = character[4][2] = character[4][3] = character[3][1] = '_';
        character[2][4] = character[3][4] = '|';
        character[2][2] = character[3][2] = '.';
        character[3][0] = '\\';
        character[2][0] = '/';
        character[4][0] = '<';
        character[4][4] = '\'';
        return character;
    }

    vs h()
    {
        vs character = getCharGrid(4, 5);

        character[1][0] = character[2][0] = character[3][0] = character[1][2] = character[2][4] = character[3][2] = character[3][4] = '|';
        character[0][1] = character[1][3] = character[3][1] = character[3][3] = '_';
        character[2][2] = '.';

        return character;
    }

    vs i()
    {
        vs character = getCharGrid(4, 3);
        character[0][0] = character[0][2] = character[2][1] = ' ';
        character[0][1] = character[1][1] = character[3][1] = '_';
        character[1][0] = '<';
        character[1][2] = '>';
        character[2][0] = character[2][2] = character[3][0] = character[3][2] = '|';
        return character;
    }

    vs j()
    {
        vs character = getCharGrid(5, 4);

        character[2][1] = character[2][3] = character[3][1] = character[3][3] = '|';
        character[0][2] = character[1][2] = character[4][1] = character[4][2] = '_';
        character[4][0] = character[1][1] = '<';
        character[4][3] = '\'';
        character[1][3] = '>';
        return character;
    }

    vs k()
        {
            vs character = getCharGrid(4,5);

            character[0][1] = character[3][1] = character[3][3] = character[1][3] = character[1][4] = '_';
            character[1][0] = character[1][2] = character[2][0] = character[3][0] = '|';
            character[2][2] = character[2][4] = '/';
            character[3][2] = character[3][4] = '\\';

            return character;
        }

    vs l()
    {
        vs character = getCharGrid(4, 3);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[3][1] = '_';
        character[1][0] = character[2][0] = character[3][0] = character[1][2] = character[2][2] = character[3][2] = '|';
        return character;
    }


        vs m()
        {
            vs character = getCharGrid(4,7);

            //Enter the character grid in ROWS X COLS
            character[1][0]='.';
            character[1][1]=character[1][3]=character[1][5]=character[3][1]=character[3][3]=character[3][5]='_';
            character[2][0]=character[3][0]=character[2][6]=character[3][6]=character[3][2]=character[3][4]=character[3][0]=character[3][6]='|';
            character[2][2]=character[2][4]='\'';

            return character;
        }



    vs n()
    {
        vs character = getCharGrid(4, 5);
        character[1][0] = '.';
        character[1][1] = character[1][3] = character[3][1] = character[3][3] = '_';
        character[1][2] = character[1][4] = character[2][1] = character[2][3] = ' ';
        character[2][0] = character[2][4] = character[3][0] = character[3][2] = character[3][4] = '|';
        character[2][2] = '\'';
        return character;
    }

    vs o()
    {
        vs character = getCharGrid(3, 5);

        // Enter the character grid in ROWS X COLS
        character[0][1] = character[0][2] = character[0][3] = character[2][3] = character[2][1] = character[2][2] = '_';
        character[1][0] = character[2][4] = '/';
        character[1][4] = character[2][0] = '\\';
        character[1][2] = '.';
        return character;
    }


    vs p()
        {
            vs character = getCharGrid(5, 5);

        // Enter the character grid in ROWS X COLS
        character[1][1] = character[1][2] = character[3][3] = character[1][3] = character[4][1] = '_';
        character[2][0] = character[4][0] = character[4][2] = character[3][0] = '|';
        character[2][2] = '.';
        character[2][4] = '\\';
        character[3][4] = '/';

        return character;
        }




        vs q()
        {
            vs character = getCharGrid(4,5);

            character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][2] = character[2][3] = character[3][0] = character[3][1] = ' ';
            character[0][1] = character[0][2] = character[0][3] = character[2][1] = character[3][3] = '_';
            character[1][4] = character[2][4] = character[3][2] = character[3][4] = '|';
            character[1][0] = '/';
            character[1][2] = '.';
            character[2][0] = '\\';

            return character;
        }




        vs r()
        {
            vs character = getCharGrid(rows,cols);

        character[1][1] = character[1][3] = character[2][3] = character[3][1] = '_';
        character[2][0] = character[3][0] = character[3][2] = '|';
        character[2][4] = '>';
        character[2][2] = '\'';

            return character;
        }



    vs s()
    {
        vs character = getCharGrid(5, 5);
        character[0][0] = character[0][1] = character[0][2] = character[0][3] = character[1][0] = ' ';
        character[1][1] = character[1][2] = character[1][3] = character[2][1] = character[3][1] = character[3][2] = '_';
        character[2][0] = character[2][3] = '<';
        character[2][2] = '-';
        character[3][0] = character[3][3] = '/';
        return character;
    }

    vs t()
    {
        vs character = getCharGrid(4, 5);
        character[0][2] = character[1][0] = character[1][4] = character[3][2] = '_';
        character[1][1] = character[1][3] = character[2][1] = character[2][3] = character[3][1] = character[3][3] = '|';

        return character;
    }

    vs u()
    {
        vs character = getCharGrid(4, 5);
        character[2][0] = character[2][2] = character[2][4] = character[3][4] = '|';
        character[3][0] = '`';
        character[1][1] = character[3][1] = character[3][2] = character[1][3] = character[3][3] = '_';

        return character;
    }

    vs v()
    {
        vs character = getCharGrid(4, 5);

        // Enter the character grid in ROWS X COLS
        character[1][1] = character[3][1] = character[3][2] = character[1][3] = '_';
        character[2][0] = character[2][2] = character[2][4] = character[3][0] = '|';
        character[3][3] = '/';

        return character;
    }


        vs w()
        {
            vs character = getCharGrid(3,7);

            character[0][1]=character[0][3]=character[0][5]=character[2][1]=character[2][2]=character[2][4]= '_';
            character[2][0]=character[1][0]=character[1][2]=character[1][4]=character[1][6]= '|';
            character[2][3]=character[2][5]='/';

            return character;
        }



    vs x()

    {
        vs character = getCharGrid(5, 5);
        character[1][0] = character[1][1] = character[3][2] = '_';
        character[2][0] = character[2][2] = character[3][1] = character[3][3] = '\\';
        character[2][3] =
            character[3][0] = '/';
        return character;
    }

    vs y()
        {
            vs character = getCharGrid(4,5);
            character[0][1]=character[0][3]=character[2][1]=character[3][1]=character[3][2]=character[3][3]='_';character[0][2]=' ';
            character[1][0]=character[1][2]=character[1][4]='|';
            character[2][4]='|';
            character[2][0]='`';
            character[3][4]='\'';
            character[2][2]='.';
            character[3][0]='<';
            return character;
        }


    vs z()
        {
            vs character = getCharGrid(4,4);
        character[1][1] = character[3][1] = character[3][2] = character[3][3] = character[1][2] = character[1][3] = '_';
        character[3][0] = character[2][1] = character[2][3] = '/';
        character[1][0] = '.';

        return character;
        }



    // Numbers

    vs zero()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][2] = character[1][3] = character[2][1] = character[2][3] = character[3][1] = character[3][2] = character[3][3] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[2][2] = '/';
        character[1][0] = character[2][0] = character[1][4] = character[2][4] = '|';
        character[3][0] = '`';
        character[3][4] = '\'';
        return character;
    }

    vs one()
    {
        vs character = getCharGrid(4, 3);
        character[0][0] = character[0][2] = character[1][1] = character[2][1] = ' ';
        character[0][1] = character[3][1] = '_';
        character[1][0] = '/';
        character[1][2] = character[2][0] = character[2][2] = character[3][0] = character[3][2] = '|';
        return character;
    }

    vs two()
    {
        vs character = getCharGrid(5, 6);
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[3][0] = '<';
        character[1][4] = character[3][4] = '>';
        character[2][1] = character[2][3] = '/';
        return character;
    }

    vs three()
    {
        vs character = getCharGrid(5, 5);
        character[0][0] = character[1][3] = character[2][0] = character[2][3] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[0][4] = character[1][1] = character[1][2] = character[2][2] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = character[2][1] = character[3][0] = '<';
        character[1][4] = character[3][4] = '/';
        character[2][4] = '\\';
        return character;
    }

    vs four()
    {
        vs character = getCharGrid(5, 6);

        character[0][0] = character[0][1] = character[0][4] = character[0][5] = character[1][0] = character[1][3] = character[1][5] = character[2][2] = character[2][3] = character[3][0] = character[3][1] = character[3][5] = ' ';
        character[0][2] = character[0][3] = character[2][1] = character[3][3] = '_';
        character[1][1] = character[2][0] = '/';
        character[1][2] = character[2][4] = '.';
        character[1][4] = character[2][5] = character[3][2] = character[3][4] = '|';

        return character;
    }


        vs five()
        {
            vs character = getCharGrid(4,5);

            character[0][1] = character[0][2] = character[0][3]=character[1][2]=character[1][3]=character[2][1]=character[2][2]=character[3][1]=character[3][2]=character[3][3]='_';

            character[1][0]=character[1][4]=character[3][0]='|';
            character[2][0]=  96;
            character[2][4]=  '\\';
            character[3][4]= '/';

            return character;
        }




        vs six()
        {
            vs character = getCharGrid(4,5);

            character[0][1] = character[0][2] = character[0][3] = character[1][2] = character[1][3] = character[3][1] = character[3][2] = character[3][3] = '_';
            character[1][0] = character[2][0] = '|';
            character[1][4] = '>';
            character[3][0]  =96;
            character[3][4] = '/';
            character[2][2] = '.';
            character[2][4] = '\\';

            return character;
        }



    vs seven()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][2] = character[1][3] = character[2][0] = character[2][2] = character[2][4] = character[3][3] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[1][1] = character[3][1] = '_';
        character[1][0] = character[1][4] = '|';
        character[2][1] = character[2][3] = character[3][0] = character[3][2] = '/';
        return character;
    }

    vs eight()
    {
        vs character = getCharGrid(4, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][1] = character[2][3] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[3][1] = character[3][2] = character[3][3] = '_';
        character[1][0] = '<';
        character[1][2] = character[2][2] = '.';
        character[1][4] = '>';
        character[2][0] = character[3][4] = '/';
        character[2][4] = character[3][0] = '\\';
        return character;
    }

    vs nine()
    {
        vs character = getCharGrid(5, 5);
        character[0][0] = character[0][4] = character[1][1] = character[1][3] = character[2][2] = character[2][3] = character[3][0] = character[3][4] = character[4][0] = character[4][1] = character[4][2] = character[4][3] = character[4][4] = ' ';
        character[0][1] = character[0][2] = character[0][3] = character[2][1] = character[3][2] = '_';
        character[1][0] = character[1][4] = '|';
        character[1][2] = '.';
        character[2][0] = '`';
        character[2][4] = character[3][1] = character[3][3] = '/';
        return character;
    }
};

#endif

#ifndef SMALL_H
#define SMALL_H
#include <string.h>


class Small : public Fonts
{
    static const int rows = 5;
    static const int cols = 10;

public:
    Small() : Fonts(rows, cols) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(5, 7);
        character[0] = "   _   ";
        character[1] = "  /_\\  ";
        character[2] = " / _ \\ ";
        character[3] = "/_/ \\_\\";
        character[4] = "       ";
        return character;
    }


        vs B()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___  ";
            character[1] = "| _ ) ";
            character[2] = "| _ \\";
            character[3] = "|___/";
            character[4] = "     ";

            return character;
        }



        vs C()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "  ___ ";
            character[1] = " / __|";
            character[2] = "| (__ ";
            character[3] = " \\___|";
            character[4] = "      ";

            return character;
        }



        vs D()
        {
            vs character = getCharGrid(5,6);

            character[0] = " ___  ";
            character[1] = "|   \\ ";
            character[2] = "| |) |";
            character[3] = "|___/ ";
            character[4] = "      ";

            return character;
        }



        vs E()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "| __|";
            character[2] = "| _| ";
            character[3] = "|___|";
            character[4] = "     ";

            return character;
        }



        vs F()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "| __|";
            character[2] = "| _| ";
            character[3] = "|_|  ";
            character[4] = "     ";

            return character;
        }



        vs G()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "  ___ ";
            character[1] = " / __|";
            character[2] = "| (_ |";
            character[3] = " \\___|";
            character[4] = "      ";

            return character;
        }



        vs H()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _  _ ";
            character[1] = "| || |";
            character[2] = "| __ |";
            character[3] = "|_||_|";
            character[4] = "      ";

            return character;
        }



        vs I()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "|_ _|";
            character[2] = " | | ";
            character[3] = "|___|";
            character[4] = "     ";

            return character;
        }



        vs J()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "    _ ";
            character[1] = " _ | |";
            character[2] = "| || |";
            character[3] = " \\__/ ";
            character[4] = "      ";

            return character;
        }



        vs K()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _  __";
            character[1] = "| |/ /";
            character[2] = "| ' < ";
            character[3] = "|_|\\_\\";
            character[4] = "      ";

            return character;
        }



        vs L()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _    ";
            character[1] = "| |   ";
            character[2] = "| |__ ";
            character[3] = "|____|";
            character[4] = "      ";

            return character;
        }



        vs M()
        {
            vs character = getCharGrid(5, 8);

            character[0] = " __  __ ";
            character[1] = "|  \\/  |";
            character[2] = "| |\\/| |";
            character[3] = "|_|  |_|";
            character[4] = "       ";

            return character;
        }



        vs N()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _  _ ";
            character[1] = "| \\| |";
            character[2] = "| .` |";
            character[3] = "|_|\\_|";
            character[4] = "      ";

            return character;
        }



        vs O()
        {
            vs character = getCharGrid(5, 7);

            character[0] = "  ___  ";
            character[1] = " / _ \\ ";
            character[2] = "| (_) |";
            character[3] = " \\___/ ";
            character[4] = "       ";

            return character;
        }



        vs P()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "| _ \\";
            character[2] = "|  _/";
            character[3] = "|_|  ";
            character[4] = "     ";

            return character;
        }



        vs Q()
        {
            vs character = getCharGrid(5, 7);

            character[0] = "  ___  ";
            character[1] = " / _ \\ ";
            character[2] = "| (_) |";
            character[3] = " \\__\\_\\";
            character[4] = "       ";

            return character;
        }



        vs R()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "| _ \\";
            character[2] = "|   /";
            character[3] = "|_|_\\";
            character[4] = "     ";

            return character;
        }



        vs S()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "/ __|";
            character[2] = "\\__ \\";
            character[3] = "|___/";
            character[4] = "     ";

            return character;
        }



        vs T()
        {
            vs character = getCharGrid(5, 7);

            character[0] = " _____ ";
            character[1] = "|_   _|";
            character[2] = "  | |  ";
            character[3] = "  |_|  ";
            character[4] = "       ";

            return character;
        }



        vs U()
        {
            vs character = getCharGrid(5, 7);

            character[0] = " _   _ ";
            character[1] = "| | | |";
            character[2] = "| |_| |";
            character[3] = " \\___/ ";
            character[4] = "       ";

            return character;
        }



        vs V()
        {
            vs character = getCharGrid(5, 7);

            character[0] = "__   __";
            character[1] = "\\ \\ / /";
            character[2] = " \\ V / ";
            character[3] = "  \\_/  ";
            character[4] = "       ";

            return character;
        }



        vs W()
        {
            vs character = getCharGrid(5, 10);

            character[0] = "__      __";
            character[1] = "\\ \\    / /";
            character[2] = " \\ \\/\\/ / ";
            character[3] = "  \\_/\\_/  ";
            character[4] = "          ";

            return character;
        }



        vs X()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "__  __";
            character[1] = "\\ \\/ /";
            character[2] = " >  < ";
            character[3] = "/_/\\_\\";
            character[4] = "      ";

            return character;
        }



        vs Y()
        {
            vs character = getCharGrid(5, 7);

            character[0] = "__   __";
            character[1] = "\\ \\ / /";
            character[2] = " \\ V / ";
            character[3] = "  |_|  ";
            character[4] = "       ";

            return character;
        }



        vs Z()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ____";
            character[1] = "|_  /";
            character[2] = " / / ";
            character[3] = "/___|";
            character[4] = "     ";

            return character;
        }


    // Lowercase


        vs a()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " __ _ ";
            character[2] = "/ _` |";
            character[3] = "\\__,_|";
            character[4] = "      ";

            return character;
        }



        vs b()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _    ";
            character[1] = "| |__ ";
            character[2] = "| '_ \\";
            character[3] = "|_.__/";
            character[4] = "      ";

            return character;
        }



        vs c()
        {
            vs character = getCharGrid(5, 4);

            character[0] = "    ";
            character[1] = " __ ";
            character[2] = "/ _|";
            character[3] = "\\__|";
            character[4] = "    ";

            return character;
        }



        vs d()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "    _ ";
            character[1] = " __| |";
            character[2] = "/ _` |";
            character[3] = "\\__,_|";
            character[4] = "      ";

            return character;
        }



        vs e()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "      ";
            character[1] = " ___ ";
            character[2] = "/ -_)";
            character[3] = "\\___|";
            character[4] = "     ";

            return character;
        }



        vs f()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "  __ ";
            character[1] = " / _|";
            character[2] = "|  _|";
            character[3] = "|_|  ";
            character[4] = "     ";

            return character;
        }



        vs g()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " __ _ ";
            character[2] = "/ _` |";
            character[3] = "\\__, |";
            character[4] = "|___/ ";

            return character;
        }



        vs h()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _    ";
            character[1] = "| |_  ";
            character[2] = "| ' \\ ";
            character[3] = "|_||_|";
            character[4] = "      ";

            return character;
        }



        vs i()
        {
            vs character = getCharGrid(5, 3);

            character[0] = " _ ";
            character[1] = "(_)";
            character[2] = "| |";
            character[3] = "|_|";
            character[4] = "   ";

            return character;
        }



        vs j()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "   _ ";
            character[1] = "  (_)";
            character[2] = "  | |";
            character[3] = " _/ |";
            character[4] = "|__/ ";

            return character;
        }




        vs k()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " _   ";
            character[1] = "| |__";
            character[2] = "| / /";
            character[3] = "|_\\_\\";
            character[4] = "     ";

            return character;
        }




        vs l()
        {
            vs character = getCharGrid(5, 3);

            character[0] = " _ ";
            character[1] = "| |";
            character[2] = "| |";
            character[3] = "|_|";
            character[4] = "   ";

            return character;
        }




        vs m()
        {
            vs character = getCharGrid(5, 7);

            character[0] = "       ";
            character[1] = " _ __  ";
            character[2] = "| '  \\ ";
            character[3] = "|_|_|_|";
            character[4] = "       ";

            return character;
        }




        vs n()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " _ _  ";
            character[2] = "| ' \\ ";
            character[3] = "|_||_|";
            character[4] = "      ";

            return character;
        }




        vs o()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "     ";
            character[1] = " ___ ";
            character[2] = "/ _ \\";
            character[3] = "\\___/";
            character[4] = "     ";

            return character;
        }




        vs p()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " _ __ ";
            character[2] = "| '_ \\";
            character[3] = "| .__/";
            character[4] = "|_|   ";

            return character;
        }




        vs q()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " __ _ ";
            character[2] = "/ _` |";
            character[3] = "\\__, |";
            character[4] = "   |_|";

            return character;
        }




        vs r()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "     ";
            character[1] = " _ _ ";
            character[2] = "| '_|";
            character[3] = "|_|  ";
            character[4] = "     ";

            return character;
        }




        vs s()
        {
            vs character = getCharGrid(5, 4);

            character[0] = "    ";
            character[1] = " ___";
            character[2] = "(_-<";
            character[3] = "/__/";
            character[4] = "    ";

            return character;
        }




        vs t()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " _   ";
            character[1] = "| |_ ";
            character[2] = "|  _|";
            character[3] = " \\__|";
            character[4] = "     ";

            return character;
        }




        vs u()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " _  _ ";
            character[2] = "| || |";
            character[3] = " \\_,_|";
            character[4] = "      ";

            return character;
        }




        vs v()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "     ";
            character[1] = "__ __";
            character[2] = "\\ V /";
            character[3] = " \\_/ ";
            character[4] = "     ";

            return character;
        }




        vs w()
        {
            vs character = getCharGrid(5, 8);

            character[0] = "        ";
            character[1] = "__ __ __";
            character[2] = "\\ V  V /";
            character[3] = " \\_/\\_/ ";
            character[4] = "        ";

            return character;
        }




        vs x()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "     ";
            character[1] = "__ __";
            character[2] = "\\ \\ /";
            character[3] = "/_\\_\\";
            character[4] = "     ";

            return character;
        }




        vs y()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "      ";
            character[1] = " _  _ ";
            character[2] = "| || |";
            character[3] = " \\_, |";
            character[4] = " |__/ ";

            return character;
        }




        vs z()
        {
            vs character = getCharGrid(5, 4);

            character[0] = "    ";
            character[1] = " ___";
            character[2] = "|_ /";
            character[3] = "/__|";
            character[4] = "    ";

            return character;
        }



    // Numbers


        vs zero()
        {
            vs character = getCharGrid(5, 6);

            character[0] = "  __  ";
            character[1] = " /  \\ ";
            character[2] = "| () |";
            character[3] = " \\__/ ";
            character[4] = "      ";

            return character;
        }




        vs one()
        {
            vs character = getCharGrid(5, 3);

            character[0] = " _ ";
            character[1] = "/ |";
            character[2] = "| |";
            character[3] = "|_|";
            character[4] = "   ";

            return character;
        }




        vs two()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "|_  )";
            character[2] = " / / ";
            character[3] = "/___|";
            character[4] = "     ";

            return character;
        }




        vs three()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ____";
            character[1] = "|__ /";
            character[2] = " |_ \\";
            character[3] = "|___/";
            character[4] = "     ";

            return character;
        }




        vs four()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " _ _  ";
            character[1] = "| | | ";
            character[2] = "|_  _|";
            character[3] = "  |_| ";
            character[4] = "      ";

            return character;
        }




        vs five()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "| __|";
            character[2] = "|__ \\";
            character[3] = "|___/";
            character[4] = "     ";

            return character;
        }




        vs six()
        {
            vs character = getCharGrid(5, 5);

            character[0] = "  __ ";
            character[1] = " / / ";
            character[2] = "/ _ \\";
            character[3] = "\\___/";
            character[4] = "     ";

            return character;
        }




        vs seven()
        {
            vs character = getCharGrid(5, 6);

            character[0] = " ____ ";
            character[1] = "|__  |";
            character[2] = "  / / ";
            character[3] = " /_/  ";
            character[4] = "      ";

            return character;
        }




        vs eight()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "( _ )";
            character[2] = "/ _ \\";
            character[3] = "\\___/";
            character[4] = "     ";

            return character;
        }




        vs nine()
        {
            vs character = getCharGrid(5, 5);

            character[0] = " ___ ";
            character[1] = "/ _ \\";
            character[2] = "\\_, /";
            character[3] = " /_/ ";
            character[4] = "     ";

            return character;
        }



    // Space

    vs space()
    {
        vs character = getCharGrid(1, 1);

        character[0] = " ";

        return character;
    }
};
#endif

#ifndef THREED_DIAGONAL_H
#define THREED_DIAGONAL_H
#include <string.h>


class ThreeD_Diagonal : public Fonts
{
    static const int rows = 12;
    static const int cols = 20;

public:
    ThreeD_Diagonal() : Fonts(rows, cols) {}

    // Uppercase

    vs A()
    {
        vs character = getCharGrid(12, 17);

        character[0] = "    ,---,          ";
        character[1] = "   '  .' \\        ";
        character[2] = "  /  ;    '.       ";
        character[3] = " :  :       \\     ";
        character[4] = " :  |   /\\   \\   ";
        character[5] = " |  :  ' ;.   :    ";
        character[6] = " |  |  ;/  \\   \\ ";
        character[7] = " '  :  | \\  \\ ,' ";
        character[8] = " |  |  '  '--'     ";
        character[9] = " |  :  :           ";
        character[10]= " |  | ,'           ";
        character[11]= " `--''             ";

        return character;
    }


        vs E()
        {
            vs character = getCharGrid(13, 13);

            character[0] = "     ,---,.   ";
            character[1] = "   ,'  .' |   ";
            character[2] = " ,---.'   |   ";
            character[3] = " |   |   .'   ";
            character[4] = " :   :  |-,   ";
            character[5] = " :   |  ;/|   ";
            character[6] = " |   :   .'   ";
            character[7] = " |   |  |-,   ";
            character[8] = " '   :  ;/|   ";
            character[9] = " |   |    \\  ";
            character[10]= " |   :   .'   ";
            character[11]= " |   | ,'     ";
            character[12]= " `----'       ";

            return character;
        }


    vs Q()
    {
        vs character = getCharGrid(13,18);

        character[0] = "     ,----..      ";
        character[1] = "    /   /   \\     ";
        character[2] = "   /   .     :    ";
        character[3] = "  .   /   ;.  \\   ";
        character[4] = " .   ;   /  ` ;   ";
        character[5] = " ;   |  ; \\ ; |   ";
        character[6] = " |   :  | ; | '   ";
        character[7] = " .   |  ' ' ' :   ";
        character[8] = " '   ;  \\; /  |   ";
        character[9] = "  \\   \\  ',  . \\  ";
        character[10]= "   ;   :      ; | ";
        character[11]= "    \\   \\ .'`--\"  ";
        character[12]= "     `---`        ";

        return character;
    }

};
#endif

#ifndef ASCII_H
#define ASCII_H


namespace ascii
{
    enum FontName
    {
        sevenstar,
        boomer,
        straight,
        starwar,
        carlos,
        banner,
        block,
        amongus,
        drpepper,
        small,
        threeD_diagonal
    };

    class Ascii
    {

    public:
        std::unique_ptr<Fonts> font;
        Ascii(const FontName &fontName)
        {
            if (fontName == FontName::sevenstar)
            {
                this->font.reset(new SevenStar());
            }
            else if (fontName == FontName::boomer)
            {
                this->font.reset(new Boomer());
            }
            else if (fontName == FontName::straight)
            {
                this->font.reset(new Straight());
            }
            else if (fontName == FontName::starwar)
            {
                this->font.reset(new Starwar());
            }
            else if (fontName == FontName::carlos)
            {
                this->font.reset(new Carlos());
            }
            else if (fontName == FontName::banner)
            {
                this->font.reset(new Banner());
            }
            else if (fontName == FontName::block)
            {
                this->font.reset(new Block());
            }
            else if (fontName == FontName::amongus)
            {
                this->font.reset(new Amongus());
            }
            else if (fontName == FontName::drpepper)
            {
                this->font.reset(new Drpepper());
            }
            else if (fontName == FontName::small)
            {
                this->font.reset(new Small());
            }
            else if (fontName == FontName::threeD_diagonal)
            {
                this->font.reset(new ThreeD_Diagonal());
            }
            else
            {
                exit(500);
            }
        }


        void reset()
        {
            font->resetMatrix();
        }

        void print(const std::string &text)
        {
            vs character;

            for (size_t i = 0; i < text.size(); i++)
            {
                char c = text[i];

                // Uppercase alphabets
                if (c == 'A')
                    character = font->A();
                else if (c == 'B')
                    character = font->B();
                else if (c == 'C')
                    character = font->C();
                else if (c == 'D')
                    character = font->D();
                else if (c == 'E')
                    character = font->E();
                else if (c == 'F')
                    character = font->F();
                else if (c == 'G')
                    character = font->G();
                else if (c == 'H')
                    character = font->H();
                else if (c == 'I')
                    character = font->I();
                else if (c == 'J')
                    character = font->J();
                else if (c == 'K')
                    character = font->K();
                else if (c == 'L')
                    character = font->L();
                else if (c == 'M')
                    character = font->M();
                else if (c == 'N')
                    character = font->N();
                else if (c == 'O')
                    character = font->O();
                else if (c == 'P')
                    character = font->P();
                else if (c == 'Q')
                    character = font->Q();
                else if (c == 'R')
                    character = font->R();
                else if (c == 'S')
                    character = font->S();
                else if (c == 'T')
                    character = font->T();
                else if (c == 'U')
                    character = font->U();
                else if (c == 'V')
                    character = font->V();
                else if (c == 'W')
                    character = font->W();
                else if (c == 'X')
                    character = font->X();
                else if (c == 'Y')
                    character = font->Y();
                else if (c == 'Z')
                    character = font->Z();

                // Lowercase alphabets
                else if (c == 'a')
                    character = font->a();
                else if (c == 'b')
                    character = font->b();
                else if (c == 'c')
                    character = font->c();
                else if (c == 'd')
                    character = font->d();
                else if (c == 'e')
                    character = font->e();
                else if (c == 'f')
                    character = font->f();
                else if (c == 'g')
                    character = font->g();
                else if (c == 'h')
                    character = font->h();
                else if (c == 'i')
                    character = font->i();
                else if (c == 'j')
                    character = font->j();
                else if (c == 'k')
                    character = font->k();
                else if (c == 'l')
                    character = font->l();
                else if (c == 'm')
                    character = font->m();
                else if (c == 'n')
                    character = font->n();
                else if (c == 'o')
                    character = font->o();
                else if (c == 'p')
                    character = font->p();
                else if (c == 'q')
                    character = font->q();
                else if (c == 'r')
                    character = font->r();
                else if (c == 's')
                    character = font->s();
                else if (c == 't')
                    character = font->t();
                else if (c == 'u')
                    character = font->u();
                else if (c == 'v')
                    character = font->v();
                else if (c == 'w')
                    character = font->w();
                else if (c == 'x')
                    character = font->x();
                else if (c == 'y')
                    character = font->y();
                else if (c == 'z')
                    character = font->z();

                // Numbers
                else if (c == '0')
                    character = font->zero();
                else if (c == '1')
                    character = font->one();
                else if (c == '2')
                    character = font->two();
                else if (c == '3')
                    character = font->three();
                else if (c == '4')
                    character = font->four();
                else if (c == '5')
                    character = font->five();
                else if (c == '6')
                    character = font->six();
                else if (c == '7')
                    character = font->seven();
                else if (c == '8')
                    character = font->eight();
                else if (c == '9')
                    character = font->nine();

                // for space
                else if (c == ' ')
                    character = font->space();

                font->pushChar(character);
            }
            font->printvector();
            font->resetMatrix();
        }
    };
} // namespace ascii

#endif


// Если возникают конфликты в твоем проекте просто закомментируй
using namespace std;
using namespace ascii;
#endif //UTILIB_H


