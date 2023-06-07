#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "s22_display.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "ble/client.h"
#include "services/gap/ble_svc_gap.h"
#include "ble/utils/utils.h"

#define TAG "s22-ble"

static void ble_central_host_task(void *param);

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