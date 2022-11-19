import requests
from datetime import datetime
from typing import Union

from .constants import OPEN_WEATHER_URL
from weather.constants import Weather


class WeatherClient:

    @staticmethod
    def get_weather(city) -> Union[Weather, None]:
        response = requests.get(url=OPEN_WEATHER_URL.format(city=city), headers={'Content-Type': "application/json"})
        if not response.ok:
            return

        response = response.json()

        return Weather(
            temperature=response['main']['temp'],
            feels_like=response['main']['feels_like'],
            weather_type=response['weather'][0]['main'],
            city=city,
            date_added=datetime.now(),
        )
