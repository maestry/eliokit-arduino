#pragma once

#include <stdint.h>

typedef void (*gpioExpanderCallback_t)(uint8_t value);
typedef void (*gpioExpanderLedsDeselectionCallback_t)(void);

void gpioExpanderInit(void);

void configGpioExpander(uint8_t i2cPort, uint8_t chipID, uint8_t port, uint8_t configuration, uint8_t invertConfiguration, uint8_t writeValue);

uint8_t gpioExpanderReadVbusDet(void);
uint8_t gpioExpanderReadVsunDet(void);

// SPI sdCARD CD control
void gpioExpanderWriteSPICS(uint8_t value);

// analog select SW
void gpioExpanderSelectAnalogVBAT(void);
void gpioExpanderDeselectAnalogVBAT(void);
void gpioExpanderSelectAnIn(void);
void gpioExpanderDeselectAnIn(void);

// vibration motor
void gpioExpanderSetVibrationMotorOn(void);
void gpioExpanderSetVibrationMotorOff(void);

// status LED
void gpioExpanderSetStatusLedOn(void);
void gpioExpanderSetStatusLedOff(void);

// wireless charger chip
void gpioExpanderReadWCST(uint8_t* wcst0, uint8_t* wcst1);
