#include "elioiot.h"

size_t length = 0;
rmt_item32_t *items = NULL;

static RingbufHandle_t ringbufHandle = NULL;

void setup() {
  // put your setup code here, to run once:
  IRReceiverInit();
}

void loop() {
  size_t length = 0;
  rmt_item32_t *items = NULL;
  items = (rmt_item32_t *) xRingbufferReceive(ringbufHandle, &length, portMAX_DELAY);

	prepareIRDataToTX(items, length);

	printf("IR RX data length %d \n", length);
	printf("Values: \n");
	for (int i = 0; i < (length/4) ; i++)
	{
			printf("{.val=0x%04x},", items[i].val);
	}
	printf("\n");



	vRingbufferReturnItem(ringbufHandle, (void *) items);
}

static void IRReceiverInit(void)
{
	int32_t err;

	rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX((gpio_num_t)GPIO_IR_RX, (rmt_channel_t)IR_RMT_RX_CHANNEL);

	err = rmt_config(&rmt_rx_config);
	if (err != ESP_OK)
		printf("rmt_config failed %d \n", err);

	err = rmt_driver_install(IR_RMT_RX_CHANNEL, 1024, 0);
	if (err != ESP_OK)
		printf("rmt_driver_install failed %d \n", err);

	err = rmt_get_ringbuf_handle(IR_RMT_RX_CHANNEL, &ringbufHandle);
	if (err != ESP_OK)
		printf("rmt_get_ringbuf_handle failed %d \n", err);

	err = rmt_rx_start(IR_RMT_RX_CHANNEL, true);
	if (err != ESP_OK)
		printf("rmt_rx_start failed %d \n", err);
}

static void prepareIRDataToTX(rmt_item32_t* items, uint32_t itemsCount)
{
	rmt_item32_t temp;

	for (int i = 0; i < itemsCount; i++)
	{
		temp = items[i];
		items[i].level0 = temp.level1;
		items[i].level1 = temp.level0;
	}
}
