

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------
// ----------------------------------------------------------
// --------------------- S3 BOARD-------------------------

#define SYSTEM_BEEP_FREQ    							2000
#define SYSTEM_BEEP_ON_TIME_MS							1000

#define SYSTEM_FACTORY_RESET_BTN_PRESSED_MIN_TIME		15000
#define SYSTEM_WIFI_CONFIGURATION_BTN_PRESSED_MIN_TIME	5000
#define SYSTEM_SHORT_BTN_PRESSED_MAX_TIME				2000
#define SYSTEM_SHORT_BTN_PRESSED_MIN_TIME				200

// ----------------------------------------------------------
// I2C BUS DEF
#define I2C_0_MASTER_NUM              	0
#define I2C_0_MASTER_FREQ_HZ          	10000 // because tvoc sensor I2C is slow
#define I2C_0_MASTER_TIMEOUT_MS			20
#define I2C_1_MASTER_NUM              	1
#define I2C_1_MASTER_FREQ_HZ          	10000
// ----------------------------------------------------------
// Analog inputs
#define GPIO_ADC_CH_0	     					1
// ----------------------------------------------------------
// BOOT CFG BTN
#define GPIO_INPUT_BOOT_BTN_CFG    					0
// ----------------------------------------------------------

// ---------------- GPIO INT input -------------------------
#define GPIO_INPUT_GPIO_INT_INTERRUPT    			38

// --------------------- IR TX --------------------------
#define IR_RTM_TX_CHANNEL 							RMT_CHANNEL_1
#define GPIO_IR_TX									45

// --------------------- IR RX --------------------------
#define IR_RMT_RX_CHANNEL 							RMT_CHANNEL_4
#define GPIO_IR_RX									16

// ----------------------------------------------------------
// BUZZER
#define GPIO_OUTPUT_BUZZER			     			46
#define BUZZER_LEDC_TIMER							LEDC_TIMER_1
#define BUZZER_LEDC_CHANNEL							LEDC_CHANNEL_1
// ----------------------------------------------------------
//
// ---------------- RGB LED chain output --------------------
#define RMT_OUTPUT_LED_CHAIN		     			3
#define LED_RMT_CHANNEL								RMT_CHANNEL_0
// ----------------------------------------------------------
//
// ------------ Accelerometer digital input -----------------
//#define GPIO_INPUT_ACCEL_INT1_INT2    			48

// ----------------------------------------------------------
//
// ------------- Charge status digital input ----------------
//#define GPIO_INPUT_CHARGE_STATUS 				37
// ----------------------------------------------------------
//
// ------------- VIN CHG status digital input ----------------
//#define GPIO_INPUT_VIN_CHG_STATUS 				33
// ----------------------------------------------------------
//
// ----------------------------------------------------------
// I2C BUS PINS
#define GPIO_I2C_1_BUS_SDA			 				4
#define GPIO_I2C_1_BUS_SCL			 				5
#define GPIO_I2C_2_BUS_SDA			 				44
#define GPIO_I2C_2_BUS_SCL			 				2
// ----------------------------------------------------------
//
// ----------------------------------------------------------
// SPI BUS PINS
#define GPIO_SPIBUS_MOSI		 				39
#define GPIO_SPIBUS_MISO						41
#define GPIO_SPIBUS_CLK							40
// ----------------------------------------------------------
//
// ------------------ OV2640 Camera input -------------------
#define CAM_PIN_PWDN    	-1 //power down is not used
#define CAM_PIN_RESET   	-1 //software reset will be performed
#define CAM_PIN_XCLK    	14
#define CAM_PIN_SIOD    	-1
#define CAM_PIN_SIOC    	-1

#define CAM_PIN_D7      	21
#define CAM_PIN_D6      	13
#define CAM_PIN_D5      	12
#define CAM_PIN_D4      	10
#define CAM_PIN_D3      	17
#define CAM_PIN_D2      	8
#define CAM_PIN_D1      	18
#define CAM_PIN_D0      	9
#define CAM_PIN_VSYNC   	48
#define CAM_PIN_HREF    	47
#define CAM_PIN_PCLK    	11

#define CAM_LEDC_TIMER	 	LEDC_TIMER_0
#define CAM_LEDC_CHANNEL 	LEDC_CHANNEL_0


extern SemaphoreHandle_t	boardPhysics;

#ifdef __cplusplus
}
#endif
