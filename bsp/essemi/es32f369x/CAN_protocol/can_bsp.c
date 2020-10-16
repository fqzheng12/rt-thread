///*
// * ????:???? CAN ??????
// * ????? can_sample ???????
// * ??????:can_sample can1
// * ????:???????????? CAN ????,???????? CAN ??
// * ????:?? CAN ??????,??????????????????
//*/

//#include <rtthread.h>
//#include "rtdevice.h"

//#define CAN_DEV_NAME       "can"      /* CAN ???? */

//static struct rt_semaphore rx_sem;     /* ?????????? */
//static rt_device_t can_dev;            /* CAN ???? */

///* ???????? */
//static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
//{
//    /* CAN ??????????,???????,????????? */
//    rt_sem_release(&rx_sem);

//    return RT_EOK;
//}

//static void can_rx_thread(void *parameter)
//{
//    int i;
//    rt_err_t res;
//    struct rt_can_msg rxmsg = {0};

//    /* ???????? */
//    rt_device_set_rx_indicate(can_dev, can_rx_call);

//#ifdef RT_CAN_USING_HDR
//    struct rt_can_filter_item items[5] =
//    {
//        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x100~0x1ff,hdr ? - 1,??????? */
//        RT_CAN_FILTER_ITEM_INIT(0x300, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x300~0x3ff,hdr ? - 1 */
//        RT_CAN_FILTER_ITEM_INIT(0x211, 0, 0, 1, 0x7ff, RT_NULL, RT_NULL), /* std,match ID:0x211,hdr ? - 1 */
//        RT_CAN_FILTER_STD_INIT(0x486, RT_NULL, RT_NULL),                  /* std,match ID:0x486,hdr ? - 1 */
//        {0x555, 0, 0, 1, 0x7ff, 7,}                                       /* std,match ID:0x555,hdr ? 7,???? 7 ???? */
//    };
//    struct rt_can_filter_config cfg = {5, 1, items}; /* ??? 5 ???? */
//    /* ??????? */
//    res = rt_device_control(can_dev, RT_CAN_CMD_SET_FILTER, &cfg);
//    RT_ASSERT(res == RT_EOK);
//#endif

//    while (1)
//    {
//        /* hdr ?? - 1,????? uselist ?????? */
//        rxmsg.hdr = -1;
//        /* ????????? */
//        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
//        /* ? CAN ?????? */
//        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
//        /* ???? ID ??? */
//        rt_kprintf("ID:%x", rxmsg.id);
//        for (i = 0; i < 8; i++)
//        {
//            rt_kprintf("%2x", rxmsg.data[i]);
//        }

//        rt_kprintf("\n");
//    }
//}

//int can_sample(int argc, char *argv[])
//{
//    struct rt_can_msg msg = {0};
//    rt_err_t res;
//    rt_size_t  size;
//    rt_thread_t thread;
//    char can_name[RT_NAME_MAX];

//    if (argc == 2)
//    {
//        rt_strncpy(can_name, argv[1], RT_NAME_MAX);
//    }
//    else
//    {
//        rt_strncpy(can_name, CAN_DEV_NAME, RT_NAME_MAX);
//    }
//    /* ?? CAN ?? */
//    can_dev = rt_device_find(can_name);
//    if (!can_dev)
//    {
//        rt_kprintf("find %s failed!\n", can_name);
//        return RT_ERROR;
//    }

//    /* ??? CAN ????? */
//    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

//    /* ???????????? CAN ?? */
//    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
//    RT_ASSERT(res == RT_EOK);
//    /* ???????? */
//    thread = rt_thread_create("can_rx", can_rx_thread, RT_NULL, 1024, 10, 10);
//    if (thread != RT_NULL)
//    {
//        rt_thread_startup(thread);
//    }
//    else
//    {
//        rt_kprintf("create can_rx thread failed!\n");
//    }

//    msg.id = 0x78;              /* ID ? 0x78 */
//    msg.ide = RT_CAN_STDID;     /* ???? */
//    msg.rtr = RT_CAN_DTR;       /* ??? */
//    msg.len = 8;                /* ????? 8 */
//    /* ???? 8 ???? */
//    msg.data[0] = 0x00;
//    msg.data[1] = 0x11;
//    msg.data[2] = 0x22;
//    msg.data[3] = 0x33;
//    msg.data[4] = 0x44;
//    msg.data[5] = 0x55;
//    msg.data[6] = 0x66;
//    msg.data[7] = 0x77;
//    /* ???? CAN ?? */
//    size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
//    if (size == 0)
//    {
//        rt_kprintf("can dev write data failed!\n");
//    }

//    return res;
//}
///* ??? msh ????? */
//MSH_CMD_EXPORT(can_sample, can device sample);
