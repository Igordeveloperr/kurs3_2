from sqlalchemy import Column, Integer, String, ForeignKey
from sqlalchemy.orm import relationship
from MyBaseModel import *

class ClientApplication(MyBaseModel):
    __tablename__ = "requests_client_applications"

    id = Column(Integer, primary_key=True, index=True)
    resident_name = Column(String)
    comments = Column(String)
    object_num_id = Column(Integer, ForeignKey("apartment_buildings.id"))
    apartment_buildings = relationship("ApartmentBuilding", back_populates="requests_client_applications")
    object_type_id = Column(Integer)
    client_id = Column(Integer, ForeignKey("users.id"))
    users = relationship("User", back_populates="requests_client_applications")