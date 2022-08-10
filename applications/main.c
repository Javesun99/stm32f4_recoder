/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "crc16.h"
#include <fal.h>
#include <easyflash.h>
/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 6)
uint8_t LED_FLAG=0;
extern uint32_t Threshold;
extern void thread_vs(void);
extern int mailbox_sample(void);
extern int iwdg_sample(void);
extern int uart_data_sample();
void thread_test_entry(void)
{
    fal_init ();
    if(easyflash_init()==EF_NO_ERR)
    {
        ef_get_env_blob("Threshold",&Threshold,4,NULL);
    }
        Threshold++;
        ef_set_env_blob("Threshold",&Threshold,4);
}
void print_reboot (void)
{
    rt_kprintf("Threshold is :%d\n",Threshold);
}
MSH_CMD_EXPORT(print_reboot,Print reboot);




int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    iwdg_sample();
    rt_thread_mdelay(2000);
    // thread_vs();//已弃用
    mailbox_sample();
    uart_data_sample();
//    fal_init();
//    thread_test_entry();
    while (1)
    {
        if(LED_FLAG==0)
        {
            rt_pin_write(LED0_PIN, PIN_HIGH);
            rt_thread_mdelay(500);
            rt_pin_write(LED0_PIN, PIN_LOW);
            rt_thread_mdelay(500);
        }
        if(LED_FLAG==1)
        {
            rt_pin_write(LED0_PIN, PIN_HIGH);
            rt_thread_mdelay(50);
            rt_pin_write(LED0_PIN, PIN_LOW);
            rt_thread_mdelay(50);
        }
        if(LED_FLAG==2)
        {
            rt_pin_write(LED0_PIN, PIN_HIGH);
            rt_thread_mdelay(10);
            rt_pin_write(LED0_PIN, PIN_LOW);
            rt_thread_mdelay(10);
        }

    }

    return RT_EOK;
}



