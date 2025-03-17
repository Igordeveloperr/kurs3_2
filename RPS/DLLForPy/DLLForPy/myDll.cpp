#include "pch.h"
#include "myDll.h"


int number_of_words(const char* textPtr) {
    int count = 0;
    bool inWord = false;

    // �������� �� ������� ������� ������
    while (*textPtr) {
        if (std::isalpha(*textPtr) || std::isdigit(*textPtr)) {
            // ���� ������� ������ - ����� ��� �����, ������ �� ������ �����
            inWord = true;
        }
        else if (std::isspace(*textPtr)) {
            // ���� ������� ������ - ������, � �� ���� ������ �����, ����������� �������
            if (inWord) {
                count++;
                inWord = false;
            }
        }
        else {
            // ���� ������� ������ - ����������, � �� ���� ������ �����, ����������� �������
            if (inWord) {
                count++;
                inWord = false;
            }
        }
        textPtr++;
    }

    // ���� ������ ������������� ������, ����������� �������
    if (inWord) {
        count++;
    }

    return count;
}

int main() {
     
}