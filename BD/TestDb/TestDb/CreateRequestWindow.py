from tkinter import Toplevel, Label, Button, Text, messagebox
from tkinter.ttk import Combobox
from AddUserWindow import *
from AddAddressWindow import *

class CreateRequestWindow:
    def __init__(self, parent, db, on_close_callback=None):
        # Создаем новое окно
        self.window = Toplevel(parent)
        self.window.title("Редактирование заявки")
        self.window.geometry("400x500")
        self.window.resizable(False, False)

        self.db = db
        self.on_close_callback = on_close_callback  # Сохраняем callback-функцию

        # Привязываем обработчик к событию закрытия окна
        self.window.protocol("WM_DELETE_WINDOW", self.on_close)

        # Получаем списки пользователей и адресов из базы данных
        self.users = self.db.get_users()  # Список объектов User
        self.addresses = self.db.get_apartments()  # Список объектов ApartmentBuilding

        # Создаем словари для хранения соответствий между строковым представлением и id
        self.user_dict = {str(user): user.id for user in self.users}
        self.address_dict = {str(address): address.id for address in self.addresses}

        # Метка и выпадающий список для первого пользователя
        Label(self.window, text="Выберите клиента:").pack(pady=5)
        self.user1_combobox = Combobox(self.window, values=list(self.user_dict.keys()))
        self.user1_combobox.pack(pady=5)
        self.user1_combobox.bind("<KeyRelease>", lambda event: self.filter_users(self.user1_combobox))
        # Кнопка "Создать" для добавления нового пользователя
        Button(self.window, text="Создать", command=self.open_add_user_window).pack(pady=5)

        # Метка и выпадающий список для адреса
        Label(self.window, text="Выберите адрес:").pack(pady=5)
        self.address_combobox = Combobox(self.window, values=list(self.address_dict.keys()))
        self.address_combobox.pack(pady=5)
        self.address_combobox.bind("<KeyRelease>", lambda event: self.filter_addresses(self.address_combobox))
        Button(self.window, text="Создать", command=self.open_add_address_window).pack(pady=5)

        # Метка и выпадающий список для второго пользователя
        Label(self.window, text="Выберите собственника:").pack(pady=5)
        self.user2_combobox = Combobox(self.window, values=list(self.user_dict.keys()))
        self.user2_combobox.pack(pady=5)
        self.user2_combobox.bind("<KeyRelease>", lambda event: self.filter_users(self.user2_combobox))
        # Кнопка "Создать" для добавления нового пользователя
        Button(self.window, text="Создать", command=self.open_add_user_window).pack(pady=5)


        # Метка и текстовое поле для комментария
        Label(self.window, text="Комментарий:").pack(pady=5)
        self.comment_text = Text(self.window, height=5, width=40)
        self.comment_text.pack(pady=5)

        # Кнопка "Создать"
        Button(self.window, text="Создать", command=self.create_request).pack(pady=10)

    def refresh_addresses(self):
        # Получаем обновленный список адресов из базы данных
        self.addresses = self.db.get_apartments()
    
        # Обновляем словарь соответствий
        self.address_dict = {str(address): address.id for address in self.addresses}
    
        # Обновляем значения в Combobox
        self.address_combobox["values"] = list(self.address_dict.keys())

    def open_add_address_window(self):
        # Открываем окно для добавления нового адреса
        AddAddressWindow(self.window, self.db, self.refresh_addresses)

    def refresh_users(self):
        # Обновляем список пользователей после добавления нового
        self.users = self.db.get_users()
        self.user_dict = {str(user): user.id for user in self.users}
        self.user1_combobox["values"] = list(self.user_dict.keys())
        self.user2_combobox["values"] = list(self.user_dict.keys())

    def open_add_user_window(self):
        # Открываем окно для добавления нового пользователя
        AddUserWindow(self.window, self.db, self.refresh_users)

    def filter_addresses(self, combobox):
        # Получаем введенный текст
        search_text = combobox.get()

        # Выполняем поиск адресов по подстроке
        filtered_addresses = self.db.search_addresses_by_text(search_text)

        if filtered_addresses:
            combobox["values"] = [str(address) for address in filtered_addresses]
        else:
            messagebox.showinfo("Поиск", "Адреса не найдены.")

    def filter_users(self, combobox):
        # Получаем введенный текст
        search_text = combobox.get()

        # Выполняем поиск пользователей по подстроке
        filtered_users = self.db.search_users_by_name(search_text)

        if filtered_users:
            combobox["values"] = [str(user) for user in filtered_users]
        else:
            messagebox.showinfo("Поиск", "Пользователи не найдены.")

    def create_request(self):
        # Получаем выбранные значения
        selected_user1_str = self.user1_combobox.get()
        selected_address_str = self.address_combobox.get()
        selected_user2_str = self.user2_combobox.get()
        comment = self.comment_text.get("1.0", "end-1c")  # Получаем текст из Text

        # Получаем id выбранных значений из словарей
        user1_id = self.user_dict.get(selected_user1_str)
        address_id = self.address_dict.get(selected_address_str)
        user2_id = self.user_dict.get(selected_user2_str)

        # Проверяем, что id найдены
        if user1_id and address_id and user2_id:
            request = self.db.get_request_by_data(user1_id, address_id, selected_user2_str, comment)

            if request:
                messagebox.showinfo("Ошибка!", "Данная запись уже существует!")
            else:
                self.db.create_request(user1_id, address_id, selected_user2_str, comment)
                self.on_close()  # Закрываем окно и вызываем callback
        else:
            messagebox.showinfo("Ошибка!", "Выбраны не все поля!")

    def on_close(self):
        # Вызываем callback-функцию, если она передана
        if self.on_close_callback:
            self.on_close_callback()
        
        # Закрываем окно
        self.window.destroy()