#include <Wire.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);

void setup() {
  Serial.begin(115200);
  Wire.begin(15,16); // SDA, SCL

  if (!aht.begin()) {
    Serial.println("AHT21 not found");
    while (1) delay(10);
  }
  if (!ens160.begin()) {
    Serial.println("ENS160 not found");
    while (1) delay(10);
  }
  ens160.setMode(ENS160_OPMODE_STD);

}

void loop() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  Serial.print("Temp: ");  Serial.print(temp.temperature);
  Serial.print(" C, Hum: "); Serial.print(hum.relative_humidity);
  Serial.println(" %");

  if (ens160.available()) {
    ens160.measure(true);
    Serial.print("eCO2: "); Serial.print(ens160.geteCO2());
    Serial.print(" ppm, TVOC: "); Serial.print(ens160.getTVOC());
    Serial.print(" ppb, AQI: "); Serial.println(ens160.getAQI());
  }
  delay(1000);
  Serial.println();

}
