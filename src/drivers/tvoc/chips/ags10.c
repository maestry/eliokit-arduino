
#include "ags10.h"
#include "../../i2c/i2cPeripheral.h"


#define READ_BUF_LENGTH			16
#define WRITE_BUF_LENGTH		16
#define AGS10_ADD				0x1A
#define AGS10_W_R_INTERVAL_MS	50
#define I2C_SPEED				10000

static uint8_t readBuf[READ_BUF_LENGTH];
static uint8_t writeBuf[WRITE_BUF_LENGTH];

static uint8_t calcCRC8(uint8_t *dat, uint8_t Num);


void AGS10getTVOCppb(uint32_t* tvoc, uint8_t* preHeatStage, uint8_t* crcError)
{
	writeBuf[0] = 0;
	*crcError = 1;

	i2cPeripheralMasterWriteStopDelayReadDevice(I2C_0_MASTER_NUM, AGS10_ADD, writeBuf, 1, readBuf, 5, 200 / portTICK_RATE_MS, 50, 1);

	if(calcCRC8(readBuf, 4) == readBuf[4])
	{
		*crcError = 0;
		*preHeatStage = readBuf[0] & 0x01;
		*tvoc =	((uint32_t)readBuf[1] << 16) | ((uint32_t)readBuf[2] << 8) | (uint32_t)readBuf[3];
	}
}



//**************************************************************
// Function name: Calc_CRC8
// Function: CRC8 calculation, initial value: 0xFF, polynomial:
// 0x31 (x8+ x5+ x4+1)
//**************************************************************
static uint8_t calcCRC8(uint8_t *dat, uint8_t Num)
{
	uint8_t i,byte,crc=0xFF;

	for(byte=0; byte<Num; byte++)
	{
		crc^=(dat[byte]);
		for(i=0;i<8;i++)
		{
			if(crc & 0x80) crc=(crc<<1)^0x31;
			else crc=(crc<<1);
		}
	}
	return crc;
}
