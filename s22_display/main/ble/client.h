#ifndef _S22_BLE_CLIENT_
#define _S22_BLE_CLIENT_

#ifdef __cplusplus
extern "C" {
#endif

/* 16 Bit SPP Service UUID */
#define GATT_SPP_SVC_UUID                                  0xABF0

/* 16 Bit SPP Service Characteristic UUID */
#define GATT_SPP_CHR_UUID                                  0xABF1


void ble_s22_client_on_reset(int reason);
void ble_s22_client_on_sync(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif