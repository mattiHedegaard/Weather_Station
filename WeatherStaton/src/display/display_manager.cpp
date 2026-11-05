// Display setup
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <SPI.h>

// Fonts
#include <Fonts/FreeSansBold18pt7b.h>

DisplayType* display = nullptr;

void SetupDisplay(int csPin, int dcPin, int rstPin, int busyPin, int rotation) {
  display = new GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT>(GxEPD2_370_GDEY037T03(csPin, dcPin, rstPin, busyPin));

  SPI.begin(18,-1,17,csPin);
  display.init(115200,true,50,false);
  delay(100);
  display.setRotation(rotation);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  
  /*
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,25);
    display.setTextSize(1);
    display.print("Display is setup!");
  } while (display.nextPage());
  */
  
  delay(10);
}