from weather.constants import Weather

class StorageInterface:

    def __init__(self):
        self._initialize()

    def _initialize(self):
        raise NotImplementedError

    def get_last_weather(self) -> Weather:
        raise NotImplementedError

    def save_weather(self, weather: Weather):
        raise NotImplementedError
