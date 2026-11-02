#include <WiFi.h>
#include "src/display/display_manager.h"

const char* ssid = "FBISurveillanceVan";
const char* password = "Petersborg2";

void ConnectToWiFi() {
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0,25);
    display.setTextSize(1);
    display.printf("Setting up WiFi:\n%s",ssid);
  } while (display.nextPage());
  delay(250);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.printf("Connecting to %s\n",ssid);

  int i = 1;
  while (WiFi.status() != WL_CONNECTED && i <= 100){
    Serial.print(i); Serial.print(" ");
    i++;
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("\nWiFi failed to connect");

    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.printf("WiFi Failed:\n%s",ssid);
    } while (display.nextPage());
    delay(250);
  } else{
    Serial.printf("\nconnected to %s\n",ssid);
    Serial.print("Local ESP IP: ");
    Serial.println(WiFi.localIP());

    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0,25);
      display.setTextSize(1);
      display.printf("WiFi Connected:\n%s",ssid);
    } while (display.nextPage());
    delay(250);
  }
}