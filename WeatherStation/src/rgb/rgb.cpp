#include <Arduino.h>

void RgbSetColor(const int pins[3], uint8_t r, uint8_t g, uint8_t b){
  analogWrite(pins[0], r);
  analogWrite(pins[1], g);
  analogWrite(pins[2], b);
}