![GitHub release (latest by date)](https://img.shields.io/github/v/release/maestry/eliokit-arduino?style=for-the-badge)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/)
[![GitHub all releases](https://img.shields.io/github/downloads/maestry/eliokit-arduino/total?style=for-the-badge)](https://github.com/maestry/eliokit-arduino/releases)


# Arduino library for ELIO IOT Boards

The Eliokit library is a collection of functions and classes that simplify the development of Arduino projects based on ELIO IOT Dev Board. It includes features for serial communication, digital and analog pin management, as well as sensor and actuator management.

## Installation

To install the Eliokit library, follow the installation guide available at the following link: https://elioiot.com/arduino

## Documentation

For more information on using ELIO IOT boards, consult the general documentation available at the following link: https://elioiot.com/guide

## Support

For any questions or problems related to using this library, please visit our support forum on Discord at the following link: https://discord.gg/pspP3XSbkn

## Example Code

Here is an example code that shows how to use the Eliokit library to turn on and off the vibration motor:

```js
#include "eliokit.h"

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


