import sqlite3

from storages.base import StorageInterface
from storages import db_queries
from weather.constants import Weather


class DbStorage(StorageInterface):

    def _initialize(self):
        self.con = sqlite3.connect("db.sqlite3")
        self.cur = self.con.cursor()
        self.cur.execute(db_queries.CREATE_TABLE)

    def get_last_weather(self) -> Weather:
        result = self.cur.execute(db_queries.SELECT_LAST_WEATHER)
        result = result.fetchone()
        return Weather(*result)

    def save_weather(self, weather: Weather):
        params = list(vars(weather).values())
        self.cur.execute(db_queries.INSERT_WEATHER, params)
        self.con.commit()

    def close(self):
        self.cur.close()
        self.con.close()

    def __del__(self):
        self.close()
