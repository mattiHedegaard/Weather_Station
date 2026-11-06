#pragma once
#include <Arduino.h>

extern const int rgbPins[3];
void RgbSetColor(uint8_t r, uint8_t g, uint8_t b, const int pins[3] = rgbPins);