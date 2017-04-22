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

void pic_display (void)
{
  unsigned long int i;
	unsigned char j,temp1,temp2,temp3;


	sendCMD(0x10);	       //Start picture update

	for(i=0;i<61440;i++)
	{
		temp1 = 0XFF;
		for(j=0;j<4;j++)
		{
			temp2 = temp1&0xc0 ;
			if(temp2 == 0xc0)
				temp3 = 0x00;
			else if(temp2 == 0x00)
				temp3 = 0x03;
			else
				temp3 = 0x04;

			temp1 <<= 2;
			temp3 <<= 4;
			j++;

			temp2 = temp1&0xc0 ;
			if(temp2 == 0xc0)
				temp3 |= 0x00;
			else if(temp2 == 0x00)
				temp3 |= 0x03;
			else
				temp3 |= 0x04;

			temp1 <<= 2;

			sendData(temp3);
		}
	}
}
void waitForRedy(){
  unsigned char busy;
  do
  {
    sendCMD(0x71);
    busy = TFT_BUSY;
    busy =!(busy & 0x01);
  }
  while(busy);
  delay(200);
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

  //TCON SETTING
  sendCMD(0X60);
  sendData(0x22);

  //Resolution
  sendCMD(0x61);
  sendData (0x02);   //source 640
  sendData (0x80);
  sendData (0x01);   //gate 384
  sendData (0x80);

  //VDCS SETTING
  sendCMD(0X82);
  sendData(0x1E);    //decide by LUT file

  //FLASH MODE
  sendCMD(0xe5);
  sendData(0x03);

  //pic_display();
  //POWER ON
  sendCMD(0x04);
  waitForRedy();

  //display refresh
  sendCMD(0x12);
  waitForRedy();

  sendCMD(0x02);
  waitForRedy();

  sendCMD(0x07);
  sendData(0xa5);
}

void loop()
{

}
