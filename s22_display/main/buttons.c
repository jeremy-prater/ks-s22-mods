#include <button.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "string.h"
#include "ble/client.h"
#include "ui/ui.h"

#define TAG "buttons"

static void button_left_event(button_t *btn, button_state_t state);
static void button_middle_event(button_t *btn, button_state_t state);
static void button_right_event(button_t *btn, button_state_t state);

static button_t button_left = {
    .gpio = GPIO_NUM_39,
    .internal_pull = false,
    .pressed_level = 0,
    .autorepeat = false,
    .callback = button_left_event,
};

static button_t button_middle = {
    .gpio = GPIO_NUM_38,
    .internal_pull = false,
    .pressed_level = 0,
    .autorepeat = false,
    .callback = button_middle_event,
};

static button_t button_right = {
    .gpio = GPIO_NUM_37,
    .internal_pull = false,
    .pressed_level = 0,
    .autorepeat = false,
    .callback = button_right_event,
};

static void button_left_event(button_t *btn, button_state_t state)
{
    switch (state)
    {
    case BUTTON_PRESSED:
        break;
    case BUTTON_RELEASED:
        break;
    case BUTTON_CLICKED:
        ESP_LOGI(TAG, "Left button clicked");
        if (lv_scr_act() == ui_BootConnect)
        {
            ble_select_next_euc(true);
        }
        break;
    case BUTTON_PRESSED_LONG:
        ESP_LOGI(TAG, "Left button long press");
        break;
    }
}

static void button_middle_event(button_t *btn, button_state_t state)
{
    switch (state)
    {
    case BUTTON_PRESSED:
        break;
    case BUTTON_RELEASED:
        break;
    case BUTTON_CLICKED:
        ESP_LOGI(TAG, "Middle button clicked");
        if (lv_scr_act() == ui_BootConnect)
        {
            ble_connect_selected_euc();
        }
        break;
    case BUTTON_PRESSED_LONG:
        ESP_LOGI(TAG, "Middle button long press");
        break;
    }
}

static void button_right_event(button_t *btn, button_state_t state)
{
    switch (state)
    {
    case BUTTON_PRESSED:
        break;
    case BUTTON_RELEASED:
        break;
    case BUTTON_CLICKED:
        ESP_LOGI(TAG, "Right button clicked");
        if (lv_scr_act() == ui_BootConnect)
        {
            ble_select_next_euc(false);
        }

        break;
    case BUTTON_PRESSED_LONG:
        ESP_LOGI(TAG, "Right button long press");
        break;
    }
}

void buttons_init()
{
    ESP_LOGI(TAG, "Initializing buttons");
    ESP_ERROR_CHECK(button_init(&button_left));
    ESP_ERROR_CHECK(button_init(&button_middle));
    ESP_ERROR_CHECK(button_init(&button_right));
}
