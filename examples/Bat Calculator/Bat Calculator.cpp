#include "resources/Utilib.h"

int main() {
    initialization(false);
    Ascii font = Ascii(small);
    setColor(12);
    font.print("Bat Calculator");
    font.print("test");
    setColor(15);

    float first_number = get_input<float>("Ведите число: ");
    print(first_number);
    char boperator = get_input<char>("Ведите оператор: ");
    print(boperator);
    float second_number = get_input<float>("Ведите число: ");
    print(second_number);

    std::string command = "Calculator.bat \"" + std::to_string(first_number) + "\" \"" + std::string(1, boperator) + "\" \"" + std::to_string(second_number) + "\"";
    std::cout << "CMD: " << command << std::endl;
    system(command.c_str());

    return 0;
}
