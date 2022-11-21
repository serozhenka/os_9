#include "weatherthread.h"

WeatherThread::WeatherThread(QObject *parent)
    : QThread{parent}
{

}

WeatherThread::~WeatherThread(){
    this->terminate();
}

void WeatherThread::log(QString toLog){
    QFile file("log.txt");
    if (!file.open(QIODevice::Append)) return;

    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << " | " << toLog << ".\n";

    emit EventHappened(toLog);
}

void WeatherThread::run(){
    isStopped = false;

    HANDLE hPipe;
    LPTSTR lpvMessage = (LPTSTR)L"Default message from client.";
    CHAR*  chBuf = new CHAR[BUFSIZE / sizeof(char) + 1];
    BOOL   fSuccess = FALSE;
    DWORD  cbRead, dwMode;
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
            log( "Could not open pipe. Error: " + QString::number(GetLastError()) );
            return;
        }

        if (!WaitNamedPipe(lpszPipename, 10000)) {
            log("Could not open pipe: 10 second wait timed out");
            return;
        }
    }

    emit ChangeStatus("Під'єднано");
    log("The pipe has been connected successfully");

    // The pipe connected; change to message-read mode.

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle
        &dwMode,  // new pipe mode
        NULL,     // don't set maximum bytes
        NULL);    // don't set maximum time
    if (!fSuccess) {
        log( "SetNamedPipeHandleState failed. Error: "  + QString::number(GetLastError()) );
        return;
    }

    do {
        // Read from the pipe.
        fSuccess = ReadFile(
        hPipe,    // pipe handle
        chBuf,    // buffer to receive reply
        BUFSIZE,  // size of buffer
        &cbRead,  // number of bytes read
        NULL);    // not overlapped

        chBuf[cbRead] = 0;

        QMutex mutex;
        mutex.lock();
            if(isStopped) break;
            emit InfoObtained(Weather(chBuf));
        mutex.unlock();
    } while (fSuccess);

    if (!fSuccess) {
        log( "ReadFile from pipe failed. Error: " + QString::number(GetLastError()) );
        return;
    }

    CloseHandle(hPipe);
    log("Disconnected from pipe succesfully");

    emit ChangeStatus("Не під'єднано");

    return;
}

void WeatherThread::kill()
{
    this->isStopped = true;
}

bool WeatherThread::isActive(){
    return !isStopped;
}
