#include "boardDetector.h"
#include "../i2c/i2cPeripheral.h"

// IO boards
#define XL9335_ADD 0x20

// Media board
#define BS8112A_3_ADDRESS 0x50

uint8_t boardDetectorDetectS3Board(void)
{
	uint8_t result = 0;

	for (int i = 1; i < 127; i++)
	{
		i2cPeripheralTestAddressPresence(I2C_0_MASTER_NUM, i, 1);
	}

	return result;
}

uint8_t boardI2CScan(uint8_t i2cNum)
{
	uint8_t result = 0;

	for (int i = 0; i < 127; i++)
	{
		i2cPeripheralTestAddressPresence(i2cNum, i, 1);
	}

	return result;
}

uint8_t boardDetectorDetectMediaBoard(void)
{
	uint8_t result = 0;

	if (0 == i2cPeripheralTestAddressPresence(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, 1))
	{
		printf("Media board detected \n");
		result = 1;
	}

	return result;
}

uint8_t boardDetectorDetectIOBoards(void)
{
	uint8_t result = 0;

	for (int i = 0; i < 8; i++)
	{
		if (0 == i2cPeripheralTestAddressPresence(I2C_1_MASTER_NUM, XL9335_ADD + i, 1))
		{
			result |= 1 << i;
		}
	}

	return result;
}
