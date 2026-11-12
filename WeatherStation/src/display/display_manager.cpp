// Display setup
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <SPI.h>

// Fonts
#include <Fonts/FreeSansBold18pt7b.h>

// pins
#define EPD_CS 5
#define EPD_DC 6
#define EPD_RST 7
#define EPD_BUSY 4

GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT> display(GxEPD2_370_GDEY037T03(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void SetupDisplay(int rotation) {
  SPI.begin(18,-1,17,EPD_CS);
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