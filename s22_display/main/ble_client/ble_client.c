#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#define TAG "ble:client"

#define SCAN_DURATION_SECONDS 5

static esp_err_t init_ble_gap(void);
static void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

void ble_scan_task(void *pvParameters);

void start_ble_scan(const char *const ble_name)
{
    xTaskCreatePinnedToCore(&ble_scan_task, "ble_client_scan_task", 6 * 1024, ble_name, 2, NULL, 0);
}

#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(*a))

static const char *ble_gap_evt_names[] = {"ADV_DATA_SET_COMPLETE",
                                          "SCAN_RSP_DATA_SET_COMPLETE",
                                          "SCAN_PARAM_SET_COMPLETE",
                                          "SCAN_RESULT",
                                          "ADV_DATA_RAW_SET_COMPLETE",
                                          "SCAN_RSP_DATA_RAW_SET_COMPLETE",
                                          "ADV_START_COMPLETE",
                                          "SCAN_START_COMPLETE",
                                          "AUTH_CMPL",
                                          "KEY",
                                          "SEC_REQ",
                                          "PASSKEY_NOTIF",
                                          "PASSKEY_REQ",
                                          "OOB_REQ",
                                          "LOCAL_IR",
                                          "LOCAL_ER",
                                          "NC_REQ",
                                          "ADV_STOP_COMPLETE",
                                          "SCAN_STOP_COMPLETE",
                                          "SET_STATIC_RAND_ADDR",
                                          "UPDATE_CONN_PARAMS",
                                          "SET_PKT_LENGTH_COMPLETE",
                                          "SET_LOCAL_PRIVACY_COMPLETE",
                                          "REMOVE_BOND_DEV_COMPLETE",
                                          "CLEAR_BOND_DEV_COMPLETE",
                                          "GET_BOND_DEV_COMPLETE",
                                          "READ_RSSI_COMPLETE",
                                          "UPDATE_WHITELIST_COMPLETE"};

const char *ble_gap_evt_str(uint8_t event)
{
    if (event >= SIZEOF_ARRAY(ble_gap_evt_names))
    {
        return "UNKNOWN";
    }
    return ble_gap_evt_names[event];
}

static esp_ble_scan_params_t s22_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = 0x50,
    .scan_window = 0x30,
    .scan_duplicate = BLE_SCAN_DUPLICATE_ENABLE,
};

static esp_err_t init_ble_gap(void)
{
    esp_err_t ret;

    if ((ret = esp_ble_gap_register_callback(ble_gap_event_handler)) != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ble_gap_register_callback failed: %d", ret);
        return ret;
    }
    return ret;
}

static void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {
    /*
     * SCAN
     * */
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
    {
        ESP_LOGV(TAG, "BLE GAP EVENT SCAN_PARAM_SET_COMPLETE");
        // SEND_BLE_CB();
        break;
    }
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
    {
        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
        switch (scan_result->scan_rst.search_evt)
        {
        case ESP_GAP_SEARCH_INQ_RES_EVT:
        {
            // handle_ble_device_result(&scan_result->scan_rst);
            break;
        }
        case ESP_GAP_SEARCH_INQ_CMPL_EVT:
            ESP_LOGV(TAG, "BLE GAP EVENT SCAN DONE: %d", scan_result->scan_rst.num_resps);
            // SEND_BLE_CB();
            break;
        default:
            break;
        }
        break;
    }
    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
    {
        ESP_LOGV(TAG, "BLE GAP EVENT SCAN CANCELED");
        break;
    }

    /*
     * ADVERTISEMENT
     * */
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        ESP_LOGV(TAG, "BLE GAP ADV_DATA_SET_COMPLETE");
        break;

    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        ESP_LOGV(TAG, "BLE GAP ADV_START_COMPLETE");
        break;

    /*
     * AUTHENTICATION
     * */
    case ESP_GAP_BLE_AUTH_CMPL_EVT:
        if (!param->ble_security.auth_cmpl.success)
        {
            ESP_LOGE(TAG, "BLE GAP AUTH ERROR: 0x%x", param->ble_security.auth_cmpl.fail_reason);
        }
        else
        {
            ESP_LOGI(TAG, "BLE GAP AUTH SUCCESS");
        }
        break;

    case ESP_GAP_BLE_KEY_EVT: // shows the ble key info share with peer device to the user.
        ESP_LOGI(TAG, "BLE GAP KEY type = %d", param->ble_security.ble_key.key_type);
        break;

    case ESP_GAP_BLE_PASSKEY_NOTIF_EVT: // ESP_IO_CAP_OUT
        // The app will receive this evt when the IO has Output capability and the peer device IO has Input capability.
        // Show the passkey number to the user to input it in the peer device.
        ESP_LOGI(TAG, "BLE GAP PASSKEY_NOTIF passkey:%" PRIu32, param->ble_security.key_notif.passkey);
        break;

    case ESP_GAP_BLE_NC_REQ_EVT: // ESP_IO_CAP_IO
        // The app will receive this event when the IO has DisplayYesNO capability and the peer device IO also has DisplayYesNo capability.
        // show the passkey number to the user to confirm it with the number displayed by peer device.
        ESP_LOGI(TAG, "BLE GAP NC_REQ passkey:%" PRIu32, param->ble_security.key_notif.passkey);
        // esp_ble_confirm_reply(param->ble_security.key_notif.bd_addr, true);
        break;

    case ESP_GAP_BLE_PASSKEY_REQ_EVT: // ESP_IO_CAP_IN
        // The app will receive this evt when the IO has Input capability and the peer device IO has Output capability.
        // See the passkey number on the peer device and send it back.
        ESP_LOGI(TAG, "BLE GAP PASSKEY_REQ");
        // esp_ble_passkey_reply(param->ble_security.ble_req.bd_addr, true, 1234);
        break;

    case ESP_GAP_BLE_SEC_REQ_EVT:
        ESP_LOGI(TAG, "BLE GAP SEC_REQ");
        // Send the positive(true) security response to the peer device to accept the security request.
        // If not accept the security request, should send the security response with negative(false) accept value.
        // esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;

    default:
        ESP_LOGV(TAG, "BLE GAP EVENT %s", ble_gap_evt_str(event));
        break;
    }
}

void ble_scan_task(void *pvParameters)
{
    init_ble_gap();
    ESP_LOGI(TAG, "Scanning for S22 : %s", (const char *)pvParameters);

    esp_ble_gap_set_scan_params(&s22_scan_params);

    esp_ble_gap_start_scanning(5);

    vTaskDelete(NULL);
}