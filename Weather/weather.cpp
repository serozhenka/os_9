#include "weather.h"

float Weather::getTemperature() const

{
    return temperature;
}

float Weather::getFeelsLike() const
{
    return feelsLike;
}

const QString &Weather::getCity() const
{
    return city;
}

const QString &Weather::getWeatherType() const
{
    return weatherType;
}

const QString &Weather::getDate() const
{
    return date;
}

Weather::Weather(CHAR* json){
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(json);

    if(!doc.isNull() && doc.isObject()) {
        obj = doc.object();
    }

    temperature = obj["temperature"].toDouble();
    feelsLike = obj["feels_like"].toDouble();
    weatherType = obj["weather_type"].toString();
    city = obj["city"].toString();
    date = obj["date_added"].toString();
};
