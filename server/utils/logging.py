import logging
import sys

def get_logger(name):
    logger = logging.getLogger(name)
    logger.setLevel(logging.INFO)
    setup_stdout_stream_handler(logger, logging.INFO)
    return logger


def setup_stdout_stream_handler(logger: logging.Logger, loglevel: int):
    stdout_stream_handler = logging.StreamHandler()
    stdout_stream_handler.setStream(sys.stdout)
    stdout_stream_handler.setLevel(loglevel)

    formatter = logging.Formatter(
        "%(asctime)s | %(name)s | %(levelname)s:  %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )
    stdout_stream_handler.setFormatter(formatter)
    logger.addHandler(stdout_stream_handler)
