#include <Arduino.h>
#include <SPI.h>

#include "main.h"


void initDisplay();

inline void sendCMD(uint8_t index)
{
    TFT_CS_LOW;
    TFT_DC_LOW;
    SPI.transfer(index);
    TFT_CS_HIGH;
}

inline void sendData(uint8_t data)
{
    TFT_CS_LOW;
    TFT_DC_HIGH;
    SPI.transfer(data);
    TFT_CS_HIGH;
}

void MCU_write_flash(uint8_t command)
{
  SPI.transfer(command);
}

void pic_display (unsigned char write)
{
  unsigned long int i;
	unsigned char j,temp1,temp2,temp3;


	sendCMD(0x10);	       //Start picture update

	for(i=0;i<61440;i++)
	{
		temp1 = 0XFF;
    sendData(write);
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


			//sendData(temp3);
		}
	}
}
void waitForRedy(){
  unsigned char busy;
  do
  {
    //sendCMD(0x71);
    busy = TFT_BUSY;
    busy =! busy;
  }
  while(busy);
  Serial.println("Redy");
  delay(200);
}

void setupPins(){
  pinMode(TFT_CS_PIN,OUTPUT);
  pinMode(TFT_DC_PIN,OUTPUT);
  pinMode(TFT_RST_PIN,OUTPUT);
  pinMode(TFT_BUSY_PIN,INPUT);
}
void setup()
{
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Setup");
    setupPins();
    SPI.begin();
    SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));

    initDisplay();                                      //init display
    SPI.endTransaction();
}

void initDisplay()
{
  TFT_RST_LOW;				//module reset
  delay(1000);
  TFT_RST_HIGH;
  delay(1000);
  waitForRedy();
  Serial.println("Reset");

  /**********************************release flash sleep**********************************/
		sendCMD(0X65);			//FLASH CONTROL
		sendCMD(0x01);

		TFT_CS_LOW;					//MFCSB À­µÍ
		MCU_write_flash(0xAB);
		TFT_CS_HIGH;					//MFCSB À­¸ß

		sendCMD(0X65);			//FLASH CONTROL
		sendCMD(0x00);
	/**********************************release flash sleep**********************************/


  //POWER SETTING
  sendCMD  (0x01);
  sendData (0x37); //110111
  sendData (0x00);

  //PANNEL SETTING
  sendCMD  (0x00);
  sendData (0xCF); //11001111
  sendData (0x08); //1000

  //booster
  sendCMD  (0x06);
  sendData (0xc7); //11000111
  sendData (0xcc); //11001100
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

  pic_display(0xFF);

  //POWER ON
  sendCMD(0x04);
  Serial.println("Power on");
  //waitForRedy();

  //display refresh
  sendCMD(0x12);
  delay(100);
  waitForRedy();
  /**********************************flash sleep**********************************/
  		sendCMD(0X65);			//FLASH CONTROL
  		sendCMD(0x01);

  		TFT_CS_LOW;					//MFCSB À­µÍ
  		MCU_write_flash(0xB9);
  		TFT_CS_HIGH;					//MFCSB À­¸ß

  		sendCMD(0X65);			//FLASH CONTROL
  		sendCMD(0x00);
  /**********************************flash sleep**********************************/
  sendCMD(0x02);
  waitForRedy();
  //
  sendCMD(0x07);
  sendData(0xa5);
}

unsigned char write = 1;
void loop()
{
}
