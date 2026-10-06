#include <sensorFunctions.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SensirionI2CSen5x.h>
#include "config.h"
#include <sys/_stdint.h>

SensirionI2CSen5x sen5x;
Adafruit_BME280 bme;

SensorStatus setupSensors()
{
  SensorStatus status{};
  uint16_t error;
  // char errorMessage[256];
  Wire.begin(SDA_PIN, SCL_PIN); // start I2C (SDA, SCL)
  
  // Initialize  SEN55 sensor
  sen5x.begin(Wire);
  delay(200);
  error = sen5x.deviceReset();
  if (error) {
    errorToString(error, status.sen55Reset, sizeof(status.sen55Reset));
  }
  
  // status.sen55Reset = sen5x.deviceReset();
  delay(500);
  error = sen5x.startMeasurement();
  if (error) {
    errorToString(error, status.sen55Start, sizeof(status.sen55Start));
  }
  // status.sen55Start = sen5x.startMeasurement();
  status.bme280 = bme.begin(0x77);
  return status;
}


void readSensors(Measurement& current) {
  uint16_t error;
  char errorMessage[256];

  float pm1, pm25, pm4, pm10, humidity, temperature;
  float voc, nox, humidity2, temperature2, pressure; // humidity2, temperature2 -> from BME280

  error = sen5x.readMeasuredValues(
    pm1, pm25, pm4, pm10, humidity, temperature, voc, nox);
  
  if (error) {
    Serial.print("Error SEN55: ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else {
    current.timestamp = time(nullptr);
    current.cPm1p0 = pm1;
    current.cPm2p5 = pm25;
    current.cPm4p0 = pm4;
    current.cPm10p0 = pm10;
    current.humiditySEN55 = humidity;
    current.tempSEN55 = temperature;
    current.vocIndex = voc;
    current.noxIndex = nox;

    ///// Print values into serial 
    Serial.print("T (sen55): ");
    if (isnan(temperature)) {
      Serial.print("n/a");
    } else {
      Serial.print(temperature);
    }
    Serial.print("\t");
    Serial.print("Hum (sen55): ");
    if (isnan(humidity)) {
      Serial.print("n/a");
    } else {
      Serial.print(humidity);
    }
    Serial.print("\t");
    Serial.print("PM1.0: ");
    Serial.print(pm1);
    Serial.print("\t");
    Serial.print("PM2.5: ");
    Serial.print(pm25);
    Serial.print("\t");
    Serial.print("PM4: ");
    Serial.print(pm4);
    Serial.print("\t");
    Serial.print("PM10: ");
    Serial.print(pm10);
    Serial.print("\t");
    Serial.print("VOC: ");
    if (isnan(voc)) {
      Serial.print("n/a");
    } else {
      Serial.print(voc);
    }
    Serial.print("\t");
    Serial.print("NOx: ");
    if (isnan(nox)) {
      Serial.println("n/a");
    } else {
      Serial.println(nox);
    }
    /////////////
  }
  temperature2 = bme.readTemperature();
  current.tempBME = temperature2;
  Serial.print("T (bme280): ");
  Serial.print(temperature2);
  Serial.print("\t");

  humidity2 = bme.readHumidity();
  current.humidityBME = humidity2;
  Serial.print("Hum (bme280): ");
  Serial.print(humidity2);
  Serial.print("\t");

  pressure = bme.readPressure()/100.0F;
  current.pressure = pressure;
  Serial.print("P: ");
  Serial.println(pressure);
}
