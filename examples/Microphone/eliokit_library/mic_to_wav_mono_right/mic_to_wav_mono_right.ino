// Save audio from PDM microphone to SD Card in wav format

/* Microphone has following pins:
    VDD
    GND
    DOUT - connected to DIN I2S pin on ESP32
    CLK  - connected to WS I2S pin on ESP32
    LR   - not connected to ESP32. Microphone has internal pull-down to GND for this pin. */

// uncomment to demonstrate that valid wav files are being generated
// #define GENERATE_DEMO_WAV

// Raw PCM data is inside the header
#ifdef GENERATE_DEMO_WAV
#include "pcm_sample.h"
#endif

#include <driver/i2s.h>
#include "FS.h"
#include "SD_MMC.h"
#include "elioiot.h"

// IO boards
#define XL9335_ADD 0x20

// I2S perhiperhal number
#define I2S_CHANNEL I2S_NUM_0 // I2S_NUM_1 doesn't support PDM

// I2S pins
#define I2S_PIN_BIT_CLOCK I2S_PIN_NO_CHANGE // not used
#define I2S_PIN_WORD_SELECT 42
#define I2S_PIN_DATA_OUT I2S_PIN_NO_CHANGE // not used
#define I2S_PIN_DATA_IN 43

// I2S CONFIG PARAMS
#define I2S_MODE (I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM)
#define I2S_SAMPLE_RATE 8000
#define I2S_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define I2S_CHANNEL_FMT I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_COMM_FMT I2S_COMM_FORMAT_PCM
#define I2S_INTERRUPT_PRIO ESP_INTR_FLAG_LEVEL1
#define I2S_DMA_BUF_COUNT 4
#define I2S_DMA_BUF_SIZE 1000
#define I2S_ENABLE_ACCURATE_CLK true

bool I2S_Init()
{
  i2s_config_t i2s_config;
  memset(&i2s_config, 0, sizeof(i2s_config));

  i2s_config.mode = (i2s_mode_t)I2S_MODE;
  i2s_config.sample_rate = I2S_SAMPLE_RATE;
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE;
  i2s_config.channel_format = I2S_CHANNEL_FMT;
  i2s_config.communication_format = (i2s_comm_format_t)I2S_COMM_FMT;
  i2s_config.intr_alloc_flags = I2S_INTERRUPT_PRIO;
  i2s_config.dma_buf_count = I2S_DMA_BUF_COUNT;
  i2s_config.dma_buf_len = I2S_DMA_BUF_SIZE;
  i2s_config.use_apll = I2S_ENABLE_ACCURATE_CLK;

  i2s_pin_config_t pins = {
      .bck_io_num = I2S_PIN_BIT_CLOCK,
      .ws_io_num = I2S_PIN_WORD_SELECT,
      .data_out_num = I2S_PIN_DATA_OUT,
      .data_in_num = I2S_PIN_DATA_IN};

  if (i2s_driver_install(I2S_CHANNEL, &i2s_config, 0, NULL) != ESP_OK)
  {
    Serial.println("i2s_driver_install() error");
    return false;
  }

  if (i2s_set_pin(I2S_NUM_0, &pins) != ESP_OK)
  {
    Serial.println("i2s_set_pin() error");
    return false;
  }
}

void I2S_Quit()
{
  if (i2s_driver_uninstall(I2S_CHANNEL) != ESP_OK)
  {
    Serial.println("i2s_driver_uninstall() error");
  }
}

// Create a file and add wav header to it so we can play it from PC later
bool create_wav_file(char *song_name, uint32_t duration, uint16_t num_channels, const uint32_t sampling_rate, uint16_t bits_per_sample)
{
  // data size in bytes - > this amount of data should be recorded from microphone later
  uint32_t data_size = sampling_rate * num_channels * bits_per_sample * duration / 8;

  while (!sdCardMountFileSystem())
  {
    Serial.println("Card Mount Failed");
    return false;
  }

  /* *************** ADD ".WAV" HEADER *************** */
  uint8_t CHUNK_ID[4] = {'R', 'I', 'F', 'F'};
  sdCardCreateWriteCloseFile("folderName", song_name, CHUNK_ID, 4);

  uint32_t chunk_size = data_size + 36;
  uint8_t CHUNK_SIZE[4] = {chunk_size, chunk_size >> 8, chunk_size >> 16, chunk_size >> 24};
  sdCardAppendCloseFile("folderName", song_name, CHUNK_SIZE, 4);

  uint8_t FORMAT[4] = {'W', 'A', 'V', 'E'};
  sdCardAppendCloseFile("folderName", song_name, FORMAT, 4);

  uint8_t SUBCHUNK_1_ID[4] = {'f', 'm', 't', ' '};
  sdCardAppendCloseFile("folderName", song_name, SUBCHUNK_1_ID, 4);

  uint8_t SUBCHUNK_1_SIZE[4] = {0x10, 0x00, 0x00, 0x00};
  sdCardAppendCloseFile("folderName", song_name, SUBCHUNK_1_SIZE, 4);

  uint8_t AUDIO_FORMAT[2] = {0x01, 0x00};
  sdCardAppendCloseFile("folderName", song_name, AUDIO_FORMAT, 2);

  uint8_t NUM_CHANNELS[2] = {num_channels, num_channels >> 8};
  sdCardAppendCloseFile("folderName", song_name, NUM_CHANNELS, 2);

  uint8_t SAMPLING_RATE[4] = {sampling_rate, sampling_rate >> 8, sampling_rate >> 16, sampling_rate >> 24};
  sdCardAppendCloseFile("folderName", song_name, SAMPLING_RATE, 4);

  uint32_t byte_rate = num_channels * sampling_rate * bits_per_sample / 8;
  uint8_t BYTE_RATE[4] = {byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24};
  sdCardAppendCloseFile("folderName", song_name, BYTE_RATE, 4);

  uint16_t block_align = num_channels * bits_per_sample / 8;
  uint8_t BLOCK_ALIGN[2] = {block_align, block_align >> 8};
  sdCardAppendCloseFile("folderName", song_name, BLOCK_ALIGN, 2);

  uint8_t BITS_PER_SAMPLE[2] = {bits_per_sample, bits_per_sample >> 8};
  sdCardAppendCloseFile("folderName", song_name, BITS_PER_SAMPLE, 2);

  uint8_t SUBCHUNK_2_ID[4] = {'d', 'a', 't', 'a'};
  sdCardAppendCloseFile("folderName", song_name, SUBCHUNK_2_ID, 4);

  uint8_t SUBCHUNK_2_SIZE[4] = {data_size, data_size >> 8, data_size >> 16, data_size >> 24};
  sdCardAppendCloseFile("folderName", song_name, SUBCHUNK_2_SIZE, 4);

  // Actual data should be appended after this point later
  return true;
}

void microphone_record(char *song_name, uint32_t duration)
{
  // Add wav header to the file so we can play it from PC later
  // Mono
  if (!create_wav_file(song_name, duration, 1, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE))
  {
    Serial.println("Error during wav header creation");
    return;
  }

  // Buffer to receive data from microphone
  const size_t BUFFER_SIZE = 500;
  uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);

  // Initialize I2S
  I2S_Init();

  // data size in bytes - > this amount of data should be recorded from microphone
  uint32_t data_size = I2S_SAMPLE_RATE * I2S_BITS_PER_SAMPLE * duration / 8;

  // Record until "file_size" bytes have been read from mic.
  uint32_t counter = 0;
  uint32_t bytes_written;
  Serial.println("Recording started");
  while (counter != data_size)
  {
    // Check for file size overflow
    if (counter > data_size)
    {
      Serial.println("File is corrupted. data_size must be multiple of BUFFER_SIZE. Please modify BUFFER_SIZE");
      break;
    }

    // Read data from microphone
    if (i2s_read(I2S_CHANNEL, buf, BUFFER_SIZE, &bytes_written, portMAX_DELAY) != ESP_OK)
    {
      Serial.println("i2s_read() error");
    }

    if (bytes_written != BUFFER_SIZE)
    {
      Serial.println("Bytes written error");
    }

    // Save data to SD card
    sdCardAppendCloseFile("folderName", song_name, buf, BUFFER_SIZE);

    // Increment the counter
    counter += BUFFER_SIZE;
  }
  Serial.println("Recording finished");

  I2S_Quit();
  free(buf);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
  i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  gpioExpanderInit();
  spiPeripheralInit();
  while (!sdCardBusInit())
  {
    Serial.println("Errore inizializzazione card");
    delay(1000);
  }
  microphone_record("mono_right.wav", 15);
}

void loop()
{
}