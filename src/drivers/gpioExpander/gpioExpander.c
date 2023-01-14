#include "gpioExpander.h"
#include "chips/XL9535.h"
#include "drivers/boards/s3_bsp.h"
#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define GPIO_EXPANDER_PORT0_DEFAULT_CONFIG	0xFA // 11111010
#define GPIO_EXPANDER_PORT0_DEFAULT_OUT		0x01
#define GPIO_EXPANDER_PORT1_DEFAULT_CONFIG	0x3F // 00111111
#define GPIO_EXPANDER_PORT1_DEFAULT_OUT		0x80

#define ESP_INTR_FLAG_DEFAULT 					0

static SemaphoreHandle_t 						analogMutex;
static SemaphoreHandle_t 						irMutex;
static SemaphoreHandle_t 						ledsVibMutex;


void gpioExpanderInit(void)
{
	analogMutex = xSemaphoreCreateMutex();
	irMutex = xSemaphoreCreateMutex();
	ledsVibMutex = xSemaphoreCreateMutex();

	// Configure gpio expander of S3 board
	configGpioExpander(I2C_0_MASTER_NUM, 0, 0, GPIO_EXPANDER_PORT0_DEFAULT_CONFIG, 0, GPIO_EXPANDER_PORT0_DEFAULT_OUT);
	configGpioExpander(I2C_0_MASTER_NUM, 0, 1, GPIO_EXPANDER_PORT1_DEFAULT_CONFIG, 0, GPIO_EXPANDER_PORT1_DEFAULT_OUT);
}

void configGpioExpander(uint8_t i2cPort, uint8_t chipID, uint8_t port, uint8_t configuration, uint8_t invertConfiguration, uint8_t writeValue)
{
	XL9335ConfigPort(i2cPort, chipID, port, configuration);
	XL9335InvertPort(i2cPort, chipID, port, invertConfiguration);
	XL9335WritePort(i2cPort, chipID, port, writeValue);
}

void gpioExpanderReadWCST(uint8_t* wcst0, uint8_t* wcst1)
{
	uint8_t value;

	XL9335ReadPort(I2C_0_MASTER_NUM,0, 1, &value);

	if(wcst1 != NULL)
	{
		if (value & 0x01)
			*wcst1 = 1;
		else
			*wcst1 = 0;
	}

	if(wcst0 != NULL)
	{
		if (value & 0x02)
			*wcst0 = 1;
		else
			*wcst0 = 0;
	}
}

void gpioExpanderWriteSPICS(uint8_t value)
{
	XL9335WritePortPin(I2C_0_MASTER_NUM, 0, 0, 0, value);
}

uint8_t gpioExpanderReadVbusDet(void)
{
	uint8_t value = 0;

	XL9335ReadPort(I2C_0_MASTER_NUM, 0, 0, &value);

	if (value & 0x08) // bit 3 port 0
		value = 1;
	else
		value = 0;

	return value;
}

uint8_t gpioExpanderReadVsunDet(void)
{
	uint8_t value = 0;

	XL9335ReadPort(I2C_0_MASTER_NUM, 0, 0, &value);

	if (value & 0x02) // bit 1 port 0
		value = 1;
	else
		value = 0;

	return value;
}

// ------------ ANALOG_SELECT -----------------
void gpioExpanderSelectAnalogVBAT(void)
{
	xSemaphoreTake(analogMutex, portMAX_DELAY);
	XL9335WritePortPin(I2C_0_MASTER_NUM,0, 0, 2, 0);
}

void gpioExpanderDeselectAnalogVBAT(void)
{
	xSemaphoreGive(analogMutex);
}

void gpioExpanderSelectAnIn(void)
{
	xSemaphoreTake(analogMutex, portMAX_DELAY);
	XL9335WritePortPin(I2C_0_MASTER_NUM,0, 0, 2, 1);
}

void gpioExpanderDeselectAnIn(void)
{
	xSemaphoreGive(analogMutex);
}
// ------------------------------------------------

// ------------ VIBRATION MOTOR -----------------
void gpioExpanderSetVibrationMotorOn(void)
{
	XL9335WritePortPin(I2C_0_MASTER_NUM, 0, 1, 6, 1);
}

void gpioExpanderSetVibrationMotorOff(void)
{
	XL9335WritePortPin(I2C_0_MASTER_NUM, 0, 1, 6, 0);
}
// ------------------------------------------------

// ------------ STATUS LED -----------------
void gpioExpanderSetStatusLedOn(void)
{
	XL9335WritePortPin(I2C_0_MASTER_NUM, 0, 1, 7, 0);
}

void gpioExpanderSetStatusLedOff(void)
{
	XL9335WritePortPin(I2C_0_MASTER_NUM, 0, 1, 7, 1);
}
// ------------------------------------------------
