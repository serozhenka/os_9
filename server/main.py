import time
import sys

import settings

import win32pipe, win32file, pywintypes, win32api
import threading

from utils.data import func
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

    def _start_job(self, target_func, *args):
        self.logger.info("Starting new client thread")
        thread = threading.Thread(target=target_func, args=(*args,))
        thread.start()

    def run(self):
        while True:
            self.logger.info("Waiting for client")
            pipe = self._create_pipe()
            self._connect_pipe(pipe)
            self._start_job(func, pipe)

def pipe_client():
    print("pipe client")
    quit = False

    while not quit:
        try:
            handle = win32file.CreateFile(
                r'\\.\pipe\Foo',
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                0,
                None
            )
            res = win32pipe.SetNamedPipeHandleState(handle, win32pipe.PIPE_READMODE_MESSAGE, None, None)
            if res == 0:
                print(f"SetNamedPipeHandleState return code: {res}")
            while True:
                resp = win32file.ReadFile(handle, 64*1024)
                print(f"message: {resp}")
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe, bye bye")
                quit = True

if __name__ == '__main__':

    if sys.argv[1] == "server":
        server = PipeServer()
        server.run()

    elif sys.argv[1] == "client":
        pipe_client()

