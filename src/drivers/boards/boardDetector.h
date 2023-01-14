#pragma once

#include <stdlib.h>

uint8_t boardDetectorDetectMediaBoard(void);
uint8_t boardDetectorDetectIOBoards(void);
uint8_t boardDetectorDetectS3Board(void);
uint8_t boardI2CScan(uint8_t i2cNum);
