#include "elioiot.h"

void setup()
{
	Serial.begin(9600);
	while (!Serial)
		;
	i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
	gpioExpanderInit();
	spiPeripheralInit();
	accelGyroInit();
}

void loop()
{
	int32_t x, y, z;
	x = accelerometerGetxRotation();
	y = accelerometerGetyRotation();
	z = accelerometerGetzRotation();

	Serial.print(x);
	Serial.print('\t');
	Serial.print(y);
	Serial.print('\t');
	Serial.println(z);
	delay(5000);
}

int32_t accelerometerGetxRotation(void)
{
	accelSampleRAW_t accelerationSample;
	double xyModulus;
	int32_t result = -1;

	accelGetXYZSampleRAW(&accelerationSample);
	xyModulus = sqrt(pow((double)accelerationSample.X, 2) + pow((double)accelerationSample.Y, 2));

	if (accelerationSample.X < 0 && accelerationSample.Y < 0)
	{
		result = (int32_t)(-asin((double)(accelerationSample.X) / xyModulus) * (180 / M_PI));
	}
	else if (accelerationSample.X > 0 && accelerationSample.Y < 0)
		result = (int32_t)(359.99 - asin((double)(accelerationSample.X) / xyModulus) * (180 / M_PI));
	else
		result = (int32_t)(180 + asin((double)(accelerationSample.X) / xyModulus) * (180 / M_PI));

	return result;
}

int32_t accelerometerGetyRotation(void)
{
	accelSampleRAW_t accelerationSample;
	double yzModulus;
	int32_t result = -1;

	accelGetXYZSampleRAW(&accelerationSample);
	yzModulus = sqrt(pow((double)accelerationSample.Y, 2) + pow((double)accelerationSample.Z, 2));

	if (accelerationSample.Y > 0 && accelerationSample.Z < 0)
	{
		result = (int32_t)(180 - asin((double)(accelerationSample.Y) / yzModulus) * (180 / M_PI));
	}
	else if (accelerationSample.Y < 0 && accelerationSample.Z < 0)
		result = (int32_t)(180 - asin((double)(accelerationSample.Y) / yzModulus) * (180 / M_PI));
	else if (accelerationSample.Y < 0 && accelerationSample.Z > 0)
		result = (int32_t)(360 + asin((double)(accelerationSample.Y) / yzModulus) * (180 / M_PI));
	else
		result = (int32_t)(asin((double)(accelerationSample.Y) / yzModulus) * (180 / M_PI));

	return result;
}

int32_t accelerometerGetzRotation(void)
{
	accelSampleRAW_t accelerationSample;
	double zxModulus;
	int32_t result = -1;

	accelGetXYZSampleRAW(&accelerationSample);
	zxModulus = sqrt(pow((double)accelerationSample.Z, 2) + pow((double)accelerationSample.X, 2));

	result = (int32_t)(asin((double)(accelerationSample.Z) / zxModulus) * (180 / M_PI));

	return result;
}

int32_t accelerometerGetxAccelerationG(void)
{
	accelSampleG_t accelerationSampleG;

	accelGetXYZSampleG(&accelerationSampleG);

	return accelerationSampleG.X;
}

int32_t accelerometerGetyAccelerationG(void)
{
	accelSampleG_t accelerationSampleG;

	accelGetXYZSampleG(&accelerationSampleG);

	return accelerationSampleG.Y;
}

int32_t accelerometerGetzAccelerationG(void)
{
	accelSampleG_t accelerationSampleG;

	accelGetXYZSampleG(&accelerationSampleG);

	return accelerationSampleG.Z;
}
