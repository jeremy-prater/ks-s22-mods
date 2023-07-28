#include "battery.h"
#include "driver/i2c.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"

#define TAG "m5_pmic"

// Ported from https://github.com/m5stack/M5Stack/blob/master/src/utility/Power.cpp

#define IP5306_ADDR (0x75 << 1)
#define IP5306_REG_SYS_CTL0 (0x00)
#define IP5306_REG_SYS_CTL1 (0x01)
#define IP5306_REG_SYS_CTL2 (0x02)
#define IP5306_REG_CHG_CTL0 (0x20)
#define IP5306_REG_CHG_CTL1 (0x21)
#define IP5306_REG_CHG_CTL2 (0x22)
#define IP5306_REG_CHG_CTL3 (0x23)
#define IP5306_REG_CHG_DIG (0x24)
#define IP5306_REG_READ0 (0x70)
#define IP5306_REG_READ1 (0x71)
#define IP5306_REG_READ3 (0x78)

//- REG_CTL0
#define BOOST_ENABLE_BIT (0x20)
#define CHARGE_OUT_BIT (0x10)
#define BOOT_ON_LOAD_BIT (0x04)
#define BOOST_OUT_BIT (0x02)
#define BOOST_BUTTON_EN_BIT (0x01)

//- REG_CTL1
#define BOOST_SET_BIT (0x80)
#define WLED_SET_BIT (0x40)
#define SHORT_BOOST_BIT (0x20)
#define VIN_ENABLE_BIT (0x04)

//- REG_CTL2
#define SHUTDOWNTIME_MASK (0x0c)
#define SHUTDOWNTIME_64S (0x0c)
#define SHUTDOWNTIME_32S (0x04)
#define SHUTDOWNTIME_16S (0x08)
#define SHUTDOWNTIME_8S (0x00)

//- REG_READ0
#define CHARGE_ENABLE_BIT (0x08)

//- REG_READ1
#define CHARGE_FULL_BIT (0x08)

//- REG_READ2
#define LIGHT_LOAD_BIT (0x20)
#define LOWPOWER_SHUTDOWN_BIT (0x01)

//- CHG
#define CURRENT_100MA (0x01 << 0)
#define CURRENT_200MA (0x01 << 1)
#define CURRENT_400MA (0x01 << 2)
#define CURRENT_800MA (0x01 << 3)
#define CURRENT_1600MA (0x01 << 4)

#define BAT_4_2V (0x00)
#define BAT_4_3V (0x01)
#define BAT_4_3_5V (0x02)
#define BAT_4_4V (0x03)

#define CHG_CC_BIT (0x20)

static m5_battery_info_t pmic_info;

static void pmic_write_byte(uint8_t reg, uint8_t value)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, IP5306_ADDR | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, value, true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, portMAX_DELAY));
    i2c_cmd_link_delete(cmd);
}

static uint8_t pmic_read_byte(uint8_t reg)
{
    uint8_t data;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, IP5306_ADDR | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg, true));

    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, IP5306_ADDR | I2C_MASTER_READ, true));
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, &data, true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, portMAX_DELAY));
    i2c_cmd_link_delete(cmd);

    return data;
}

static bool pmic_is_charging()
{
    uint8_t charging = pmic_read_byte(IP5306_REG_READ0);
    return charging & CHARGE_ENABLE_BIT;
}

static bool pmic_is_charging_full()
{
    uint8_t charging = pmic_read_byte(IP5306_REG_READ1);
    return charging & CHARGE_FULL_BIT;
}

static uint8_t pmic_get_battery_level()
{
    uint8_t level = pmic_read_byte(IP5306_REG_READ3);
    switch (level & 0xF0)
    {
    case 0x00:
        return 100;
    case 0x80:
        return 75;
    case 0xC0:
        return 50;
    case 0xE0:
        return 25;
    default:
        return 0;
    }
}

static void pmic_update_task(void *param)
{
    while (true)
    {
        bool is_charging = pmic_is_charging();
        bool is_full = pmic_is_charging_full();
        uint8_t level = pmic_get_battery_level();

        ESP_LOGI(TAG, "Update PMIC data : charging %d", is_charging);
        ESP_LOGI(TAG, "Update PMIC data : full %d", is_full);
        ESP_LOGI(TAG, "Update PMIC data : level %d", level);

        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 second
    }
}

void pmic_init()
{
    ESP_LOGI(TAG, "Initalizing I2C / PMIC");

    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = GPIO_NUM_22,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000, // 100 khz
        .clk_flags = 0,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_conf));

    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    // Set charge current rate to 400ma
    uint8_t current = pmic_read_byte(IP5306_REG_CHG_DIG);
    pmic_write_byte(IP5306_REG_CHG_DIG, (current & 0xE0) | CURRENT_400MA);

    // Set max charge voltage to 4.2vdc
    uint8_t voltage = pmic_read_byte(IP5306_REG_CHG_CTL0);
    pmic_write_byte(IP5306_REG_CHG_CTL0, (voltage & 0xFC) | BAT_4_2V);

    // End charge current 200ma
    uint8_t end_current = pmic_read_byte(IP5306_REG_CHG_CTL1);
    pmic_write_byte(IP5306_REG_CHG_CTL1, (end_current & 0x3F) | 0x00);

    // Add volt 28mv
    uint8_t voltage_offset = pmic_read_byte(IP5306_REG_CHG_CTL2);
    pmic_write_byte(IP5306_REG_CHG_CTL2, (voltage_offset & 0xFC) | 0x02);

    // Vin charge CC
    uint8_t charge_cc = pmic_read_byte(IP5306_REG_CHG_CTL3);
    pmic_write_byte(IP5306_REG_CHG_CTL3, (charge_cc & 0xDF) | 0x20);

    xTaskCreatePinnedToCore(pmic_update_task, "pmic_update", 4096, NULL, 0, NULL, 1);
}

m5_battery_info_t *pmic_get_info()
{
    return &pmic_info;
}
