// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"
#include "src/rgb/rgb.h"
#include "src/roomInfo/room_info_manager.h"

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
void loop() {
    if (IsTouchPressed(BTN_PIN) || firstLoop){
      UpdateAQSensor();

      firstLoop = false;
      display.firstPage();
      do {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(0,25);
        display.setTextSize(1);
        display.print("Temp: "); display.println(GetRoomTemp());
        display.print("Humidity: "); display.println(GetRoomHumidity());
        display.print("eCO2: "); display.println(GetEco2());
        display.print("TVOC: "); display.println(GetTvoc());
        display.print("AQI: "); display.println(GetAqi(LED_Y));
      } while (display.nextPage());
    }
}