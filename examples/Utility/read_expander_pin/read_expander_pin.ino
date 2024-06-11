#include <Arduino.h>
#include <Wire.h>
#include "elioiot.h"

PCAL9535A::PCAL9535A<TwoWire> gpio(Wire);
  
void setup() {  
  Wire.setPins(4, 5);
  Serial.begin(115200);
  Serial.println("Starting...");

  gpio.begin();      // use default address 0

}

void loop() {

  printAllGPIO();  
  delay(500);

}

void printAllGPIO() {

  int state = gpio.readGPIO16();  
  for (int i = 0; i < 16; i++) {
    bool b = state & (1 << 15);
    Serial.print(b);
    state = state << 1;
  }
  Serial.println();

}