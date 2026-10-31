// Source files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"
#include "src/touch/touch_button.h"

// Pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40
#define LED_Y 41
#define BTN_PIN 46

// RGB setup
void RgbSetColor(uint8_t r, uint8_t g, uint8_t b){
  analogWrite(RGB_R, r);
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}

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
    rgbState = (rgbState + 1) % 3;

    const char* label = "";
    switch (rgbState){
        case 0:
          RgbSetColor(0,0,255);
          label = "LED (0,0,255)";
        break;
        case 1:
          RgbSetColor(0,255,0);
          label = "LED (0,255,0)";
        break;
        case 2:
          RgbSetColor(255,0,0);
          label = "LED (255,0,0)";
        break;
      }

    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.print(label);
    } while (display.nextPage());
  }
  
}