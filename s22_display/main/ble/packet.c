#include "ble/packet.h"
#include "string.h"

s22_packet s22_packet_new()
{
    s22_packet packet = {
        .header = {0xAA, 0x55},
        .command = 0x00,
        .footer = {0x14, 0x5A, 0x5A}};

    memset(packet.payload, 0, sizeof(packet.payload));

    return packet;
}

s22_packet s22_packet_from_raw_bytes(uint8_t *data)
{
    s22_packet packet;
    memcpy(&packet, data, sizeof(s22_packet));

    return packet;
}