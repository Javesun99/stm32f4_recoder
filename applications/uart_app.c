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
uint8_t READY_FLAG=0;
/* 结构体初始化 */
void struct_init(Video_struct *video_struct)
{
    video_struct->HEAD[0]=0xAA;
    video_struct->HEAD[1]=0x55;
    video_struct->LENGTH[0]=0x02;
    video_struct->LENGTH[1]=0x2A;
    video_struct->ID[0]=0x00;
    video_struct->ID[1]=0x00;
    video_struct->ID[2]=0x00;
    video_struct->ID[3]=0x00;
    video_struct->ID[4]=0x00;
    video_struct->ID[5]=0x00;
    video_struct->ID[6]=0x00;
    video_struct->ID[7]=0x00;
    video_struct->ID[8]=0x00;
    video_struct->ID[9]=0x00;
    video_struct->ID[10]=0x00;
    video_struct->ID[11]=0x00;
    video_struct->ID[12]=0x00;
    video_struct->ID[13]=0x00;
    video_struct->ID[14]=0x00;
    video_struct->ID[15]=0x00;
    video_struct->ID[16]=0x00;
    video_struct->FRAME_TYPE=0x11;
    video_struct->MES_TYPE=0x58;
    video_struct->MES_TIMES[0]=0x00;
    video_struct->MES_TIMES[1]=0xC8;
    video_struct->MES_COUNT[0]=0x00;
    video_struct->MES_COUNT[1]=0x00;
    video_struct->WARNING=0x00;
    video_struct->DATAHEAD[0]=0xFF;
    video_struct->DATAHEAD[1]=0xD8;
    video_struct->DATAEND[0]=0xFF;
    video_struct->DATAEND[1]=0xD9;
    video_struct->Reserved_1=0x00000000;
    video_struct->Reserved_2=0x00000000;
    video_struct->LCRC=0x00;
    video_struct->HCRC=0x00;
    rt_memset(video_struct->DATA,0,512);
}

/* 串口设备句柄 */
static rt_device_t serial;

void uart_dma_init(void)
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
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);// | RT_DEVICE_FLAG_DMA_TX);
    /* 发送字符串 */
    // rt_device_write(serial, 0, buf, (sizeof(buf) - 1));
    // rt_device_write(serial, 0, buf, 554);
    /* 关闭设备 */
    // rt_device_close(serial);

}
INIT_APP_EXPORT(uart_dma_init);

void dma_data(uint8_t *buf)
{
    rt_device_write(serial, 0, buf, 554);
}

void sturct_trans(uint8_t *buf)
{
    rt_device_write(serial, 0, buf, 9);
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




#define DATA_CMD_END 0x4D  /* 结束位设置为 \r，即回车符 */
#define ONE_DATA_MAXLEN 20 /* 不定长数据的最大长度 */
/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* 接收数据回调函数 */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(void)
{
    char ch;

    while (rt_device_read(serial, 0, &ch, 1) == 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}

/* 数据解析线程 */
void data_parsing(void)
{
    char ch;
    char data[ONE_DATA_MAXLEN]={0};
    static char i = 0;
    while(1)
    {
        ch = uart_sample_get_char();
        if (ch == DATA_CMD_END)
        {
            data[i++] = '\0';
            if(data[0] == 0xAA && data[1] == 0x55 && data[5] == 0x13 && data[6] == 0x01)
            {
                READY_FLAG=1;
                rt_kprintf("READY_FLAG==1");
            }
            i = 0;
            rt_memset(data,0,ONE_DATA_MAXLEN);
            continue;
        }
        i = (i >= ONE_DATA_MAXLEN - 1) ? ONE_DATA_MAXLEN - 1 : i;
        data[i++] = ch;
    }
}



int uart_data_sample()
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];

    rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_rx_ind);
    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", (void (*)(void *parameter))data_parsing, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}



