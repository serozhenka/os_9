from datetime import datetime
from dataclasses import dataclass

import settings


OPEN_WEATHER_URL = (
    'https://api.openweathermap.org/data/2.5/weather?'
    'q={city}&'
    f'appid={settings.OPEN_WEATHER_KEY}&units=metric'
)

@dataclass
class Weather:
    temperature: float
    feels_like: float
    weather_type: str
    city: str
    date_added: datetime
