#include "sdCard.h"

#include "drivers/spi/spiPeripheral.h"
#include "drivers/gpioExpander/gpioExpander.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "drivers/boards/s3_bsp.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define MOUNT_POINT "/sdcard"

#define PIN_NUM_MISO 	GPIO_SPIBUS_MISO
#define PIN_NUM_MOSI 	GPIO_SPIBUS_MOSI
#define PIN_NUM_CLK  	GPIO_SPIBUS_CLK

#define SPI_DMA_CHAN    SPI_DMA_CH_AUTO


static sdmmc_card_t *card;
const char mount_point[] = MOUNT_POINT;
static esp_vfs_fat_sdmmc_mount_config_t mountConfig = {
    .format_if_mount_failed = true, // SD card will be partitioned and formatted in case when mounting fails.
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};
static sdmmc_host_t host;

uint8_t sdCardBusInit(void)
{
	uint8_t result = 0;
	esp_err_t err;

	spiPeripheralMasterTakeMutex();

    host.flags = SDMMC_HOST_FLAG_SPI | SDMMC_HOST_FLAG_DEINIT_ARG;
    host.slot = SDSPI_DEFAULT_HOST;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;
    host.io_voltage = 3.3f;
	host.init = &sdspi_host_init;
    host.set_bus_width = NULL;
	host.get_bus_width = NULL;
	host.set_bus_ddr_mode = NULL;
	host.set_card_clk = &sdspi_host_set_card_clk;
	host.do_transaction = &sdspi_host_do_transaction;
	host.deinit_p = &sdspi_host_remove_device;
	host.io_int_enable = &sdspi_host_io_int_enable;
	host.io_int_wait = &sdspi_host_io_int_wait;
	host.command_timeout_ms = 0;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000
    };
    err = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CHAN);

    spiPeripheralMasterGiveMutex();

    if(!err)
    	result = 1;

    return result;
}

uint8_t sdCardMountFileSystem(void)
{
	static uint8_t firstMount = 1;
	static sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
	uint8_t result = 0;
	esp_err_t err;

	slot_config.host_id = host.slot;
	slot_config.gpio_cs = 0;

	printf("sdCard: Mounting filesystem \n");

	spiPeripheralMasterTakeMutex();

	err = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mountConfig, &card);

	if (err != ESP_OK) {
		if (err == ESP_FAIL)
	    {
			printf("sdCard: Failed to mount filesystem. \n");
	    } else if(ESP_ERR_TIMEOUT)
	    {
	    	printf("sdCard: Failed to initialize the card: %s \n", esp_err_to_name(err));
	    } else {
	    	printf("sdCard: Failed to initialize the card: %s \n", esp_err_to_name(err));
	    }
	 }else{
		 printf("sdCard: Filesystem mounted \n");
		 result = 1;
		 if(firstMount)
		 {
			 firstMount = 0;
			 //sdmmc_card_print_info(stdout, card);
		 }
	 }

	spiPeripheralMasterGiveMutex();

	return result;
}

void sdCardUnmountFileSystem(void)
{
	spiPeripheralMasterTakeMutex();

	esp_vfs_fat_sdmmc_unmount();

	spiPeripheralMasterGiveMutex();
}

uint8_t sdCardCreateWriteCloseFile(char* folder, char* name, uint8_t* data, uint32_t dataLength)
{
	uint8_t	result = 0;
	FILE* f;
	char filePath[64] = {0};

	spiPeripheralMasterTakeMutex();

	strcat(filePath, MOUNT_POINT);
	strcat(filePath, "/");
	strcat(filePath, folder);
	mkdir(filePath, 777);
	strcat(filePath, "/");
	replaceChar(name, ':', '-');
	strcat(filePath, name);

	printf("sdCardCreateWriteCloseFile \n");

	f = fopen(filePath, "w");
	if (f != NULL)
	{
		result = 1;
		fwrite(data, 1, dataLength, f);
		fclose(f);
		printf("Write file OK path: %s Data length %d\n", filePath, dataLength);
	}else{
		printf("error opening path: %s Data length %d\n", filePath, dataLength);
	}

	spiPeripheralMasterGiveMutex();

	return result;
}
