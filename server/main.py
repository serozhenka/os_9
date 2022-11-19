import json
import time
import threading

import win32pipe
import win32file
import pywintypes

import settings
from scheduler import Scheduler
from storages.db import DbStorage
from utils import logging as logging_utils


class PipeServer:
    def __init__(self):
        self.logger = logging_utils.get_logger("server")

    def _create_pipe(self):
        self.logger.info("Creating additional pipe")
        return win32pipe.CreateNamedPipe(
            settings.PIPE_NAME,
            win32pipe.PIPE_ACCESS_DUPLEX,
            win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
            win32pipe.PIPE_UNLIMITED_INSTANCES,
            65536, 65536, 0, None,
        )

    def _connect_pipe(self, pipe):
        win32pipe.ConnectNamedPipe(pipe, None)
        self.logger.info("Client connected")

    def _func(self, pipe):
        storage = DbStorage()

        while True:
            self.logger.info(f"Sending message")
            weather = storage.get_last_weather()
            weather_json = json.dumps(vars(weather))

            try:
                win32file.WriteFile(pipe, str.encode(weather_json))
            except pywintypes.error as e:
                if e.args[0] == 232:
                    self.logger.info("Client disconnected")
                    break
            time.sleep(settings.SEND_TO_CLIENT_EVERY_N_SECONDS)

        win32file.CloseHandle(pipe)

    def _start_job(self, target_func, *args):
        self.logger.info("Starting new client thread")
        thread = threading.Thread(target=target_func, args=(*args,))
        thread.start()

    def run(self):
        while True:
            self.logger.info("Waiting for client")
            pipe = self._create_pipe()
            self._connect_pipe(pipe)
            self._start_job(self._func, pipe)


if __name__ == '__main__':
    scheduler = Scheduler()
    server = PipeServer()
    threading.Thread(target=scheduler.run, args=(), daemon=True).start()
    threading.Thread(target=server.run, args=(), daemon=True).start()

    while True:
        time.sleep(1)
