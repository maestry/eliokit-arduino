#include "spiPeripheral.h"



SemaphoreHandle_t 	spiPeripheralMutex = NULL;



void spiPeripheralInit(void)
{
	spiPeripheralMutex = xSemaphoreCreateMutex();
}

void spiPeripheralMasterTakeMutex()
{
	xSemaphoreTake(spiPeripheralMutex, portMAX_DELAY);
}

void spiPeripheralMasterGiveMutex()
{
	xSemaphoreGive(spiPeripheralMutex);
}
