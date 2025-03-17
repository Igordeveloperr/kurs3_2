#include "pch.h"
#include "myDll.h"


int number_of_words(const char* textPtr) {
    int count = 0;
    bool inWord = false;

    // Проходим по каждому символу строки
    while (*textPtr) {
        if (std::isalpha(*textPtr) || std::isdigit(*textPtr)) {
            // Если текущий символ - буква или цифра, значит мы внутри слова
            inWord = true;
        }
        else if (std::isspace(*textPtr)) {
            // Если текущий символ - пробел, и мы были внутри слова, увеличиваем счетчик
            if (inWord) {
                count++;
                inWord = false;
            }
        }
        else {
            // Если текущий символ - пунктуация, и мы были внутри слова, увеличиваем счетчик
            if (inWord) {
                count++;
                inWord = false;
            }
        }
        textPtr++;
    }

    // Если строка заканчивается словом, увеличиваем счетчик
    if (inWord) {
        count++;
    }

    return count;
}

int main() {
     
}