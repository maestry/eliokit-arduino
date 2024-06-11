/*
 * touchButton.c
 *
 *  Created on: Jul 16, 2022
 *      Author: marcelo
 */

#include "touchButton.h"
#include "chips/BS8112A-3.h"
#include <string.h>
#include "driver/i2c.h"
#include "drivers/boards/s3_bsp.h"

void touchButtonInit(void)
{
	uint8_t kth[11];
	uint8_t key12Mode;
	uint8_t irqOMS;
	int32_t err;

	BS8112A_3InitSettingsToWrite();

	err = BS8112A_3WriteSettings();
	printf("BS8112A_3WriteSettings err: %d \n", err); // err 263

	vTaskDelay(1000 / portTICK_RATE_MS);

	err = BS8112A_3ReadKeysThreshold(kth);
	printf("BS8112A_3ReadKeysThreshold err: %d \n", err);
	for (int i = 0; i < 11; i++)
	{
		printf("Key threshold: %d  %d \n", i, kth[i]);
	}

	vTaskDelay(1000 / portTICK_RATE_MS);

	err = BS8112A_3ReadKeysThreshold(kth);
	printf("BS8112A_3ReadKeysThreshold err: %d \n", err);
	for (int i = 0; i < 11; i++)
	{
		printf("Key threshold: %d  %d \n", i, kth[i]);
	}

	/*
		BS8112A_3ReadKeyThreshold(1, &kth);
		printf("Key threshold: %d  %d \n", 1, kth);

		BS8112A_3ReadKeyThreshold(2, &kth);
		printf("Key threshold: %d  %d \n", 2, kth);

		BS8112A_3ReadKeyThreshold(3, &kth);
		printf("Key threshold: %d  %d \n", 3, kth);

		BS8112A_3ReadKeyThreshold(4, &kth);
		printf("Key threshold: %d  %d \n", 4, kth);

		BS8112A_3ReadKeyThreshold(5, &kth);
		printf("Key threshold: %d  %d \n", 5, kth);

		BS8112A_3ReadKeyThreshold(6, &kth);
		printf("Key threshold: %d  %d \n", 6, kth);

		BS8112A_3ReadKeyThreshold(7, &kth);
		printf("Key threshold: %d  %d \n", 7, kth);

		BS8112A_3ReadKeyThreshold(8, &kth);
		printf("Key threshold: %d  %d \n", 8, kth);

		BS8112A_3ReadKeyThreshold(9, &kth);
		printf("Key threshold: %d  %d \n", 9, kth);

	*/

	vTaskDelay(1000 / portTICK_RATE_MS);

	err = BS8112A_3ReadKey12Mode(&key12Mode);
	printf("BS8112A_3ReadKey12Mode err: %d \n", err);
	printf("Key 12 mode: %d \n", key12Mode);

	vTaskDelay(1000 / portTICK_RATE_MS);

	err = BS8112A_3ReadIRQ_OMS(&irqOMS);
	printf("BS8112A_3ReadIRQ_OMS err: %d \n", err);
	printf("IRQ OMS: %d \n", irqOMS);

	vTaskDelay(1000 / portTICK_RATE_MS);

	err = BS8112A_3ReadIRQ_OMS(&irqOMS);
	printf("BS8112A_3ReadIRQ_OMS err: %d \n", err);
	printf("IRQ OMS: %d \n", irqOMS);
}

int32_t touchButtonReadStatus(uint8_t *kStatus)
{
	return BS8112A_3ReadKeysStatus(kStatus);
}
