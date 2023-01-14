
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GDEY0154D67_WIDTH   200
#define GDEY0154D67_HEIGHT  200

typedef uint8_t (*readEINKBUSY_fn_t)(void);
typedef void (*writeEINKCLK_fn_t)(uint8_t);
typedef void (*writeEINKMOSI_fn_t)(uint8_t);
typedef void (*writeEINKCS_fn_t)(uint8_t);
typedef void (*writeEINKRST_fn_t)(uint8_t);
typedef void (*writeEINKDC_fn_t)(uint8_t);
typedef void (*OSdelayMS_fn_t)(uint32_t);

#define GDEY0154D67_BUSY_PIN		EINK_DISPLAY_INPUT_BUSY
#define GDEY0154D67_RESET_PIN		EINK_DISPLAY_OUTPUT_RESET
#define GDEY0154D67_CS_PIN			EINK_DISPLAY_OUTPUT_CS
#define GDEY0154D67_MOSI_PIN		EINK_DISPLAY_OUTPUT_SDI
#define GDEY0154D67_DC_PIN			EINK_DISPLAY_OUTPUT_DC
#define GDEY0154D67_CLK_PIN			EINK_DISPLAY_OUTPUT_SCK

#define GDEY0154D67_CS_INVERTED		0

typedef struct
{
	writeEINKCLK_fn_t writeCLK;
	writeEINKMOSI_fn_t writeMOSI;
	writeEINKCS_fn_t writeCS;
	writeEINKRST_fn_t writeRST;
	writeEINKDC_fn_t writeDC;
	readEINKBUSY_fn_t readBUSY;
	OSdelayMS_fn_t delayMS;
} eInkGDEY0154D67DisplayHandler_t;


void GDEY0154D67_HW_Init(eInkGDEY0154D67DisplayHandler_t* eInkDisplay);
void GDEY0154D67_full_display_Clear(void);
void GDEY0154D67_printImage(unsigned char *Image, uint8_t flipVertical, uint8_t invert);

