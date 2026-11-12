#include <ESP8266WiFi.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.print("\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("connecting to ");
  Serial.print(ssid); Serial.println("...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED and i < 60) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect");
    return;
  }

  Serial.println("\n");
  Serial.println("Connection established!");
  Serial.print("IP adress:\t");
  Serial.println(WiFi.localIP());

}

void loop() {

}
