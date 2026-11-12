#include <ESP8266WiFi.h>

// WiFi
const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

// Weather
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* apiKey = "557548360f0a5cbcc24c517d390c8e45";
const char* city = "Aalborg,DK";

unsigned long  lastFetch = 0;
const unsigned long interval = 5000;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.print("\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("connecting to ");
  Serial.print(ssid); Serial.println("...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED and i < 60) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect");
    return;
  }

  Serial.println("\n");
  Serial.println("Connection established!");
  Serial.print("IP adress:\t");
  Serial.println(WiFi.localIP());

}

void loop() {
    
}

void GetWeather() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = String("https://api.openweathermap.org/data/2.5/weather?q=") + city + "&units=metric&appid=" + apiKey;
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
      float temp = doc["main"]["temp"];
      float feels = doc["main"]["feels_like"];
      int humidity = doc["main"]["humidity"];
      int pressure = doc["main"]["pressure"];
      float wind = doc["wind"]["speed"];
      const char* d = doc["weather"][0]["description"];

      Serial.printf("Temp: %.1f C (feels %.1f)\n", temp, feels);
      Serial.printf("Humidity: %d %%  Pressure: %d hPa\n", humidity, pressure);
      Serial.printf("Wind: %.1f m/s %s\n", wind, d);
    } 
  } else{
      Serial.print("HTTP error: ");
      Serial.println(code);
  }
  http.end();

}







