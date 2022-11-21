#ifndef WEATHER_H
#define WEATHER_H
#include <windows.h>
#include <QJsonObject>
#include <QJsonDocument>

class Weather
{
private:
    float temperature;
    float feelsLike;
    QString weatherType;
    QString city;
    QString date;
public:
    Weather(CHAR* json);
    const QString &getCity() const;
    const QString &getWeatherType() const;
    const QString &getDate() const;
    float getTemperature() const;
    float getFeelsLike() const;
};

#endif
