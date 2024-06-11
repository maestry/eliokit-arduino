#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <string.h>

extern SemaphoreHandle_t spiPeripheralMutex;

void spiPeripheralInit(void);
void spiPeripheralMasterTakeMutex();
void spiPeripheralMasterGiveMutex();
