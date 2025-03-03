# число слов в строке
def number_of_words(line):
    return sum(map(lambda x : int(x.isalpha()), line.split()))

# кол-во вхождений подстроки в строку
def number_of_defined_characters(line,symbol):
    return line.count(symbol)

# число символов в строке
def number_of_symbols(line):
    return len(line)

# кол-во уникальных символов
def number_of_universal_symbols(line):
    return len(set(line))

# консольная менюшка
def menu():
    line = input("Введите строку: ")
    print("1.Кол-во символов")
    print("2.Кол-во слов")
    print("3.Кол-во вхождений подстроки в строку")
    print("4.Кол-во уникальных символов")
    print("5.Выход")
    what_to_do = input("Введите номер команды: ")
    if what_to_do == "1":
        count = number_of_symbols(line)
        print("Кол-во символов: " + str(count))
        menu()
    elif what_to_do == "2":
        count = number_of_words(line)
        print("Кол-во слов: " + str(count))
        menu()
    elif what_to_do == "3":
        symbol = input("Введите символ для поиска: ")
        count = number_of_defined_characters(line, symbol)
        print("Найдено " + str(count) + ' символов "' + symbol + '"')
        menu()
    elif what_to_do == "4":
        count = number_of_universal_symbols(line)
        print("Кол-во уникальных символов: " + str(count))
        menu()
    elif what_to_do == "5": exit

    else:
        print("\nНеверная команда. Попробуйте еще раз.")
        menu()