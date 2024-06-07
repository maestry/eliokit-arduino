![GitHub release (latest by date)](https://img.shields.io/github/v/release/maestry/elioiot-arduino?style=for-the-badge)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/)
[![GitHub all releases](https://img.shields.io/github/downloads/maestry/elioiot-arduino/total?style=for-the-badge)](https://github.com/maestry/elioiot-arduino/releases)


# Arduino library for elioiot Board

The elioiot library is a collection of functions and classes that simplify the development of Arduino projects based on elioiot Dev Board. It includes features for serial communication, digital and analog pin management, as well as sensor and actuator management.

## Installation

To install the elioiot library, follow the installation guide available at the following link: https://elioiot.it/docs/ELIO-Arduino-IDE.pdf

## Documentation

For more information on using the elioiot library, consult the general documentation available at the following link: https://elioiot.it/guide/

## Support

For any questions or problems related to using the elioiot library, please visit our support forum on Discord at the following link: https://discord.gg/pspP3XSbkn

## Example Code

Here is an example code that shows how to use the elioiot library to turn on and off the vibration motor:

```js
#include "elioiot.h"

// Vibration Motor address
#define XL9335_ADD						0x20

void setup() {
  Serial.begin(115200);
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
  i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  gpioExpanderInit();
  spiPeripheralInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  gpioExpanderSetVibrationMotorOn();
  delay(1000);
  gpioExpanderSetVibrationMotorOff();
  delay(5000);
}

```


