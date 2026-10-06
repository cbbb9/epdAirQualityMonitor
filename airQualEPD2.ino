#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>
#include <SPI.h>
#include "GxEPD2_GFX.h"
#include <Wire.h>
#include <WiFi.h>
#include <time.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include "sensorFunctions.h"
#include "displayFunctions.h"
#include "errorMsg.h"
// #include <Wire.h>
// #include <sys/_stdint.h>

const char* ssid = "CAGDAS1 9885";
const char* password = "0;2841Kt";

// BitmapDisplay bitmaps(display);

unsigned long delayTime;
constexpr uint16_t SAMPLING_MIN_INTERVAL = 10;
constexpr uint16_t NRSAMPLES = 1440 / SAMPLING_MIN_INTERVAL; // 60*24 min/interval


const unsigned long measureInterval = 60000; // Measure values every 1 min 
// const unsigned long measureInterval = 1000; // get measurements every sec
const unsigned long displayInterval = 60000; // If measurement and display need different intervals

// const unsigned long storeInterval = 600000; // Store measurements every 10 min
const unsigned long refreshInterval = 1800000; // Full refresh every 30 min
unsigned long lastMeasure = 0;
unsigned long lastDisplay = 0; // If measurement and display need different intervals
// unsigned long lastStore = 0;
unsigned long lastClockUpdate = 0;
unsigned long lastRefresh = 0;



Measurement history[NRSAMPLES]; // Measurements over time
Measurement current; // Current measurements

uint16_t storeIndex = 0;
char timeStr[10];


void setupTime() {
  // bool wifiConnected = false;
  WiFi.begin(ssid, password);
  
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    // timeout after 10 seconds
    if (millis() - start > 20000)
    {
      Serial.println("WiFi connection failed");
      showWarning(wifiError, sizeof(wifiError) / sizeof(wifiError[0]));
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      // return false;
      delay(4000);
      return;
    }

    delay(500);
  }

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org"); // get Eur time (handles daylight saving time changes)

  struct tm timeinfo;

  start = millis();

  while (!getLocalTime(&timeinfo)) {
    if (millis() - start > 10000)
    {
      Serial.println("NTP server sync failed.");
      showWarning(ntpError, sizeof(ntpError) / sizeof(ntpError[0]));
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      // return false;
      delay(4000);
      return;
    }

    delay(500);
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void timestampToText(time_t ts, char* buf, size_t len) {
  struct tm t;
  localtime_r(&ts, &t);
  strftime(buf, len, "%H:%M", &t);
}



void setup() {

  Serial.begin(115200);
  delay(500);
  // bool status;

  // while (!Serial) {
  // delay(100);
  // }

  Serial.println();
  Serial.println("Booting...");

  setupDisplay();
  // Wire.begin(SDA_PIN, SCL_PIN); // start I2C (SDA, SCL)

  // Init display

  const char* booting[] = {"Booting..."};
  showWarning(booting, 1);

  SensorStatus status = setupSensors();

  setupTime();

  // uint16_t error;
  // char errorText[256];

  if (status.sen55Reset[0] != '\0') {
    Serial.print("Error trying to execute deviceReset(): ");
    Serial.println(status.sen55Reset);

    const char* errorMessage[] = {
      sen55Error,
      status.sen55Reset
    };

    showWarning(errorMessage, sizeof(errorMessage) / sizeof(errorMessage[0]));
    delay(3000);
  }
  
  if (status.sen55Start[0] != '\0') {
    Serial.print("Error trying to execute startMeasurement(): ");
    Serial.println(status.sen55Start);
    const char* errorMessage[] = {
      sen55Error,
      status.sen55Start
    };
    showWarning(errorMessage, sizeof(errorMessage) / sizeof(errorMessage[0]));
    delay(3000);
  }
  // display.setTextWrap(true);
  // Initialize the BME280

  if (!status.bme280) {
  Serial.println("Could not find a valid BME280 sensor, check wiring!");
  showWarning(bme280Error, sizeof(bme280Error) / sizeof(bme280Error[0]));
  delay(3000);
  }

  displayLabels();
}


void loop() {

  unsigned long now = millis();
  
  // Read Measurement
  if (now - lastMeasure >= measureInterval) {
    lastMeasure = now;
    readSensors(current);
    displayValues(current);
    timestampToText(time(nullptr), timeStr, sizeof(timeStr));
    displayTime(timeStr); // if measurement and display occur simultaneously
  }

  // if (now - lastDisplay >= displayInterval) {
  //   lastDisplay = now;
    // lastClockUpdate = now;
  //   updateDisplayFull();
  // }
}
