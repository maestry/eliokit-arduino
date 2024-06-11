/*
 * touchButton.h
 *
 *  Created on: Jul 16, 2022
 *      Author: marcelo
 */

#pragma once

#include <stdlib.h>

void touchButtonInit(void);
int32_t touchButtonReadStatus(uint8_t *kStatus);
