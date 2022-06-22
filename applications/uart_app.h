#ifndef UART_APP_H
#define UART_APP_H
#include "rtthread.h"
__packed typedef struct
{
    uint16_t   HEAD;
    uint16_t   LENGTH;//4
    uint8_t    ID[17];
    uint8_t    FRAME_TYPE;
    uint8_t    MES_TYPE;//20
    uint16_t   MES_TIMES;
    uint16_t   MES_COUNT;//4
    uint8_t    WARNING;
    uint8_t    DATAHEAD[2];
    uint8_t    DATA[512];//516
    uint8_t    DATAEND[2];//4
    uint32_t   Reserved_1;//4
    uint32_t   Reserved_2;//4
    uint8_t    HCRC[2];//4
    /* data */
}Video_struct;

void struct_init(Video_struct *video_struct);
void uart_dma_data(uint8_t *buf);
#endif // DEBUG