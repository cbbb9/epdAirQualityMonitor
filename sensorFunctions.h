#pragma once
#include <sys/_stdint.h>

struct Measurement {
  uint32_t timestamp;
  float cPm1p0;
  float cPm2p5;
  float cPm4p0;
  float cPm10p0;
  float humiditySEN55;
  float tempSEN55;
  float vocIndex;
  float noxIndex;
  float humidityBME;
  float tempBME;
  float pressure;
}; // For storing measurement values

struct SensorStatus {
  char sen55Reset[256];
  char sen55Start[256];
  bool bme280;
}; // For error reporting

void readSensors(Measurement& current);

SensorStatus setupSensors();