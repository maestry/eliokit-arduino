/*
 * humTemp.c
 *
 *  Created on: Nov 23, 2022
 *      Author: marcelo
 */

#include "humTemp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "chips/SHT40.h"

void humTempGetMeasure(double *temperature, double *humidity)
{
	SHT40StartMeasure();
	vTaskDelay(20 / portTICK_RATE_MS);
	SHT40GetMeasure(temperature, humidity);
}
