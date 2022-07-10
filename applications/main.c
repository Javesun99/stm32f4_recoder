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
/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 6)
extern void thread_vs(void);
extern int mailbox_sample(void);
extern int iwdg_sample(void);
extern int uart_data_sample();
int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    iwdg_sample();
    rt_thread_mdelay(2000);
    // thread_vs();//已弃用
    mailbox_sample();
    uart_data_sample();
    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
