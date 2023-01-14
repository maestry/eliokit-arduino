/*
 * SHT40.h
 *
 *  Created on: Nov 23, 2022
 *      Author: marcelo
 */

#pragma once

#include <stdint.h>

void SHT40StartMeasure(void);
void SHT40GetMeasure(double* temperature, double* humidity);
