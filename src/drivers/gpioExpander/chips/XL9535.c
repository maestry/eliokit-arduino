#include "XL9535.h"
#include <math.h>
#include "../../i2c/i2cPeripheral.h"

#define READ_BUF_LENGTH 16
#define WRITE_BUF_LENGTH 16
#define XL9335_ADD 0x20

#define XL9335_READ_PORT0 0x00
#define XL9335_READ_PORT1 0x01
#define XL9335_WRITE_PORT0 0x02
#define XL9335_WRITE_PORT1 0x03
#define XL9335_INVERT_PORT0 0x04
#define XL9335_INVERT_PORT1 0x05
#define XL9335_CONFIG_PORT0 0x06
#define XL9335_CONFIG_PORT1 0x07

static uint8_t readBuf[READ_BUF_LENGTH];
static uint8_t writeBuf[WRITE_BUF_LENGTH];

void XL9335ReadPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value)
{
	if (port)
		writeBuf[0] = XL9335_READ_PORT1;
	else
		writeBuf[0] = XL9335_READ_PORT0;

	i2c_master_read_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], value, 1, 1);
}

void XL9335ReadPortConfig(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value)
{
	if (port)
		writeBuf[0] = XL9335_CONFIG_PORT1;
	else
		writeBuf[0] = XL9335_CONFIG_PORT0;

	i2c_master_read_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], value, 1, 1);
}

void XL9335ReadPortPolarity(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t *value)
{
	if (port)
		writeBuf[0] = XL9335_INVERT_PORT1;
	else
		writeBuf[0] = XL9335_INVERT_PORT0;

	i2c_master_read_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], value, 1, 1);
}

void XL9335WritePort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value)
{
	if (port)
		writeBuf[0] = XL9335_WRITE_PORT1;
	else
		writeBuf[0] = XL9335_WRITE_PORT0;
	writeBuf[1] = value;

	i2c_master_write_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], &writeBuf[1], 1, 1);
}

void XL9335WritePortPin(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t bit, uint8_t value)
{
	uint8_t currentValue;

	XL9335ReadPort(i2c_num, chipID, port, &currentValue);

	if (port)
		writeBuf[0] = XL9335_WRITE_PORT1;
	else
		writeBuf[0] = XL9335_WRITE_PORT0;

	if (value)
		currentValue |= 1 << bit;
	else
		currentValue &= ~(1 << bit);
	writeBuf[1] = currentValue;

	// printf("write value %d to bit %d to port %d, so writebuf is %d \n", value, bit, port, writeBuf[1]);

	XL9335WritePort(i2c_num, chipID, port, writeBuf[1]);

	// i2c_master_write_slave(I2C_0_MASTER_NUM, XL9335_ADD + chipID, writeBuf[0], &currentValue, 1, 1);

	// i2cPeripheralMasterWriteToDevice(I2C_0_MASTER_NUM, XL9335_ADD + chipID, writeBuf, 2, 2000 / portTICK_RATE_MS, 1);
}

void XL9335InvertPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value)
{
	if (port)
		writeBuf[0] = XL9335_INVERT_PORT1;
	else
		writeBuf[0] = XL9335_INVERT_PORT0;
	writeBuf[1] = value;

	i2c_master_write_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], &writeBuf[1], 1, 1);
}

void XL9335ConfigPort(uint8_t i2c_num, uint8_t chipID, uint8_t port, uint8_t value)
{
	if (port)
		writeBuf[0] = XL9335_CONFIG_PORT1;
	else
		writeBuf[0] = XL9335_CONFIG_PORT0;
	writeBuf[1] = value;

	i2c_master_write_slave(i2c_num, XL9335_ADD + chipID, writeBuf[0], &writeBuf[1], 1, 1);
}
