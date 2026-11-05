// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"
#include "src/rgb/rgb.h"
#include "src/roomInfo/room_info_manager.h"
#include "src/weatherAPI/weatherAPI.h"
#include "src/weather_ui.h"

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

}

bool firstLoop = true;
unsigned long lastAQUpdate = 0;
int AQUpdateInterval = 1000;

unsigned long lastRoomUpdate = 0;
const unsigned long roomUpdateInterval = 5UL * 60UL * 1000UL;

// Weather data
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 20UL * 60UL * 1000UL;

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

    if (weatherUpdate || startup){
      lastWeatherUpdate = millis();
      UpdateWeather();
    }
    lastRoomUpdate = millis();

    firstLoop = false;

    uiRefresh(displayPage, LED_Y);
  }
}
