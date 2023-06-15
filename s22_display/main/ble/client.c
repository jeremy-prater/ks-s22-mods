#include "esp_log.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "ble/utils/utils.h"
#include "ble/client.h"
#include "s22_display.h"
#include "s22_model.h"
#include "ui/ui.h"

#define PEER_ADDR_VAL_SIZE 6

#define TAG "ble:client"

static int ble_s22_client_gap_event(struct ble_gap_event *event, void *arg);
QueueHandle_t spp_common_uart_queue = NULL;

uint16_t attribute_handle[CONFIG_BT_NIMBLE_MAX_CONNECTIONS + 1];
static ble_addr_t connected_addr[CONFIG_BT_NIMBLE_MAX_CONNECTIONS + 1];

static void goto_riding_screen()
{
    lv_event_t event;
    event.code = LV_EVENT_READY;
    ui_event_BootConnect(&event);
    set_speed(20);
    set_pwm(20);
}

static void goto_connect_screen()
{
    lv_event_t event;
    event.code = LV_EVENT_CANCEL;
    ui_event_BootConnect(&event);
}

static void
ble_s22_client_set_handle(const struct peer *peer)
{
    const struct peer_chr *chr;
    chr = peer_chr_find_uuid(peer,
                             BLE_UUID16_DECLARE(GATT_SPP_SVC_UUID),
                             BLE_UUID16_DECLARE(GATT_SPP_CHR_UUID));
    attribute_handle[peer->conn_handle] = chr->chr.val_handle;

    char ble_uuid_str[BLE_UUID_STR_LEN];
    ble_uuid_to_str(&chr->chr.uuid, ble_uuid_str);
    ESP_LOGI(TAG, "Found characteristic UUID : %s with handle %d", ble_uuid_str, peer->conn_handle);

    goto_riding_screen();
}

/**
 * Called when service discovery of the specified peer has completed.
 */
static void
ble_s22_client_on_disc_complete(const struct peer *peer, int status, void *arg)
{
    if (status != 0)
    {
        /* Service discovery failed.  Terminate the connection. */
        MODLOG_DFLT(ERROR, "Error: Service discovery failed; status=%d "
                           "conn_handle=%d\n",
                    status, peer->conn_handle);
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    /* Service discovery has completed successfully.  Now we have a complete
     * list of services, characteristics, and descriptors that the peer
     * supports.
     */
    MODLOG_DFLT(INFO, "Service discovery complete; status=%d "
                      "conn_handle=%d\n",
                status, peer->conn_handle);

    struct peer_svc *svc;
    struct peer_chr *chr;

    // SLIST_FOREACH(svc, &peer->svcs, next) {
    //     SLIST_FOREACH(chr, &svc->chrs, next) {
    //     }
    // }
    ble_s22_client_set_handle(peer);
#if CONFIG_BT_NIMBLE_MAX_CONNECTIONS > 1
    ble_s22_client_scan();
#endif
}

/**
 * Initiates the GAP general discovery procedure.
 */
void ble_s22_client_scan(void)
{
    ESP_LOGI(TAG, "Starting scan");
    uint8_t own_addr_type;
    struct ble_gap_disc_params disc_params;
    int rc;

    /* Figure out address to use while advertising (no privacy for now) */
    rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0)
    {
        MODLOG_DFLT(ERROR, "error determining address type; rc=%d\n", rc);
        return;
    }

    // Tell the controller to filter duplicates; we don't want to process
    // repeated advertisements from the same device.
    disc_params.filter_duplicates = 1;

    // Do an active BLE scan where we discover names/attributes
    disc_params.passive = 0;

    // Use defaults for the rest of the parameters.
    disc_params.itvl = 0;
    disc_params.window = 0;
    disc_params.filter_policy = 0;
    disc_params.limited = 0;

    rc = ble_gap_disc(own_addr_type, BLE_HS_FOREVER, &disc_params,
                      ble_s22_client_gap_event, NULL);
    if (rc != 0)
    {
        MODLOG_DFLT(ERROR, "Error initiating GAP discovery procedure; rc=%d\n",
                    rc);
    }
}

/**
 * Indicates whether we should try to connect to the sender of the specified
 * advertisement.  The function returns a positive result if the device
 * advertises connectability and support for the Alert Notification service.
 */
static int
ble_s22_client_should_connect(const struct ble_gap_disc_desc *disc)
{
    struct ble_hs_adv_fields fields;
    int rc;
    int i;

    rc = ble_addr_cmp(&disc->addr, (const ble_addr_t *)&s22_addr);
    if (rc != 0)
    {
        return 0;
    }
    ESP_LOGI(TAG, "addr_type=%d addr=%02x:%02x:%02x:%02x:%02x:%02x -> %d",
             disc->addr.type,
             disc->addr.val[5],
             disc->addr.val[4],
             disc->addr.val[3],
             disc->addr.val[2],
             disc->addr.val[1],
             disc->addr.val[0],
             rc);

    /* Check if device is already connected or not */
    for (i = 0; i <= CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++)
    {
        if (memcmp(&connected_addr[i].val, disc->addr.val, PEER_ADDR_VAL_SIZE) == 0)
        {
            MODLOG_DFLT(DEBUG, "Device already connected");
            return 0;
        }
    }

    /* The device has to be advertising connectability. */
    if (disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_ADV_IND &&
        disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_DIR_IND)
    {
        // ESP_LOGW(TAG, "unknown scan type : %d", disc->event_type);
        // return 0;
    }

    rc = ble_hs_adv_parse_fields(&fields, disc->data, disc->length_data);
    if (rc != 0)
    {
        ESP_LOGW(TAG, "ble_hs_adv_parse_fields failed : %d", rc);
    }

    if (fields.name != NULL && fields.name_is_complete != 0)
    {
        char *name = malloc(fields.name_len + 1);
        strncpy(name, (const char *)fields.name, fields.name_len);
        name[fields.name_len] = 0x00;
        ESP_LOGI(TAG, "Found BLE : %s", name);
        size_t len = strnlen(name, strlen(KS_S22_BLE_NAME));
        int match = strncmp(name, KS_S22_BLE_NAME, len);
        free(name);
        return match == 0;
    }

    // ESP_LOGI(TAG, "uuid count : 128-bit %d : 32-bit %d : 16-bit %d", fields.num_uuids128, fields.num_uuids32, fields.num_uuids16);

    // for (i = 0; i < fields.num_uuids16; i++)
    // {
    //     uint16_t uuid = ble_uuid_u16(&fields.uuids16[i].u);
    //     ESP_LOGI(TAG, "UUID %08x %08x", uuid, GATT_SPP_SVC_UUID);
    //     if (uuid == GATT_SPP_SVC_UUID)
    //     {
    //         return 1;
    //     }
    // }
    return 0;
}

/**
 * Connects to the sender of the specified advertisement of it looks
 * interesting.  A device is "interesting" if it advertises connectability and
 * support for the Alert Notification service.
 */
static void
ble_s22_client_connect_if_interesting(const struct ble_gap_disc_desc *disc)
{
    uint8_t own_addr_type;
    int rc;

    /* Don't do anything if we don't care about this advertiser. */
    if (!ble_s22_client_should_connect(disc))
    {
        return;
    }

    /* Scanning must be stopped before a connection can be initiated. */
    rc = ble_gap_disc_cancel();
    if (rc != 0)
    {
        MODLOG_DFLT(DEBUG, "Failed to cancel scan; rc=%d\n", rc);
        return;
    }

    /* Figure out address to use for connect (no privacy for now) */
    rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0)
    {
        MODLOG_DFLT(ERROR, "error determining address type; rc=%d\n", rc);
        return;
    }

    /* Try to connect the the advertiser.  Allow 30 seconds (30000 ms) for
     * timeout.
     */

    rc = ble_gap_connect(own_addr_type, &disc->addr, 30000, NULL,
                         ble_s22_client_gap_event, NULL);
    if (rc != 0)
    {
        MODLOG_DFLT(ERROR, "Error: Failed to connect to device; addr_type=%d "
                           "addr=%s; rc=%d\n",
                    disc->addr.type, addr_str(disc->addr.val), rc);
        return;
    }
}

/**
 * The nimble host executes this callback when a GAP event occurs.  The
 * application associates a GAP event callback with each connection that is
 * established.  ble_s22_client uses the same callback for all connections.
 *
 * @param event                 The event being signalled.
 * @param arg                   Application-specified argument; unused by
 *                                  ble_s22_client.
 *
 * @return                      0 if the application successfully handled the
 *                                  event; nonzero on failure.  The semantics
 *                                  of the return code is specific to the
 *                                  particular GAP event being signalled.
 */
static int
ble_s22_client_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_gap_conn_desc desc;
    struct ble_hs_adv_fields fields;
    int rc;

    switch (event->type)
    {
    case BLE_GAP_EVENT_DISC:
        rc = ble_hs_adv_parse_fields(&fields, event->disc.data,
                                     event->disc.length_data);
        if (rc != 0)
        {
            ESP_LOGW(TAG, "Failed to parse advertisement fields : %02X:%02X:%02X:%02X:%02X:%02X",
                     event->disc.addr.val[5],
                     event->disc.addr.val[4],
                     event->disc.addr.val[3],
                     event->disc.addr.val[2],
                     event->disc.addr.val[1],
                     event->disc.addr.val[0]);
            // return 0;
        }

        /* An advertisment report was received during GAP discovery. */
        print_adv_fields(&fields);

        /* Try to connect to the advertiser if it looks interesting. */
        ble_s22_client_connect_if_interesting(&event->disc);
        return 0;

    case BLE_GAP_EVENT_CONNECT:
        /* A new connection was established or a connection attempt failed. */
        if (event->connect.status == 0)
        {
            /* Connection successfully established. */
            MODLOG_DFLT(INFO, "Connection established ");
            rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
            assert(rc == 0);
            memcpy(&connected_addr[event->connect.conn_handle].val, desc.peer_id_addr.val,
                   PEER_ADDR_VAL_SIZE);
            print_conn_desc(&desc);
            MODLOG_DFLT(INFO, "\n");

            /* Remember peer. */
            rc = peer_add(event->connect.conn_handle);
            if (rc != 0)
            {
                MODLOG_DFLT(ERROR, "Failed to add peer; rc=%d\n", rc);
                return 0;
            }

            /* Perform service discovery. */
            rc = peer_disc_all(event->connect.conn_handle,
                               ble_s22_client_on_disc_complete, NULL);
            if (rc != 0)
            {
                MODLOG_DFLT(ERROR, "Failed to discover services; rc=%d\n", rc);
                return 0;
            }
        }
        else
        {
            /* Connection attempt failed; resume scanning. */
            MODLOG_DFLT(ERROR, "Error: Connection failed; status=%d\n",
                        event->connect.status);
            ble_s22_client_scan();
            goto_connect_screen();
        }

        return 0;

    case BLE_GAP_EVENT_DISCONNECT:
        /* Connection terminated. */
        MODLOG_DFLT(INFO, "disconnect; reason=%d ", event->disconnect.reason);
        print_conn_desc(&event->disconnect.conn);
        MODLOG_DFLT(INFO, "\n");

        /* Forget about peer. */
        memset(&connected_addr[event->disconnect.conn.conn_handle].val, 0, PEER_ADDR_VAL_SIZE);
        attribute_handle[event->disconnect.conn.conn_handle] = 0;
        peer_delete(event->disconnect.conn.conn_handle);

        /* Resume scanning. */
        ble_s22_client_scan();
        goto_connect_screen();

        return 0;

    case BLE_GAP_EVENT_DISC_COMPLETE:
        MODLOG_DFLT(INFO, "discovery complete; reason=%d\n",
                    event->disc_complete.reason);
        return 0;

    case BLE_GAP_EVENT_NOTIFY_RX:
        /* Peer sent us a notification or indication. */
        MODLOG_DFLT(INFO, "received %s; conn_handle=%d attr_handle=%d "
                          "attr_len=%d\n",
                    event->notify_rx.indication ? "indication" : "notification",
                    event->notify_rx.conn_handle,
                    event->notify_rx.attr_handle,
                    OS_MBUF_PKTLEN(event->notify_rx.om));

        /* Attribute data is contained in event->notify_rx.om. Use
         * `os_mbuf_copydata` to copy the data received in notification mbuf */
        return 0;

    case BLE_GAP_EVENT_MTU:
        MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d cid=%d mtu=%d\n",
                    event->mtu.conn_handle,
                    event->mtu.channel_id,
                    event->mtu.value);
        return 0;

    default:
        return 0;
    }
}

void ble_s22_client_on_reset(int reason)
{
    MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
}

void ble_s22_client_on_sync(void)
{
    int rc;

    /* Make sure we have proper identity address set (public preferred) */
    rc = ble_hs_util_ensure_addr(0);
    assert(rc == 0);
}

// void ble_client_uart_task(void *pvParameters)
// {
//     ESP_LOGI(tag, "BLE client UART task started\n");
//     int rc;
//     int i;
//     uart_event_t event;
//     for (;;) {
//         //Waiting for UART event.
//         if (xQueueReceive(spp_common_uart_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
//             switch (event.type) {
//             //Event of UART receving data
//             case UART_DATA:
//                 if (event.size) {

//                     /* Writing characteristics */
//                     uint8_t *temp = NULL;
//                     temp = (uint8_t *)malloc(sizeof(uint8_t) * event.size);
//                     if (temp == NULL) {
//                         ESP_LOGE(tag, "malloc failed,%s L#%d\n", __func__, __LINE__);
//                         break;
//                     }
//                     memset(temp, 0x0, event.size);
//                     uart_read_bytes(UART_NUM_0, temp, event.size, portMAX_DELAY);
//                     for ( i = 0; i <= CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++) {
//                         if (attribute_handle[i] != 0) {
//                             rc = ble_gattc_write_flat(i, attribute_handle[i], temp, event.size, NULL, NULL);
//                             if (rc == 0) {
//                                 ESP_LOGI(tag, "Write in uart task success!");
//                             } else {
//                                 ESP_LOGI(tag, "Error in writing characteristic rc=%d", rc);
//                             }
//                             vTaskDelay(10);
//                         }
//                     }
//                     free(temp);
//                 }
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
//     vTaskDelete(NULL);

// }
