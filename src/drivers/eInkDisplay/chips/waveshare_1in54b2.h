/*
 * gdey0154d67.h
 *
 *  Created on: Jul 13, 2022
 *      Author: marcelo
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*readGPIO_fn_t)(int);
typedef int (*writeGPIO_fn_t)(int, uint32_t);
typedef void (*OSdelayMS_fn_t)(uint32_t);


typedef struct
{
	readGPIO_fn_t readGPIO;
	writeGPIO_fn_t writeGPIO;
	uint8_t busyPin;
	uint8_t resetPin;
	uint8_t csPin;
	uint8_t mosiPin;
	uint8_t dcPin;
	uint8_t clkPin;
	uint8_t csInverted;
	OSdelayMS_fn_t delayMS;

} eInkDisplayHandler_t;


void EPD_HW_Init(eInkDisplayHandler_t* eInkDisplay);
void EPD_full_display_Clear(void);
