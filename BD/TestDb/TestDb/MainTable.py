from tkinter import *
from tkinter import ttk
import tkinter as tk
from tkinter import messagebox
from CreateRequestWindow import *
from EditRequestWindow import *

class DataTableApp:
    def __init__(self, root, db):
        self.root = root
        self.db = db
        self.current_page = 1  # Текущая страница
        self.items_per_page = 10  # Количество элементов на одной странице
        self.total_requests = self.db.get_total_requests()  # Общее количество заявок
        self.data = []  # Данные для текущей страницы
        self.load_data()  # Загружаем данные для первой страницы
        self.create_table()

    def get_total_pages(self):
        # Вычисляем общее количество страниц
        return (self.total_requests + self.items_per_page - 1) // self.items_per_page

    def load_data(self):
        # Вычисляем offset для текущей страницы
        offset = (self.current_page - 1) * self.items_per_page
        # Загружаем данные из базы данных
        db_data = self.db.get_requests(limit=self.items_per_page, offset=offset)
        self.data = self.convert_requests(db_data)

    def create_table(self):
        # Очищаем текущие виджеты
        for widget in self.root.winfo_children():
            widget.destroy()

        # Кнопка "Создать заявку"
        create_button = tk.Button(self.root, text="Создать заявку", command=self.create_request)
        create_button.grid(row=0, column=0, columnspan=1, pady=10)

        # Заголовки таблицы
        headers = ["Собственник", "Комментарий", "Адрес", "Имя клиента", "Просмотр", "Редактирование", "Удаление"]
        for col, header in enumerate(headers):
            label = tk.Label(self.root, text=header, font=("Arial", 12, "bold"))
            label.grid(row=1, column=col, padx=10, pady=5)

        # Отображаем данные
        for row, item in enumerate(self.data):
            tk.Label(self.root, text=item["owner"]).grid(row=row + 2, column=0, padx=10, pady=5)
            tk.Label(self.root, text=item["comment"]).grid(row=row + 2, column=1, padx=10, pady=5)
            tk.Label(self.root, text=item["address"]).grid(row=row + 2, column=2, padx=10, pady=5)
            tk.Label(self.root, text=item["client_name"]).grid(row=row + 2, column=3, padx=10, pady=5)

            # Кнопка просмотра
            view_button = tk.Button(self.root, text="Просмотр", command=lambda i=item: self.view_item(i))
            view_button.grid(row=row + 2, column=4, padx=5, pady=5)

            # Кнопка редактирования
            edit_button = tk.Button(self.root, text="Редактировать", command=lambda i=item: self.edit_item(i))
            edit_button.grid(row=row + 2, column=5, padx=5, pady=5)

            # Кнопка удаления
            delete_button = tk.Button(self.root, text="Удалить", command=lambda i=item: self.delete_item(i))
            delete_button.grid(row=row + 2, column=6, padx=5, pady=5)

        # Кнопки пагинации
        pagination_frame = Frame(self.root)
        pagination_frame.grid(row=len(self.data) + 3, column=0, columnspan=7, pady=10)

        # Кнопка "Назад"
        prev_button = Button(pagination_frame, text="Назад", command=self.prev_page)
        prev_button.pack(side=LEFT, padx=5)

        # Кнопка "Вперед"
        next_button = Button(pagination_frame, text="Вперед", command=self.next_page)
        next_button.pack(side=LEFT, padx=5)

        # Отображение текущей страницы
        page_label = Label(pagination_frame, text=f"Страница {self.current_page}")
        page_label.pack(side=LEFT, padx=5)

    def create_request(self):
        # Создаем окно редактирования и передаем callback-функцию
        self.edit_window = CreateRequestWindow(self.root, self.db, on_close_callback=self.update_table)

    def update_table(self):
        # Обновляем данные таблицы
        self.load_data()
        self.refresh_table()
        print("Таблица обновлена")

    def view_item(self, item):
        messagebox.showinfo("Просмотр элемента", f"Собственник: {item['owner']}\nКомментарий: {item['comment']}\nАдрес: {item['address']}\nИмя клиента: {item['client_name']}")

    def edit_item(self, item):
        EditRequestWindow(self.root, self.db, self.db.get_request_by_id(item["id"]), on_close_callback=self.update_table)

    def delete_item(self, item):
        delWindow = Tk()
        lbl = Label(delWindow, text="Вы точно хотите удалить запись?")
        lbl.pack(pady=5)
        # Функция для обработки нажатия на кнопку "Да"
        def on_yes():
            self.db.del_request_by_id(item["id"])
            self.update_table()
            delWindow.destroy()

        # Функция для обработки нажатия на кнопку "Нет"
        def on_no():
            delWindow.destroy()

        yesBtn = Button(delWindow, text="Да", command=on_yes)
        noBtn = Button(delWindow, text="Нет", command=on_no)
        yesBtn.pack(pady=5)
        noBtn.pack(pady=5)
        delWindow.mainloop()

    def refresh_table(self):
        # Обновляем таблицу с учетом текущей страницы
        self.create_table()

    def prev_page(self):
        # Переход на предыдущую страницу
        if self.current_page > 1:
            self.current_page -= 1
            self.load_data()
            self.refresh_table()

    def next_page(self):
        # Переход на следующую страницу
        total_pages = self.get_total_pages()
        if self.current_page < total_pages:
            self.current_page += 1
            self.load_data()
            self.refresh_table()

    @staticmethod
    def convert_requests(data):
        outData = []
        # Заполняем текстовое поле данными из requests
        for r, a, u in data:
            obj = {
                "id": r.id,
                "owner": r.resident_name,
                "comment": r.comments,
                "address": f"{a.city}, {a.street}, д.{a.house_number}",
                "client_name": u.name
            }
            outData.append(obj)
        return outData