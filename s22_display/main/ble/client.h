#ifndef _S22_BLE_CLIENT_
#define _S22_BLE_CLIENT_

#ifdef __cplusplus
extern "C"
{
#endif

#include "nimble/ble.h"
#include "ble/packet.h"

#define MAX_NUM_DISCOVERED_EUC 16

/* 16 Bit SPP Service UUID */
#define GATT_SPP_SVC_UUID 0xFFE0

/* 16 Bit SPP Service Characteristic UUID */
#define GATT_SPP_CHR_UUID 0xFFE1

    void ble_s22_client_on_reset(int reason);
    void ble_s22_client_on_sync(void);

    void ble_s22_client_scan(void);

    int send_packet(s22_packet *packet);

    void ble_select_next_euc(bool direction);
    void ble_connect_selected_euc();

    extern const ble_addr_t s22_addr_real;
    extern const ble_addr_t s22_addr_bad_board;
    extern const ble_addr_t s22_addr_sim;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif