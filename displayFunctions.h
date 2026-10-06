#pragma once
#include "sensorFunctions.h"
// extern GxEPD2_BW<GxEPD2_420_GDEY042T81, 300> display;

void showWarning(const char* str[], int lineCount);
void displayLabels();
void displayValues(const Measurement& current);
void setupDisplay();
void displayTime(const char timeString[10]);
