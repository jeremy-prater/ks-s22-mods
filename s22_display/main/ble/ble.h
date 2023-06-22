#ifndef _S22_BLE_
#define _S22_BLE_

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

void init_ble();
void start_ble_scan();

extern QueueHandle_t wheel_notification_queue;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif