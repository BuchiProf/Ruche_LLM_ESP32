import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    API = os.getenv("API_THINGSPEAK")
    #SECRET_KEY = os.getenv('SECRET_KEY')
    #SQLALCHEMY_DATABASE_URI = os.getenv('DATABASE_URL')
    #API_KEY = os.getenv('API_KEY')