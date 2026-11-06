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
    display.println("Conecting to WiFi:"); display.print(ssid);
  } while (display.nextPage());

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
      display.println("WiFi connected to:"); display.print(ssid);
    } while (display.nextPage());
  }
}
