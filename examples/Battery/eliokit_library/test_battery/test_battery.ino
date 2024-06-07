#include "elioiot.h"
#include "esp_adc_cal.h"

///ADC Channels
#define ADC1_VBAT_CHANNEL					(adc1_channel_t)0
#define VBAT_VOLTAGE_RATIO					2

//ADC Attenuation
#define ADC1_ATTENUATION 			        ADC_ATTEN_DB_11

//ADC Calibration
#define ADC_CALI_SCHEME     				ESP_ADC_CAL_VAL_EFUSE_TP_FIT


// Battery low voltage threshold in mV
#define SYSTEM_BATTERY_LOW_V					3.1
#define SYSTEM_BATTERY_LOW_HYSTERESIS			0.1

#define SYSTEM_INT32_VARIABLE_COUNT				256
#define SYSTEM_RAW_TOUCH_CIRCLE_VALUE			2100
#define SYSTEM_RAW_TOUCH_LEFT_VALUE				3400
#define SYSTEM_RAW_TOUCH_RIGHT_VALUE			3750
#define SYSTEM_RAW_TOUCH_SQUARE_VALUE			2910

#define SYSTEM_EVENT_SAMPLE_RATE_MS				100
#define WIRELESS_CHARGER_DEBOUNCE_PERIODS		10

SemaphoreHandle_t 	ADC1Semaphore;

static uint8_t cali_enable;
static esp_adc_cal_characteristics_t adc1_chars;
static int32_t systemInt32Variable[SYSTEM_INT32_VARIABLE_COUNT];


static uint8_t ADC1CalibrationInit(void);
static void ADC1Init(void);
static int32_t ADC1ReadRAW(adc1_channel_t channel);

void setup() {
  Serial.begin(115200);
  while(!Serial);
    // put your setup code here, to run once:
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  gpioExpanderInit();
  spiPeripheralInit();
  ADC1Init();
  Serial.println("End initialization");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Carica batteria: ");
  double c = systemCommandGetBatteryV();
  Serial.println(c);
  delay(2000);
}

double systemCommandGetBatteryV(void)
{
	double batteryV;

	gpioExpanderSelectAnalogVBAT();
	vTaskDelay(20 / portTICK_RATE_MS); // wait for V stable
	batteryV = ((double)esp_adc_cal_raw_to_voltage(ADC1ReadRAW(ADC1_VBAT_CHANNEL), &adc1_chars) * VBAT_VOLTAGE_RATIO) / 1000;
	gpioExpanderDeselectAnalogVBAT();

	return roundf(batteryV * 100) / 100;
}

uint32_t systemCommandGetBatteryFull(void)
{
	double vBat;
	static uint32_t lastResult = 0;

	vBat = systemCommandGetBatteryV();

	if(lastResult == 1 && (vBat < 3.90))
		lastResult = 0;

	if(lastResult == 0 && (vBat >= 4.05))
		lastResult = 1;

	return lastResult;
}

uint32_t systemCommandGetWirelessCharging(void)
{
	uint8_t wpc1;
	uint8_t wpc2;
	uint8_t wpc3;
	uint8_t wirelessCharging = 0;

	gpioExpanderReadWCST(NULL, &wpc1);
	vTaskDelay(10 / portTICK_RATE_MS);
	gpioExpanderReadWCST(NULL, &wpc2);
	vTaskDelay(10 / portTICK_RATE_MS);
	gpioExpanderReadWCST(NULL, &wpc3);
	vTaskDelay(10 / portTICK_RATE_MS);

	if(wpc1 && wpc2 && wpc3)
		wirelessCharging = 1;

	return wirelessCharging;
}

int32_t systemCommandGetPlugged(void)
{
	return gpioExpanderReadVbusDet();
}

int32_t systemCommandGetsunDetected(void)
{
	return gpioExpanderReadVsunDet();
}

int32_t systemCommandGetBatteryLow(void)
{
	static int32_t lastValue = 0;

	if (lastValue == 1)
	{
		if (systemCommandGetBatteryV() > (SYSTEM_BATTERY_LOW_V + SYSTEM_BATTERY_LOW_HYSTERESIS))
			lastValue = 0;
	}else{
		if (systemCommandGetBatteryV() <= SYSTEM_BATTERY_LOW_V)
			lastValue = 1;
	}

	return lastValue;
}

static int32_t ADC1ReadRAW(adc1_channel_t channel)
{
	int value;

	xSemaphoreTake(ADC1Semaphore, portMAX_DELAY);
	value = adc1_get_raw(channel);
	xSemaphoreGive(ADC1Semaphore);

	return value;
}

static void ADC1Init(void)
{
	uint8_t res;

	ADC1Semaphore = xSemaphoreCreateMutex();

	cali_enable = ADC1CalibrationInit();

    //ADC1 config
    res = adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT);
    if(res != 0)
    	printf("adc1_config_width ERROR");
    res = adc1_config_channel_atten(ADC1_VBAT_CHANNEL, ADC1_ATTENUATION);
    if(res != 0)
    	printf("adc1_config_channel_atten ERROR");
}

static uint8_t ADC1CalibrationInit(void)
{
    esp_err_t ret;
    cali_enable = 0;

    ret = esp_adc_cal_check_efuse(ADC_CALI_SCHEME);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
    	printf("ADCCalibrationInit Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
    	printf("ADCCalibrationInit eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        cali_enable = 1;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC1_ATTENUATION, (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    } else {
        printf("ADCCalibrationInit Invalid arg");
    }

    return cali_enable;
}
