#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "s22_display.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "ble/client.h"
#include "services/gap/ble_svc_gap.h"
#include "ble/utils/utils.h"
#include "s22_model.h"

#define TAG "s22-ble"

static void ble_central_host_task(void *param);
static void wheel_event_task(void *param);

QueueHandle_t wheel_notification_queue;

void init_ble()
{
    ESP_LOGI(TAG, "Initializing BLE stack");

    ESP_ERROR_CHECK(nimble_port_init());

    // Configure stack callbacks
    /* Configure the host. */
    ble_hs_cfg.reset_cb = ble_s22_client_on_reset;
    ble_hs_cfg.sync_cb = ble_s22_client_on_sync;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    int rc;
    /* Initialize data structures to track connected peers. */
    rc = peer_init(MYNEWT_VAL(BLE_MAX_CONNECTIONS), 64, 64, 64);
    assert(rc == 0);

    /* Set the default device name. */
    rc = ble_svc_gap_device_name_set("nimble-ble-spp-client");
    assert(rc == 0);

    xTaskCreate(wheel_event_task, "wheel_event", 2048, NULL, 0, NULL);
    nimble_port_freertos_init(ble_central_host_task);
}

static void ble_central_host_task(void *param)
{
    ESP_LOGI(TAG, "BLE Host Task Started");
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
}

void start_ble_scan()
{
    ESP_LOGI(TAG, "Scanning for S22 : %s", KS_S22_BLE_NAME);
    ble_s22_client_scan();
}

static uint16_t decode_2byte_unsigned(uint8_t *addr)
{
    return addr[0] + (addr[1] << 8);
}

static int16_t decode_2byte_signed(uint8_t *addr)
{
    uint16_t unsigned_value = decode_2byte_unsigned(addr);
    return *(int16_t *)&unsigned_value;
}

static uint32_t decode_4byte_unsigned(uint8_t *addr)
{
    return (addr[0] << 16) +
           (addr[1] << 24) +
           addr[2] +
           (addr[3] << 8);
}

static void wheel_event_task(void *param)
{
    wheel_notification_queue = xQueueCreate(10, sizeof(s22_packet));
    s22_packet packet;
    while (true)
    {
        if (xQueueReceive(wheel_notification_queue, &packet, portTICK_PERIOD_MS * 10))
        {
            switch (packet.command)
            {
            case 0xA9:
                uint16_t voltage = decode_2byte_unsigned(&packet.payload[0]) / 100;
                uint16_t speed = decode_2byte_unsigned(&packet.payload[2]) / 100;
                int16_t current = decode_2byte_signed(&packet.payload[8]) / 100;
                uint16_t temp = decode_2byte_unsigned(&packet.payload[10]) / 100;
                // uint32_t odo = decode_4byte_unsigned(&packet.payload[4]) / 1000;
                // ESP_LOGI(TAG, "Got VSOCT : %u %u %lu %d %u", voltage, speed, odo, current, temp);
                set_speed(speed);
                set_voltage(voltage);
                set_temp(temp);
                set_current(current);
                break;
            case 0xB9:
                // ESP_LOGI(TAG, "Got DUT");
                break;

            case 0xC9:
                // ESP_LOGI(TAG, "Got DUT");
                break;

            case 0xBB:
                // ESP_LOGI(TAG, "Got DUT");
                break;

            case 0xF1:
            case 0xF2:
            case 0xF3:
            case 0xF4:
            case 0xF5:
            case 0xF6:
                break;

            default:
                ESP_LOGW(TAG, "Unknown packet from wheel : 0x%02X", packet.command);
            }
        }
    }
}