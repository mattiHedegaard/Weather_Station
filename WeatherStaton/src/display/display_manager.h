#pragma once

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>

using DisplayType = GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT>;

extern DisplayType* display;
void SetupDisplay(int csPin, int dcPin, int rstPin, int busyPin);