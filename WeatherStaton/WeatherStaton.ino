// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"
#include "src/rgb/rgb.h"
#include "src/roomInfo/room_info_manager.h"
#include "src/weatherAPI/weatherAPI.h"

// Pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40
#define LED_Y 41
#define BTN_PIN 46
#define ROOM_SDA_PIN 15
#define ROOM_SCL_PIN 16

const int rgbPins[3] = {RGB_R, RGB_G, RGB_B};

void setup() {
  // Basic setup
  Serial.begin(115200);
  delay(1000);

  // PINS
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  SetupDisplay();
  delay(250);
  ConnectToWiFi();
  delay(250);
  SetupAQSensor(ROOM_SDA_PIN,ROOM_SCL_PIN);
  delay(250);

  display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.printf("Ready!");
    } while (display.nextPage());

}

bool firstLoop = true;
unsigned long lastAQUpdate = 0;
int AQUpdateInterval = 1000;

unsigned long lastRoomUpdate = 0;
const unsigned long roomUpdateInterval = 1UL * 60UL * 1000UL;

// Weather data
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 1UL * 60UL * 1000UL;

int displayPage = 0;
void loop() {
  if (millis() - lastAQUpdate >= AQUpdateInterval) {
    UpdateAQSensor();
    lastAQUpdate = millis();
  }

  bool buttonPressed = IsTouchPressed(BTN_PIN);
  bool roomUpdate = millis() - lastRoomUpdate >= roomUpdateInterval;
  bool weatherUpdate = millis() - lastWeatherUpdate >= weatherUpdateInterval;
  bool startup = firstLoop;

  if (buttonPressed || roomUpdate || weatherUpdate || startup) {

    // Change page only when button is pressed
    if (buttonPressed && !startup) {
      displayPage = (displayPage + 1) % 3;
    }

    if (roomUpdate || buttonPressed || startup) {
      lastRoomUpdate = millis();
    }

    if (weatherUpdate || startup) {
      UpdateWeather();
      lastWeatherUpdate = millis();
    }

    firstLoop = false;

    printInfo();
  }
}


void printInfo(){
  currentWeather current = GetCurrentWeather();

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextSize(1);
    display.setTextColor(GxEPD_BLACK);

    // =========================
    // PAGE 1 - ROOM + CURRENT WEATHER
    // =========================
    if (displayPage == 0) {

      display.setCursor(0, 10);
      display.println("ROOM");

      display.setCursor(0, 25);
      display.print("Temp: "); display.println(GetRoomTemp());
      display.print("Humidity: "); display.println(GetRoomHumidity());
      display.print("eCO2: "); display.println(GetEco2());
      display.print("TVOC: "); display.println(GetTvoc());
      display.print("AQI: "); display.println(GetAqi(LED_Y));

      display.setCursor(150, 10);
      display.println("CURRENT WEATHER");

      display.setCursor(150, 25);
      display.print("Weather: ");
      display.println(GetWeatherDescription(current.code));

      display.setCursor(150, 40);
      display.print("Temp: "); display.print(current.temp, 1); display.println(" C");

      display.setCursor(150, 55);
      display.print("Feels: "); display.print(current.feels, 1); display.println(" C");

      display.setCursor(150, 70);
      display.print("Humidity: "); display.print(current.hum); display.println(" %");

      display.setCursor(150, 85);
      display.print("Wind: "); display.print(current.windSpeed, 1); display.println(" m/s");

      display.setCursor(150, 100);
      display.print("Precip: "); display.print(current.prec, 1); display.println(" mm");
    }

    // =========================
    // PAGE 2 - HOURLY FORECAST
    // =========================
    else if (displayPage == 1) {

      display.setCursor(0, 10);
      display.println("HOURLY FORECAST");

      for (int i = 0; i < 12; i++) {
        hourlyWeather hour = GetHourlyWeather(i);

        int y = 25 + (i * 12);

        display.setCursor(0, y);
        display.print(hour.time);
        display.print(":00");

        display.setCursor(55, y);
        display.print(hour.temp, 1);
        display.print(" C");

        display.setCursor(105, y);
        display.print(GetWeatherDescription(hour.code));

        display.setCursor(220, y);
        display.print(hour.prec, 1);
        display.print(" mm");
      }
    }

    // =========================
    // PAGE 3 - DAILY FORECAST
    // =========================
    else if (displayPage == 2) {

      display.setCursor(0, 10);
      display.println("DAILY FORECAST");

      for (int i = 0; i < 7; i++) {
        dailyWeather day = GetDailyWeather(i);

        int y = 30 + (i * 20);

        display.setCursor(0, y);
        display.print(day.time);

        display.setCursor(100, y);
        display.print(GetWeatherDescription(day.code));

        display.setCursor(200, y);
        display.print(day.minTemp, 1);
        display.print("/");
        display.print(day.maxTemp, 1);
        display.println(" C");
      }
    }

  } while (display.nextPage());
}
