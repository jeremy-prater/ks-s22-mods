#include "gradient.h"
#include <esp_system.h>
#include "esp_log.h"

#define TAG "s22_gradient"

static rgb_t* gradient_buffer;

void gradient_init()
{
    ESP_LOGI(TAG, "Initializing color gradients");

    gradient_buffer = malloc(sizeof(rgb_t) * 255);

    hsv_t hsv_start = {
        .hue = HUE_RED,
        .saturation = 0xFF,
        .value = 0xFF,
    };

    hsv_t hsv_end = {
        .hue = HUE_BLUE,
        .saturation = 0xFF,
        .value = 0xFF,
    };

    rgb_fill_gradient_hsv(gradient_buffer, 0, hsv_start, 255, hsv_end, COLOR_FORWARD_HUES);

    ESP_LOGI(TAG, "Initializing color gradients complete!");
}

rgb_t * gradient_get_color(uint8_t index)
{
    return &gradient_buffer[0xFF - index];
}