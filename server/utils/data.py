import win32file, time

def func(pipe):
    count = 0
    while count < 10:
        print(f"writing message {count}")
        # convert to bytes
        some_data = str.encode(f"{count}")
        win32file.WriteFile(pipe, some_data)
        time.sleep(1)
        count += 1

    win32file.CloseHandle(pipe)