// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"
#include "src/rgb/rgb.h"
#include "src/roomInfo/room_info_manager.h"
#include "src/weatherAPI/weatherAPI.h"
#include "src/weather_ui.h"
//#include "src/ui/ui_elements.h"
//#include "src/pages/page_design.h"

// LED pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40
#define LED_Y 41
#define BTN_PIN 46
// Room sensor pins
#define ROOM_SDA_PIN 15
#define ROOM_SCL_PIN 16
// Display pins
#define EPD_CS 5
#define EPD_DC 6
#define EPD_RST 7
#define EPD_BUSY 4
// Buzzer Pin
#define BUZZ_PIN 37 

const int rgbPins[3] = {RGB_R, RGB_G, RGB_B};

void setup() {
  // Basic setup
  Serial.begin(115200);
  delay(3000);

  // PINS
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  // Boot light
  RgbSetColor(200, 15, 0);

  // Setup Systems
  SetupDisplay(3);
  delay(250);
  ConnectToWiFi();
  delay(250);
  SetupAQSensor(ROOM_SDA_PIN,ROOM_SCL_PIN);
  delay(250);

}

// initial variables
bool firstLoop = true;

// Room data
unsigned long lastAQUpdate = 0;
int AQUpdateInterval = 1000;

unsigned long lastRoomUpdate = 0;
const unsigned long roomUpdateInterval = 5UL * 60UL * 1000UL;

// Weather data
unsigned long lastWeatherUpdate = 0;
const unsigned long weatherUpdateInterval = 20UL * 60UL * 1000UL;

int displayPage = 0;
void loop() {
  // Update touch state
  TouchEvent touchState = CheckTouch(BTN_PIN);

  if (millis() - lastAQUpdate >= AQUpdateInterval) {
    //tone(BUZZ_PIN,1000);
    UpdateAQSensor();
    lastAQUpdate = millis();
  }
  noTone(BUZZ_PIN);

  bool roomUpdate = millis() - lastRoomUpdate >= roomUpdateInterval;
  bool weatherUpdate = millis() - lastWeatherUpdate >= weatherUpdateInterval;
  bool startup = firstLoop;
  firstLoop = false;

  
  if (TouchIsHold(touchState) || weatherUpdate || startup){
    if (!startup && TouchIsHold(touchState)){
      tone(BUZZ_PIN, 1760, 50);
      RgbSetColor(140, 20, 255);
    }
    lastWeatherUpdate = millis();
    UpdateWeather();
    
    uiRefresh(displayPage, LED_Y);
  }

  if (TouchIsPressed(touchState) || roomUpdate || startup) {
    if (!startup && !roomUpdate){
      displayPage = (displayPage + 1) % 3;
    }
    uiRefresh(displayPage, LED_Y);

    lastRoomUpdate = millis();
  }
  
}
