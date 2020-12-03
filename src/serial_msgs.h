#ifndef SERIAL_MSGS_H
#define SERIAL_MSGS_H

#include "main.h"
#include <stdint.h>

typedef struct {
    uint32_t id;
    uint8_t data[8];
} can_msg_t;

typedef struct {
    uint32_t start_signal;
    uint32_t seq_number;
    uint32_t num_msgs;
    can_msg_t msgs[4];
    uint32_t crc32;
} serial_msg_t;

void init_serial_msg(serial_msg_t* m);
void set_serial_crc(CRC_HandleTypeDef* hcrc, serial_msg_t* m);

#endif // SERIAL_MSGS_H
