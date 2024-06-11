
#include "i2cPeripheral.h"

#define EEPROM_WRITE_ADDR 0x00
#define EEPROM_READ_ADDR 0x01
#define EEPROM_PAGE_SIZE 16

#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */

SemaphoreHandle_t i2c1PeripheralMutex = NULL;
SemaphoreHandle_t i2c2PeripheralMutex = NULL;

void i2cPeripheralInitCustomSpeed(uint8_t i2cPort, uint32_t speed)
{
    i2c_config_t conf;

    if (i2cPort)
    {
        if (i2c2PeripheralMutex == NULL)
            i2c2PeripheralMutex = xSemaphoreCreateMutex();
    }
    else
    {
        if (i2c1PeripheralMutex == NULL)
            i2c1PeripheralMutex = xSemaphoreCreateMutex();
    }

    i2cPeripheralMasterTakeMutex(i2cPort);

    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;

    if (i2cPort)
    {
        conf.sda_io_num = GPIO_I2C_2_BUS_SDA;
        conf.scl_io_num = GPIO_I2C_2_BUS_SCL;
    }
    else
    {
        conf.sda_io_num = GPIO_I2C_1_BUS_SDA;
        conf.scl_io_num = GPIO_I2C_1_BUS_SCL;
    }

    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = speed;

    // i2c_driver_delete(i2cPort);
    i2c_param_config(i2cPort, &conf);
    i2c_driver_install(i2cPort, conf.mode, 0, 0, 0);

    i2cPeripheralMasterGiveMutex(i2cPort);
}

void i2cPeripheralMasterTakeMutex(uint8_t i2cPort)
{
    if (i2cPort)
        xSemaphoreTake(i2c2PeripheralMutex, portMAX_DELAY);
    else
        xSemaphoreTake(i2c1PeripheralMutex, portMAX_DELAY);
}

void i2cPeripheralMasterGiveMutex(uint8_t i2cPort)
{
    if (i2cPort)
        xSemaphoreGive(i2c2PeripheralMutex);
    else
        xSemaphoreGive(i2c1PeripheralMutex);
}

int32_t i2cPeripheralMasterWriteToDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t *write_buffer,
                                         size_t write_size, TickType_t ticks_to_wait, uint8_t useMutex)
{
    int32_t result;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    result = i2c_master_write_to_device(i2c_num, device_address, write_buffer, write_size, ticks_to_wait);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return result;
}

int32_t i2cPeripheralMasterWriteReadDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t *write_buffer, size_t write_size,
                                           uint8_t *read_buffer, size_t read_size, TickType_t ticks_to_wait, uint8_t useMutex)
{
    int32_t result;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    result = i2c_master_write_read_device(i2c_num, device_address, write_buffer, write_size, read_buffer, read_size, ticks_to_wait);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return result;
}

int32_t i2cPeripheralMasterReadDevice(i2c_port_t i2c_num, uint8_t device_address, uint8_t *read_buffer, size_t read_size, TickType_t ticks_to_wait, uint8_t useMutex)
{
    int32_t result;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    result = i2c_master_read_from_device(i2c_num, device_address, read_buffer, read_size, ticks_to_wait);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return result;
}

int32_t i2cPeripheralMasterWriteStopDelayReadDevice(i2c_port_t i2c_num, uint8_t device_address, const uint8_t *write_buffer, size_t write_size,
                                                    uint8_t *read_buffer, size_t read_size, TickType_t ticks_to_wait, uint32_t delayMS, uint8_t useMutex)
{
    int32_t result;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    result = i2c_master_write_to_device(i2c_num, device_address, write_buffer, write_size, ticks_to_wait);
    if (!result)
    {
        vTaskDelay(delayMS / portTICK_RATE_MS);
        result = i2c_master_read_from_device(i2c_num, device_address, read_buffer, read_size, ticks_to_wait);
    }

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return result;
}

esp_err_t i2cPeripheralMasterWriteByte(uint8_t i2c_num, uint8_t deviceaddress, uint8_t eeaddress, uint8_t byte, uint8_t useMutex)
{
    esp_err_t ret;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_WRITE_ADDR, 0);
    i2c_master_write_byte(cmd, eeaddress, 0);
    i2c_master_write_byte(cmd, byte, 0);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t eeprom_write(uint8_t i2c_num, uint8_t deviceaddress, uint16_t eeaddress, uint8_t *data, size_t size, uint8_t useMutex)
{
    esp_err_t ret = ESP_OK;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_WRITE_ADDR, 1);
    i2c_master_write_byte(cmd, eeaddress >> 8, 1);
    i2c_master_write_byte(cmd, eeaddress & 0xFF, 1);

    int bytes_remaining = size;
    int current_address = eeaddress;
    int first_write_size = ((EEPROM_PAGE_SIZE - 1) - eeaddress % (EEPROM_PAGE_SIZE - 1)) + 1;
    if (eeaddress % (EEPROM_PAGE_SIZE - 1) == 0 && eeaddress != 0)
        first_write_size = 1;
    if (bytes_remaining <= first_write_size)
    {
        i2c_master_write(cmd, data, bytes_remaining, 1);
    }
    else
    {
        i2c_master_write(cmd, data, first_write_size, 1);
        bytes_remaining -= first_write_size;
        current_address += first_write_size;
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(I2C_0_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
        if (ret != ESP_OK)
        {
            if (useMutex)
                i2cPeripheralMasterGiveMutex(i2c_num);
            return ret;
        }
        while (bytes_remaining > 0)
        {
            cmd = i2c_cmd_link_create();

            // 2ms delay period to allow EEPROM to write the page
            // buffer to memory.
            vTaskDelay(20 / portTICK_PERIOD_MS);

            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_WRITE_ADDR, 1);
            i2c_master_write_byte(cmd, current_address >> 8, 1);
            i2c_master_write_byte(cmd, current_address & 0xFF, 1);
            if (bytes_remaining <= EEPROM_PAGE_SIZE)
            {
                i2c_master_write(cmd, data + (size - bytes_remaining), bytes_remaining, 1);
                bytes_remaining = 0;
            }
            else
            {
                i2c_master_write(cmd, data + (size - bytes_remaining), EEPROM_PAGE_SIZE, 1);
                bytes_remaining -= EEPROM_PAGE_SIZE;
                current_address += EEPROM_PAGE_SIZE;
            }
            i2c_master_stop(cmd);
            ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
            if (ret != ESP_OK)
            {
                if (useMutex)
                    i2cPeripheralMasterGiveMutex(i2c_num);
                return ret;
            }
        }
    }

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

uint8_t i2cPeripheralMasterReadByte(uint8_t i2c_num, uint8_t deviceaddress, uint8_t address, uint8_t useMutex)
{
    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_WRITE_ADDR, 0);
    i2c_master_write_byte(cmd, address, 0);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_READ_ADDR, 0);

    uint8_t data;
    i2c_master_read_byte(cmd, &data, 0);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return data;
}

esp_err_t eeprom_read(uint8_t i2c_num, uint8_t deviceaddress, uint16_t eeaddress, uint8_t *data, size_t size, uint8_t useMutex)
{
    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_WRITE_ADDR, 1);
    i2c_master_write_byte(cmd, eeaddress << 8, 1);
    i2c_master_write_byte(cmd, eeaddress & 0xFF, 1);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | EEPROM_READ_ADDR, 1);

    if (size > 1)
    {
        i2c_master_read(cmd, data, size - 1, 0);
    }
    i2c_master_read_byte(cmd, data + size - 1, 1);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t i2c_master_read_slave(uint8_t i2c_num, uint8_t deviceaddress, uint8_t regaddress, uint8_t *data_rd, size_t size, uint8_t useMutex)
{
    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 0, 1);
    i2c_master_write_byte(cmd, regaddress, 1);

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    //~~~~~~~~~~~~~
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 1, 1);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, 0);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, 1);
    i2c_master_stop(cmd);
    // esp_err_t
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t i2c_master_write_slave(uint8_t i2c_num, uint8_t deviceaddress, uint8_t ctrl, uint8_t *data_wr, size_t size, uint8_t useMutex)
{
    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 0, 1);
    i2c_master_write_byte(cmd, ctrl, 1);
    // i2c_master_write_byte(cmd, data_wr[0], ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, 1);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t i2cMasterWriteDelayBetweenBytes(uint8_t i2c_num, uint8_t deviceaddress, uint8_t *data_wr, size_t size, uint8_t useMutex)
{
    i2c_cmd_handle_t cmd;
    esp_err_t ret;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 0, 1);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (size > 1)
    {
        for (int i = 0; i < (size - 1); i++)
        {
            cmd = i2c_cmd_link_create();
            i2c_master_write_byte(cmd, data_wr[i], 1);
            ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
            i2c_cmd_link_delete(cmd);
        }
    }
    cmd = i2c_cmd_link_create();
    i2c_master_write_byte(cmd, data_wr[size - 1], 0);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t i2cMasterReadDelayBetweenBytes(uint8_t i2c_num, uint8_t deviceaddress, uint8_t regaddress, uint8_t *data_rd, size_t size, uint8_t useMutex)
{
    i2c_cmd_handle_t cmd;
    esp_err_t ret;

    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 0, 1);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_write_byte(cmd, regaddress, 1);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceaddress << 1) | 1, 1);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (size > 1)
    {
        for (int i = 0; i < (size - 1); i++)
        {
            cmd = i2c_cmd_link_create();
            i2c_master_read(cmd, &data_rd[i], 1, 1);
            ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
            i2c_cmd_link_delete(cmd);
        }
    }
    cmd = i2c_cmd_link_create();
    i2c_master_read(cmd, &data_rd[size - 1], 1, 0);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 20 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    return ret;
}

esp_err_t i2cPeripheralTestAddressPresence(uint8_t i2c_num, uint8_t address, uint8_t useMutex)
{
    if (useMutex)
        i2cPeripheralMasterTakeMutex(i2c_num);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | 0, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 50 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (useMutex)
        i2cPeripheralMasterGiveMutex(i2c_num);

    if (ret == 0)
        printf("i2cPeripheralTestAddressPresence I2C: %d   Address 7 bit: %02x \n", i2c_num, address);

    return ret;
}
