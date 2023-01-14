/*
 * gdey0154d67.c
 *
 *  Created on: Jul 13, 2022
 *      Author: marcelo
 */

#include "waveshare_1in54b2.h"



//200*200///////////////////////////////////////

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2
#define RED_MODE     3

#define MAX_LINE_BYTES 25// =200/8
#define MAX_COLUMN_BYTES  200

#define ALLSCREEN_GRAGHBYTES 5000

eInkDisplayHandler_t* eInkDisplayHandler;

////////FUNCTION//////


static void SPI_Delay(uint32_t xrate);
static void SPI_Write(unsigned char value);
static void Epaper_Write_Command(unsigned char command);
static void Epaper_Write_Data(unsigned char command);
//EPD
static void waitIfBusy(eInkDisplayHandler_t* eInkDisplay);






//////////////////////SPI///////////////////////////////////
void SPI_Delay(uint32_t xrate)
{
  volatile unsigned char i;
  while(xrate)
  {
    for(i=0;i<2;i++);
    xrate--;
  }
}


static void SPI_Write(unsigned char value)
{
    unsigned char i;
   SPI_Delay(10);
    for(i=0; i<8; i++)
    {
        eInkDisplayHandler->writeGPIO(eInkDisplayHandler->clkPin, 0);
       SPI_Delay(5);
       if(value & 0x80)
    	   eInkDisplayHandler->writeGPIO(eInkDisplayHandler->mosiPin, 1);
        else
       	   eInkDisplayHandler->writeGPIO(eInkDisplayHandler->mosiPin, 0);
        value = (value << 1);
       SPI_Delay(5);
       eInkDisplayHandler->writeGPIO(eInkDisplayHandler->clkPin, 1);
        SPI_Delay(10);
    }
}

static void Epaper_Write_Command(unsigned char command)
{
  SPI_Delay(5);

  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->dcPin, 0); // command write

  SPI_Delay(5);

  if (eInkDisplayHandler->csInverted)
	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 1);
  else
	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 0);

  SPI_Write(command);

  if (eInkDisplayHandler->csInverted)
  	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 0);
    else
  	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 1);
  SPI_Delay(5);
}
static void Epaper_Write_Data(unsigned char datas)
{
  SPI_Delay(5);

  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->dcPin, 1);   // data write

  SPI_Delay(5);

  if (eInkDisplayHandler->csInverted)
	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 1);
  else
	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 0);

  SPI_Write(datas);

  if (eInkDisplayHandler->csInverted)
  	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 0);
    else
  	  eInkDisplayHandler->writeGPIO(eInkDisplayHandler->csPin, 1);
  SPI_Delay(5);
}

static void waitIfBusy(eInkDisplayHandler_t* eInkDisplay)
{
  while(eInkDisplay->readGPIO(eInkDisplay->busyPin))
	  eInkDisplayHandler->delayMS(200);
  eInkDisplayHandler->delayMS(200);
}

/////////////////EPD settings Functions/////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void EPD_HW_Init(eInkDisplayHandler_t* eInkDisplay)
{
	eInkDisplayHandler = eInkDisplay;

/*
	eInkDisplayHandler->writeGPIO(eInkDisplayHandler->resetPin, 0);  // Module reset
	eInkDisplayHandler->delayMS(10); //At least 10ms delay
	eInkDisplayHandler->writeGPIO(eInkDisplayHandler->resetPin, 1);
	eInkDisplayHandler->delayMS(10); //At least 10ms delay

	eInkDisplayHandler->delayMS(100);

	Epaper_Write_Command(0x00);        			//panel setting
	Epaper_Write_Data (0xDf);
	Epaper_Write_Data (0x0e);

	Epaper_Write_Command(0x4D);        			//FITIinternal code
	Epaper_Write_Data (0x55);

	Epaper_Write_Command(0xaa);
	Epaper_Write_Data (0x0f);

	Epaper_Write_Command(0xE9);
	Epaper_Write_Data (0x02);

	Epaper_Write_Command(0xb6);
	Epaper_Write_Data (0x11);

	Epaper_Write_Command(0xF3);
	Epaper_Write_Data (0x0a);

	Epaper_Write_Command(0x61);			//resolution setting
	Epaper_Write_Data (0xc8);
	Epaper_Write_Data (0x00);
	Epaper_Write_Data (0xc8);

	Epaper_Write_Command(0x60);        	 //Tcon setting
	Epaper_Write_Data (0x00);

	Epaper_Write_Command(0X50);
	Epaper_Write_Data(0x97);//

	Epaper_Write_Command(0XE3);
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x04);//Power on
	eInkDisplayHandler->delayMS(100);
	waitIfBusy(eInkDisplayHandler);

	*/

	printf("EPD_HW_Init start \n");

	eInkDisplayHandler->writeGPIO(eInkDisplayHandler->resetPin, 1);
	eInkDisplayHandler->delayMS(100);
	eInkDisplayHandler->writeGPIO(eInkDisplayHandler->resetPin, 0);  // Module reset
	eInkDisplayHandler->delayMS(100);
	eInkDisplayHandler->writeGPIO(eInkDisplayHandler->resetPin, 1);
	eInkDisplayHandler->delayMS(100);



	waitIfBusy(eInkDisplayHandler);
	printf("waitIfBusy pass 1 \n");
	Epaper_Write_Command(0x12);  //SWRESET
	    waitIfBusy(eInkDisplayHandler);
	    printf("waitIfBusy pass 2 \n");

	    Epaper_Write_Command(0x01); //Driver output control
	    Epaper_Write_Data(0xC7);
	    Epaper_Write_Data(0x00);
	    Epaper_Write_Data(0x01);

	    Epaper_Write_Command(0x11); //data entry mode
	    Epaper_Write_Data(0x01);

	    Epaper_Write_Command(0x44); //set Ram-X address start/end position
	    Epaper_Write_Data(0x00);
	    Epaper_Write_Data(0x18);    //0x18-->(24+1)*8=200

	    Epaper_Write_Command(0x45); //set Ram-Y address start/end position
	    Epaper_Write_Data(0xC7);    //0xC7-->(199+1)=200
	    Epaper_Write_Data(0x00);
	    Epaper_Write_Data(0x00);
	    Epaper_Write_Data(0x00);

	    Epaper_Write_Command(0x3C); //BorderWavefrom
	    Epaper_Write_Data(0x05);

	    Epaper_Write_Command(0x18); //Read built-in temperature sensor
	    Epaper_Write_Data(0x80);

	    Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	    Epaper_Write_Data(0x00);
	    Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;
	    Epaper_Write_Data(0xC7);
	    Epaper_Write_Data(0x00);
	    waitIfBusy(eInkDisplayHandler);


	    printf("EPD_HW_Init finish \n");

}


void EPD_full_display_Clear(void)
{
	/*	unsigned int i,j;
		for(j=0;j<2;j++)
		{
			Epaper_Write_Command(0x10);
			for(i=0;i<5000;i++)
			{
				Epaper_Write_Data(0x00);
			}
			SPI_Delay(100);

			Epaper_Write_Command(0x13);
			for(i=0;i<5000;i++)
			{
				Epaper_Write_Data(0xff);
			}
			SPI_Delay(100);

			Epaper_Write_Command(0x12);			//DISPLAY REFRESH
			eInkDisplayHandler->delayMS(200);	    //!!!The delay here is necessary, 200uS at least!!!
			waitIfBusy(eInkDisplayHandler);
		}       //waiting for the electronic paper IC to release the idle signal
*/
	 printf("EPD_full_display_Clear start \n");

	    unsigned int i;
	    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	    for(i=0;i<5000;i++)
	    {
	    	Epaper_Write_Data(0xff);
	    }
	    Epaper_Write_Command(0x26);   //write RAM for black(0)/white (1)
	    for(i=0;i<5000;i++)
	    {
	    	Epaper_Write_Data(0x00);
	    }
	    Epaper_Write_Command(0x22); //Display Update Control
	    Epaper_Write_Data(0xF7);
	    Epaper_Write_Command(0x20);  //Activate Display Update Sequence
	    waitIfBusy(eInkDisplayHandler);

	    printf("EPD_full_display_Clear finish \n");
}




