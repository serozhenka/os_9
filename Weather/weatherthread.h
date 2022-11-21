#ifndef WEATHERTHREAD_H
#define WEATHERTHREAD_H

#include <QThread>
#include <QtCore>
#include <QMutex>
#include <windows.h>
#include "weather.h"

#define BUFSIZE 512

class WeatherThread : public QThread {
private:
    Q_OBJECT;
    bool isStopped = true;
public:
    explicit WeatherThread(QObject *parent = nullptr);
    ~WeatherThread();
    void run();
    void kill();
    void log(QString);
    bool isActive();
signals:
    void InfoObtained(Weather);
    void ChangeStatus(QString);
    void EventHappened(QString);
};

#endif // WEATHERTHREAD_H
