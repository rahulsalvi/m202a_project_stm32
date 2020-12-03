#include "serial_msgs.h"

void init_serial_msg(serial_msg_t* m) {
    m->start_signal = 0xFEEDFACE;
    m->seq_number = 0;
}

void set_serial_crc(CRC_HandleTypeDef* hcrc, serial_msg_t* m) {
    uint32_t crc;
    crc = HAL_CRC_Calculate(hcrc, (uint32_t*)m, offsetof(serial_msg_t, crc32));
    m->crc32 = crc;
}
