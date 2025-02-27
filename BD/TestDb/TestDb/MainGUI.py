from tkinter import *
from tkinter import ttk
from db import MyDb
from MainTable import *

db = MyDb()

# точка входа
def init_app():
    global window, text_area, update_button
    window = Tk()
    window.geometry("1200x600")
    window.title("Панель администратора")
    # window.resizable(False, False)
    requests = load_requests()
    # table = DataTableApp(window, convert_requests(requests), db)
    table = DataTableApp(window, db)

    window.mainloop()

def convert_requests(data):
    outData = []
    # Заполняем текстовое поле данными из requests
    for r, a, u in data:
        obj = {
            "id" : r.id,
            "owner" : r.resident_name,
            "comment" : r.comments,
            "address" : f"{a.city}, {a.street}, д.{a.house_number}",
            "client_name" : u.name
        }
        outData.append(obj)
    return outData

# подгрузка пользователей из БД
def load_users():
    users = db.get_users()
    return users

def load_requests():
    data = db.get_requests()
    return data