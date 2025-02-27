from tkinter import Toplevel, Label, Button, Entry, messagebox

class AddAddressWindow:
    def __init__(self, parent, db, on_address_added_callback=None):
        self.window = Toplevel(parent)
        self.window.title("Добавить адрес")
        self.window.geometry("300x300")
        self.window.resizable(False, False)

        self.db = db
        self.on_address_added_callback = on_address_added_callback

        # Поля для ввода города, улицы и номера дома
        Label(self.window, text="Город:").pack(pady=5)
        self.city_entry = Entry(self.window)
        self.city_entry.pack(pady=5)

        Label(self.window, text="Улица:").pack(pady=5)
        self.street_entry = Entry(self.window)
        self.street_entry.pack(pady=5)

        Label(self.window, text="Номер дома:").pack(pady=5)
        self.house_number_entry = Entry(self.window)
        self.house_number_entry.pack(pady=5)

        # Кнопка "Добавить"
        Button(self.window, text="Добавить", command=self.add_address).pack(pady=10)

    def add_address(self):
        city = self.city_entry.get()
        street = self.street_entry.get()
        house_number = self.house_number_entry.get()

        if city and street and house_number:
            address = self.db.get_address_by_details(city, street, house_number)
            if address:
                messagebox.showinfo("Ошибка!", "Данный адрес уже существует!")
            else:
                # Сохраняем новый адрес в базу данных
                new_address = self.db.add_address(city, street, house_number)
                if new_address:
                    if self.on_address_added_callback:
                        self.on_address_added_callback()
                    self.window.destroy()
                else:
                    messagebox.showinfo("Ошибка", "Не удалось добавить адрес.")
        else:
            messagebox.showinfo("Ошибка", "Заполните все поля!")