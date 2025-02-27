class ApartmentBuilding(MyBaseModel):
    __tablename__ = "apartment_buildings"

    id = Column(Integer, primary_key=True, index=True)
    city = Column(String)
    street = Column(String)
    house_number = Column(Integer)
    requests_client_applications = relationship("ClientApplication", back_populates="apartment_buildings")

    def __str__(self):
        return f"{self.city}, {self.street}, д.{self.house_number}"


class User(MyBaseModel):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    password = Column(String)
    requests_client_applications = relationship("ClientApplication", back_populates="users")

    def __str__(self):
        return self.name