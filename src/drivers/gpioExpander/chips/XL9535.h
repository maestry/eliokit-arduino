#pragma once

#include <string.h>

void XL9335ReadPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value);
void XL9335ReadPortConfig(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value);
void XL9335ReadPortPolarity(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value);

void XL9335WritePort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value);
void XL9335WritePortPin(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t bit, uint8_t value);
void XL9335InvertPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value);
void XL9335ConfigPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value);
