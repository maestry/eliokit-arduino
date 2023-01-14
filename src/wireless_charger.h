/*
 * wireless_charger.h
 *
 *  Created on: Nov 22, 2022
 *      Author: marcelo
 */

#pragma once

#include <string.h>

typedef enum
{
	WC_NOT_PRESENT_IN_I2C_BUS,
	WC_I2C_ALREADY_CONFIGURATED,
	WC_I2C_CONFIGURATION_WRITTEN_OK,
	WC_I2C_CONFIGURATION_WRITING_ERROR
} wirelessChargerApplyConfigurationResult_t;

uint8_t wirelessChargerApplyConfiguration(void);
