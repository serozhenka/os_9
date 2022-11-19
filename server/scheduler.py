import schedule
import time

import settings
from storages.db import DbStorage
from utils import logging as logging_utils
from weather.client import WeatherClient


class Scheduler:

    def __init__(self):
        self.client = WeatherClient()
        self.logger = logging_utils.get_logger("server")

    def get_new_weather(self):
        storage = DbStorage()
        weather = self.client.get_weather(settings.WEATHER_TOWN)
        storage.save_weather(weather)
        self.logger.info("Saved new weather record")

    def run(self):
        schedule.every(settings.GET_WEATHER_EVERY_N_SECONDS).seconds.do(self.get_new_weather)
        schedule.run_all()

        while True:
            schedule.run_pending()
            time.sleep(1)

if __name__ == '__main__':
    import threading
    scheduler = Scheduler()
    threading.Thread(target=scheduler.run, args=()).start()
