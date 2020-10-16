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
#define CAN_DEV_NAME       "can"


static struct rt_semaphore rx_sem;     /* 接收信号量 */
static rt_device_t can_dev;  //can句柄


static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* 接收回调含函数*///在里面释放信号量
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void can_rx_thread(void *parameter)//can接收入口函数
{
    int i;
    rt_err_t res;
    struct rt_can_msg rxmsg = {0};

    /* 设置回调函数 */
    rt_device_set_rx_indicate(can_dev, can_rx_call);

#ifdef RT_CAN_USING_HDR//下个阶段弄清楚
    struct rt_can_filter_item items[5] =
    {
        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x100~0x1ff,hdr ? - 1,??????? */
        RT_CAN_FILTER_ITEM_INIT(0x300, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x300~0x3ff,hdr ? - 1 */
        RT_CAN_FILTER_ITEM_INIT(0x211, 0, 0, 1, 0x7ff, RT_NULL, RT_NULL), /* std,match ID:0x211,hdr ? - 1 */
        RT_CAN_FILTER_STD_INIT(0x486, RT_NULL, RT_NULL),                  /* std,match ID:0x486,hdr ? - 1 */
        {0x555, 0, 0, 1, 0x7ff, 7,}                                       /* std,match ID:0x555,hdr ? 7,???? 7 ???? */
    };
    struct rt_can_filter_config cfg = {5, 1, items}; /* ??? 5 ???? */
    /* ??????? */
    res = rt_device_control(can_dev, RT_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);
#endif

    while (1)
    {
        /* hdr ?? - 1,????? uselist 链表读取数据 */
        rxmsg.hdr = -1;
        /* 等待接收完成后回调函数释放的信号量 */
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);//获取一次信号量，信号量减一
        /* ? CAN 有信号量了之后，读取一帧数据 */
        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
        /* 将接收到的can ID打印出来*/
        rt_kprintf("ID:%x\n", rxmsg.id);
        for (i = 0; i < 8; i++)
        {
            rt_kprintf("%2x ", rxmsg.data[i]);
        }

        rt_kprintf("\n");
    }
}

int can_sample(int argc, char *argv[])
{
    struct rt_can_msg msg ;
    rt_err_t res;
    rt_size_t  size;
    rt_thread_t can_rx;
    char can_name[RT_NAME_MAX];

    if (argc == 2)
    {
        rt_strncpy(can_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(can_name, CAN_DEV_NAME, RT_NAME_MAX);
    }
    /* 注册设备，并找到*/
    can_dev = rt_device_find(can_name);
    if (!can_dev)
    {
        rt_kprintf("find %s failed!\n", can_name);
        return RT_ERROR;
    }

    /* 初始化can接收信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    /* ???????????? CAN ?? */
    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
		res = rt_device_control(can_dev, RT_CAN_CMD_SET_BAUD, (void *)CAN500kBaud);
    RT_ASSERT(res == RT_EOK);
    /* ???????? */
    can_rx = rt_thread_create("can_rx", can_rx_thread, RT_NULL, 1024, 25, 10);//动态线程
    if (can_rx != RT_NULL)
    {
        rt_thread_startup(can_rx);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }

    msg.id = 0xFF7FF;              /* ID ? 0x78 */
    msg.ide = RT_CAN_EXTID;     /* ???? */
    msg.rtr = RT_CAN_DTR;       /* ??? */
    msg.len = 8;                /* ????? 8 */
    /* ???? 8 ???? */
    msg.data[0] = 0x00;
    msg.data[1] = 0x22;
    msg.data[2] = 0x22;
    msg.data[3] = 0x33;
    msg.data[4] = 0x44;
    msg.data[5] = 0x55;
    msg.data[6] = 0x66;
    msg.data[7] = 0x77;
    /* ???? CAN ?? */
    size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }

    return res;
}
MSH_CMD_EXPORT(can_sample, can device sample);


