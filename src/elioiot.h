#include "drivers/gpioExpander/PCAL9535A.h"
extern "C"
{
#include "drivers/eInkDisplay/eInkDisplay.h"
#include "drivers/gpioExpander/gpioExpander.h"
#include "drivers/gpioExpander/chips/XL9535.h"
#include "drivers/humTemp/chips/AHT20.h"
#include "drivers/i2c/i2cPeripheral.h"
#include "drivers/sdCard/sdCard.h"
#include "drivers/accelGyro/accelGyro.h"
#include "wireless_charger.h"
#include "drivers/spi/spiPeripheral.h"
#include "drivers/touchButtonI2C/touchButton.h"
#include "drivers/ir/ir_tools.h"
#include "drivers/tvoc/tvoc.h"
}

extern struct Elioiotboard
{
  struct Mini
  {
    struct Microcontroller
    {
      const int V3V3 = 2;
      const int EN = 3;
      const int IO4 = 4;
      const int IO5 = 5;
      const int IO6 = 6;
      const int IO7 = 7;
      const int IO15 = 8;
      const int IO16 = 9;
      const int IO17 = 10;
      const int IO18 = 11;
      const int IO8 = 12;
      const int IO19 = 13;
      const int IO20 = 14;
      const int IO3 = 15;
      const int IO46 = 16;
      const int IO9 = 17;
      const int IO10 = 18;
      const int IO11 = 19;
      const int IO12 = 20;
      const int IO13 = 21;
      const int IO14 = 22;
      const int IO21 = 23;
      const int IO47 = 24;
      const int IO48 = 25;
      const int IO45 = 26;
      const int IO0 = 27;
      const int IO35 = 28;
      const int IO36 = 29;
      const int IO37 = 30;
      const int IO38 = 31;
      const int IO39 = 32;
      const int IO40 = 33;
      const int IO41 = 34;
      const int IO42 = 35;
      const int RXD0 = 36;
      const int TXD0 = 37;
      const int IO2 = 38;
      const int IO1 = 39;
      const int EPAD = 41;
    } microcontroller;

    const struct IOExpanderI2CConnector
    {
      const int INT = 1;
      const int A1 = 2;
      const int A2 = 3;
      const int IO0_0 = 4;
      const int IO0_1 = 5;
      const int IO0_2 = 6; // AN Selection
      const int IO0_3 = 7;
      const int IO0_4 = 8;
      const int IO0_5 = 9;
      const int IO0_6 = 10;
      const int IO0_7 = 11;
      const int VSS = 12;
      const int IO1_0 = 13;
      const int IO1_1 = 14;
      const int IO1_2 = 15;
      const int IO1_3 = 16;
      const int IO1_4 = 17;
      const int IO1_5 = 18;
      const int IO1_6 = 19;
      const int IO1_7 = 20;
      const int A0 = 21;
      const int SCL = 22;
      const int SDA = 23;
      const int VDD = 24;
    } ioExpanderI2CConnector;

  } mini;

  struct Demo
  {
    struct Microcontroller
    {
      const int V3V3 = 2;
      const int EN = 3;
      const int IO4 = 4;
      const int IO5 = 5;
      const int IO6 = 6;
      const int IO7 = 7;
      const int IO15 = 8;
      const int IO16 = 9;
      const int IO17 = 10;
      const int IO18 = 11;
      const int IO8 = 12;
      const int IO19 = 13;
      const int IO20 = 14;
      const int IO3 = 15;
      const int IO46 = 16;
      const int IO9 = 17;
      const int IO10 = 18;
      const int IO11 = 19;
      const int IO12 = 20;
      const int IO13 = 21;
      const int IO14 = 22;
      const int IO21 = 23;
      const int IO47 = 24;
      const int IO48 = 25;
      const int IO45 = 26;
      const int IO0 = 27;
      const int IO35 = 28;
      const int IO36 = 29;
      const int IO37 = 30;
      const int IO38 = 31;
      const int IO39 = 32;
      const int IO40 = 33;
      const int IO41 = 34;
      const int IO42 = 35;
      const int RXD0 = 36;
      const int TXD0 = 37;
      const int IO2 = 38;
      const int IO1 = 39;
      const int EPAD = 41;

    } microcontroller;

    const struct IOExpanderI2CConnector
    {
      const int INT = 1;
      const int A1 = 2;
      const int A2 = 3;
      const int IO0_0 = 4;
      const int IO0_1 = 5;
      const int IO0_2 = 6;
      const int IO0_3 = 7;
      const int IO0_4 = 8;
      const int IO0_5 = 9;
      const int IO0_6 = 10;
      const int IO0_7 = 11;
      const int VSS = 12;
      const int IO1_0 = 13;
      const int IO1_1 = 14;
      const int IO1_2 = 15;
      const int IO1_3 = 16;
      const int IO1_4 = 17;
      const int IO1_5 = 18;
      const int IO1_6 = 19;
      const int IO1_7 = 20;
      const int A0 = 21;
      const int SCL = 22;
      const int SDA = 23;
      const int VDD = 24;

    } ioExpanderI2CConnector;

  } demo;

}, elioiot;
