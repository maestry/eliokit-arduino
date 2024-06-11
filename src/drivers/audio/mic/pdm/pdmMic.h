#pragma once

#include <stdio.h>

void pdmMicInit(void);
void pdmMicRead(void *dest, size_t size, size_t *bytes_read);
