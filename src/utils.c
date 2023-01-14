/*
 * utils.c
 *
 *  Created on: May 11, 2022
 *      Author: marcelo
 */

#include "utils.h"

#include "drivers/boards/s3_bsp.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <time.h>


#define STORAGE_NAMESPACE "storage"

static uint8_t getStringFromNVS(nvs_handle_t NVShandle, const char* key, char** str);
static char* nthStrchr(const char* s, int c, int n);

int32_t nvsSystemInit(void)
{
	int32_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}

	return err;
}

uint8_t nvsGetWiFiSSIDpassword(WiFiSSIDpassword_t* SSIDpassword)
{
	size_t required_size;
    nvs_handle_t my_handle;
    uint8_t result = 1;
    int32_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    required_size = 0;
    // obtain required memory space to store blob being read from NVS
    err = nvs_get_blob(my_handle, "ssid", NULL, &required_size);
    if (err != 0 && err != ESP_ERR_NVS_NOT_FOUND){
    	printf("nvs_get_blob ssid failed %d \n", err);
    }else if(err != 0 && err == ESP_ERR_NVS_NOT_FOUND){
    	printf("SSID not present in nvs \n");
    }else if (err == 0 && required_size > 0)
    {
        nvs_get_blob(my_handle, "ssid", SSIDpassword->ssid, &required_size);
    	result = 0;
    }

    required_size = 0;
    // obtain required memory space to store blob being read from NVS
    err = nvs_get_blob(my_handle, "password", NULL, &required_size);
    if (err != 0 && err != ESP_ERR_NVS_NOT_FOUND){
    	printf("nvs_get_blob password failed %d \n", err);
    }else if(err != 0 && err == ESP_ERR_NVS_NOT_FOUND){
    	printf("Password not present in nvs \n");
    }else if (err == 0 && required_size > 0)
    {
        nvs_get_blob(my_handle, "password", SSIDpassword->password, &required_size);
    }

    // Close
    nvs_close(my_handle);

    return result;
}

uint8_t nvsSetWiFiSSIDpassword(WiFiSSIDpassword_t* SSIDpassword)
{
    nvs_handle_t my_handle;
    uint8_t result = 0;
    int32_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    err = nvs_set_blob(my_handle, "ssid", SSIDpassword->ssid, sizeof(SSIDpassword->ssid));
    if (err != 0)
    	printf("nvs_set_blob ssid failed %d \n", err);

    err = nvs_set_blob(my_handle, "password", SSIDpassword->password, sizeof(SSIDpassword->password));
    if (err != 0)
    	printf("nvs_set_blob password failed %d \n", err);

    // Commit
    err = nvs_commit(my_handle);
    if (err != 0)
    	printf("nvs_commit failed %d \n", err);

    // Close
    nvs_close(my_handle);

    return result;
}

uint8_t nvsSetFleetProvisioningAssignedCertificate(fleetProvisioningAssignedCertificate_t fleetProvisioningAssignedCertificate)
{
    nvs_handle_t my_handle;
    uint8_t result = 0;
    int32_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    err = nvs_set_str(my_handle, "FPACcId", fleetProvisioningAssignedCertificate.certificateId);
    if (err != 0)
    	printf("nvs_set_str FPACcertificateId failed: %d \n", err);

    err = nvs_set_str(my_handle, "FPACcPem", fleetProvisioningAssignedCertificate.certificatePem);
    if (err != 0)
    	printf("nvs_set_str FPACcertificatePem failed: %d \n", err);

    err = nvs_set_str(my_handle, "FPACpKey", fleetProvisioningAssignedCertificate.privateKey);
    if (err != 0)
    	printf("nvs_set_str FPACprivateKey failed: %d \n", err);

    err = nvs_set_str(my_handle, "FPACcToken", fleetProvisioningAssignedCertificate.certificateOwnershipToken);
    if (err != 0)
    	printf("nvs_set_str FPACcertificateOwnershipToken failed: %d \n", err);

    // Commit
    err = nvs_commit(my_handle);
    if (err != 0)
    	printf("nvs_commit failed %d \n", err);

    // Close
    nvs_close(my_handle);

    return result;
}

uint8_t nvsSetFleetProvisioningAssignedThingName(char* thingName)
{
    nvs_handle_t my_handle;
    uint8_t result = 0;
    int32_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    err = nvs_set_str(my_handle, "thingName", thingName);
    if (err != 0)
    	printf("nvs_set_str thingName failed: %d \n", err);

    // Commit
    err = nvs_commit(my_handle);
    if (err != 0)
    	printf("nvs_commit failed %d \n", err);

    // Close
    nvs_close(my_handle);

    return result;
}

uint8_t nvsGetFleetProvisioningAssignedThingName(char** thingName)
{
    nvs_handle_t NVShandle;
    uint8_t result = 1;
    int32_t err;
    size_t stringLengthBytes;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &NVShandle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    // read FPACcId from NVS
    result = getStringFromNVS(NVShandle, "thingName", thingName);

    // Close
    nvs_close(NVShandle);

    return result;
}

uint8_t nvsGetFleetProvisioningAssignedCertificate(fleetProvisioningAssignedCertificate_t* fleetProvisioningAssignedCertificate)
{
    nvs_handle_t NVShandle;
    uint8_t result = 1;
    int32_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &NVShandle);
    if (err != 0)
    	printf("nvs_open failed %d \n", err);

    // read FPACcId from NVS
    result = getStringFromNVS(NVShandle, "FPACcId", &fleetProvisioningAssignedCertificate->certificateId);

    // read FPACcPem from NVS
    if(result == 0)
    	result = getStringFromNVS(NVShandle, "FPACcPem", &fleetProvisioningAssignedCertificate->certificatePem);

    // read FPACpKey from NVS
    if(result == 0)
    	result = getStringFromNVS(NVShandle, "FPACpKey", &fleetProvisioningAssignedCertificate->privateKey);

    // read FPACcToken from NVS
    if(result == 0)
    	result = getStringFromNVS(NVShandle, "FPACcToken", &fleetProvisioningAssignedCertificate->certificateOwnershipToken);

    // Close
    nvs_close(NVShandle);

    return result;
}


void loadLocalDataFromNVS(void)
{
	char* 	JsonLocal;
	size_t 	JsonLocalLength;

	if(!nvsGetJSON("JSONlocal", &JsonLocal, &JsonLocalLength))
		localJSONApplyDataToSystem(cJSON_ParseWithLength(JsonLocal, JsonLocalLength));
}

void localJSONApplyDataToSystem(cJSON* 	jsonLocalData)
{
	cJSON* 	objectJSON;
	char	timeZone[64] = "UTC";

	if(jsonLocalData!= NULL)
	{
		objectJSON = cJSON_GetObjectItemCaseSensitive(jsonLocalData, "utc_offset");
		if(objectJSON != NULL && objectJSON->valuestring != NULL)
		{
			strncat(timeZone, objectJSON->valuestring, 3);
			strcat(timeZone, ":");
			strncat(timeZone, &objectJSON->valuestring[3], 2);

			if(strncmp(&timeZone[3], "+",1) == 0)
				timeZone[3] = '-';
			else
				timeZone[3] = '+';

			setenv("TZ", timeZone, 1);
			tzset();
		}
	}
}

void set_gpio_config(const gpio_config_t *pGPIOConfig)
{

	gpio_set_direction(pGPIOConfig->pin_bit_mask, pGPIOConfig->mode);

	if (pGPIOConfig->pull_up_en) {
		gpio_pullup_en(pGPIOConfig->pin_bit_mask);
	} else {
		gpio_pullup_dis(pGPIOConfig->pin_bit_mask);
	}

	if (pGPIOConfig->pull_down_en) {
		gpio_pulldown_en(pGPIOConfig->pin_bit_mask);
	} else {
		gpio_pulldown_dis(pGPIOConfig->pin_bit_mask);
	}

	gpio_set_intr_type(pGPIOConfig->pin_bit_mask, pGPIOConfig->intr_type);

	if (pGPIOConfig->intr_type) {
		gpio_intr_enable(pGPIOConfig->pin_bit_mask);
	} else {
		gpio_intr_disable(pGPIOConfig->pin_bit_mask);
	}
}

void find_partition(esp_partition_type_t type, esp_partition_subtype_t subtype, const char* name)
{
	printf("Find partition with type %s, subtype %s, label %s... \n", get_type_str(type), get_subtype_str(subtype),
                    name == NULL ? "NULL (unspecified)" : name);

    const esp_partition_t * part  = esp_partition_find_first(type, subtype, name);

    if (part != NULL) {
        printf("\tfound partition '%s' at offset 0x%x with size 0x%x \n", part->label, part->address, part->size);
    } else {
    	printf("\tpartition not found! \n");
    }
}

const char* get_type_str(esp_partition_type_t type)
{
    switch(type) {
        case ESP_PARTITION_TYPE_APP:
            return "ESP_PARTITION_TYPE_APP";
        case ESP_PARTITION_TYPE_DATA:
            return "ESP_PARTITION_TYPE_DATA";
        default:
            return "UNKNOWN_PARTITION_TYPE";
    }
}

const char* get_subtype_str(esp_partition_subtype_t subtype)
{
    switch(subtype) {
        case ESP_PARTITION_SUBTYPE_DATA_NVS:
            return "ESP_PARTITION_SUBTYPE_DATA_NVS";
        case ESP_PARTITION_SUBTYPE_DATA_PHY:
            return "ESP_PARTITION_SUBTYPE_DATA_PHY";
        case ESP_PARTITION_SUBTYPE_APP_FACTORY:
            return "ESP_PARTITION_SUBTYPE_APP_FACTORY";
        case ESP_PARTITION_SUBTYPE_DATA_FAT:
            return "ESP_PARTITION_SUBTYPE_DATA_FAT";
        default:
            return "UNKNOWN_PARTITION_SUBTYPE";
    }
}

void* mallocOnPSRAM(size_t size)
{
	return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

void* reallocOnPSRAM(void *ptr, size_t size)
{
	return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM);
}

void freeOnPSRAM(void *ptr)
{
	heap_caps_free(ptr);
}

void printAppDescription(void)
{
	uint8_t major, minor, patch;
	const esp_app_desc_t* appDescription = esp_ota_get_app_description();

	printf("Project name: %s \n", appDescription->project_name);
	printf("Version: %s \n", appDescription->version);
	printf("Compile time: %s \n", appDescription->time);
	printf("Compile date: %s \n", appDescription->date);

	versionStringToNumbers(appDescription->version, &major, &minor, &patch);

	printf("major: %d minor: %d patch: %d \n", major, minor, patch);


	printf("SHA256: ");
	for (int i = 0; i < 32; i++)
	    printf("%02X", appDescription->app_elf_sha256[i]);
	printf("\n");
}

void versionStringToNumbers(const char* versionString, uint8_t* major, uint8_t* minor, uint8_t* patch)
{
	char majorString[4] = {0};
	char minorString[4] = {0};
	char patchString[4] = {0};

	memcpy(majorString, versionString, nthStrchr(versionString, '.', 1) - versionString);
	memcpy(minorString, nthStrchr(versionString, '.', 1) + 1, nthStrchr(versionString, '.', 2) - (nthStrchr(versionString, '.', 1) + 1));
	strcpy(patchString, nthStrchr(versionString, '.', 2) + 1);

	*major = atoi(majorString);
	*minor = atoi(minorString);
	*patch = atoi(patchString);
}

static uint8_t getStringFromNVS(nvs_handle_t NVShandle, const char* key, char** str)
{
	int32_t err;
	uint8_t result = 1;
	uint32_t stringLengthBytes;

	err = nvs_get_str(NVShandle, key, NULL, &stringLengthBytes);
	if (err != 0 && err != ESP_ERR_NVS_NOT_FOUND){
		printf("nvs_get_str %s failed %d \n", key, err);
	}else if(err != 0 && err == ESP_ERR_NVS_NOT_FOUND){
		printf("nvs_get_str %s not present in nvs \n", key);
	}else if (err == 0 && stringLengthBytes > 0)
	{
		*str = heap_caps_malloc(stringLengthBytes, MALLOC_CAP_SPIRAM);
		if(*str != NULL)
		{
			nvs_get_str(NVShandle, key, *str, &stringLengthBytes);
	    	result = 0;
		}else{
			printf("getStringFromNVS heap_caps_malloc %s error \n", key);
		}
	}

	return result;
}

static char* nthStrchr(const char* s, int c, int n)
{
    int c_count;
    char* nth_ptr;

    for (c_count=1,nth_ptr=strchr(s,c);
         nth_ptr != NULL && c_count < n && c!=0;
         c_count++)
    {
         nth_ptr = strchr(nth_ptr+1, c);
    }

    return nth_ptr;
}

void printFullDateTime(void)
{
	time_t now;
	struct tm timeinfo;
	char strftimeBuf[64];

	time(&now);
	localtime_r(&now, &timeinfo);

	strftime(strftimeBuf, sizeof(strftimeBuf), "%c", &timeinfo);

	printf("date and time: %s \n", strftimeBuf);
}

void printFormated(char *out, char *in)
{
	uint32_t 	shortcutResultPosition, shortcutStartPosition, shortcutEndPosition;
	size_t 		inLength;
	char 		shortcutName[64];
	char 		shortcutResult[64];
	uint32_t 	inIndex;
	uint32_t 	inScIndex;
	uint32_t 	outIndex;
	uint32_t	shortcutIndex;

	inLength = strlen(in);
	shortcutIndex = strcspn(in, ",");

	inIndex = 0;
	inScIndex = 0;
	outIndex = 0;
	while(inIndex < inLength)
	{
		shortcutResultPosition = strcspn(&in[inIndex], "&"); // if shortcutPosition == inLength break
		if(shortcutResultPosition > (shortcutIndex-inIndex))
		{
			// copy last section before ','
			strncpy(&out[outIndex], &in[inIndex], shortcutIndex-inIndex);
			outIndex += shortcutIndex-inIndex;
			out[outIndex] = 0;
			break;
		}else{
			// copy section before '&'
			strncpy(&out[outIndex], &in[inIndex], shortcutResultPosition);
			outIndex += shortcutResultPosition;
			inIndex += shortcutResultPosition + 1;
			// execute shortcut
			shortcutStartPosition = strcspn(&in[inScIndex], ",");
			inScIndex += shortcutStartPosition + 1;
			shortcutEndPosition = strcspn(&in[inScIndex], ",");
			if(inScIndex < inLength)
			{
				strncpy(shortcutName, &in[inScIndex], shortcutEndPosition);
				shortcutName[shortcutEndPosition] = 0;
				shortcutGetValue(shortcutResult, NULL, cJSON_CreateString(rtrim(ltrim(shortcutName))));
				// copy shortcut result
				strcpy(&out[outIndex], shortcutResult);
				outIndex += strlen(shortcutResult);
			}else{
				// shortcut result expected using '&' but no shortcut defined after ','
				strcpy(&out[outIndex], "?");
				outIndex += 1;
			}

		}
	}
}

char* ltrim(char* in)
{
	uint32_t 	inLength;
	uint32_t	index;

	inLength = strlen(in);

	for(index = 0; index < inLength; index++)
	{
		if(in[index] != ' ')
			break;
	}

	return &in[index];
}

char* rtrim(char* in)
{
	uint32_t 	inLength;
	int32_t	index;

	inLength = strlen(in);

	for(index = inLength - 1; index >= 0; index--)
	{
		if(in[index] == ' ')
			in[index] = 0;
		else
			break;
	}

	return in;
}

void replaceChar(char* string, char originalChar, char replacement)
{
	uint32_t i = 0;

	while (string[i])
	{
		if (string[i] == originalChar)
			string[i] = replacement;
		i++;
	}
}




