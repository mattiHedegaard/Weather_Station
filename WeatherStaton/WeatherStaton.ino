// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"
#include "src/rgb/rgb.h"

// Pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40
#define LED_Y 41
#define BTN_PIN 46

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

  display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.print("Ready!");
    } while (display.nextPage());

}
int rgbState = 0;
void loop() {

  if (IsTouchPressed(BTN_PIN)){
    const char* label = "";
    switch (rgbState){
        case 0:
          RgbSetColor(rgbPins,0,0,255);
          label = "LED Blue";
        break;
        case 1:
          RgbSetColor(rgbPins,0,255,0);
          label = "LED Green";
        break;
        case 2:
          RgbSetColor(rgbPins,255,0,0);
          label = "LED Red";
        break;
        case 3:
          RgbSetColor(rgbPins,255,255,255);
          label = "LED White";
        break;
      }

    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.print(label);
    } while (display.nextPage());

    rgbState = (rgbState + 1) % 4;
  }
  
}