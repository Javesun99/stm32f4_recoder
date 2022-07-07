#ifndef UART_APP_H
#define UART_APP_H
#include "rtthread.h"
__packed typedef struct
{
    uint8_t   HEAD[2];
    uint8_t   LENGTH[2];//4
    uint8_t    FRAME_TYPE;
    uint8_t    MES_TYPE;//20
    uint8_t   MES_TIMES[2];
    uint8_t   MES_COUNT[2];//4
    uint8_t    WARNING;
    uint8_t    DATAHEAD[2];
    uint8_t    DATA[512];//516
    uint8_t    DATAEND[2];//4
    uint32_t   Reserved_1;//4
    uint32_t   Reserved_2;//4
    uint8_t    LCRC;
    uint8_t    HCRC;
    /* data */
}Video_struct;




void struct_init(Video_struct *video_struct);
void uart_dma_init(void);
void dma_data(uint8_t *buf);
void data_parsing(void);
void sturct_trans(uint8_t *buf);
#endif // DEBUG