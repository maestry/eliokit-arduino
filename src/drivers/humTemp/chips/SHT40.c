/*
 * SHT40.c
 *
 *  Created on: Nov 23, 2022
 *      Author: marcelo
 */

#include "SHT40.h"
#include "drivers/i2c/i2cPeripheral.h"

#define SHT40_ADDRESS			0x44 	// SHT40-AD1B
#define SHT4X_CMD_MEASURE_HPM 	0xFD

#define READ_BUF_LENGTH			16
#define WRITE_BUF_LENGTH		16

static uint8_t readBuf[READ_BUF_LENGTH];
static uint8_t writeBuf[WRITE_BUF_LENGTH];


void SHT40StartMeasure(void)
{
	writeBuf[0] = SHT4X_CMD_MEASURE_HPM;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, SHT40_ADDRESS, writeBuf, 1, 20 / portTICK_RATE_MS, 1);
}

void SHT40GetMeasure(double* temperature, double* humidity)
{
	uint16_t tTicks, rhTicks;
	uint8_t checkSumT, checksumRH;
    /**
     * formulas for conversion of the sensor signals, optimized for fixed point
     * algebra:
     * Temperature = 175 * S_T / 65535 - 45
     * Relative Humidity = 125 * (S_RH / 65535) - 6
     */
    i2cPeripheralMasterReadDevice(I2C_0_MASTER_NUM, SHT40_ADDRESS, readBuf, 6, 20 / portTICK_RATE_MS, 1);
    checkSumT = readBuf[2];
    checksumRH = readBuf[5];

    tTicks = ((uint16_t)readBuf[0] << 8) + readBuf[1];
    rhTicks = ((uint16_t)readBuf[3] << 8) + readBuf[4];

    if(temperature != NULL)
    	*temperature = -45 + 175 * (double)tTicks/65535;

    if(humidity != NULL)
    {
        *humidity = -6 + 125 * (double)rhTicks/65535;

        if (*humidity > 100)
        	*humidity = 100;
        else if (*humidity < 0)
        	*humidity = 0;
    }
}
