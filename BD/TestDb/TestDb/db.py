from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy import or_, cast, func
from User import *
from ClientApplication import *
from ApartmentBuilding import *

class MyDb():
    def __init__(self):
        self.engine = create_engine("postgresql://postgres:13579@localhost/mydb")
        self.Session = sessionmaker(autoflush=False, bind=self.engine)

    def get_address_by_details(self, city, street, house_number):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Ищем адрес по городу, улице и номеру дома
                address = db.query(ApartmentBuilding).filter(
                    ApartmentBuilding.city == city,
                    ApartmentBuilding.street == street,
                    ApartmentBuilding.house_number == house_number
                ).first()
                return address
        except SQLAlchemyError as e:
            print(f"Ошибка при поиске адреса: {e}")
            return None

    def add_address(self, city, street, house_number):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Создаем новый объект ApartmentBuilding
                new_address = ApartmentBuilding(
                    city=city,
                    street=street,
                    house_number=house_number
                )
                db.add(new_address)
                db.commit()
                print(f"Адрес успешно добавлен: {city}, {street}, {house_number}")
                return new_address
        except SQLAlchemyError as e:
            db.rollback()
            print(f"Ошибка при добавлении адреса: {e}")
            return None

    def get_user_by_name_and_password(self, name, password):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Ищем пользователя по имени и паролю
                user = db.query(User).filter(
                    User.name == name,
                    User.password == password
                ).first()
                return user
        except SQLAlchemyError as e:
            print(f"Ошибка при поиске пользователя: {e}")
            return None

    def add_user(self, username, password):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Создаем нового пользователя
                new_user = User(name=username, password=password)
                db.add(new_user)
                db.commit()
                print(f"Пользователь {username} успешно добавлен!")
                return new_user
        except SQLAlchemyError as e:
            db.rollback()
            print(f"Ошибка при добавлении пользователя: {e}")
            return None

    def search_addresses_by_text(self, search_text):
        with self.Session(autoflush=False, bind=self.engine) as db:
            # Если search_text пустой, возвращаем все адреса
            if not search_text:
                addresses = db.query(ApartmentBuilding).all()
                return addresses

            # Преобразуем house_number в текст для поиска
            house_number_as_text = cast(ApartmentBuilding.house_number, String)

            # Выполняем поиск адресов по подстроке
            addresses = db.query(ApartmentBuilding).filter(
                or_(
                    ApartmentBuilding.city.ilike(f"%{search_text}%"),  # Поиск по городу
                    ApartmentBuilding.street.ilike(f"%{search_text}%"),  # Поиск по улице
                    house_number_as_text.ilike(f"%{search_text}%")  # Поиск по номеру дома (как текст)
                )
            ).all()
            return addresses

    def search_users_by_name(self, search_text):
        with self.Session(autoflush=False, bind=self.engine) as db:
            # Выполняем поиск пользователей по подстроке
            users = db.query(User).filter(
                or_(
                    User.name.ilike(f"%{search_text}%"),  # Поиск по имени (регистронезависимый)
                )
            ).all()
            return users
            
    def get_users(self):
        with self.Session(autoflush=False, bind=self.engine) as db:
            users = db.query(User).order_by(User.id.desc()).all()
            return users
    
    def get_apartments(self):
        with self.Session(autoflush=False, bind=self.engine) as db:
            apartments = db.query(ApartmentBuilding).order_by(ApartmentBuilding.id.desc()).all()
            return apartments
    
    def get_total_requests(self):
        with self.Session(autoflush=False, bind=self.engine) as db:
            # Выполняем запрос для получения общего количества заявок
            total_requests = db.query(func.count(ClientApplication.id)).scalar()
            return total_requests
        
    def get_requests(self, limit=None, offset=None):
        with self.Session(autoflush=False, bind=self.engine) as db:
            # Выполняем запрос с пагинацией
            query = db.query(ClientApplication, ApartmentBuilding, User)\
                .join(ApartmentBuilding)\
                .join(User)\
                .order_by(ClientApplication.id.desc())

            if limit is not None:
                query = query.limit(limit)
            if offset is not None:
                query = query.offset(offset)

            requests = query.all()
            return requests
        
    def get_request_by_id(self, id):
        with self.Session(autoflush=False, bind=self.engine) as db:
            request = (
                db.query(ClientApplication)
                .filter(ClientApplication.id == id)
                .first()
            )
            return request
    
    def del_request_by_id(self, id):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Находим запись по id
                request = db.query(ClientApplication).filter(ClientApplication.id == id).first()
                if request:
                    # Удаляем запись
                    db.delete(request)
                    db.commit()
                    print(f"Запись с id={id} успешно удалена.")
                else:
                    print(f"Запись с id={id} не найдена.")
        except SQLAlchemyError as e:
            db.rollback()
            print(f"Ошибка при удалении записи: {e}")
    
    def get_request_by_data(self, client_id, apart_id, resident_name, comments):
        with self.Session(autoflush=False, bind=self.engine) as db:
            # Находим запись по id
            request = db.query(ClientApplication).filter(ClientApplication.client_id == client_id).filter(ClientApplication.object_num_id == apart_id).filter(ClientApplication.resident_name == resident_name).filter(ClientApplication.comments == comments).first()
            return request

    def create_request(self, client_id, apart_id, resident_name, comments):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Создаем новый объект ClientApplication
                new_request = ClientApplication(
                    client_id=client_id,
                    object_num_id=apart_id,
                    resident_name=resident_name,
                    comments=comments,
                    object_type_id = 1
                )
            
                # Добавляем объект в сессию
                db.add(new_request)
            
                # Фиксируем изменения в базе данных
                db.commit()
            
                print("Заявка успешно создана!")
                return new_request  # Возвращаем созданный объект (опционально)
    
        except SQLAlchemyError as e:
            # В случае ошибки откатываем изменения
            db.rollback()
            print(f"Ошибка при создании заявки: {e}")
            return None
        
    def update_request(self, request_id, client_id, address_id, owner_name, comment):
        try:
            with self.Session(autoflush=False, bind=self.engine) as db:
                # Находим заявку по ID
                request = db.query(ClientApplication).filter(ClientApplication.id == request_id).first()
                
                if request:
                    # Обновляем поля заявки
                    request.client_id = client_id
                    request.object_num_id = address_id
                    request.resident_name = owner_name
                    request.comments = comment
                    
                    # Фиксируем изменения в базе данных
                    db.commit()
                    print("Заявка успешно обновлена!")
                    return request  # Возвращаем обновленный объект (опционально)
                else:
                    print("Заявка с указанным ID не найдена.")
                    return None
        
        except SQLAlchemyError as e:
            # В случае ошибки откатываем изменения
            db.rollback()
            print(f"Ошибка при обновлении заявки: {e}")
            return None