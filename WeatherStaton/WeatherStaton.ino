// ino files
#include "src/display/display_manager.h"
#include "src/wifi/wifi_manager.h"

// LED pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40
#define LED_Y 41

void RgbSetColor(uint8_t r, uint8_t g, uint8_t b){
  analogWrite(RGB_R, r);
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}

void setup() {
  // Basic setup
  Serial.begin(115200);
  delay(1000);

  // LED's
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(LED_Y, OUTPUT);

  SetupDisplay();
  delay(250);
  ConnectToWiFi();

}

uint8_t hue = 0;   // global, keeps its value between loop() runs

void loop() {
  uint8_t r, g, b;
  uint8_t pos = hue;

  if (pos < 85) {            // red -> green
    r = 255 - pos * 3;
    g = pos * 3;
    b = 0;
  } else if (pos < 170) {    // green -> blue
    pos -= 85;
    r = 0;
    g = 255 - pos * 3;
    b = pos * 3;
  } else {                   // blue -> red
    pos -= 170;
    r = pos * 3;
    g = 0;
    b = 255 - pos * 3;
  }

  RgbSetColor(r, g, b);
  hue++;        // wraps from 255 to 0 by itself
  delay(20);    // lower = faster
}