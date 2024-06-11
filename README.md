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

Here is an example code that shows how to use the elioiot library to read all value of the board:

```C
#include <Arduino.h>
#include <Wire.h>
#include "elioiot.h"

PCAL9535A::PCAL9535A<TwoWire> gpio(Wire);
Elioiotboard : Elioiotboard Elioiotboard();

void setup()
{
  Serial.begin(250000);
  Serial.println("Starting...");

  gpio.begin(); // use default address 0
}

void loop()
{
  printAllGPIO();
  delay(500);
}

void printAllGPIO()
{

  int state = gpio.readGPIO16();
  for (int i = 0; i < 16; i++)
  {
    bool b = state & (1 << 15);
    Serial.print(b);
    state = state << 1;
  }
  Serial.println();
  delay(1000);
}

```
