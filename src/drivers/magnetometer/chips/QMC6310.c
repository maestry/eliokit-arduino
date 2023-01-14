#include "QMC6310.h"
#include <math.h>
#include "../../i2c/i2cPeripheral.h"


#define READ_BUF_LENGTH			16
#define WRITE_BUF_LENGTH		16
#define QMC6310_ADD				QMC6310U_IIC_ADDR


static uint8_t readBuf[READ_BUF_LENGTH];
static uint8_t writeBuf[WRITE_BUF_LENGTH];




void setQMC6310Enable(void)
{
	i2cPeripheralMasterTakeMutex(I2C_0_MASTER_NUM);


	writeBuf[0] = 0x29; // undocumented axis sign register
	writeBuf[1] = 0x06; // undocumented axis sign configuration
	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 2, 2000 / portTICK_RATE_MS, 0);

	writeBuf[0] = QMC6310_CTL_REG_ONE; // control register 1
	writeBuf[1] = 0xC1; // ODR: 10Hz
	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 2, 2000 / portTICK_RATE_MS, 0);

	writeBuf[0] = QMC6310_CTL_REG_TWO; // control register 2
	writeBuf[1] = 0x00; // 30 Gauss range
	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 2, 2000 / portTICK_RATE_MS, 0);

	i2cPeripheralMasterGiveMutex(I2C_0_MASTER_NUM);
}

void getQMC6310ID(uint8_t* id)
{
	writeBuf[0] = QMC6310_CHIP_ID_REG;

	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 1, id, 1, 2000 / portTICK_RATE_MS, 1);
}

void getQMC6310XYZGaussRAW(int16_t* x, int16_t* y, int16_t* z, uint32_t* gaussFactor, uint8_t* ovfl, uint8_t* dataValid)
{
	uint8_t status;

	// Read status register
	writeBuf[0] = QMC6310_STATUS_REG;

	i2cPeripheralMasterTakeMutex(I2C_0_MASTER_NUM);

	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 1, &status, 1, 2000 / portTICK_RATE_MS, 0);

	*dataValid = status & 0x01 ? 1 : 0;
	*ovfl = status & 0x02 ? 1 : 0;

	if(*dataValid == 1 && *ovfl == 0)
	{
		writeBuf[0] = QMC6310_DATA_OUT_X_LSB_REG;
		i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, QMC6310_ADD, writeBuf, 1, readBuf, 6, 2000 / portTICK_RATE_MS, 0);

		*x = (int16_t)readBuf[1];
		*x = (*x * 256) + (int16_t) readBuf[0];
		*y = (int16_t)readBuf[3];
		*y = (*y * 256) + (int16_t) readBuf[2];
		*z = (int16_t)readBuf[5];
		*z = (*z * 256) + (int16_t) readBuf[4];

		*gaussFactor = 1000;
	}

	i2cPeripheralMasterGiveMutex(I2C_0_MASTER_NUM);
}



void getQMC6310Heading(double* heading)
{
	int16_t x, y, z;
	uint32_t gaussFactor;
	uint8_t ovfl, dataValid;

	getQMC6310XYZGaussRAW(&x, &y, &z, &gaussFactor, &ovfl, &dataValid);

	*heading = atan2(y, x) * 180 / M_PI;
}




