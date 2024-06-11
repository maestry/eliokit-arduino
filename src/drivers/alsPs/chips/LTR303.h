/*
 * LTR303.h
 *
 *  Created on: Aug 8, 2022
 *      Author: marcelo
 */

#pragma once

#define LTR303_ADDR 0x29 // default address

// LTR303 register addresses
#define LTR303_CONTR 0x80
#define LTR303_MEAS_RATE 0x85
#define LTR303_PART_ID 0x86
#define LTR303_MANUFAC_ID 0x87
#define LTR303_DATA_CH1_0 0x88
#define LTR303_DATA_CH1_1 0x89
#define LTR303_DATA_CH0_0 0x8A
#define LTR303_DATA_CH0_1 0x8B
#define LTR303_STATUS 0x8C
#define LTR303_INTERRUPT 0x8F
#define LTR303_THRES_UP_0 0x97
#define LTR303_THRES_UP_1 0x98
#define LTR303_THRES_LOW_0 0x99
#define LTR303_THRES_LOW_1 0x9A
#define LTR303_INTR_PERS 0x9E
