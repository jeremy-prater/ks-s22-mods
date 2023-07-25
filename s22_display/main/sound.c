#include "sound.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "driver/i2s.h"

#define TAG "sound"

void sound_init()
{
    ESP_LOGI(TAG, "Sound system init : I2S / DAC ready");
}