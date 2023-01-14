/*
 * i2c.h
 *
 *  Created on: Jul 16, 2022
 *      Author: marcelo
 */

#pragma once

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdio.h>
#include <string.h>
#include "../boards/s3_bsp.h"

extern SemaphoreHandle_t i2c0PeripheralMutex;
extern SemaphoreHandle_t i2c1PeripheralMutex;

void i2cPeripheralInitCustomSpeed(uint8_t i2cPort, uint32_t speed);
void i2cPeripheralMasterTakeMutex(uint8_t i2cPort);
void i2cPeripheralMasterGiveMutex(uint8_t i2cPort);

int32_t i2cPeripheralMasterWriteToDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t* write_buffer, size_t write_size, TickType_t ticks_to_wait, uint8_t useMutex);
int32_t i2cPeripheralMasterReadDevice(i2c_port_t i2c_num, uint8_t device_address, uint8_t* read_buffer, size_t read_size, TickType_t ticks_to_wait, uint8_t useMutex);
int32_t i2cPeripheralMasterWriteReadDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t* write_buffer, size_t write_size,
                                       	   uint8_t* read_buffer, size_t read_size, TickType_t ticks_to_wait, uint8_t useMutex);
int32_t i2cPeripheralMasterWriteStopDelayReadDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t* write_buffer, size_t write_size,
                                       uint8_t* read_buffer, size_t read_size, TickType_t ticks_to_wait, uint32_t delayMS, uint8_t useMutex);
esp_err_t i2cPeripheralMasterWriteByte(uint8_t i2c_num, uint8_t deviceaddress, uint8_t eeaddress, uint8_t byte, uint8_t useMutex);
esp_err_t eeprom_write(uint8_t i2c_num, uint8_t deviceaddress, uint16_t eeaddress, uint8_t* data, size_t size, uint8_t useMutex);
uint8_t i2cPeripheralMasterReadByte(uint8_t i2c_num, uint8_t deviceaddress, uint8_t address, uint8_t useMutex);
esp_err_t eeprom_read(uint8_t i2c_num, uint8_t deviceaddress, uint16_t eeaddress, uint8_t* data, size_t size, uint8_t useMutex);

esp_err_t i2c_master_read_slave(uint8_t i2c_num, uint8_t deviceaddress, uint8_t regaddress, uint8_t* data_rd, size_t size, uint8_t useMutex);
esp_err_t i2c_master_write_slave(uint8_t i2c_num, uint8_t deviceaddress, uint8_t ctrl,  uint8_t* data_wr, size_t size, uint8_t useMutex);

esp_err_t i2cPeripheralTestAddressPresence(uint8_t i2c_num, uint8_t address, uint8_t useMutex);

esp_err_t i2cMasterWriteDelayBetweenBytes(uint8_t i2c_num, uint8_t deviceaddress, uint8_t* data_wr, size_t size, uint8_t useMutex);
esp_err_t i2cMasterReadDelayBetweenBytes(uint8_t i2c_num, uint8_t deviceaddress, uint8_t regaddress, uint8_t* data_rd, size_t size, uint8_t useMutex);


