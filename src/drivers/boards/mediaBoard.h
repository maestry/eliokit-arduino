#pragma once

#include <stdint.h>

#define MEDIA_MIC_CLK 43
#define MEDIA_MIC_DATA 42
#define MEDIA_SPK_DIN 6
#define MEDIA_SPK_BCLK 15
#define MEDIA_SPK_LRCLK 7
#define EINK_DISPLAY_OUTPUT_SCK GPIO_SPIBUS_CLK
#define EINK_DISPLAY_OUTPUT_SDI GPIO_SPIBUS_MOSI

uint8_t mediaBoardInitialize(void);
void mediaBoardInterruptHandler(void);
