/*
 * LTR303.h
 *
 *  Created on: Aug 8, 2022
 *      Author: marcelo
 */

#pragma once

#define LTR553_ADDR 0x23 // default address

// LTR303 register addresses
// R/W
#define LTR553_ALS_CONTR 0x80
#define LTR553_PS_CONTR 0x81
#define LTR553_PS_LED 0x82
#define LTR553_PS_N_PULSES 0x83
#define LTR553_PS_MEAS_RATE 0x84
#define LTR553_ALS_MEAS_RATE 0x85
// R
#define LTR553_PART_ID 0x86
#define LTR553_MANUFAC_ID 0x87
#define LTR553_ALS_DATA_CH1_0 0x88
#define LTR553_ALS_DATA_CH1_1 0x89
#define LTR553_ALS_DATA_CH0_0 0x8A
#define LTR553_ALS_DATA_CH0_1 0x8B
#define LTR553_ALS_PS_STATUS 0x8C
#define LTR553_PS_DATA_0 0x8D
#define LTR553_PS_DATA_1 0x8E
// R/W
#define LTR553_INTERRUPT 0x8F
#define LTR553_PS_THRES_UP_0 0x90
#define LTR553_PS_THRES_UP_1 0x91
#define LTR553_PS_THRES_LOW_0 0x92
#define LTR553_PS_THRES_LOW_1 0x93
#define LTR553_OFFSET_1 0x94
#define LTR553_OFFSET_0 0x95
#define LTR553_ALS_THRES_UP_0 0x97
#define LTR553_ALS_THRES_UP_1 0x98
#define LTR553_ALS_THRES_LOW_0 0x99
#define LTR553_ALS_THRES_LOW_1 0x9A
#define LTR553_INTERRUPT_PERSIST 0x9E

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t PSDataStatus : 1;
		uint8_t PSInterruptStatus : 1;
		uint8_t ALSDataStatus : 1;
		uint8_t ALSInterruptStatus : 1;
		uint8_t ALSDataGainRange : 3;
		uint8_t ALSDataValid : 1;
	};
} LTR553StatusRegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t interruptMode : 2;
		uint8_t interruptPolarity : 1;
		uint8_t reserved : 5;
	};
} LTR553InterruptRegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t psMode : 2;
		uint8_t psGain : 2;
		uint8_t reserved1 : 1; // must be 0
		uint8_t psSaturationIE : 1;
		uint8_t reserved2 : 2; // must be 0
	};
} LTR553PSControlRegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t ALSmode : 1;
		uint8_t SWreset : 1;
		uint8_t ALSGain : 3;
		uint8_t reserved : 3;
	};
} LTR553ALSControlRegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t LEDCurrent : 3;
		uint8_t LEDCurrentDuty : 2;
		uint8_t LEDPMF : 3;
	};
} LTR553PSLEDRegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t PSMeasRate : 4;
		uint8_t reserved : 4;
	};
} LTR553PSMEASRATERegister_t;

typedef union
{
	uint8_t value;
	struct
	{
		uint8_t ALSMeasRate : 3;
		uint8_t ALsIntTime : 3;
		uint8_t reserved : 2;
	};
} LTR553ALSMEASRATERegister_t;
