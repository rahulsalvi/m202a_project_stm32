#include "serial_msgs.h"

void init_serial_msg(serial_msg_t* m) {
    m->start_signal = 0xFEEDFACE;
    m->seq_number = 0;
}

void swap_endianness(uint32_t* src, uint32_t* dst) {
    uint8_t* src8 = (uint8_t*)src;
    uint8_t* dst8 = (uint8_t*)dst;
    dst8[0] = src8[3];
    dst8[1] = src8[2];
    dst8[2] = src8[1];
    dst8[3] = src8[0];
}

void set_serial_crc(CRC_HandleTypeDef* hcrc, serial_msg_t* m) {
    const int crc_process_length = offsetof(serial_msg_t, crc32) / sizeof(uint32_t);
    uint32_t buf[crc_process_length];
    for (int i = 0; i < crc_process_length; i++) {
        swap_endianness(&((uint32_t*)m)[i], &buf[i]);
    }
    uint32_t crc = HAL_CRC_Calculate(hcrc, buf, crc_process_length);
    m->crc32 = crc;
}
