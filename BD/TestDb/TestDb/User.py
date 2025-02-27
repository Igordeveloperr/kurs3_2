from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import relationship
from MyBaseModel import *

class User(MyBaseModel):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    password = Column(String)
    requests_client_applications = relationship("ClientApplication", back_populates="users")

    def __str__(self):
        return self.name