#pragma once

#include <stdio.h>

void BS8112A_3InitSettingsToWrite(void);
int32_t BS8112A_3WriteSettings(void);
int32_t BS8112A_3ReadKeysThreshold(uint8_t *kth);
int32_t BS8112A_3ReadKeyThreshold(uint8_t kNumber, uint8_t *kth);
void BS8112A_3SetKeyThreshold(uint8_t kNumber, uint8_t kth);
int32_t BS8112A_3ReadIRQ_OMS(uint8_t *irq_oms);
void BS8112A_3SetIRQ_OMS(uint8_t irq_oms);
int32_t BS8112A_3ReadKey12Mode(uint8_t *key12Mode);
void BS8112A_3SetKey12Mode(uint8_t key12Mode);
int32_t BS8112A_3ReadLSC(uint8_t *lsc);
void BS8112A_3SetLSC(uint8_t lsc);
int32_t BS8112A_3ReadKeysStatus(uint8_t *kStatus);
