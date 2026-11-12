// WiFi setup
#include <ESP8266WiFi.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

// Screen setup
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Weather
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* apiKey = "557548360f0a5cbcc24c517d390c8e45";
const char* city = "Aalborg,DK";

unsigned long  lastFetch = 0;
const unsigned long interval = 10UL * 60UL * 1000UL;

// Pin setup
#define UPDATE_BTN D0
#define RGB_RED D7
#define RGB_GREEN D6
#define RGB_BLUE D5
#define YELLOW_LED D8

// Button setup
int lastBtn = HIGH;
unsigned long lastChange = 0;

// State Variables
int state = 0;
int states = 3;
int stateUpdateInterval = 10000;
int lastStateChange = 0;
uint16_t textColor = SSD1306_WHITE;

// Weather Variables
int weatherWarningBlinkInterval = 500;
int weatherWarningLastBlink = 0;
bool boxIsWhite = true;

void setup() {
  Serial.begin(9600);
  Wire.begin(D2,D1);
  delay(100);
  Serial.print("\n");

  // Screen 
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 not found"));
    for (;;);
  }

  // LED's
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  // Button
  pinMode(UPDATE_BTN, INPUT_PULLUP);

  //WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("connecting to ");
  Serial.print(ssid); Serial.println("...");

  int i = 0;
  char load[] = "/-\\/-\\";
  while (WiFi.status() != WL_CONNECTED and i < 60) {
    (i % 2 == 0) ? digitalWrite(YELLOW_LED, HIGH) : digitalWrite(YELLOW_LED, LOW);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Connecting to WiFi...");
    display.setTextSize(3);
    display.print(load[i % 6]);
    display.display();

    delay(250);
    Serial.print(++i); Serial.print(' ');
  }
  digitalWrite(YELLOW_LED, LOW);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect");
    RgbRed();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Failed to connect...");
    display.display();
    delay(5000);
    return;
  }

  for (int i = 0; i < 15; i++){
    (i % 2 == 0) ? RgbGreen() : RgbTurnOff();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("WiFi connected!");
    display.display();

    delay(100);
  }
  RgbTurnOff();

  GetWeather();
  lastStateChange = millis();

  Serial.println("\n");
  Serial.println("Connection established!");
  Serial.print("IP adress:\t");
  Serial.println(WiFi.localIP());

}

// Weather Data
float temp;
float feels;
int humidity;
int pressure;
float wind;
String d;
int weatherId;
int weatherWarningIds[] = {
  200, 201, 202, 210, 211, 212, 221, 230, 231, 232,  // Thunderstorms
  502, 503, 504, 511, 521, 522,                      // Rain
  602, 621, 622                                      // Snow
};

void GetWeather() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Loading weather...");
  display.display();

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
      temp = doc["main"]["temp"];
      feels = doc["main"]["feels_like"];
      humidity = doc["main"]["humidity"];
      pressure = doc["main"]["pressure"];
      wind = doc["wind"]["speed"];
      d = doc["weather"][0]["description"].as<String>();
      weatherId = doc["weather"][0]["id"];

      Serial.printf("Temp: %.1f C (feels %.1f)\n", temp, feels);
      Serial.printf("Humidity: %d %%  Pressure: %d hPa\n", humidity, pressure);
      Serial.printf("Wind: %.1f m/s %s\n", wind, d.c_str());
      Serial.printf("Id: %d\n", weatherId);
    } 
  } else{
      Serial.print("HTTP error: ");
      Serial.println(code);
  }
  http.end();

}

void loop() {
  int btn = digitalRead(UPDATE_BTN);

  if(millis() - lastFetch >= interval || CheckBtn(btn)){
    Serial.println("fetching weather");
    lastFetch = millis();
    GetWeather();
  }

  // State
  if (millis() - lastStateChange >= stateUpdateInterval){
    state++;
    if (state == states) {
      state = 0;
    }

    lastStateChange = millis();
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(textColor);
  display.setCursor(0,0);
  switch(state){
    case 0:
      display.setTextSize(1);
      display.printf("Temp: %.1f C\n \n%s\n", temp, d.c_str());
    break;

    case 1:
      display.setTextSize(1);
      display.printf("Humidity: %d %%\n \nPressure: %d hPa\n", humidity, pressure);
    break;

    case 2:
      display.setTextSize(1);
      display.printf("Wind: %.1f m/s\n \nFeels like %.1f C\n", wind, feels);
    break;
  }

  // LED weather information
  (temp <= 0) ? RgbBlue() : RgbTurnOff();
  
  bool isWarning = ArrayContainsInt(weatherWarningIds, weatherId);
  if (isWarning){
    String warningMsg = "!!!";
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(warningMsg, 0, 0, &x1, &y1, &w, &h);

    int boxW = w+1;
    int boxH = h+4;
    int boxX = SCREEN_WIDTH - boxW;
    int boxY = 0;

    if(millis() - weatherWarningLastBlink >= weatherWarningBlinkInterval){
      boxIsWhite = !boxIsWhite;
      weatherWarningLastBlink = millis();
    }
    display.drawRect(boxX, boxY, boxW, boxH, (boxIsWhite) ? SSD1306_WHITE : SSD1306_BLACK);
    display.setCursor(boxX+1,boxY+2);
    display.print(warningMsg);

    (weatherId >= 600 && weatherId < 700) ? RgbWhite() : RgbRed();
    } else {
      RgbTurnOff();
    }

  unsigned long timeLeft = stateUpdateInterval - (millis() - lastStateChange);
  int lineEndX = map(timeLeft, 0, stateUpdateInterval, 0, SCREEN_WIDTH - 1);
  lineEndX = constrain(lineEndX, 0, SCREEN_WIDTH - 1);

  display.setCursor(0,0);
  display.drawLine(0, SCREEN_HEIGHT - 1, lineEndX, SCREEN_HEIGHT - 1, SSD1306_WHITE);
  display.display();

  // Air quality
  int airQuality = 5;
  int good = 5;
  (airQuality < good) ? digitalWrite(YELLOW_LED, HIGH) : digitalWrite(YELLOW_LED, LOW);

}

bool CheckBtn(int btn){
  if (btn != lastBtn && millis() - lastChange > 50) {
    lastChange = millis();
    lastBtn = btn;
    if (btn == LOW) {
      return true;
    }
  }

  return false;
}

bool ArrayContainsInt(const int arr[], int contains){
  int count = sizeof(arr) / sizeof(arr[0]);

  for (int i = 0; i < count; i++){
    if (arr[i] == contains){
      return true;
      break;
    }
  }
  return false;

}

void RgbTurnOff(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbRed(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbGreen(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbBlue(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbYellow(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbPurple(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbCyan(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbWhite(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, HIGH);
}




