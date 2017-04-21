#include <Arduino.h>
#include <SPI.h>

#include "main.h"


void initDisplay();

inline void sendCMD(uint8_t index)
{
    TFT_DC_LOW;
    TFT_CS_LOW;
    SPI.transfer(index);
    TFT_CS_HIGH;
}

inline void sendData(uint8_t data)
{
    TFT_DC_HIGH;
    TFT_CS_LOW;
    SPI.transfer(data);
    TFT_CS_HIGH;
}

void setup()
{
    initDisplay();                                      //init display
}

void initDisplay()
{
  //POWER SETTING
  sendCMD  (0x01);
  sendData (0x37);
  sendData (0x00);
  //PANNEL SETTING
  sendCMD  (0x00);
  sendData (0xCF);
  sendData (0x08);

  //booster
  sendCMD  (0x06);
  sendData (0xc7);
  sendData (0xcc);
  sendData (0x28);

  //PLL setting
  sendCMD  (0x30);
  sendData (0x3c);

  //TEMPERATURE SETTING
  sendCMD(0X41);
  sendData(0x00);

  //VCOM AND DATA INTERVAL SETTING
  sendCMD(0X50);
  sendData(0x77);

  sendCMD(0X60);     //TCON SETTING
  sendData(0x22);

  sendCMD(0x61);         //tres
  sendData (0x02);   //source 640
  sendData (0x80);
  sendData (0x01);   //gate 384
  sendData (0x80);


  sendCMD(0X82);     //VDCS SETTING
  sendData(0x1E);    //decide by LUT file

  sendCMD(0xe5);     //FLASH MODE
  sendData(0x03);
}

void loop()
{

}
