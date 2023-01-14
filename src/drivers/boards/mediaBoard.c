#include "mediaBoard.h"
#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "drivers/eInkDisplay/eInkDisplay.h"
#include "drivers/touchButtonI2C/touchButton.h"
#include "drivers/audio/amplifier/I2s/i2sout.h"
#include "drivers/audio/mic/pdm/pdmMic.h"
#include "drivers/boards/s3_bsp.h"
#include "drivers/gpioExpander/gpioExpander.h"

static void OSDelayMS(uint32_t delayMS);
static void writeEINKCLK(uint8_t value);
static void writeEINKMOSI(uint8_t value);
static void writeEINKCS(uint8_t value);
static void writeEINKDC(uint8_t value);
static void writeEINKRST(uint8_t value);
static uint8_t readEINKBUSY(void);
static void initializeEINKgpios(void);

static uint8_t keysStatus[11];


uint8_t mediaBoardInitialize(void)
{
	uint8_t 							result = 0;
	eInkGDEY0154D67DisplayHandler_t 	eInkDisplay;

	// Expander Config For Media Board, initialize gpios
	initializeEINKgpios();

	// Init touch buttons
	touchButtonInit();

	// Init PDM MIC
	//pdmMicInit();

	// Init I2S OUT
	//i2sOutInit();

	// Init eink
	eInkDisplay.delayMS = OSDelayMS;
	eInkDisplay.writeCLK = writeEINKCLK;
	eInkDisplay.writeMOSI = writeEINKMOSI;
	eInkDisplay.writeCS = writeEINKCS;
	eInkDisplay.writeRST = writeEINKRST;
	eInkDisplay.writeDC = writeEINKDC;
	eInkDisplay.readBUSY = readEINKBUSY;

	eInkDisplayInit(&eInkDisplay);
	eInkDisplayPrintStartImage();

	printf("Media Board Initialized \n");

	return result;
}





void mediaBoardInterruptHandler(void)
{
/*
	touchButtonReadStatus(keysStatus);

	for(int i = 0; i < 11; i++)
	{
		ets_printf("Touch Key %d value: %d \n", i, keysStatus[i]);
	}
*/

}





static void initializeEINKgpios(void)
{
	//gpioExpanderConfigForMediaBoard();
/*
	gpio_config_t io_conf = { };
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = ((1ULL<<EINK_DISPLAY_OUTPUT_SCK) | (1ULL<<EINK_DISPLAY_OUTPUT_SDI));
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	gpio_config(&io_conf);

	gpio_set_level(EINK_DISPLAY_OUTPUT_SDI, 0);
	gpio_set_level(EINK_DISPLAY_OUTPUT_SCK, 1);
	*/
}

static void OSDelayMS(uint32_t delayMS)
{
	vTaskDelay(delayMS / portTICK_RATE_MS);
}

static void writeEINKCLK(uint8_t value)
{
	gpio_set_level(EINK_DISPLAY_OUTPUT_SCK, value);
}

static void writeEINKMOSI(uint8_t value)
{
	gpio_set_level(EINK_DISPLAY_OUTPUT_SDI, value);
}

static void writeEINKCS(uint8_t value)
{
	//gpioExpanderWriteEINKCS(value);
}

static void writeEINKDC(uint8_t value)
{
	///gpioExpanderWriteEINKDC(value);
}

static void writeEINKRST(uint8_t value)
{
	//gpioExpanderWriteEINKRST(value);
}

static uint8_t readEINKBUSY(void)
{
	return 0;//gpioExpanderReadEINKBUSY();
}
