#include <eliokit.h>

// IO boards
#define XL9335_ADD						0x20

// Media board
#define BS8112A_3_ADDRESS				0x50

void setup() {
  Serial.begin(115200);
    // put your setup code here, to run once:
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  //gpioExpanderInit();
  spiPeripheralInit();
  // put your setup code here, to run once:
  touchButtonInit();

}

void loop() {
  
  delay(100);
  if (readQuare()) {
    Serial.println("Pressed square");
  }
  if (readCicle()) {
    Serial.println("Pressed cicle");
  }
    if (readUp()) {
    Serial.println("Pressed >");
  }
  if (readDown()) {
    Serial.println("Pressed <");
  }
  // put your main code here, to run repeatedly:
  touchButtonGetValue();


}
