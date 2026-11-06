#include <Arduino.h>

// RGB Pins
#define RGB_R 38
#define RGB_G 39
#define RGB_B 40

const int rgbPins[3] = {RGB_R, RGB_G, RGB_B};

void RgbSetColor(uint8_t r, uint8_t g, uint8_t b, const int pins[3] = rgbPins){
  analogWrite(pins[0], r);
  analogWrite(pins[1], g);
  analogWrite(pins[2], b);
}