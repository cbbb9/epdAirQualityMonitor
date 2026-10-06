#include <sys/_stdint.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
// #include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#include <GxEPD2_BW.h>
#include <gdey/GxEPD2_420_GDEY042T81.h>
// #include <errorMsg.h>
#include "displayFunctions.h"
// #include "sensorFunctions.h"
#include "config.h"

GxEPD2_BW<GxEPD2_420_GDEY042T81, 300> display(
  GxEPD2_420_GDEY042T81(
      /*CS=*/ CS_PIN,
      /*DC=*/ DC_PIN,
      /*RST=*/ RST_PIN,
      /*BUSY=*/ BUSY_PIN
  )
);

void setupDisplay() {

  SPI.begin(SCK_PIN, -1, MOSI_PIN, CS_PIN); //start SPI, e-paper disp doesn't need MISO -> hence -1
  delay(100);

  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display.fillScreen(GxEPD_WHITE);

}


void showWarning(const char* str[], int lineCount) 
{
  const int lineSpacing = 4;
  // int lineCount = sizeof(str) / sizeof(str[0]);
  int totalHeight = 0;

  display.setRotation(0);
  display.setFont(&FreeSans9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // display.setFullWindow();
  // display.fillScreen(GxEPD_WHITE);
  // display.drawRoundRect(10, 10, 379, 279, 3, GxEPD_BLACK); 26.09//
  
  // for (int i = 0; i < 2; i++) 
  for (int i = 0; i < lineCount; i++) 

  {
    int16_t tbx, tby; uint16_t tbw, tbh;
    display.getTextBounds(str[i], 0, 0, &tbx, &tby, &tbw, &tbh);
    totalHeight += tbh;

    if (i < lineCount - 1)
    {
      totalHeight += lineSpacing;
    }
  }

  int startY = (display.height() - totalHeight) /2;

  display.firstPage();
  
  do 
  {
    // display.fillScreen(GxEPD_WHITE);
    display.drawRoundRect(10, 10, 379, 279, 3, GxEPD_BLACK);
    int y = startY;

    for (int i = 0; i < lineCount; i++)
    {
      int16_t x1, y1;
      uint16_t w, h;

      display.getTextBounds(str[i], 0, 0, &x1, &y1, &w, &h);

      // center horizontally
      int x = (display.width() - w) / 2;

      // baseline position
      y += h;

      display.setCursor(x, y);
      display.print(str[i]);

      y += lineSpacing;
    }
  }
  while (display.nextPage());
  // int y = startY;


  // display.display(false);
}

void displayValues(const Measurement& current)
{
  display.setFont(&FreeSans9pt7b);
  display.setPartialWindow(70, 128, 110, 145);
  display.firstPage();
  do
  {
    display.fillRect(70, 128, 110, 145, GxEPD_WHITE);
    display.setCursor(72, 145);
    display.print(current.humidityBME);
    display.setCursor(72, 205);

    if (isnan(current.vocIndex)) {
      display.print("n/a");
    } else {
      display.print(current.vocIndex);
    }

    display.setCursor(72, 265);

    if (isnan(current.noxIndex)) {
      display.print("n/a");
    } else {
      display.print(current.noxIndex);
    }
  } while(display.nextPage());

  display.setPartialWindow(266, 65, 110, 205);
  display.firstPage();
  do 
  {
    display.fillRect(266, 65, 110, 205, GxEPD_WHITE);
    display.setCursor(268, 80);

    if (isnan(current.cPm1p0)) {
      display.print("n/a");
    } else {
      display.print(current.cPm1p0);
    }

    display.setCursor(268, 140);
    if (isnan(current.cPm2p5)) {
      display.print("n/a");
    } else {
      display.print(current.cPm2p5);
    }

    display.setCursor(268, 200);
    if (isnan(current.cPm4p0)) {
      display.print("n/a");
    } else {
      display.print(current.cPm4p0);
    }

    display.setCursor(268, 260);
    if (isnan(current.cPm10p0)) {
      display.print("n/a");
    } else {
      display.print(current.cPm10p0);
    }
  }
  while (display.nextPage());

  display.setPartialWindow(13, 70, 74, 35);
  display.firstPage();

  do
  {
    display.fillRect(13, 70, 77, 35, GxEPD_WHITE);
    display.setFont(&FreeSans18pt7b);
    display.setCursor(18, 100);
    display.printf("%.1f", current.tempBME);
  }
  while (display.nextPage());
}

void displayLabels()
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSansBold9pt7b);
  display.setCursor(36, 145);
  display.print("RH: ");

  display.setCursor(23, 205);
  display.print("VOC: ");

  display.setCursor(24, 265);
  display.print("NOx: ");

  display.setCursor(200, 80);
  display.print("PM 1.0: ");
  // string 5
  display.setCursor(200, 140);
  display.print("PM 2.5: ");
  // string 6
  display.setCursor(200, 200);
  display.print("PM 4.0: ");
  // string 7
  display.setCursor(190, 260);
  display.print("PM 10.0: ");

  display.setFont(&FreeSansBold9pt7b);
  display.setCursor(93, 84);
  display.print("o");
  display.setFont(&FreeSans18pt7b);
  display.setCursor(42, 100);
  display.print("       C");
  display.display();
}

void displayTime(const char timeString[10])
{
  display.setPartialWindow(15, 12, 125, 48);
  display.firstPage();
  do
  {
    display.fillRect(15, 12, 125, 48, GxEPD_WHITE);
    display.setFont(&FreeSansBold24pt7b);
    display.setCursor(19, 51);
    display.print(timeString);
  } while (display.nextPage());
}