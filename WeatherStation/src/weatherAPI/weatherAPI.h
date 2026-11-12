#pragma once
#include <Arduino.h>

//Current weather
struct currentWeather {
  float temp;
  float feels;
  bool isDay;
  int hum;
  float rain;
  float snowfall;
  float showers;
  float prec;
  int code;
  int cloudCover;
  float windSpeed;
  int windDir;
  float windGust;
  float pressure;
};

// Hourly weather Data
struct hourlyWeather {
  int time;
  float temp;
  int hum;
  float rain;
  float snowfall;
  float showers;
  float prec;
  int code;
  int cloudCover;
  float windSpeed;
  int windDir;
  float windGust;
  float snowDepth;
  float pressure;
};

// Daily weather
struct dailyWeather {
  String time;
  int code;
  float maxTemp;
  float minTemp;
  float rain;
  float showers;
  float snowfall;
  float windSpeed;
  float windGusts;
  int windDir;
  float prec;
};

const char* GetWeatherDescription(int code);

bool checkCodeWarning(int code);

void UpdateWeather();

currentWeather GetCurrentWeather();
hourlyWeather GetHourlyWeather(int hour);
dailyWeather GetDailyWeather(int day);
