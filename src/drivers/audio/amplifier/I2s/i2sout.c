#include "i2sout.h"

#include "drivers/boards/mediaBoard.h"
#include "driver/i2s.h"
#include "esp_system.h"

#define I2S_OUT_SAMPLE_RATE		16000
#define I2S_OUT_I2S_CH 			1


void i2sOutInit(void) // Consumes 33284 bytes of memory!
{
    // Set the I2S configuration as PDM and 16bits per sample
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = I2S_OUT_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = 0,
    };

    // Set the pinout configuration (set using menuconfig)
    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = MEDIA_SPK_BCLK,
        .ws_io_num = MEDIA_SPK_LRCLK,
        .data_out_num = MEDIA_SPK_DIN,
        .data_in_num = I2S_PIN_NO_CHANGE,
    };

    // Call driver installation function before any I2S R/W operation.
    i2s_driver_install(I2S_OUT_I2S_CH, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_OUT_I2S_CH, &pin_config);
    i2s_set_clk(I2S_OUT_I2S_CH, I2S_OUT_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
}
