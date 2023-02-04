/*
 * accelerometer.c
 *
 *  Created on: May 12, 2022
 *      Author: marcelo
 */

#include "accelGyro.h"

#include "freertos/FreeRTOS.h"
#include <string.h>

#include "../accelGyro/chips/lsm6ds3tr-c_reg.h"
#include "../i2c/i2cPeripheral.h"
#include "drivers/boards/s3_bsp.h"

#define WRITE_BUF_LENGTH	16

static stmdev_ctx_t accelerometer_ctx;
static uint8_t write_buf[WRITE_BUF_LENGTH];

static int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
static int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);

void accelGyroInit(void) // https://www.st.com/resource/en/application_note/an5130-lsm6ds3trc-alwayson-3d-accelerometer-and-3d-gyroscope-stmicroelectronics.pdf
{
	lsm6ds3tr_c_int1_route_t int1 = {0};
	lsm6ds3tr_c_xl_hm_mode_t lsm6ds3tr_c_xl_hm_mode;
	lsm6ds3tr_c_g_hm_mode_t lsm6ds3tr_c_g_hm_mode;
	lsm6ds3tr_c_odr_xl_t lsm6ds3tr_c_odr_xl;
	lsm6ds3tr_c_odr_g_t lsm6ds3tr_c_odr_g;
	lsm6ds3tr_c_fs_xl_t lsm6ds3tr_c_fs_xl;
	lsm6ds3tr_c_fs_g_t lsm6ds3tr_c_fs_g;

	accelerometer_ctx.write_reg = platform_write;
	accelerometer_ctx.read_reg = platform_read;

	// set data rate
	lsm6ds3tr_c_odr_xl = LSM6DS3TR_C_XL_ODR_208Hz;
	lsm6ds3tr_c_xl_data_rate_set(&accelerometer_ctx, lsm6ds3tr_c_odr_xl);
	lsm6ds3tr_c_odr_g = LSM6DS3TR_C_XL_ODR_208Hz;
	lsm6ds3tr_c_gy_data_rate_set(&accelerometer_ctx, lsm6ds3tr_c_odr_g);

	// set high performance mode
	lsm6ds3tr_c_xl_hm_mode = LSM6DS3TR_C_XL_HIGH_PERFORMANCE;
	lsm6ds3tr_c_xl_power_mode_set(&accelerometer_ctx, lsm6ds3tr_c_xl_hm_mode);
	lsm6ds3tr_c_g_hm_mode = LSM6DS3TR_C_GY_HIGH_PERFORMANCE;
	lsm6ds3tr_c_gy_power_mode_set(&accelerometer_ctx, lsm6ds3tr_c_g_hm_mode);

	// set scale
	lsm6ds3tr_c_fs_xl = LSM6DS3TR_C_4g;
	lsm6ds3tr_c_xl_full_scale_set(&accelerometer_ctx, lsm6ds3tr_c_fs_xl);
	lsm6ds3tr_c_fs_g = LSM6DS3TR_C_2000dps;
	lsm6ds3tr_c_gy_full_scale_set(&accelerometer_ctx, lsm6ds3tr_c_fs_g);

	// interrupt open-drain, active low, latched
	lsm6ds3tr_c_pin_mode_set(&accelerometer_ctx, LSM6DS3TR_C_OPEN_DRAIN);
	lsm6ds3tr_c_pin_polarity_set(&accelerometer_ctx, LSM6DS3TR_C_ACTIVE_LOW);
	lsm6ds3tr_c_int_notification_set(&accelerometer_ctx, LSM6DS3TR_C_INT_LATCHED);

	 // wake up detection configuration
	lsm6ds3tr_c_wkup_dur_set(&accelerometer_ctx, 2); // wake up duration 3 * (1/208): 14ms
	lsm6ds3tr_c_wkup_threshold_set(&accelerometer_ctx, 2); //

	// tap detection configuration
	lsm6ds3tr_c_act_mode_set(&accelerometer_ctx, LSM6DS3TR_C_PROPERTY_DISABLE); // disable inactivity
	lsm6ds3tr_c_tap_mode_set(&accelerometer_ctx, LSM6DS3TR_C_ONLY_SINGLE); // single tap
	lsm6ds3tr_c_xl_hp_path_internal_set(&accelerometer_ctx, LSM6DS3TR_C_USE_SLOPE); // slope input to tap detection
	lsm6ds3tr_c_tap_threshold_x_set(&accelerometer_ctx, 2); // threshold 0.125G (4g/32)
	lsm6ds3tr_c_tap_shock_set(&accelerometer_ctx, 3); // 3*8*(1/208) 115ms aprox
	lsm6ds3tr_c_tap_quiet_set(&accelerometer_ctx, 3); // 3*4*(1/208) 57 ms aprox
	lsm6ds3tr_c_tap_detection_on_z_set(&accelerometer_ctx, 1); // tap detection on axis z

	// Free fall detection configuration
	lsm6ds3tr_c_ff_dur_set(&accelerometer_ctx, 6); // 28 ms aprox
	lsm6ds3tr_c_ff_threshold_set(&accelerometer_ctx, LSM6DS3TR_C_FF_TSH_312mg);

	// set interrupts
	int1.int1_double_tap = 0;	// double tap
	int1.int1_ff = 1;			// free fall
	int1.int1_wu = 1;			// wake up
	int1.int1_single_tap = 1;	// single tap
	lsm6ds3tr_c_pin_int1_route_set(&accelerometer_ctx, int1);
}

void accelGetXYZSampleRAW(accelSampleRAW_t* accelerationSample)
{
	lsm6ds3tr_c_acceleration_raw_get(&accelerometer_ctx, (int16_t*)accelerationSample);
}

void gyroGetXYZSampleRAW(angularRateSampleRAW_t* angularRateSample)
{
	lsm6ds3tr_c_angular_rate_raw_get(&accelerometer_ctx, (int16_t*) angularRateSample);
}

void accelGetXYZSampleG(accelSampleG_t* accelerationSample)
{
	accelSampleRAW_t sampleRAW;
	accelScale_t scale;

	scale = accellGetScale();
	accelGetXYZSampleRAW(&sampleRAW);

	switch(scale)
	{
		case ACCEL_SCALE_2g:
			accelerationSample->X = (double)sampleRAW.X * 0.000061;
			accelerationSample->Y = (double)sampleRAW.Y * 0.000061;
			accelerationSample->Z = (double)sampleRAW.Z * 0.000061;
			break;
		case ACCEL_SCALE_4g:
			accelerationSample->X = (double)sampleRAW.X * 0.000122;
			accelerationSample->Y = (double)sampleRAW.Y * 0.000122;
			accelerationSample->Z = (double)sampleRAW.Z * 0.000122;
			break;
		case ACCEL_SCALE_8g:
			accelerationSample->X = (double)sampleRAW.X * 0.000244;
			accelerationSample->Y = (double)sampleRAW.Y * 0.000244;
			accelerationSample->Z = (double)sampleRAW.Z * 0.000244;
			break;
		case ACCEL_SCALE_16g:
			accelerationSample->X = (double)sampleRAW.X * 0.000488;
			accelerationSample->Y = (double)sampleRAW.Y * 0.000488;
			accelerationSample->Z = (double)sampleRAW.Z * 0.000488;
			break;
	}
}

void gyroGetXYZSampleDps(angularRateSampleDps_t* angularRateSample)
{
	angularRateSampleRAW_t sampleRAW;
	gyroScale_t scale;

	scale = gyroGetScale();
	gyroGetXYZSampleRAW(&sampleRAW);

	switch(scale)
	{
		case GYRO_SCALE_125dps:
			angularRateSample->Xpitch = (double)sampleRAW.Xpitch * 0.0004375;
			angularRateSample->Yroll = (double)sampleRAW.Yroll * 0.0004375;
			angularRateSample->Zyaw = (double)sampleRAW.Zyaw * 0.0004375;
			break;
		case GYRO_SCALE_250dps:
			angularRateSample->Xpitch = (double)sampleRAW.Xpitch * 0.00875;
			angularRateSample->Yroll = (double)sampleRAW.Yroll * 0.00875;
			angularRateSample->Zyaw = (double)sampleRAW.Zyaw * 0.00875;
			break;
		case GYRO_SCALE_500dps:
			angularRateSample->Xpitch = (double)sampleRAW.Xpitch * 0.0175;
			angularRateSample->Yroll = (double)sampleRAW.Yroll * 0.0175;
			angularRateSample->Zyaw = (double)sampleRAW.Zyaw * 0.0175;
			break;
		case GYRO_SCALE_1000dps:
			angularRateSample->Xpitch = (double)sampleRAW.Xpitch * 0.035;
			angularRateSample->Yroll = (double)sampleRAW.Yroll * 0.035;
			angularRateSample->Zyaw = (double)sampleRAW.Zyaw * 0.035;
			break;
		case GYRO_SCALE_2000dps:
			angularRateSample->Xpitch = (double)sampleRAW.Xpitch * 0.07;
			angularRateSample->Yroll = (double)sampleRAW.Yroll * 0.07;
			angularRateSample->Zyaw = (double)sampleRAW.Zyaw * 0.07;
			break;
	}
}

void accelGyroGetAllInterruptSources(accelGyroInterruptStatus_t* accelGyroInterruptStatus)
{
	lsm6ds3tr_c_all_sources_t lsm6ds3tr_c_all_sources;

	lsm6ds3tr_c_all_sources_get(&accelerometer_ctx, &lsm6ds3tr_c_all_sources);

	accelGyroInterruptStatus->tnda = lsm6ds3tr_c_all_sources.status_reg.tda; // temperature new data available
	accelGyroInterruptStatus->gnda = lsm6ds3tr_c_all_sources.status_reg.gda; // gyroscope new data available
	accelGyroInterruptStatus->xlnda = lsm6ds3tr_c_all_sources.status_reg.xlda; // accelerometer new data available
	accelGyroInterruptStatus->singleTap = lsm6ds3tr_c_all_sources.tap_src.tap_sign; // single tap event
	accelGyroInterruptStatus->doubleTap = lsm6ds3tr_c_all_sources.tap_src.double_tap; // double tap event
	accelGyroInterruptStatus->freeFall = lsm6ds3tr_c_all_sources.wake_up_src.ff_ia; // free fall
	accelGyroInterruptStatus->wakeUp = lsm6ds3tr_c_all_sources.wake_up_src.wu_ia; // wake-up
}

int32_t accelGetTemperatureRAW(void)
{
	int16_t result;

	lsm6ds3tr_c_temperature_raw_get(&accelerometer_ctx, &result);

	return (int32_t) result;
}

double accelGetTemperatureC(void)
{
	int16_t result;

	lsm6ds3tr_c_temperature_raw_get(&accelerometer_ctx, &result);

	return (double) result/256;
}

accelScale_t accellGetScale(void)
{
	lsm6ds3tr_c_fs_xl_t scale;

	lsm6ds3tr_c_xl_full_scale_get(&accelerometer_ctx, &scale);

	return scale;
}

gyroScale_t gyroGetScale(void)
{
	lsm6ds3tr_c_fs_g_t scale;

	lsm6ds3tr_c_gy_full_scale_get(&accelerometer_ctx, &scale);

	return scale;
}

uint8_t accelGyroGetDeviceID(void)
{
	uint8_t result;

	lsm6ds3tr_c_device_id_get(&accelerometer_ctx, &result);

	return result;
}

static int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{
	int32_t result;

    write_buf[0] = Reg;
    memcpy(&write_buf[1], Bufp, len);
    result = i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, (LSM6DS3TR_C_I2C_ADD_L >> 1), write_buf, len + 1, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);

	return result;
}

static int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
	return i2cPeripheralMasterWriteReadDevice(I2C_0_MASTER_NUM, (LSM6DS3TR_C_I2C_ADD_L  >> 1), &Reg, 1, Bufp, len, I2C_0_MASTER_TIMEOUT_MS / portTICK_RATE_MS, 1);
}
