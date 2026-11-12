#pragma once

void SetupAQSensor(int sda, int scl);
void UpdateAQSensor();

int GetEco2();
int GetTvoc();
int GetAqi(int pin);

float GetRoomTemp();
float GetRoomHumidity();