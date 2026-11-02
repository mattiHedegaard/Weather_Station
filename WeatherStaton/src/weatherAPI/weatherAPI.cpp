// Weather setup
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <map>

int weatherWarningIds[] = {75,82,86,95,96,97,99};
String url = String("https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=9.92&daily=weather_code,temperature_2m_max,temperature_2m_min,rain_sum,showers_sum,snowfall_sum,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant,precipitation_sum&hourly=temperature_2m,relative_humidity_2m,precipitation,weather_code,cloud_cover,wind_speed_10m,wind_direction_10m,wind_gusts_10m,snow_depth,surface_pressure,rain,snowfall,showers&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,rain,showers,snowfall,weather_code,cloud_cover,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m,precipitation&timezone=Europe%2FBerlin&wind_speed_unit=ms&forecast%3Flatitude=57.05&forecast_hours=12");

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

// Hourly weather Data


void GetHourlyForecast() {
  const int FORECAST_HOURS = 12;
  int hourTime[FORECAST_HOURS];
  float hourTemp[FORECAST_HOURS];
  int hourHum[FORECAST_HOURS];
  float hourRain[FORECAST_HOURS];
  float hourSnowfall[FORECAST_HOURS];
  float hourShowers[FORECAST_HOURS];
  float hourPrec[FORECAST_HOURS];
  int hourCode[FORECAST_HOURS];
  int hourCloudCover[FORECAST_HOURS];
  float hourWindSpeed[FORECAST_HOURS];
  int hourWindDir[FORECAST_HOURS];
  float hourWindGust[FORECAST_HOURS];
  float hourSnowDepth[FORECAST_HOURS];
  float hourPressure[FORECAST_HOURS];

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Loading hourly forecast...");
  display.display();

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, url)) {
    Serial.println("http.begin() failed");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("http.begin() failed...");
    display.display();
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
      for (int i = 0; i < FORECAST_HOURS; i++) {
        const char* t = doc["hourly"]["time"][i];
        hourTime[i] = t ? atoi(t+11) : 9999;
        hourTemp[i] = doc["hourly"]["temperature_2m"][i];
        hourHum[i] = doc["hourly"]["relative_humidity_2m"][i];
        hourRain[i] = doc["hourly"]["rain"][i];
        hourSnowfall[i] = doc["hourly"]["snowfall"][i];;
        hourShowers[i] = doc["hourly"]["showers"][i];;
        hourPrec[i] = doc["hourly"]["precipitation"][i];
        hourCode[i] = doc["hourly"]["weather_code"][i];
        hourCloudCover[i] = doc["hourly"]["cloud_cover"][i];
        hourWindSpeed[i] = doc["hourly"]["wind_speed_10m"][i];
        hourWindDir[i] = doc["hourly"]["wind_direction_10m"][i];
        hourWindGust[i] = doc["hourly"]["wind_gusts_10m"][i];
        hourSnowDepth[i] = doc["hourly"]["snow_depth"][i];
        hourPressure[i] = doc["hourly"]["surface_pressure"][i];
      }
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(code);
  }
  http.end();
}

// current weather data
void GetCurrentWeather(){
  float currentTemp;
  float currentFeels;
  bool currentIsDay;
  int currentHum;
  float currentRain;
  float currentSnowfall;
  float currentShowers;
  float currentPrec;
  int currentCode;
  int currentCloudCover;
  float currentWindSpeed;
  int currentWindDir;
  float currentWindGust;
  float currentPressure;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Loading current weather...");
  display.display();

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, url)) {
    Serial.println("http.begin() failed");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("http.begin() failed...");
    display.display();
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
      currentTemp = doc["current"]["temperature_2m"];
      currentFeels = doc["current"]["apparent_temperature"];
      currentIsDay = doc["current"]["is_day"].as<int>() == 1;
      currentHum = doc["current"]["relative_humidity_2m"];
      currentRain = doc["current"]["rain"];
      currentSnowfall = doc["current"]["snowfall"];
      currentShowers = doc["current"]["showers"];
      currentPrec = doc["current"]["precipitation"];
      currentCode = doc["current"]["weather_code"];
      currentCloudCover = doc["current"]["cloud_cover"];
      currentWindSpeed = doc["current"]["wind_speed_10m"];
      currentWindDir = doc["current"]["wind_direction_10m"];
      currentWindGust = doc["current"]["wind_gusts_10m"];
      currentPressure = doc["current"]["surface_pressure"];
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(code);
  }
  http.end();
}

// Daily weather data
void GetDailyForecast(){
  const int FORECAST_DAYS = 7;
  String dailyTime[FORECAST_DAYS];
  int dailyCode[FORECAST_DAYS];
  float dailyMaxTemp[FORECAST_DAYS];
  float dailyMinTemp[FORECAST_DAYS];
  float dailyRain[FORECAST_DAYS];
  float dailyShowers[FORECAST_DAYS];
  float dailySnowfall[FORECAST_DAYS];
  float dailyWindSpeed[FORECAST_DAYS];
  float dailyWindGusts[FORECAST_DAYS];
  int dailyWindDir[FORECAST_DAYS];
  float dailyPrec[FORECAST_DAYS];
  //this is not right but the right track
  struct DailyWeather {
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
    float precipitation;
  };

  return weather;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Loading daily forecast...");
  display.display();

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  if (!http.begin(client, url)) {
    Serial.println("http.begin() failed");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("http.begin() failed...");
    display.display();
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
      for (int i = 0; i < FORECAST_DAYS; i++) {
        dailyTime[i] = doc["daily"]["time"][i].as<String>();
        dailyCode[i] = doc["daily"]["weather_code"][i];
        dailyMaxTemp[i] = doc["daily"]["temperature_2m_max"][i];
        dailyMinTemp[i] = doc["daily"]["temperature_2m_min"][i];
        dailyRain[i] = doc["daily"]["rain_sum"][i];
        dailyShowers[i] = doc["daily"]["showers_sum"][i];
        dailySnowfall[i] = doc["daily"]["snowfall_sum"][i];
        dailyWindSpeed[i] = doc["daily"]["wind_speed_10m_max"][i];
        dailyWindGusts[i] = doc["daily"]["wind_gusts_10m_max"][i];
        dailyWindDir[i] = doc["daily"]["wind_direction_10m_dominant"][i];
        dailyPrec[i] = doc["daily"]["precipitation_sum"][i];
      }
      return 
    }
  } else {
    Serial.print("HTTP error: ");
    Serial.println(code);
  }
  http.end();
}
