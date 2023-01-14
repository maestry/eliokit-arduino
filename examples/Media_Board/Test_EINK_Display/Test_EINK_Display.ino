#include "driver/gpio.h"
#include "image.h"


#include "eliokit.h"

eInkDisplayHandler_t eInkDisplay;

#define EINK_DISPLAY_OUTPUT_SCK		 		GPIO_SPIBUS_CLK
#define EINK_DISPLAY_OUTPUT_SDI		 		GPIO_SPIBUS_MOSI

// IO boards
#define XL9335_ADD						0x20

// Media board
#define BS8112A_3_ADDRESS				0x50

void setup() {
  // put your setup code here, to run once:
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  //gpioExpanderInit();
  spiPeripheralInit();
  initializeEINKgpios();
  
  	// Init eink
  printf("End \n");
  

	eInkDisplay.delayMS = OSDelayMS;
	eInkDisplay.writeEINKCLK = writeEINKCLK;
	eInkDisplay.writeEINKMOSI = writeEINKMOSI;
	eInkDisplay.writeEINKCS = gpioExpanderWriteEINKCS;
	eInkDisplay.writeEINKRST = gpioExpanderWriteEINKRST;
	eInkDisplay.writeEINKDC = gpioExpanderWriteEINKDC;
	eInkDisplay.readEINKBUSY = gpioExpanderReadEINKBUSY;
  eInkDisplayInit(&eInkDisplay);


}

void loop() {
  // put your main code here, to run repeatedly:

    eInkDisplayPrintStartImage();
    printf("Test \n");
    boardDetectorDetectMediaBoard();
    delay(5000);
    eInkDisplayPrintImage(gImage_1);
    delay(5000);
    eInkDisplayPrintImage(gImage_2);
    delay(5000);
    eInkDisplayPrintImage(gImage_6);
    delay(5000);

}

static void OSDelayMS(uint32_t delayMS)
{
	vTaskDelay(delayMS / portTICK_RATE_MS);
}

static void writeEINKCLK(uint8_t value)
{
	gpio_set_level((gpio_num_t)EINK_DISPLAY_OUTPUT_SCK, value);
}

static void writeEINKMOSI(uint8_t value)
{
	gpio_set_level((gpio_num_t)EINK_DISPLAY_OUTPUT_SDI, value);
}

static void initializeEINKgpios(void)
{
	gpioExpanderConfigForMediaBoard();

	gpio_config_t io_conf = { };
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = ((1ULL<<EINK_DISPLAY_OUTPUT_SCK) | (1ULL<<EINK_DISPLAY_OUTPUT_SDI));
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	gpio_config(&io_conf);

	gpio_set_level((gpio_num_t)EINK_DISPLAY_OUTPUT_SDI, 0);
	gpio_set_level((gpio_num_t)EINK_DISPLAY_OUTPUT_SCK, 1);
}

uint8_t boardDetectorDetectMediaBoard(void)
{
	uint8_t result = 0;

	if (0 == i2cPeripheralTestAddressPresence(I2C_1_MASTER_NUM, BS8112A_3_ADDRESS, 1))
	{
		printf("Media board detected \n");
		result = 1;
	}


	return result;
}