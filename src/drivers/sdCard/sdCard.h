#pragma once

#include <string.h>

uint8_t sdCardBusInit(void);
uint8_t sdCardMountFileSystem(void);
void sdCardUnmountFileSystem(void);
uint8_t sdCardCreateWriteCloseFile(char *folder, char *name, uint8_t *data, uint32_t dataLength);
uint8_t sdCardAppendCloseFile(char *folder, char *name, uint8_t *data, uint32_t dataLength);
