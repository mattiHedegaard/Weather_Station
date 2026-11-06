// Weather setup
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weatherAPI.h"
#include "src/rgb/rgb.h"


String url = String("https://api.open-meteo.com/v1/forecast?latitude=57.039968&longitude=9.931585&daily=weather_code,temperature_2m_max,temperature_2m_min,rain_sum,showers_sum,snowfall_sum,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant,precipitation_sum&hourly=temperature_2m,relative_humidity_2m,precipitation,weather_code,cloud_cover,wind_speed_10m,wind_direction_10m,wind_gusts_10m,snow_depth,surface_pressure,rain,snowfall,showers&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,rain,showers,snowfall,weather_code,cloud_cover,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m,precipitation&timezone=Europe%2FBerlin&wind_speed_unit=ms&forecast%3Flatitude=57.05&forecast_hours=12");

// Weather codes
const char* GetWeatherDescription(int code) {
  switch (code) {
    case 0:  return "Clear sky";
    case 1:  return "Mainly clear";
    case 2:  return "Partly cloudy";
    case 3:  return "Overcast";
    case 45: return "Fog";
    case 48: return "Rime fog";
    case 51: return "Light drizzle";
    case 53: return "Moderate drizzle";
    case 55: return "Dense drizzle";
    case 56: return "Light freezing drizzle";
    case 57: return "Dense freezing drizzle";
    case 61: return "Slight rain";
    case 63: return "Moderate rain";
    case 65: return "Heavy rain";
    case 66: return "Light freezing rain";
    case 67: return "Heavy freezing rain";
    case 71: return "Slight snowfall";
    case 73: return "Moderate snowfall";
    case 75: return "Heavy snowfall";
    case 77: return "Snow grains";
    case 80: return "Slight showers";
    case 81: return "Moderate showers";
    case 82: return "Violent showers";
    case 85: return "Slight snow showers";
    case 86: return "Heavy snow showers";
    case 95: return "Thunderstorm";
    case 96: return "Thunderstorm with slight hail";
    case 99: return "Thunderstorm with heavy hail";
    default: return "Unknown weather";
  }
}

int weatherWarningIds[] = {75,82,86,95,96,97,99};

bool checkCodeWarning(int code) {
  for (int i = 0; i < sizeof(weatherWarningIds) / sizeof(weatherWarningIds[0]); i++) {
    if (code == weatherWarningIds[i]) {
      return true;
    }
  }
  return false;
}

//Current weather
currentWeather cw;

// Hourly weather Data
const int FORECAST_HOURS = 12;
hourlyWeather hw[FORECAST_HOURS];

// Daily weather
const int FORECAST_DAYS = 7;
dailyWeather dw[FORECAST_DAYS];

void UpdateWeather(){
  RgbSetColor(140, 20, 255);
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, url)) {
    Serial.println("http.begin() failed");
    return;
  }

  int code = http.GET();
  if (code == 200) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, http.getString());
    if (err){
      Serial.print("JSON error: ");
      Serial.println(err.c_str());
    } else {
      //Current weather
      cw.temp = doc["current"]["temperature_2m"];
      cw.feels = doc["current"]["apparent_temperature"];
      cw.isDay = doc["current"]["is_day"].as<int>() == 1;
      cw.hum = doc["current"]["relative_humidity_2m"];
      cw.rain = doc["current"]["rain"];
      cw.snowfall = doc["current"]["snowfall"];
      cw.showers = doc["current"]["showers"];
      cw.prec = doc["current"]["precipitation"];
      cw.code = doc["current"]["weather_code"];
      cw.cloudCover = doc["current"]["cloud_cover"];
      cw.windSpeed = doc["current"]["wind_speed_10m"];
      cw.windDir = doc["current"]["wind_direction_10m"];
      cw.windGust = doc["current"]["wind_gusts_10m"];
      cw.pressure = doc["current"]["surface_pressure"];

      //Houtly Forecast
      for (int i = 0; i < FORECAST_HOURS; i++) {
        const char* t = doc["hourly"]["time"][i];

        hw[i].time = t ? atoi(t + 11) : 9999;
        hw[i].temp = doc["hourly"]["temperature_2m"][i];
        hw[i].hum = doc["hourly"]["relative_humidity_2m"][i];
        hw[i].rain = doc["hourly"]["rain"][i];
        hw[i].snowfall = doc["hourly"]["snowfall"][i];
        hw[i].showers = doc["hourly"]["showers"][i];
        hw[i].prec = doc["hourly"]["precipitation"][i];
        hw[i].code = doc["hourly"]["weather_code"][i];
        hw[i].cloudCover = doc["hourly"]["cloud_cover"][i];
        hw[i].windSpeed = doc["hourly"]["wind_speed_10m"][i];
        hw[i].windDir = doc["hourly"]["wind_direction_10m"][i];
        hw[i].windGust = doc["hourly"]["wind_gusts_10m"][i];
        hw[i].snowDepth = doc["hourly"]["snow_depth"][i];
        hw[i].pressure = doc["hourly"]["surface_pressure"][i];
      }

      //Daily Forecast
      for (int i = 0; i < FORECAST_DAYS; i++) {
        dw[i].time = doc["daily"]["time"][i].as<String>();
        dw[i].code = doc["daily"]["weather_code"][i];
        dw[i].maxTemp = doc["daily"]["temperature_2m_max"][i];
        dw[i].minTemp = doc["daily"]["temperature_2m_min"][i];
        dw[i].rain = doc["daily"]["rain_sum"][i];
        dw[i].showers = doc["daily"]["showers_sum"][i];
        dw[i].snowfall = doc["daily"]["snowfall_sum"][i];
        dw[i].windSpeed = doc["daily"]["wind_speed_10m_max"][i];
        dw[i].windGusts = doc["daily"]["wind_gusts_10m_max"][i];
        dw[i].windDir = doc["daily"]["wind_direction_10m_dominant"][i];
        dw[i].prec = doc["daily"]["precipitation_sum"][i];
      }
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(code);
  }
  http.end();
  RgbSetColor(0,0,0);
}

currentWeather GetCurrentWeather(){
  return cw;
}

hourlyWeather GetHourlyWeather(int hour){
  return hw[hour];
}

dailyWeather GetDailyWeather(int day){
  return dw[day];
}
