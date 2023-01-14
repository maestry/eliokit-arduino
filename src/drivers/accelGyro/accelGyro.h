/*
 * accelerometer.h
 *
 *  Created on: May 12, 2022
 *      Author: marcelo
 */

#pragma once

#include <stdlib.h>

typedef struct __attribute__((packed))
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} accelSampleRAW_t;

typedef struct __attribute__((packed))
{
	int16_t Xpitch;
	int16_t Yroll;
	int16_t Zyaw;
} angularRateSampleRAW_t;

typedef struct __attribute__((packed))
{
	double X;
	double Y;
	double Z;
} accelSampleG_t;

typedef struct __attribute__((packed))
{
	double Xpitch;
	double Yroll;
	double Zyaw;
} angularRateSampleDps_t;

typedef enum
{
	ACCEL_HIGH_PERFORMANCE                    = 0x04,
	ACCEL_CONT_LOW_PWR_4                      = 0x03,
	ACCEL_CONT_LOW_PWR_3                      = 0x02,
	ACCEL_CONT_LOW_PWR_2                      = 0x01,
	ACCEL_CONT_LOW_PWR_12bit                  = 0x00,
	ACCEL_SINGLE_LOW_PWR_4                    = 0x0B,
	ACCEL_SINGLE_LOW_PWR_3                    = 0x0A,
	ACCEL_SINGLE_LOW_PWR_2                    = 0x09,
	ACCEL_SINGLE_LOW_PWR_12bit                = 0x08,
	ACCEL_HIGH_PERFORMANCE_LOW_NOISE          = 0x14,
	ACCEL_CONT_LOW_PWR_LOW_NOISE_4            = 0x13,
	ACCEL_CONT_LOW_PWR_LOW_NOISE_3            = 0x12,
	ACCEL_CONT_LOW_PWR_LOW_NOISE_2            = 0x11,
	ACCEL_CONT_LOW_PWR_LOW_NOISE_12bit        = 0x10,
	ACCEL_SINGLE_LOW_PWR_LOW_NOISE_4          = 0x1B,
	ACCEL_SINGLE_LOW_PWR_LOW_NOISE_3          = 0x1A,
	ACCEL_SINGLE_LOW_PWR_LOW_NOISE_2          = 0x19,
	ACCEL_SINGLE_LOW_LOW_NOISE_PWR_12bit      = 0x18,
} accelOperation_mode_t;

typedef enum
{
	ACCEL_ODR_OFF            = 0x00,
	ACCEL_ODR_1Hz6_LP_ONLY   = 0x01,
	ACCEL_ODR_12Hz5          = 0x02,
	ACCEL_ODR_25Hz           = 0x03,
	ACCEL_ODR_50Hz           = 0x04,
	ACCEL_ODR_100Hz          = 0x05,
	ACCEL_ODR_200Hz          = 0x06,
	ACCEL_ODR_400Hz          = 0x07,
	ACCEL_ODR_800Hz          = 0x08,
	ACCEL_ODR_1k6Hz          = 0x09,
	ACCEL_SET_SW_TRIG        = 0x32,  /* Use this only in SINGLE mode */
  	ACCEL_SET_PIN_TRIG       = 0x12,  /* Use this only in SINGLE mode */
} accelODR_t;

typedef enum
{
  ACCEL_SCALE_2g     = 0,
  ACCEL_SCALE_4g     = 2,
  ACCEL_SCALE_8g     = 3,
  ACCEL_SCALE_16g    = 1,
} accelScale_t;

typedef enum
{
  GYRO_SCALE_250dps     = 0,
  GYRO_SCALE_125dps     = 1,
  GYRO_SCALE_500dps     = 2,
  GYRO_SCALE_1000dps    = 4,
  GYRO_SCALE_2000dps    = 6,
} gyroScale_t;

typedef struct
{
  uint8_t tnda                      : 1;
  uint8_t gnda                      : 1;
  uint8_t xlnda                     : 1;
  uint8_t singleTap                 : 1;
  uint8_t doubleTap                 : 1;
  uint8_t freeFall               	: 1;
  uint8_t wakeUp	               	: 1;
} accelGyroInterruptStatus_t;

void accelGyroInit(void);
void accelGetXYZSampleRAW(accelSampleRAW_t* accelerationSample);
void accelGetXYZSampleG(accelSampleG_t* accelerationSample);
void gyroGetXYZSampleDps(angularRateSampleDps_t* angularRateSample);
gyroScale_t gyroGetScale(void);
int32_t accelGetTemperatureRAW(void);
double accelGetTemperatureC(void);
void accelGyroGetAllInterruptSources(accelGyroInterruptStatus_t* accelGyroInterruptStatus);
uint8_t accelGyroGetDeviceID(void);
int32_t accelSetOperationMode(accelOperation_mode_t mode);
int32_t accelSetDataRate(accelODR_t odr);
int32_t accelSetScale(accelScale_t scale);
accelScale_t accellGetScale(void);

