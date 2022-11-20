#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <string.h>
#include <iostream>

#define BUFSIZE 65536

int main() {
    HANDLE hPipe;
    LPTSTR lpvMessage = (LPTSTR)L"Default message from client.";
    CHAR*  chBuf = new CHAR[BUFSIZE / sizeof(char) + 1];
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    LPTSTR lpszPipename = (LPTSTR)L"\\\\.\\pipe\\Foo";
    // Try to open a named pipe; wait for it, if necessary.

    while (1) {
        hPipe = CreateFile(
            lpszPipename,   // pipe name
            GENERIC_READ |  // read and write access
            GENERIC_WRITE,
            0,              // no sharing
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,              // default attributes
            NULL);          // no template file

      // Break if the pipe handle is valid.

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs.

        if (GetLastError() != ERROR_PIPE_BUSY) {
            printf("Could not open pipe. GLE=%d\n", GetLastError());
            return -1;
        }

        // All pipe instances are busy, so wait for 20 seconds.

        if (!WaitNamedPipe(lpszPipename, 20000)) {
            printf("Could not open pipe: 20 second wait timed out.");
            return -1;
        }
    }

    // The pipe connected; change to message-read mode.

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle
        &dwMode,  // new pipe mode
        NULL,     // don't set maximum bytes
        NULL);    // don't set maximum time
    if (!fSuccess) {
        printf("SetNamedPipeHandleState failed. GLE=%d\n", GetLastError());
        return -1;
    }

    do {
        // Read from the pipe.
        fSuccess = ReadFile(
            hPipe,    // pipe handle
            chBuf,    // buffer to receive reply
            BUFSIZE,  // size of buffer
            &cbRead,  // number of bytes read
            NULL);    // not overlapped
        std::cout << "success: " << fSuccess << std::endl;
        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;
        chBuf[cbRead] = 0;
        std::cout << "its string: " << chBuf << "read byes: " << (int) cbRead << std::endl;
    } while (fSuccess);  // repeat loop if ERROR_MORE_DATA

    if (!fSuccess) {
        printf("ReadFile from pipe failed. GLE=%d\n", GetLastError());
        return -1;
    }

    printf("\n<End of message, press ENTER to terminate connection and exit>");
    _getch();

    CloseHandle(hPipe);

    return 0;
}