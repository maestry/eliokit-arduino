/*
 * tvoc.c
 *
 *  Created on: Dec 2, 2022
 *      Author: marcelo
 */


#include "tvoc.h"
#include "chips/ags10.h"

void tvocGetPPB(double* tvocPPB, uint8_t* dataValid)
{
	uint32_t tvoc;
	uint8_t preHeatStage;
	uint8_t crcError;

	AGS10getTVOCppb(&tvoc, &preHeatStage, &crcError);

	if(!preHeatStage && !crcError)
	{
		if(dataValid != NULL)
			*dataValid = 1;
	}else{
		if(dataValid != NULL)
			*dataValid = 0;
	}

	*tvocPPB = (double) tvoc;
}
