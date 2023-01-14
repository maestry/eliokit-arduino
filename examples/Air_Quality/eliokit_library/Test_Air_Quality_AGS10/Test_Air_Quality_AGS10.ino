#include "eliokit.h"

uint32_t tvoc;
uint8_t preHeatStage;
uint8_t crcError;
uint8_t id;

void setup() {
  Serial.begin(115200);
    // put your setup code here, to run once:
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  spiPeripheralInit();

}

void loop() {
  // put your main code here, to run repeatedly:
  
	double 		tvocPPB;
	uint8_t 	dataValid;

	tvocGetPPB(&tvocPPB, &dataValid);

	if(dataValid)
	{   
    Serial.print("Sensor value: ");
    Serial.print(tvocPPB);
    Serial.println("ppb");
	}
  delay(1000);
}

double tvocTaskGetPPB(void)
{
	double tvocPPB;

	tvocGetPPB(&tvocPPB, NULL);

	return tvocPPB;
}

