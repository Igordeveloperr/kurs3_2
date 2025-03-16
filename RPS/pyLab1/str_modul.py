import re
import string
from tkinter import *

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

MAX_CHARS_IN_TEXT = 14000

def enter_text_handler(e,text_field, word_count, is_pali_var, char_count_var):
    text = text_field.get("1.0", "end-1c")
    words_in_text = number_of_words(text)
    word_count.set("Кол-во слов: " + str(words_in_text))
    is_pali_var.set("Строка палиндром: " + ("Да" if is_palindrome(text) else "Нет"))
    char_count_var.set("Кол-во символов: " + str(len(text)))

def enter_text_press_handler(e,text_field, word_count, is_pali_var, char_count_var):
    text = text_field.get("1.0", "end-1c")
    is_pali_var.set("Строка палиндром: " + ("Да" if is_palindrome(text) else "Нет"))
    char_count_var.set("Кол-во символов: " + str(len(text)))
    # Блокируем текстовое поле, если количество слов превышает лимит
    if len(text) >= MAX_CHARS_IN_TEXT:
        text_field.config(state="disabled")  # Блокируем ввод

def del_duplicate_handler(e, text_field, word_count, is_pali_var, char_count_var):
    textF = text_field.get("1.0", "end-1c")
    text = remove_duplicates(textF)
    if len(textF) >= MAX_CHARS_IN_TEXT:
        text_field.config(state="normal")  
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text) 
    else:
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text)
    enter_text_handler(e, text_field, word_count, is_pali_var, char_count_var)

def title_handler(e, text_field, word_count, is_pali_var, char_count_var):
    textF = text_field.get("1.0", "end-1c")
    text = capitalize_words(textF)
    if len(textF) >= MAX_CHARS_IN_TEXT:
        text_field.config(state="normal")  
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text) 
    else:
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text)
    enter_text_handler(e, text_field, word_count, is_pali_var, char_count_var)

def on_paste(event, text_field, word_count, is_pali_var, char_count_var):
    try:
        # Получаем текст из буфера обмена
        clipboard_text = text_field.clipboard_get()
        # Получаем текущий текст
        text = text_field.get("1.0", "end-1c")
        if len(text) + len(clipboard_text) > MAX_CHARS_IN_TEXT:
            # text_field.clipboard_clear()
            return "break"
    except TclError:
        # Если буфер обмена пуст или содержит не текстовые данные
        return
    
def handleBackspace(e,text_field, word_count, is_pali_var, char_count_var):
    text_field.config(state="normal")
    text = text_field.get("1.0", "end-1c")
    words_in_text = number_of_words(text)
    word_count.set("Кол-во слов: " + str(words_in_text))
    is_pali_var.set("Строка палиндром: " + ("Да" if is_palindrome(text) else "Нет"))
    char_count_var.set("Кол-во символов: " + str(len(text)))


def handleDelete(e,text_field, word_count, is_pali_var, char_count_var):
    text_field.config(state="normal")
    text = text_field.get("1.0", "end-1c")
    words_in_text = number_of_words(text)
    word_count.set("Кол-во слов: " + str(words_in_text))
    is_pali_var.set("Строка палиндром: " + ("Да" if is_palindrome(text) else "Нет"))
    char_count_var.set("Кол-во символов: " + str(len(text)))



def init_user_intarface():
    window = Tk()
    window.title("Строки")
    window.geometry('500x500')
    window.resizable(width=False, height=False)
    # окно в центр
    window.update_idletasks()
    w, h = window.winfo_width(), window.winfo_height()
    window.geometry(f"+{(window.winfo_screenwidth()-w)//2}+{(window.winfo_screenheight()-h)//2}")

    word_count = StringVar(window)
    word_count.set("Кол-во слов: 0")
    is_pali_var = StringVar(window)
    is_pali_var.set("Строка палиндром: ")
    char_count_var = StringVar(window)
    char_count_var.set("Кол-во символов: 0")

    title = Label(window,text = "Ввод данных", font = 40)
    title.pack()

    text_field = Text(window,width = 100, height = 20,font="Verdana 12", wrap=WORD)
    text_field.bind("<KeyRelease>", lambda e: enter_text_handler(e,text_field, word_count, is_pali_var, char_count_var), '+')
    text_field.bind("<KeyPress>", lambda e: enter_text_press_handler(e,text_field, word_count, is_pali_var, char_count_var), '+')
    text_field.bind("<BackSpace>", lambda e: handleBackspace(e,text_field, word_count, is_pali_var, char_count_var), '+')
    text_field.bind("<Delete>", lambda e: handleDelete(e,text_field, word_count, is_pali_var, char_count_var), '+')
    text_field.bind("<<Paste>>", lambda e: on_paste(e, text_field, word_count, is_pali_var, char_count_var), '+')

    #докидываем бегунок
    scr = Scrollbar(window,command = text_field.yview )
    scr.pack(side=RIGHT,fill=Y)
    text_field.config(yscrollcommand = scr.set)
    text_field.pack()
    
    wordCountLabel = Label(window, textvariable=word_count, font = 40)
    wordCountLabel.place(x=30,y= 410)

    strPaliLabel = Label(window, textvariable=is_pali_var, font = 40)
    strPaliLabel.place(x=30,y= 470)

    char_count_lbl = Label(window, textvariable=char_count_var, font = 40)
    char_count_lbl.place(x=30,y= 440)

    delDuplicateBtn = Button(window, text="Удалить повтор.слова", font=40, command=lambda:del_duplicate_handler(1, text_field, word_count, is_pali_var, char_count_var))
    delDuplicateBtn.place(x=300,y= 410)

    titleBtn = Button(window, text="Формат заголовка", font=40, command=lambda:title_handler(1, text_field, word_count, is_pali_var, char_count_var))
    titleBtn.place(x=300,y= 450)

    window.mainloop()