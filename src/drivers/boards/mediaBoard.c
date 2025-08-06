#include "mediaBoard.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "drivers/spi/spiPeripheral.h"
#include "drivers/eInkDisplay/eInkDisplay.h"
#include "drivers/touchButtonI2C/touchButton.h"
#include "drivers/audio/amplifier/I2s/i2sout.h"
#include "drivers/audio/mic/pdm/pdmMic.h"
#include "drivers/boards/s3_bsp.h"
#include "esp32-hal-gpio.h"

// Ritardo in millisecondi basato su FreeRTOS
static void OSDelayMS(uint32_t delayMS) {
    vTaskDelay(pdMS_TO_TICKS(delayMS));
}

// Callback per clock SPI bit-bang
static void writeEINKCLK(uint8_t value) {
    digitalWrite(EINK_DISPLAY_OUTPUT_SCK, value ? HIGH : LOW);
}

// Callback per MOSI SPI bit-bang
static void writeEINKMOSI(uint8_t value) {
    digitalWrite(EINK_DISPLAY_OUTPUT_SDI, value ? HIGH : LOW);
}

// Callback per Chip Select
static void writeEINKCS(uint8_t value) {
    digitalWrite(EINK_DISPLAY_OUTPUT_CS, value ? HIGH : LOW);
}

// Callback per Data/Command
static void writeEINKDC(uint8_t value) {
    digitalWrite(EINK_DISPLAY_OUTPUT_DC, value ? HIGH : LOW);
}

// Callback per Reset display
static void writeEINKRST(uint8_t value) {
    digitalWrite(EINK_DISPLAY_OUTPUT_RESET, value ? HIGH : LOW);
}

// Callback per Busy
static uint8_t readEINKBUSY(void) {
    return (uint8_t)(digitalRead(EINK_DISPLAY_INPUT_BUSY) == HIGH);
}

// Inizializzazione board multimedia
uint8_t mediaBoardInitialize(void) {
    // Inizializza il mutex SPI
    spiPeripheralInit();

    // Configura i GPIO per l'e-Ink
    pinMode(EINK_DISPLAY_OUTPUT_SCK, OUTPUT);
    pinMode(EINK_DISPLAY_OUTPUT_SDI, OUTPUT);
    pinMode(EINK_DISPLAY_OUTPUT_CS, OUTPUT);
    pinMode(EINK_DISPLAY_OUTPUT_DC, OUTPUT);
    pinMode(EINK_DISPLAY_OUTPUT_RESET, OUTPUT);
    pinMode(EINK_DISPLAY_INPUT_BUSY, INPUT);

    // Prepara handler e-Ink
    eInkGDEY0154D67DisplayHandler_t handler;
    handler.writeCLK  = writeEINKCLK;
    handler.writeMOSI = writeEINKMOSI;
    handler.writeCS   = writeEINKCS;
    handler.writeDC   = writeEINKDC;
    handler.writeRST  = writeEINKRST;
    handler.readBUSY  = readEINKBUSY;
    handler.delayMS   = OSDelayMS;

    eInkDisplayInit(&handler);

    // Inizializza periferiche addizionali
    touchButtonInit();
    i2sOutInit();
    pdmMicInit();

    return 0;
}

// Handler per interrupt (es. touch button)
void mediaBoardInterruptHandler(void) {
    uint8_t status;
    if (touchButtonReadStatus(&status) == 0) {
        // Qui gestisci gli stati dei tasti
    }
}
