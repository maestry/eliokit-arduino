#include "drivers/gpioExpander/PCAL9535A.h"
extern "C" {
// da eliminare
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
