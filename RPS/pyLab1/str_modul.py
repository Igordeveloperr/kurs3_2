import re
import string

# подсчет кол-ва слов
number_of_words = lambda line: len(re.findall(r'\b\w+\b', line))

# проверка является ли строка палиндромом
is_palindrome = lambda s: s.strip() != "" and s.replace(" ", "").lower() == s.replace(" ", "").lower()[::-1]

# удаление повторяющихся слов
def remove_duplicates(s):
    seen = set()  # Множество для отслеживания уже встреченных слов
    result = []   # Список для хранения результата

    # Разделяем строку на токены (слова и не-слова, включая пунктуацию)
    tokens = re.findall(r'\w+|\W+', s)

    # Лямбда-функция для обработки токенов
    process_token = lambda token: (
        token if not token.isalpha() or token.lower() not in seen else ''
    )

    # Применяем лямбда-функцию к каждому токену
    for token in tokens:
        if token.isalpha():  # Если токен — слово
            lower_token = token.lower()  # Приводим к нижнему регистру
            if lower_token not in seen:  # Если слово еще не встречалось
                seen.add(lower_token)    # Добавляем в множество
                result.append(token)    # Добавляем в результат
        else:  # Если токен — не слово (пунктуация или пробел)
            result.append(token)  # Добавляем в результат как есть

    # Собираем строку из токенов
    return ''.join(result)

# все слова в строке с заглавной
capitalize_words = lambda s: ''.join(word.capitalize() for word in re.findall(r'\w+|\W+', s))

# менюшка
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