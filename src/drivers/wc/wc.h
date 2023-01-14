#pragma once

#include <stdint.h>

void wcWrite256BytesConfigurationToChip(void);
void wcRead256BytesConfigurationToChip(uint8_t* readBuffer);

void wcGetStatus(uint8_t* st0, uint8_t* st1);
uint8_t wcIsConnectedToI2C(void);
uint8_t wcReadAndVerifyConfiguration(void);
