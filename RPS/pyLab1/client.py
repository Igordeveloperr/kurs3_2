from str_modul import number_of_words, is_palindrome
from tkinter import *
import xmlrpc.client


server = xmlrpc.client.ServerProxy("http://localhost:9000/")
MAX_CHARS_IN_TEXT = 2000


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

def del_duplicate_handler(e, text_field, word_count, is_pali_var, char_count_var, delDuplicateBtn, titleBtn):
    textF = text_field.get("1.0", "end-1c")
    try:
        text_field.config(state="disabled")  # Блокируем ввод
        delDuplicateBtn.config(state="disabled")
        titleBtn.config(state="disabled")

        text = server.remove_duplicates(textF)

        delDuplicateBtn.config(state="normal")
        titleBtn.config(state="normal")
        text_field.config(state="normal")  # Блокируем ввод
    except Exception as e:
        print("Error")

    if len(textF) >= MAX_CHARS_IN_TEXT:
        text_field.config(state="normal")  
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text) 
    else:
        text_field.delete("1.0", "end")
        text_field.insert("1.0", text)
    enter_text_handler(e, text_field, word_count, is_pali_var, char_count_var)

def title_handler(e, text_field, word_count, is_pali_var, char_count_var, delDuplicateBtn, titleBtn):
    textF = text_field.get("1.0", "end-1c")
    try:
        text_field.config(state="disabled")  # Блокируем ввод
        delDuplicateBtn.config(state="disabled")
        titleBtn.config(state="disabled")

        text = server.capitalize_words(textF)

        delDuplicateBtn.config(state="normal")
        titleBtn.config(state="normal")
        text_field.config(state="normal")  # Блокируем ввод
    except Exception as e:
        print("Error")
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
        is_pali_var.set("Строка палиндром: " + ("Да" if is_palindrome(text) else "Нет"))
        char_count_var.set("Кол-во символов: " + str(len(text)))
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
    window.geometry('600x600')
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

    titleBtn = Button(window, text="Формат заголовка", font=40, command=lambda:title_handler(1, text_field, word_count, is_pali_var, char_count_var, delDuplicateBtn, titleBtn))
    titleBtn.place(x=300,y= 460)

    delDuplicateBtn = Button(window, text="Удалить повтор.слова", font=40, command=lambda:del_duplicate_handler(1, text_field, word_count, is_pali_var, char_count_var, delDuplicateBtn, titleBtn))
    delDuplicateBtn.place(x=300,y= 410)

    window.mainloop()

init_user_intarface()