#include "Utilib.h"


void incalization(bool logs)
{
    if (logs == true)
    {
        Timer timer;
        timer.startTime();
        //initUTF8();
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
        //initUTF8();
    }
}