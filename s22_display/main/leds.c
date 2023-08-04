#include "leds.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "s22_model.h"

#define TAG "M5_LEDS"

static rgb_t led_colors[NUM_M5_LEDS];

static led_strip_t led_strip = {
    .type = LED_STRIP_SK6812,
    .is_rgbw = false,
#ifdef LED_STRIP_BRIGHTNESS
    .brightness = 255,
#endif
    .length = NUM_M5_LEDS,
    .gpio = M5_LED_GPIO,
    .channel = RMT_CHANNEL_0,
    .buf = NULL,
};

static led_scene_t led_scene = LED_SCENE_BLE_SCANNING;
static void led_task(void *pvParameters);
static void leds_update();
static void scene_update();

static void leds_scene_ble_scan(TickType_t delta_time);
static void leds_scene_riding(TickType_t delta_time);

void leds_init()
{
    led_strip_install();
    memset(led_colors, 0x00, sizeof(rgb_t) * NUM_M5_LEDS);
    ESP_ERROR_CHECK(led_strip_init(&led_strip));

    xTaskCreatePinnedToCore(led_task, "leds", 4096, NULL, 0, NULL, 1);

    ESP_LOGI(TAG, "Initialized LED strips");
}

void leds_set_colors(rgb_t *colors)
{
    memcpy(led_colors, colors, sizeof(rgb_t) * NUM_M5_LEDS);
}

void leds_set_color(uint8_t led, rgb_t color)
{
    if (led >= NUM_M5_LEDS)
    {
        return;
    }
    led_colors[led] = color;
}

static void led_task(void *pvParameters)
{
    while (1)
    {
        scene_update();

        leds_update();

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

static void leds_update()
{
    led_strip_set_pixels(&led_strip, 0, NUM_M5_LEDS, led_colors);
    led_strip_wait(&led_strip, portMAX_DELAY);
    ESP_ERROR_CHECK(led_strip_flush(&led_strip));
}

static void scene_update()
{
    static TickType_t old_time = portMAX_DELAY;
    TickType_t time = xTaskGetTickCount();

    if (old_time > time)
    {
        old_time = time;
        return;
    }

    TickType_t delta_time = (time - old_time) * 10;
    old_time = time;

    // ESP_LOGI(TAG, "Scene delta-time : %lu", delta_time);

    switch (led_scene)
    {
    case LED_SCENE_BLE_SCANNING:
        leds_scene_ble_scan(delta_time);
        break;
    case LED_SCENE_RIDING:
        leds_scene_riding(delta_time);
        break;
    case LED_SCENE_WARNING:
        break;
    case LED_SCENE_DANGER:
        break;
    }
}

void leds_set_scene(led_scene_t scene)
{
    ESP_LOGI(TAG, "Setting LED Scene : %d", scene);
    led_scene = scene;
}

static void leds_scene_ble_scan(TickType_t delta_time)
{
    const uint8_t step = 63;
    static uint8_t offset = 0;

    for (uint8_t index = 0; index < 5; index++)
    {
        rgb_t color = {
            .r = 0,
            .g = 0,
            .b = ((index + offset) % 5) * step,
        };
        leds_set_color(index, color);
        leds_set_color(index + 5, color);
    }
    offset++;
    if (offset == 5)
    {
        offset = 0;
    }
}

static void leds_scene_riding(TickType_t delta_time)
{
    rgb_t * speed_color = get_speed_color();
    rgb_t * pwm_color = get_power_color();
    for (uint8_t index = 0; index < 5; index++)
    {
        leds_set_color(index + 5, *speed_color);
        leds_set_color(index, *pwm_color);
    }
}
