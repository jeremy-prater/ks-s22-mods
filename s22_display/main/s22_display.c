/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "s22_display.h"

#include "lvgl.h"
#include "lvgl_helpers.h"

#if LV_USE_DEMO_WIDGETS
#include "widgets/lv_demo_widgets.h"
#else
#include "ui/ui.h"
#endif

#include "ble/ble.h"
#include "host/ble_hs.h"

#define TAG "s22-main"
#define LV_TICK_PERIOD_MS 1

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void create_ui_application(void);

const ble_addr_t s22_addr_real = {
    .type = BLE_ADDR_PUBLIC,
    .val = {
        0x39,
        0xE3,
        0x01,
        0xC3,
        0xC5,
        0xC2
        }};

const ble_addr_t s22_addr_bad_board = {
    .type = BLE_ADDR_PUBLIC,
    .val = {
        0x76,
        0xF1,
        0x01,
        0xC3,
        0xC5,
        0xC2
        }};

const ble_addr_t s22_addr_sim = {
    .type = BLE_ADDR_PUBLIC,
    .val = {
        0x13,
        0x71,
        0xDA,
        0x7D,
        0x1A,
        0x00
        }};

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init_ble();

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, 1);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

static void guiTask(void *pvParameter)
{

    (void)pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    lv_color_t *buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);

    static lv_disp_draw_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

    disp_drv.draw_buf = &disp_buf;
    lv_disp_t *display = lv_disp_drv_register(&disp_drv);

#if defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT
    lv_disp_set_rotation(display, LV_DISP_ROT_NONE);
#elif defined CONFIG_DISPLAY_ORIENTATION_LANDSCAPE
    lv_disp_set_rotation(display, LV_DISP_ROT_90);
#elif defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT_INVERTED
    lv_disp_set_rotation(display, LV_DISP_ROT_180);
#elif defined CONFIG_DISPLAY_ORIENTATION_LANDSCAPE_INVERTED
    lv_disp_set_rotation(display, LV_DISP_ROT_270);
#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000 * S22_DISPLAY_FPS_MS));

    /* Create the demo application */
#if LV_USE_DEMO_WIDGETS
    lv_demo_widgets();
#else
    create_ui_application();
#endif

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    /* A task should NEVER return */
    free(buf1);
    free(buf2);
    vTaskDelete(NULL);
}

static void btn_event_cb(lv_obj_t *btn, lv_event_t event)
{
    if (event.code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t *label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

static void create_ui_application(void)
{
    ui_init();
}

static void lv_tick_task(void *arg)
{
    (void)arg;

    lv_tick_inc(LV_TICK_PERIOD_MS * S22_DISPLAY_FPS_MS);
}