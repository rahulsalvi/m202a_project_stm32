#include "main.h"
#include "SEGGER_RTT.h"

#include "serial_msgs.h"

extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart5;
extern CRC_HandleTypeDef hcrc;

CAN_RxHeaderTypeDef rx_hdr;
CAN_FilterTypeDef filter;

serial_msg_t ser_msg;

void setup() {
    init_serial_msg(&ser_msg);

    filter.FilterActivation = CAN_FILTER_ENABLE;
    filter.FilterBank = 0;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan1, &filter);

    HAL_CAN_Start(&hcan1);
    HAL_Delay(500);
    SEGGER_RTT_printf(0, "started CAN1: %d\n", READ_REG(hcan1.Instance->BTR));
}

void loop() {
    int msgs_available = HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0);
    while (msgs_available > 0) {
        ser_msg.num_msgs = 0;
        for (int i = 0; i < 4 && i < msgs_available; i++) {
            HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_hdr, ser_msg.msgs[i].data);
            ser_msg.msgs[i].id = rx_hdr.ExtId;
            ser_msg.num_msgs++;
        }
        set_serial_crc(&hcrc, &ser_msg);
        SEGGER_RTT_printf(0, "CRC: %d\n", ser_msg.crc32);
        SEGGER_RTT_printf(0, "%d CAN msgs sent\n", ser_msg.num_msgs);
        HAL_UART_Transmit(&huart5, (uint8_t*)&ser_msg, sizeof(ser_msg), HAL_MAX_DELAY);
        ser_msg.seq_number++;
        msgs_available -= ser_msg.num_msgs;
    }
    HAL_Delay(10);
}
