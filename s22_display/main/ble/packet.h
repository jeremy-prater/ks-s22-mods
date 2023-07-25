#ifndef _S22_BLE_PACKET_
#define _S22_BLE_PACKET_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    typedef struct
    {
        uint8_t header[2];
        uint8_t payload[14];
        uint8_t command;
        uint8_t footer[3];
    } s22_packet;

    s22_packet s22_packet_new();
    s22_packet s22_packet_from_raw_bytes(uint8_t *data);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif