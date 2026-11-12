// Display setup
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <SPI.h>

// Fonts
#include <Fonts/FreeSansBold18pt7b.h>

#define EPD_CS 5
#define EPD_DC 6
#define EPD_RST 7
#define EPD_BUSY 4

GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT>display(GxEPD2_370_GDEY037T03(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void SetupDisplay(){
  SPI.begin(18,-1,17,EPD_CS);
  display.init(115200,true,50,false);
  display.setRotation(3);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,25);
    display.setTextSize(1);
    display.print("Display setup!");
  } while (display.nextPage());

  delay(1000);
}

// Wifi setup
#include <WiFi.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

void ConnectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.printf("Connecting to %s\n",ssid);

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,25);
    display.setTextSize(1);
    display.printf("Connecting to:\n%s",ssid);
  } while (display.nextPage());

  int i = 1;
  while (WiFi.status() != WL_CONNECTED && i <= 100){
    Serial.print(i); Serial.print(" ");
    i++;
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("\nWiFi failed to connect");
  } else{
    Serial.printf("\nconnected to %s\n",ssid);
    Serial.print("Local ESP IP: ");
    Serial.println(WiFi.localIP());

    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.printf("Connection succesfull: %s",ssid);
    } while (display.nextPage());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  SetupDisplay();

  ConnectToWiFi();

}

void loop() {
  

}
