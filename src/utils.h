/*
 * utils.h
 *
 *  Created on: May 11, 2022
 *      Author: marcelo
 */

#pragma once

#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"
#include "mbedtls/md.h"
#include <string.h>
#include <stdarg.h>
#include "cJSON.h"

typedef struct {
    uint8_t ssid[32];           /**< SSID of the AP. Null terminated string. */
    uint8_t password[64];       /**< Password of the AP. Null terminated string. */
} WiFiSSIDpassword_t;

typedef struct {
	char* certificateId;
	char* certificatePem;
	char* privateKey;
	char* certificateOwnershipToken;
} fleetProvisioningAssignedCertificate_t;

extern const char default_program_json_start[] asm("_binary_default_program_json_start");
extern const char default_program_json_end[]   asm("_binary_default_program_json_end");

int32_t nvsSystemInit(void);
uint8_t nvsGetWiFiSSIDpassword(WiFiSSIDpassword_t* SSIDpassword);
uint8_t nvsSetWiFiSSIDpassword(WiFiSSIDpassword_t* SSIDpassword);
uint8_t nvsSetDefaultJSONprogram(void);
uint8_t nvsSetJSON(char* key, const char* program, size_t programLengthBytes);
uint8_t nvsGetJSON(char* key, char** program, size_t* programLengthBytes);
void loadLocalDataFromNVS(void);
void localJSONApplyDataToSystem(cJSON* 	jsonLocalData);
void localizationJSONFileDownloadAndProcess(void);
void processLocalFileInformationJson(char* url, unsigned char *data, size_t dataLength, char* sha256);
uint8_t nvsSetFleetProvisioningAssignedCertificate(fleetProvisioningAssignedCertificate_t fleetProvisioningAssignedCertificate);
uint8_t nvsGetFleetProvisioningAssignedCertificate(fleetProvisioningAssignedCertificate_t* fleetProvisioningAssignedCertificate);
uint8_t nvsSetFleetProvisioningAssignedThingName(char* thingName);
uint8_t nvsGetFleetProvisioningAssignedThingName(char** thingName);
void set_gpio_config(const gpio_config_t *pGPIOConfig);
void find_partition(esp_partition_type_t type, esp_partition_subtype_t subtype, const char* name);
const char* get_type_str(esp_partition_type_t type);
const char* get_subtype_str(esp_partition_subtype_t subtype);
void* mallocOnPSRAM(size_t size);
void* reallocOnPSRAM(void *ptr, size_t size);
void freeOnPSRAM(void *ptr);
void printAppDescription(void);
void versionStringToNumbers(const char* versionString, uint8_t* major, uint8_t* minor, uint8_t* patch);
void printFullDateTime(void);
void printFormated(char *out, char *in);
char* ltrim(char* in);
char* rtrim(char* in);
void replaceChar(char* string, char originalChar, char replacement);
