/*
 * eInkDisplay.h
 *
 *  Created on: Jul 13, 2022
 *      Author: marcelo
 */

#pragma once


#include <stdlib.h>
//#include "chips/waveshare_1in54b2.h"
//#include "chips/GDEW0154M09.h"
#include "chips/GDEY0154D67.h"

void eInkDisplayInit(eInkGDEY0154D67DisplayHandler_t* eInkDisplay);
void eInkDisplayPrintStartImage(void);
void eInkDisplayPrintWhiteScreen(void);
