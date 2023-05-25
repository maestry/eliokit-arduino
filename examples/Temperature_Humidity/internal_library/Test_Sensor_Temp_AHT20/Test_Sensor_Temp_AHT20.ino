#include <eliokit.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Serial.println("AHT20 found");
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  spiPeripheralInit();
  Wire.begin(4,5);
  setAHT20Enable();
  Serial.println("AHT20 found");
}

void loop() {
  int32_t humidity, temp;
  getAHT20HumTemp(&humidity, &temp);
  
  Serial.print("Temperature: "); Serial.print(temp); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println("% rH");

  delay(500);
}