/*
 * lightSensor.c
 *
 *  Created on: Aug 8, 2022
 *      Author: marcelo
 */


#include "alsPs.h"

#include <drivers/alsPs/chips/LTR553ALS.h>
#include <string.h>
#include "drivers/i2c/i2cPeripheral.h"
#include "drivers/boards/s3_bsp.h"


#define WRITE_BUF_LENGTH	16

static uint8_t write_buf[WRITE_BUF_LENGTH];

static void setALSIntegrationMeasurementRate(void);
static void setPSControlRegister(LTR553PSControlRegister_t LTR553PSControlRegister);
static void setPSLEDRegister(LTR553PSLEDRegister_t LTR553PSLEDRegister);
static void setPSMEASRATERegister(LTR553PSMEASRATERegister_t LTR553PSMEASRATERegister);
static void setALSMEASRATERegister(LTR553ALSMEASRATERegister_t LTR553ALSMEASRATERegister);
static void setALSControlRegister(LTR553ALSControlRegister_t LTR553ALSControlRegister);
static void getALSCH0andCH1RawValues(double* ch1Raw, double* ch0Raw);
static void getPSADCRawValue(double* psADCRawValue);
static void getALSPSStatusRegister(LTR553StatusRegister_t* LTR553StatusRegister);
static void setInterruptRegister(LTR553InterruptRegister_t LTR553InterruptRegister);

void ALSPSInit(void) // user need wait almost 10ms after init, gain set to 1X
{
	LTR553InterruptRegister_t 	LTR553InterruptRegister;
	LTR553PSLEDRegister_t		LTR553PSLEDRegister;
	LTR553PSMEASRATERegister_t	LTR553PSMEASRATERegister;
	LTR553ALSMEASRATERegister_t	LTR553ALSMEASRATERegister;

	// configure interrupts
	LTR553InterruptRegister.value = 0;  // Interrupt pin is INACTIVE / high impedance state
	setInterruptRegister(LTR553InterruptRegister);

	// set PS led configuration
	LTR553PSLEDRegister.value = 0;
	LTR553PSLEDRegister.LEDCurrent = 4; // 100mA
	LTR553PSLEDRegister.LEDCurrentDuty = 3; // 100%
	LTR553PSLEDRegister.LEDPMF = 3; // 60Khz
	setPSLEDRegister(LTR553PSLEDRegister);

	// set PS measurement rate
	LTR553PSMEASRATERegister.value = 0;
	LTR553PSMEASRATERegister.PSMeasRate = 3; // 200mS
	setPSMEASRATERegister(LTR553PSMEASRATERegister);

	// set ALS measurement rate
	LTR553ALSMEASRATERegister.value = 0;
	LTR553ALSMEASRATERegister.ALSMeasRate = 3; // 500ms
	LTR553ALSMEASRATERegister.ALsIntTime = 0; // 100ms
	setALSMEASRATERegister(LTR553ALSMEASRATERegister);
}

void ALSPSGetLightValues(double* visiblePlusIR, double* IR)
{
	getALSCH0andCH1RawValues(IR, visiblePlusIR);
}

void ALSPSGetPSValue(double* ps)
{
	getPSADCRawValue(ps);
}

void ALSPSEnableALS(void)
{
	LTR553ALSControlRegister_t	LTR553ALSControlRegister;

	// set ALS active mode
	LTR553ALSControlRegister.value = 0;
	LTR553ALSControlRegister.ALSmode = 1; // active mode
	LTR553ALSControlRegister.SWreset = 0;
	LTR553ALSControlRegister.ALSGain = 0; // gain 1x
	setALSControlRegister(LTR553ALSControlRegister);
}

void ALSPSDisableALS(void)
{
	LTR553ALSControlRegister_t	LTR553ALSControlRegister;

	// set ALS active mode
	LTR553ALSControlRegister.value = 0;
	LTR553ALSControlRegister.ALSmode = 0; // active mode
	LTR553ALSControlRegister.SWreset = 0;
	LTR553ALSControlRegister.ALSGain = 0; // gain 1x
	setALSControlRegister(LTR553ALSControlRegister);
}

void ALSPSEnablePS(void)
{
	LTR553PSControlRegister_t	LTR553PSControlRegister;

	// set PS active mode
	LTR553PSControlRegister.value = 0;
	LTR553PSControlRegister.psSaturationIE = 1;
	LTR553PSControlRegister.psMode = 3; // active mode
	setPSControlRegister(LTR553PSControlRegister);
}

void ALSPSDisablePS(void)
{
	LTR553PSControlRegister_t	LTR553PSControlRegister;

	// set PS active mode
	LTR553PSControlRegister.value = 0;
	LTR553PSControlRegister.psSaturationIE = 1;
	LTR553PSControlRegister.psMode = 0; // active mode
	setPSControlRegister(LTR553PSControlRegister);
}


void ALSPSCheckInterrupt(double* visiblePlusIR, double* IR, uint8_t* ALSDataValid, double* ps, uint8_t* PSDataValid)
{
	LTR553StatusRegister_t LTR553StatusRegister;

	getALSPSStatusRegister(&LTR553StatusRegister);

	if(LTR553StatusRegister.ALSInterruptStatus && LTR553StatusRegister.ALSDataValid)
	{
		getALSCH0andCH1RawValues(IR, visiblePlusIR);
		*ALSDataValid = 1;
	}else{
		*ALSDataValid = 0;
	}
	if(LTR553StatusRegister.PSInterruptStatus)
	{
		getPSADCRawValue(ps);
		*PSDataValid = 1;
	}else{
		*PSDataValid = 0;
	}
}

static void setALSIntegrationMeasurementRate(void)
{
	write_buf[0] = LTR553_ALS_MEAS_RATE;
	write_buf[1] = 0x03; //100mS integration, 500mS measurement rate

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setALSControlRegister(LTR553ALSControlRegister_t LTR553ALSControlRegister)
{
	write_buf[0] = LTR553_ALS_CONTR;
	write_buf[1] = LTR553ALSControlRegister.value;

	printf("setALSControlRegister %d \n", write_buf[1]);

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setPSControlRegister(LTR553PSControlRegister_t LTR553PSControlRegister)
{
	write_buf[0] = LTR553_PS_CONTR;
	write_buf[1] = LTR553PSControlRegister.value;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setPSLEDRegister(LTR553PSLEDRegister_t LTR553PSLEDRegister)
{
	write_buf[0] = LTR553_PS_LED;
	write_buf[1] = LTR553PSLEDRegister.value;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setPSMEASRATERegister(LTR553PSMEASRATERegister_t LTR553PSMEASRATERegister)
{
	write_buf[0] = LTR553_PS_MEAS_RATE;
	write_buf[1] = LTR553PSMEASRATERegister.value;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setALSMEASRATERegister(LTR553ALSMEASRATERegister_t LTR553ALSMEASRATERegister)
{
	write_buf[0] = LTR553_ALS_MEAS_RATE;
	write_buf[1] = LTR553ALSMEASRATERegister.value;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void getALSCH0andCH1RawValues(double* ch1Raw, double* ch0Raw)
{
	uint8_t ch1_0, ch1_1, ch0_0, ch0_1, regAddress; // channel 1 IR, channel 0 vis+IR

	regAddress = LTR553_ALS_DATA_CH1_0;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &ch1_0, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	regAddress = LTR553_ALS_DATA_CH1_1;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &ch1_1, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	regAddress = LTR553_ALS_DATA_CH0_0;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &ch0_0, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	regAddress = LTR553_ALS_DATA_CH0_1;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &ch0_1, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	*ch1Raw = (uint16_t)((ch1_1 << 8) | ch1_0);
	*ch0Raw = (uint16_t)((ch0_1 << 8) | ch0_0);
}

static void getPSADCRawValue(double* psADCRawValue)
{
	uint8_t regAddress, data0, data1;

	regAddress = LTR553_PS_DATA_0;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &data0, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	regAddress = LTR553_PS_DATA_1;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &data1, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	*psADCRawValue = (uint16_t)((data1 << 8) | data0);
}

static void getALSPSStatusRegister(LTR553StatusRegister_t* LTR553StatusRegister)
{
	uint8_t regAddress;

	regAddress = LTR553_ALS_PS_STATUS;
	i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, LTR553_ADDR, &regAddress, 1, &LTR553StatusRegister->value, 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}

static void setInterruptRegister(LTR553InterruptRegister_t LTR553InterruptRegister)
{
	write_buf[0] = LTR553_INTERRUPT;
	write_buf[1] = LTR553InterruptRegister.value;

	i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, LTR553_ADDR, write_buf, 2, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}
