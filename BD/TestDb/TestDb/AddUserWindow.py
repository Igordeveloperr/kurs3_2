from tkinter import Toplevel, Label, Button, Text, messagebox, Entry
from tkinter.ttk import Combobox

class AddUserWindow:
    def __init__(self, parent, db, on_user_added_callback=None):
        self.window = Toplevel(parent)
        self.window.title("Добавить пользователя")
        self.window.geometry("300x200")
        self.window.resizable(False, False)

        self.db = db
        self.on_user_added_callback = on_user_added_callback

        # Поля для ввода имени и пароля
        Label(self.window, text="Имя пользователя:").pack(pady=5)
        self.username_entry = Entry(self.window)
        self.username_entry.pack(pady=5)

        Label(self.window, text="Пароль:").pack(pady=5)
        self.password_entry = Entry(self.window, show="*")
        self.password_entry.pack(pady=5)

        # Кнопка "Добавить"
        Button(self.window, text="Добавить", command=self.add_user).pack(pady=10)

    def add_user(self):
        username = self.username_entry.get()
        password = self.password_entry.get()

        if username and password:
            user = self.db.get_user_by_name_and_password(username, password)
            if user:
                messagebox.showinfo("Ошибка!", "Данный пользователь уже существует!")
            else:
                # Сохраняем нового пользователя в базу данных
                self.db.add_user(username, password)
                if self.on_user_added_callback:
                    self.on_user_added_callback()
                self.window.destroy()
        else:
            messagebox.showinfo("Ошибка!", "Заполните все поля!")