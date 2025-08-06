
#include "GDEY0154D67.h"


//200*200///////////////////////////////////////

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2
#define RED_MODE     3

#define MAX_LINE_BYTES 25// =200/8
#define MAX_COLUMN_BYTES  200

#define ALLSCREEN_GRAGHBYTES 5000

static eInkGDEY0154D67DisplayHandler_t* eInkDisplayHandler;

////////FUNCTION//////


static void SPI_Delay(uint32_t xrate);
static void SPI_Write(unsigned char value);
static void Epaper_Write_Command(unsigned char command);
static void Epaper_Write_Data(unsigned char data);
//EPD
static void waitIfBusy(eInkGDEY0154D67DisplayHandler_t* eInkDisplay);






//////////////////////SPI///////////////////////////////////
static void SPI_Delay(uint32_t xrate)
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
   SPI_Delay(20);
    for(i=0; i<8; i++)
    {
    	eInkDisplayHandler->writeCLK(0);
       SPI_Delay(10);
       if(value & 0x80)
    	   eInkDisplayHandler->writeMOSI(1);
        else
       	   eInkDisplayHandler->writeMOSI(0);
        value = (value << 1);
       SPI_Delay(10);
       eInkDisplayHandler->writeCLK(1);
        SPI_Delay(20);
    }
}

static void Epaper_Write_Command(unsigned char command)
{
  SPI_Delay(10);

  eInkDisplayHandler->writeDC(0); // command write

  SPI_Delay(10);

  if (GDEY0154D67_CS_INVERTED)
	  eInkDisplayHandler->writeCS(1);
  else
	  eInkDisplayHandler->writeCS(0);

  SPI_Write(command);

  if (GDEY0154D67_CS_INVERTED)
  	  eInkDisplayHandler->writeCS(0);
    else
  	  eInkDisplayHandler->writeCS(1);
  SPI_Delay(10);
}
static void Epaper_Write_Data(unsigned char data)
{
  //SPI_Delay(5);

  eInkDisplayHandler->writeDC(1);   // data write

  SPI_Delay(20);

  if (GDEY0154D67_CS_INVERTED)
	  eInkDisplayHandler->writeCS(1);
  else
	  eInkDisplayHandler->writeCS(0);

  SPI_Write(data);

  if (GDEY0154D67_CS_INVERTED)
  	  eInkDisplayHandler->writeCS(0);
    else
  	  eInkDisplayHandler->writeCS(1);
  SPI_Delay(20);
}

static void waitIfBusy(eInkGDEY0154D67DisplayHandler_t* eInkDisplay)
{
	while(!eInkDisplay->readBUSY())
		eInkDisplayHandler->delayMS(50);

  eInkDisplayHandler->delayMS(100);
}

/////////////////EPD settings Functions/////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void GDEY0154D67_HW_Init(eInkGDEY0154D67DisplayHandler_t* eInkDisplay)
{
	eInkDisplayHandler = eInkDisplay;

	printf("EPD_HW_Init start \n");

	waitIfBusy(eInkDisplayHandler);

	eInkDisplayHandler->writeRST(1);
	eInkDisplayHandler->delayMS(50);
	eInkDisplayHandler->writeRST(0);  // Module reset
	eInkDisplayHandler->delayMS(50);
	eInkDisplayHandler->writeRST(1);
	eInkDisplayHandler->delayMS(50);

	waitIfBusy(eInkDisplayHandler);

	Epaper_Write_Command(0x12);  //SWRESET
	waitIfBusy(eInkDisplayHandler);

	Epaper_Write_Command(0x01); //Driver output control
	Epaper_Write_Data(0xC7);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x11); //data entry mode
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

	Epaper_Write_Command(0x45); //set Ram-Y address start/end position
	Epaper_Write_Data(0xC7);   //0xC7-->(199+1)=200
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x3C); //BorderWavefrom
	Epaper_Write_Data(0x05);

	Epaper_Write_Command(0x18); //Reading temperature sensor
	Epaper_Write_Data(0x80);

	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;
	Epaper_Write_Data(0xC7);
	Epaper_Write_Data(0x00);
	waitIfBusy(eInkDisplayHandler);

	Epaper_Write_Command(0x04);//Power on
	eInkDisplayHandler->delayMS(200);
	waitIfBusy(eInkDisplayHandler);

	printf("EPD_HW_Init finish \n");
}


void GDEY0154D67_full_display_Clear(void)
{
	unsigned int i,k;

	printf("EPD_full_display_Clear start \n");

	Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(k=0;k<250;k++)
	{
		for(i=0;i<25;i++)
		{
	    	Epaper_Write_Data(0xff);
	    }
	 }
	Epaper_Write_Command(0x22); //Display Update Control
	Epaper_Write_Data(0xF7);
	Epaper_Write_Command(0x20); //Activate Display Update Sequence

	eInkDisplayHandler->delayMS(200);
	waitIfBusy(eInkDisplayHandler);

	printf("EPD_full_display_Clear finish \n");
}

void GDEY0154D67_printImage(unsigned char *Image, uint8_t flipVertical, uint8_t invert)
{
	Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)

	eInkDisplayHandler->writeDC(1);   // data write

	SPI_Delay(5);

	if (GDEY0154D67_CS_INVERTED)
		eInkDisplayHandler->writeCS(1);
	else
		eInkDisplayHandler->writeCS(0);

	if(flipVertical)
	{
		if (invert)
		{
			for(int i=199; i>=0; i--)
				for(int j=0; j<25; j++)
					SPI_Write(0xFF - Image[(i * 25) + j]);
		}else{
			for(int i=199; i>=0; i--)
				for(int j=0; j<25; j++)
					SPI_Write(Image[(i * 25) + j]);
		}
	}else{
		if (invert)
		{
			for(int i=0; i<200; i++)
				for(int j=0; j<25; j++)
					SPI_Write(0XFF - Image[(i * 25) + j]);
		}else{
			for(int i=0; i<200; i++)
				for(int j=0; j<25; j++)
					SPI_Write(Image[(i * 25) + j]);
		}
	}

	if (GDEY0154D67_CS_INVERTED)
		eInkDisplayHandler->writeCS(0);
	else
		eInkDisplayHandler->writeCS(1);

	SPI_Delay(5);

	Epaper_Write_Command(0x22); //Display Update Control
	Epaper_Write_Data(0xF7);
	Epaper_Write_Command(0x20); //Activate Display Update Sequence

	eInkDisplayHandler->delayMS(200);
	waitIfBusy(eInkDisplayHandler);
}

