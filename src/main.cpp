#include <Arduino.h>
#include <SPI.h>

#include "main.h"
// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one
//#include <GxGDEP015OC1/GxGDEP015OC1.cpp>
//#include <GxGDE0213B1/GxGDE0213B1.cpp>
//#include <GxGDEH029A1/GxGDEH029A1.cpp>
//#include <GxGDEW027C44/GxGDEW027C44.cpp>
//#include <GxGDEW042T2/GxGDEW042T2.cpp>
#include <GxGDEW075Z09/GxGDEW075Z09.cpp>

// uncomment next line for drawBitmap() test, (consumes RAM on ESP8266)
//#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

//GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, D3, D4); // arbitrary selection of D3, D4 selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
GxEPD_Class display(io); // default selection of D4, D2

void showBitmapExample()
{
#ifdef _GxBitmapExamples_H_
  display.drawBitmap(BitmapExample1, sizeof(BitmapExample1));
  Serial.println("BitmapExample1");
  delay(2000);
  display.drawBitmap(BitmapExample2, sizeof(BitmapExample2));
  Serial.println("BitmapExample2");
  delay(2000);
  display.setRotation(2);
  display.fillScreen(GxEPD_WHITE);
  Serial.println("White");
  // bitmap may have been shortened to fit to available RAM
  uint16_t h = min(GxEPD_HEIGHT, sizeof(BitmapExample1) * 8 / GxEPD_WIDTH);
  Serial.println(h);
  display.drawBitmap(0, 0, BitmapExample1, GxEPD_WIDTH, h, GxEPD_RED);
  display.update();
  delay(2000);
#endif
}

void showFont(const char name[], const GFXfont* f, uint16_t color)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(name);
  display.println(" !\"#$%&'()*+,-./");
  display.println("0123456789:;<=>?");
  display.println("@ABCDEFGHIJKLMNO");
  display.println("PQRSTUVWXYZ[\\]^_");
  display.setTextColor(color);
  display.println("`abcdefghijklmno");
  display.println("pqrstuvwxyz{|}~ ");
  display.update();
  delay(5000);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init();

  Serial.println("setup done");
}

unsigned char write = 1;
void loop()
{
  //showBitmapExample();
  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b, GxEPD_WHITE);
  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b, GxEPD_RED);
  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b, GxEPD_BLACK);
  delay(10000);
}
