/*
 * lightSensor.h
 *
 *  Created on: Aug 8, 2022
 *      Author: marcelo
 */

#pragma once

#include <stdlib.h>

void ALSPSInit(void);
void ALSPSGetLightValues(double *visiblePlusIR, double *IR);
void ALSPSGetPSValue(double *ps);
void ALSPSCheckInterrupt(double *visiblePlusIR, double *IR, uint8_t *ALSDataValid, double *ps, uint8_t *PSDataValid);
void ALSPSEnableALS(void);
void ALSPSDisableALS(void);
void ALSPSEnablePS(void);
void ALSPSDisablePS(void);
