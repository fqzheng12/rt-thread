/*
 * Copyright (C) 2018 Shanghai Eastsoft Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-01-14     wangyq        the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <can_bsp.h>


#define LED_PIN 18
 
 

int main(void)
{
    int count = 1;
//	  int data[8]={0xff,0xee,12,55,33,76,23,45};
//		int data[8]={0x1,0x2};
//	 int i;
    /* set PF00 pin mode to output */
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
	

    while (count++)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
//			  if((rt_tick_get()>2000)&(rt_tick_get()%10)) can_send(2,1,5,data);
//			if(rt_tick_get()>2000) can_send(2,1,5,data);
    }
    return RT_EOK;
		
	
		
}
