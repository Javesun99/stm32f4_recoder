#include "uart_app.h"
#include "drv_usart.h"
/*
 * 程序清单：这是一个串口设备 开启 DMA 模式后使用例程
 * 例程导出了 uart_dma_sample 命令到控制终端
 * 命令调用格式：uart_dma_sample uart1
 * 命令解释：命令第二个参数是要使用的串口设备名称，为空则使用默认的串口设备
 * 程序功能：通过串口输出字符串 "hello RT-Thread!"，并通过串口输出接收到的数据，然后打印接收到的数据。
*/

#include <rtthread.h>
#include <rtdevice.h>

#define SAMPLE_UART_NAME       "uart1"      /* 串口设备名称 */
Video_struct video_struct;
/* 结构体初始化 */
void struct_init(Video_struct *video_struct)
{
    video_struct->HEAD=0xAA55;
    video_struct->LENGTH=0x0200;
    video_struct->ID[0]=0x00;
    video_struct->FRAME_TYPE=0x11;
    video_struct->MES_TYPE=0x58;
    video_struct->MES_TIMES=0x00C8;
    video_struct->MES_COUNT=0x0000;
    video_struct->WARNING=0x00;
    video_struct->DATAHEAD[0]=0xFF;
    video_struct->DATAHEAD[1]=0xD8;
    video_struct->DATAEND[0]=0xFF;
    video_struct->DATAEND[1]=0xD9;
    video_struct->Reserved_1=0x00000000;
    video_struct->Reserved_2=0x00000000;
    video_struct->HCRC[0]=0x00;
    video_struct->HCRC[1]=0x00;
    rt_memset(video_struct->DATA,0,512);
}

/* 串口设备句柄 */
static rt_device_t serial;

void uart_dma_data(uint8_t *buf)
{
    char uart_name[RT_NAME_MAX];
    rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    /* 查找串口设备 */
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART_NAME);
    }

    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 发送字符串 */
    // rt_device_write(serial, 0, buf, (sizeof(buf) - 1));
    rt_device_write(serial, 0, buf, 554);
    /* 关闭设备 */
    rt_device_close(serial);

}

static int uart_dma_sample(int argc, char *argv[])
{
    char str[] = "hello RT-Thread!\r\n";
    char uart_name[RT_NAME_MAX];
    rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);

    /* 查找串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));


}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart_dma_sample, uart device dma sample);