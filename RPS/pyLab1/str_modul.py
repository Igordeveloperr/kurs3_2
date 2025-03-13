import re
import string

# count num of words
number_of_words = lambda line: len(re.findall(r'\b\w+\b', line))

# check str is palyndrom or not
is_palindrome = lambda s: s.strip() != "" and s.replace(" ", "").lower() == s.replace(" ", "").lower()[::-1]

# remove duplicate words
remove_duplicates = lambda s: ' '.join(
    filter(
        lambda x, seen=set(): not (x in seen or seen.add(x)),
        ''.join(char if char not in string.punctuation else ' ' for char in s).split()
    )
)


# all words start with upper case
capitalize_words = lambda s: ''.join(word.capitalize() for word in re.findall(r'\w+|\W+', s))

# console menu
def menu():
    line = input("Введите строку: ")
    print("1.Проверка на палиндром")
    print("2.Кол-во слов")
    print("3.Удалить повтор.слова")
    print("4.Формат заголовка")
    print("5.Выход")
    what_to_do = input("Введите номер команды: ")
    if what_to_do == "1":
        print("Строка палиндром: " + ("Да" if is_palindrome(line) else "Нет"))
        menu()
    elif what_to_do == "2":
        print("Кол-во слов: " + str(number_of_words(line)))
        menu()
    elif what_to_do == "3":
        print("Строка без повторений: " + remove_duplicates(line))
        menu()
    elif what_to_do == "4":
        print("Заголовок: " + capitalize_words(line))
        menu()
    elif what_to_do == "5": exit

    else:
        print("\nНеверная команда. Попробуйте еще раз.")
        menu()