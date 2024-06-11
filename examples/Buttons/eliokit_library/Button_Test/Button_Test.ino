#include <Arduino.h>
#include <Wire.h>
#include "PCAL9535A.h"

PCAL9535A::PCAL9535A<TwoWire> gpio(Wire);

void setup()
{
  Wire.setPins(4, 5);
  Serial.begin(115200);
  Serial.println("Starting...");

  gpio.begin(); // use default address 0
}

void loop()
{

  int state = gpio.digitalRead(4);
  if (!state)
  {
    Serial.println("Pressed button down");
  }
  state = gpio.digitalRead(5);
  if (!state)
  {
    Serial.println("Pressed button up");
  }
  delay(50);
}
