/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "st7789.h"
#include "driver/gpio.h"

#define ST7789_GPIO_MOSI GPIO_NUM_23
#define ST7789_GPIO_SCLK GPIO_NUM_18
#define ST7789_GPIO_CS GPIO_NUM_19
#define ST7789_GPIO_DC GPIO_NUM_22
#define ST7789_GPIO_RESET GPIO_NUM_19
#define ST7789_GPIO_BACKLIGHT GPIO_NUM_19

#define ST7789_SPI_HOST VSPI_HOST
#define ST7789_DMA_CHAN 2
#define ST7789_DISPLAY_WIDTH 240
#define ST7789_DISPLAY_HEIGHT 240
#define ST7789_BUFFER_SIZE 20

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Hello world!");

    st7789_driver_t display = {
        .pin_mosi = ST7789_GPIO_MOSI,
        .pin_sclk = ST7789_GPIO_SCLK,
        .pin_cs = ST7789_GPIO_CS,
        .pin_dc = ST7789_GPIO_DC,
        .pin_reset = ST7789_GPIO_RESET,
        .pin_backlight = ST7789_GPIO_BACKLIGHT,
        .spi_host = ST7789_SPI_HOST,
        .dma_chan = ST7789_DMA_CHAN,
        .display_width = ST7789_DISPLAY_WIDTH,
        .display_height = ST7789_DISPLAY_HEIGHT,
        .buffer_size = ST7789_BUFFER_SIZE * ST7789_DISPLAY_WIDTH, // 2 buffers with 20 lines
    };

    ESP_ERROR_CHECK(st7789_init(&display));

    st7789_reset(&display);
    st7789_lcd_init(&display);

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), WiFi%s%s%s, ",
             CONFIG_IDF_TARGET,
             chip_info.cores,
             (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
             (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    ESP_LOGI(TAG, "silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        ESP_LOGI(TAG, "Get flash size failed");
        return;
    }

    ESP_LOGI(TAG, "%" PRIu32 "MB %s flash", flash_size / (uint32_t)(1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());

    while (true)
    {
        
    }
}