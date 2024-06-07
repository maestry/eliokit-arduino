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

extern struct Elioboard
{
  struct Mini
  {
    struct Microcontroller
    {
      int V3V3 = 2;
      int EN = 3;
      int IO4 = 4;
      int IO5 = 5;
      int IO6 = 6;
      int IO7 = 7;
      int IO15 = 8;
      int IO16 = 9;
      int IO17 = 10;
      int IO18 = 11;
      int IO8 = 12;
      int IO19 = 13;
      int IO20 = 14;
      int IO3 = 15;
      int IO46 = 16;
      int IO9 = 17;
      int IO10 = 18;
      int IO11 = 19;
      int IO12 = 20;
      int IO13 = 21;
      int IO14 = 22;
      int IO21 = 23;
      int IO47 = 24;
      int IO48 = 25;
      int IO45 = 26;
      int IO0 = 27;
      int IO35 = 28;
      int IO36 = 29;
      int IO37 = 30;
      int IO38 = 31;
      int IO39 = 32;
      int IO40 = 33;
      int IO41 = 34;
      int IO42 = 35;
      int RXD0 = 36;
      int TXD0 = 37;
      int IO2 = 38;
      int IO1 = 39;
      int EPAD = 41;
    } microcontroller;

    struct IOExpanderI2CConnector
    {
      int INT = 1;
      int A1 = 2;
      int A2 = 3;
      int IO0_0 = 4;
      int IO0_1 = 5;
      int IO0_2 = 6; // AN Selection
      int IO0_3 = 7;
      int IO0_4 = 8;
      int IO0_5 = 9;
      int IO0_6 = 10;
      int IO0_7 = 11;
      int VSS = 12;
      int IO1_0 = 13;
      int IO1_1 = 14;
      int IO1_2 = 15;
      int IO1_3 = 16;
      int IO1_4 = 17;
      int IO1_5 = 18;
      int IO1_6 = 19;
      int IO1_7 = 20;
      int A0 = 21;
      int SCL = 22;
      int SDA = 23;
      int VDD = 24;
    } ioExpanderI2CConnector;

  } mini;

  struct Demo
  {
    int V3V3 = 2;
    int EN = 3;
    int IO4 = 4;
    int IO5 = 5;
    int IO6 = 6;
    int IO7 = 7;
    int IO15 = 8;
    int IO16 = 9;
    int IO17 = 10;
    int IO18 = 11;
    int IO8 = 12;
    int IO19 = 13;
    int IO20 = 14;
    int IO3 = 15;
    int IO46 = 16;
    int IO9 = 17;
    int IO10 = 18;
    int IO11 = 19;
    int IO12 = 20;
    int IO13 = 21;
    int IO14 = 22;
    int IO21 = 23;
    int IO47 = 24;
    int IO48 = 25;
    int IO45 = 26;
    int IO0 = 27;
    int IO35 = 28;
    int IO36 = 29;
    int IO37 = 30;
    int IO38 = 31;
    int IO39 = 32;
    int IO40 = 33;
    int IO41 = 34;
    int IO42 = 35;
    int RXD0 = 36;
    int TXD0 = 37;
    int IO2 = 38;
    int IO1 = 39;
    int EPAD = 41;
  } microcontroller;

  struct IOExpanderI2CConnector
  {
    int INT = 1;
    int A1 = 2;
    int A2 = 3;
    int IO0_0 = 4;
    int IO0_1 = 5;
    int IO0_2 = 6; // AN Selection
    int IO0_3 = 7;
    int IO0_4 = 8;
    int IO0_5 = 9;
    int IO0_6 = 10;
    int IO0_7 = 11;
    int VSS = 12;
    int IO1_0 = 13;
    int IO1_1 = 14;
    int IO1_2 = 15;
    int IO1_3 = 16;
    int IO1_4 = 17;
    int IO1_5 = 18;
    int IO1_6 = 19;
    int IO1_7 = 20;
    int A0 = 21;
    int SCL = 22;
    int SDA = 23;
    int VDD = 24;
  } ioExpanderI2CConnector;
} demo;
}
, elioboard;
