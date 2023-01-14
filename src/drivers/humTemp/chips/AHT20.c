#include "AHT20.h"
#include <math.h>
#include "../../i2c/i2cPeripheral.h"


#define READ_BUF_LENGTH			16
#define WRITE_BUF_LENGTH		16

static uint8_t readBuf[READ_BUF_LENGTH];
static uint8_t writeBuf[WRITE_BUF_LENGTH];

static void getAHT20Status(uint8_t* busy, uint8_t* calibrated);
static uint8_t calcCRC8(uint8_t *dat, uint8_t Num);

void setAHT20Enable(void)
{
	uint8_t busy;
	uint8_t calibrated;

	i2cPeripheralMasterTakeMutex(I2C_0_MASTER_NUM);

	vTaskDelay(40 / portTICK_RATE_MS);

	// Read Status
	getAHT20Status(&busy, &calibrated);

	// If not calibrated, calibrate
	if (!calibrated)
	{
		printf("calibrate \n");
		writeBuf[0] = 0xBE;
		writeBuf[1] = 0x08;
		writeBuf[2] = 0x00;
		i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, AHT20_IIC_ADDR, writeBuf, 3, 2000 / portTICK_RATE_MS, 0);
		vTaskDelay(10 / portTICK_RATE_MS);
	}

	i2cPeripheralMasterGiveMutex(I2C_0_MASTER_NUM);
}


void getAHT20HumTemp(int32_t* hum, int32_t* temp)
{
	uint8_t busy;
	uint8_t calibrated;
	uint32_t hRAW, tRAW;

	i2cPeripheralMasterTakeMutex(I2C_0_MASTER_NUM);


	writeBuf[0] = AHT20_CMD_TRIGGER;
	writeBuf[1] = 0x33;
	writeBuf[2] = 0x00;
	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, AHT20_IIC_ADDR, writeBuf, 3, 2000 / portTICK_RATE_MS, 0);

	vTaskDelay(100 / portTICK_RATE_MS);

	// Read Status
	getAHT20Status(&busy, &calibrated);

	if(busy)
	{
		vTaskDelay(100 / portTICK_RATE_MS);
		getAHT20Status(&busy, &calibrated);
	}

	if(!busy)
	{
		writeBuf[0] = AHT20_CMD_STATUS;
		i2cPeripheralMasterReadDevice(I2C_0_MASTER_NUM, AHT20_IIC_ADDR, readBuf, 7, 2000 / portTICK_RATE_MS, 0);

		if (calcCRC8(&readBuf[0], 6) == readBuf[6])
		{
			hRAW = readBuf[1];
			hRAW <<= 8;
			hRAW |= readBuf[2];
			hRAW <<= 4;
			hRAW |= readBuf[3] >> 4;
			*hum = (int32_t)(((float)hRAW * 10000) / 0x100000);

			tRAW = readBuf[3] & 0x0F;
			tRAW <<= 8;
			tRAW |= readBuf[4];
			tRAW <<= 8;
			tRAW |= readBuf[5];
			*temp = (int32_t)((((float)tRAW * 200 / 0x100000) - 50) * 100);
		}
	}

	i2cPeripheralMasterGiveMutex(I2C_0_MASTER_NUM);
}



static void getAHT20Status(uint8_t* busy, uint8_t* calibrated)
{
	writeBuf[0] = AHT20_CMD_STATUS;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, AHT20_IIC_ADDR, writeBuf, 1, readBuf, 1, 2000 / portTICK_RATE_MS, 0);

	*busy = readBuf[0] & AHT20_STATUS_BUSY ? 1 : 0;
	*calibrated = readBuf[0] & AHT20_STATUS_CALIBRATED ? 1 : 0;
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
