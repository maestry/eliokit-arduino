#include "BS8112A-3.h"
#include "../../i2c/i2cPeripheral.h"

#define BS8112A_3_ADDRESS 0x50

#define READ_BUF_LENGTH 16

static uint8_t readBuf[READ_BUF_LENGTH];

static uint8_t settingsToWrite[19];

// touch threshold 8 to 63

void BS8112A_3InitSettingsToWrite(void)
{
	settingsToWrite[0] = 0xB0;	// register address
	settingsToWrite[1] = 0x01;	// option 1
	settingsToWrite[2] = 0x00;	// reserved
	settingsToWrite[3] = 0x83;	// reserved
	settingsToWrite[4] = 0xF3;	// reserved
	settingsToWrite[5] = 0x98;	// option 2
	settingsToWrite[6] = 63;	// key 1 threshold
	settingsToWrite[7] = 63;	// key 2 threshold
	settingsToWrite[8] = 63;	// key 3 threshold
	settingsToWrite[9] = 63;	// key 4 threshold
	settingsToWrite[10] = 63;	// key 5 threshold
	settingsToWrite[11] = 63;	// key 6 threshold
	settingsToWrite[12] = 63;	// key 7 threshold
	settingsToWrite[13] = 63;	// key 8 threshold
	settingsToWrite[14] = 63;	// key 9 threshold
	settingsToWrite[15] = 63;	// key 10 threshold
	settingsToWrite[16] = 63;	// key 11 threshold
	settingsToWrite[17] = 0xC0; // key 12 config as IRQ
	settingsToWrite[18] = 0x00; // checksum

	for (int i = 1; i < 18; i++)
		settingsToWrite[18] += settingsToWrite[i];
}

int32_t BS8112A_3WriteSettings(void)
{

	return i2cMasterWriteDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, settingsToWrite, 19, 1);

	// return i2cPeripheralMasterWriteToDevice(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, settingsToWrite, 19, 20 / portTICK_RATE_MS, 1);
}

int32_t BS8112A_3ReadKeysThreshold(uint8_t *kth)
{
	int32_t err;

	// err = i2cPeripheralMasterWriteReadDevice(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, writeBuf, 1, readBuf, 11, 20 / portTICK_RATE_MS, 1);
	err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, 0xB5, readBuf, 11, 1);

	kth[0] = readBuf[0] & 0x3F;
	kth[1] = readBuf[1] & 0x3F;
	kth[2] = readBuf[2] & 0x3F;
	kth[3] = readBuf[3] & 0x3F;
	kth[4] = readBuf[4] & 0x3F;
	kth[5] = readBuf[5] & 0x3F;
	kth[6] = readBuf[6] & 0x3F;
	kth[7] = readBuf[7] & 0x3F;
	kth[8] = readBuf[8] & 0x3F;
	kth[9] = readBuf[9] & 0x3F;
	kth[10] = readBuf[10] & 0x3F;

	return err;
}

int32_t BS8112A_3ReadKeyThreshold(uint8_t kNumber, uint8_t *kth)
{
	int32_t err = 1;
	uint8_t regAddress;

	if (kNumber > 0 && kNumber < 12)
	{
		regAddress = 0xB4 + kNumber;

		err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, regAddress, readBuf, 1, 1);

		*kth = readBuf[0] & 0x3F;
	}

	return err;
}

void BS8112A_3SetKeyThreshold(uint8_t kNumber, uint8_t kth)
{
	if (kNumber > 0 && kNumber < 12)
	{
		settingsToWrite[5 + kNumber] = kth; // key kNumber threshold
		for (int i = 0; i < 18; i++)
			settingsToWrite[18] += settingsToWrite[i];
	}
}

int32_t BS8112A_3ReadIRQ_OMS(uint8_t *irq_oms)
{
	int32_t err;
	uint8_t regAddress;

	regAddress = 0xB0;

	// err = i2cPeripheralMasterWriteReadDevice(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, writeBuf, 1, irq_oms, 1, 20 / portTICK_RATE_MS, 1);
	err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, regAddress, irq_oms, 1, 1);

	*irq_oms = *irq_oms & 0x01;

	return err;
}

void BS8112A_3SetIRQ_OMS(uint8_t irq_oms)
{
	settingsToWrite[1] = irq_oms & 0x01; // option 1

	for (int i = 0; i < 18; i++)
		settingsToWrite[18] += settingsToWrite[i];
}

int32_t BS8112A_3ReadKey12Mode(uint8_t *key12Mode)
{
	int32_t err;
	uint8_t regAddress;

	regAddress = 0xC0;
	err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, regAddress, key12Mode, 1, 1);

	*key12Mode = (*key12Mode & 0x40);

	return err;
}

void BS8112A_3SetKey12Mode(uint8_t key12Mode)
{
	settingsToWrite[17] = key12Mode & 0x40; // key 12 config as IRQ

	for (int i = 0; i < 18; i++)
		settingsToWrite[18] += settingsToWrite[i];
}

int32_t BS8112A_3ReadLSC(uint8_t *lsc)
{
	int32_t err;
	uint8_t regAddress;

	regAddress = 0xB4;

	// err = i2cPeripheralMasterWriteReadDevice(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, writeBuf, 1, lsc, 1, 20 / portTICK_RATE_MS, 1);
	err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, regAddress, lsc, 1, 1);

	*lsc = (*lsc & 0x40) >> 6;

	return err;
}

void BS8112A_3SetLSC(uint8_t lsc)
{
	settingsToWrite[5] = 0x98;

	if (lsc)
		settingsToWrite[5] |= 0x40;
	else
		settingsToWrite[5] &= ~(0x40);

	for (int i = 0; i < 18; i++)
		settingsToWrite[18] += settingsToWrite[i];
}

int32_t BS8112A_3ReadKeysStatus(uint8_t *kStatus)
{
	int32_t err;
	uint8_t regAddress;

	regAddress = 0x09;
	err = i2cMasterReadDelayBetweenBytes(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, regAddress, readBuf, 2, 1);
	// err = i2cPeripheralMasterWriteReadDevice(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, writeBuf, 1, readBuf, 2, 20 / portTICK_RATE_MS, 1);

	kStatus[0] = readBuf[0] & 0x01;
	kStatus[1] = (readBuf[0] & 0x02) >> 1;
	kStatus[2] = (readBuf[0] & 0x04) >> 2;
	kStatus[3] = (readBuf[0] & 0x08) >> 3;
	kStatus[4] = (readBuf[0] & 0x10) >> 4;
	kStatus[5] = (readBuf[0] & 0x20) >> 5;
	kStatus[6] = (readBuf[0] & 0x40) >> 6;
	kStatus[7] = (readBuf[0] & 0x80) >> 7;
	kStatus[8] = readBuf[1] & 0x01;
	kStatus[9] = (readBuf[1] & 0x02) >> 1;
	kStatus[10] = (readBuf[1] & 0x04) >> 2;

	return err;
}
