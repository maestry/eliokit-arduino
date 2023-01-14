
#pragma once

#include <string.h>


/* vendor chip id*/
#define AHT20_IIC_ADDR				0x38

#define AHT20_CMD_STATUS 			0x71

#define AHT20_CMD_CALIBRATE 0xE1     ///< Calibration command
#define AHT20_CMD_TRIGGER 0xAC       ///< Trigger reading command
#define AHT20_CMD_SOFTRESET 0xBA     ///< Soft reset command
#define AHT20_STATUS_BUSY 0x80       ///< Status bit for busy
#define AHT20_STATUS_CALIBRATED 0x08 ///< Status bit for calibrated


void setAHT20Enable(void);
void getAHT20HumTemp(int32_t* hum, int32_t* temp);
