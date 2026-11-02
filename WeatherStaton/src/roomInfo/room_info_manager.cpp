#include <Wire.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);

void SetupAQSensor(int sda, int scl){
  Wire.begin(sda,scl); // SDA, SCL

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

void UpdateAQSensor() {
  if (ens160.available()) {
    ens160.measure(true);
  }
}

int GetEco2(){
  return ens160.geteCO2();
}
int GetTvoc(){
  return ens160.getTVOC();
}
int GetAqi(int pin){
  int aqi = ens160.getAQI();
  digitalWrite(pin, (aqi >= 3) ? HIGH : LOW);

  return aqi;
}
float GetRoomTemp(){
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  return temp.temperature;
}
float GetRoomHumidity(){
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  return hum.relative_humidity;
}