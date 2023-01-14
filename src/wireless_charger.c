#include "wireless_charger.h"
#include "drivers/wc/wc.h"
#include "drivers/i2c/i2cPeripheral.h"

static uint8_t wirelessChargerIsPresentInBus(void);


uint8_t wirelessChargerApplyConfiguration(void)
{
	wirelessChargerApplyConfigurationResult_t result;

	if(wirelessChargerIsPresentInBus())
	{
		if(wcReadAndVerifyConfiguration())
		{
			// already well configured
			result = WC_I2C_ALREADY_CONFIGURATED;
		}else{
			// need to be configured
			wcWrite256BytesConfigurationToChip();
			if(wcReadAndVerifyConfiguration())
			{
				result = WC_I2C_CONFIGURATION_WRITTEN_OK;
			}else{
				result = WC_I2C_CONFIGURATION_WRITING_ERROR;
			}
		}
	}else{
		result = WC_NOT_PRESENT_IN_I2C_BUS;
	}

	return result;
}

static uint8_t wirelessChargerIsPresentInBus(void)
{
	return wcIsConnectedToI2C();
}
