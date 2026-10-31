#pragma once

void SetupDisplay();
#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>

extern GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT> display;