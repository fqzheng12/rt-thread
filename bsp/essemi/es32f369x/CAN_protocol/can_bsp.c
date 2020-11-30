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


/* 定时器的控制块 */
static rt_timer_t timer1;//设置循环定时器进行循环发送电机速度信息
static struct rt_semaphore rx_sem;     /* 接收信号量 */
static struct rt_semaphore tx_sem;     /* 发送信号量 */
static rt_device_t can_dev;  //can句柄
struct rt_can_msg rxmsg = {0};
struct rt_can_msg txmsg = {0};//发送帧
rt_can_ExtendID ExtendID = {0};
rt_can_var can_var={0};



static void timeout1(void *parameter)
{
	
	
	 rt_sem_release(&tx_sem);

//    return RT_EOK;
//	  rt_size_t  size;
////    rt_kprintf("periodic timer is timeout %d\n", cnt);
///*111000(56，状态信息)|0（预留）|11（单帧）|111111（功能码）|000001（目标节点地址）|000010|（源节点地址）=0x70ff042*/
//    txmsg.id = 0x70FF042;              /* ID ? 0x78 */
////    msg.ide = RT_CAN_EXTID;     /* 扩展帧 */
//		txmsg.ide = 1;     /* 扩展帧 */
//    txmsg.rtr = RT_CAN_DTR;       /* 数据帧 */
//    txmsg.len = 8;                /* ????? 8 */
//    /* ???? 8 ???? */
////    txmsg.data[0] = ;
//    can_var.Drive_V5v=0x99;
//    can_var.Drive_Vbus=0xaa;
//    can_var.Cmd_Vq=0xbb;
//    can_var.Drive_Ibus=0xcc;
//    can_var.Drive_temp=0xdd;
//    can_var.Motor_speed=0xeeff;
//	
//    txmsg.data[1] = can_var.Drive_V5v;
//    txmsg.data[2] = can_var.Drive_Vbus;
//    txmsg.data[3] = can_var.Cmd_Vq;
//    txmsg.data[4] = can_var.Drive_Ibus;
//    txmsg.data[5] = can_var.Drive_temp;
//    txmsg.data[6] = (can_var.Motor_speed>>8);
//    txmsg.data[7] = can_var.Motor_speed;
//    /* ???? CAN ?? */
//    size = rt_device_write(can_dev, 0, &txmsg, sizeof(txmsg));
//    if (size == 0)
//    {
//        rt_kprintf("can dev write data failed!\n");
//    }
}

static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* 接收回调函数*///在里面释放信号量
    rt_sem_release(&rx_sem);

    return RT_EOK;
}


static void can_tx_thread(void *parameter)//can接收入口函数
{
  while(1)
	{
	rxmsg.hdr = -1;
        /* 等待接收完成后回调函数释放的信号量 */
  rt_sem_take(&tx_sem, RT_WAITING_FOREVER);//获取一次信号量，信号量减一
	rt_size_t  size;
//    rt_kprintf("periodic timer is timeout %d\n", cnt);
/*111000(56，状态信息)|0（预留）|11（单帧）|111111（功能码）|000001（目标节点地址）|000010|（源节点地址）=0x70ff042*/
    txmsg.id = 0x70FF042;              /* ID ? 0x78 */
//    msg.ide = RT_CAN_EXTID;     /* 扩展帧 */
		txmsg.ide = 1;     /* 扩展帧 */
    txmsg.rtr = RT_CAN_DTR;       /* 数据帧 */
    txmsg.len = 8;                /* ????? 8 */
    /* ???? 8 ???? */
//    txmsg.data[0] = ;
    can_var.Drive_V5v=0x99;
    can_var.Drive_Vbus=0xaa;
    can_var.Cmd_Vq=0xbb;
    can_var.Drive_Ibus=0xcc;
    can_var.Drive_temp=0xdd;
    can_var.Motor_speed=0xeeff;
	
    txmsg.data[1] = can_var.Drive_V5v;
    txmsg.data[2] = can_var.Drive_Vbus;
    txmsg.data[3] = can_var.Cmd_Vq;
    txmsg.data[4] = can_var.Drive_Ibus;
    txmsg.data[5] = can_var.Drive_temp;
    txmsg.data[6] = (can_var.Motor_speed>>8);
    txmsg.data[7] = can_var.Motor_speed;
    /* ???? CAN ?? */
    size = rt_device_write(can_dev, 0, &txmsg, sizeof(txmsg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }
	}	

}
static void can_rx_thread(void *parameter)//can接收入口函数
{
//    int i;
    rt_err_t res;
    
		

    /* 设置回调函数 */
    rt_device_set_rx_indicate(can_dev, can_rx_call);

#ifdef RT_CAN_USING_HDR//下个阶段弄清楚
    struct rt_can_filter_item items[1] =
    {
//        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x100~0x1ff,hdr 为 - 1,设置默认过滤表 */
//        RT_CAN_FILTER_ITEM_INIT(0x300, 0, 0, 1, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x300~0x3ff,hdr ? - 1 */
//        RT_CAN_FILTER_ITEM_INIT((0x55<<18), 0, 0, 0, (0x7ff<<21 | 6), RT_NULL, RT_NULL), /* std,match ID:0x211,hdr 为 - 1 */
//        RT_CAN_FILTER_STD_INIT(0x486, RT_NULL, RT_NULL),                  /* std,match ID:0x486,hdr 为 - 1 */
        {0x02, 1, 0, 0, 0x3f<<3, 0} //不使用默认过滤表，必须先设置0号过滤表，最后一位，mask得移位3位，底层驱动没有做                                 
    };//以上过滤表实现扩展帧后6位必须为0x02的帧才会被接收，假设控制器目标地址为0x02，即帧目标地址为控制器的帧才能被接收。
    struct rt_can_filter_config cfg = {1, 1, items}; /* ??? 5 ???? */
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
			     if(rxmsg.ide==1)//如果是远程帧的话就把帧ID按协议分解
					 {
					 ExtendID.can_cmd=((rxmsg.id>>21)&0x7f);//右移22位留下高7位。
					 ExtendID.can_kbit=((rxmsg.id>>20)&0x01);//右移21位留下高1位。
					 ExtendID.can_fflag=((rxmsg.id>>18)&0x7);//右移18位留下高3位。
					 ExtendID.can_func=((rxmsg.id>>12)&0x3f);//右移12位留下高6位。 
				   ExtendID.can_dest=((rxmsg.id>>6)&0x3f);//右移6位留下高6位。
				   ExtendID.can_addr=(rxmsg.id&0x3f);//留下高6位。

					  
					 
			
        /* 将接收到的can ID打印出来*/
           rt_kprintf("ID:%x\n", rxmsg.id);
					 rt_kprintf("\n");
//        for (i = 0; i < 8; i++)
//        {
           rt_kprintf("%2x ", ExtendID.can_cmd);//打出来验证下分解的有没有问题
				   rt_kprintf("%2x ", ExtendID.can_kbit);
				   rt_kprintf("%2x ", ExtendID.can_fflag);
				   rt_kprintf("%2x ", ExtendID.can_func);
				   rt_kprintf("%2x ", ExtendID.can_dest);
				   rt_kprintf("%2x ", ExtendID.can_addr);
//        }

           rt_kprintf("\n");
					 switch(ExtendID.can_cmd)
					   {
						 case(11):
						 can_emergency();
						 break;
						 
						 case(26):
						 can_exception();
						 break;
						 
						 case(41):
						 can_control_cmd();
						 break;
						 
						 case(56):
						 can_drive_staus();
						 break;
						 
						 case(71):
						 can_device_message();
						 break;
						 
						 case(86):
						 can_up_download();
						 break;
						 
						 case(101):
						 can_upgrade();
						 break;
					   }
					 }
						
    }
}

int can_sample(int argc, char *argv[])
{
//    struct rt_can_msg msg ;
    rt_err_t res;
//    rt_size_t  size;
    rt_thread_t can_rx;
	  rt_thread_t can_tx;
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
		/* 初始化can发送信号量 */
    rt_sem_init(&tx_sem, "tx_sem", 0, RT_IPC_FLAG_FIFO);
		

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
/*can发送线程*/	
		can_tx = rt_thread_create("can_tx", can_tx_thread, RT_NULL, 1024, 25, 10);//动态线程
    if (can_tx != RT_NULL)
    {
        rt_thread_startup(can_tx);
    }
    else
    {
        rt_kprintf("create can_tx thread failed!\n");
    }
		
/*设置循环定时器进行驱动板状态信息发送*/
timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 3000,//3秒钟释放一个信号量，发送数据。
                             RT_TIMER_FLAG_PERIODIC);		
    /* 启动定时器1 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);		
		
//    msg.id = 0x7FF;              /* ID ? 0x78 */
////    msg.ide = RT_CAN_EXTID;     /* 扩展帧 */
//		msg.ide = 0;     /* 扩展帧 */
//    msg.rtr = RT_CAN_DTR;       /* 数据帧 */
//    msg.len = 8;                /* ????? 8 */
//    /* ???? 8 ???? */
//    msg.data[0] = 0x00;
//    msg.data[1] = 0x22;
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

    return res;
}
MSH_CMD_EXPORT(can_sample, can device sample);

//int timer1_sample(void)
//{
//    /* 创建定时器1  周期定时器 */
//    timer1 = rt_timer_create("timer1", timeout1,
//                             RT_NULL, 10,
//                             RT_TIMER_FLAG_PERIODIC);

//    /* 启动定时器1 */
//    if (timer1 != RT_NULL) rt_timer_start(timer1);

////    /* 创建定时器2 单次定时器 */
////    timer2 = rt_timer_create("timer2", timeout2,
////                             RT_NULL,  30,
////                             RT_TIMER_FLAG_ONE_SHOT);

////    /* 启动定时器2 */
////    if (timer2 != RT_NULL) rt_timer_start(timer2);
//    return 0;
//}

/* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(timer1_sample, timer sample);

/*报警信息处理*/
void can_emergency(void)
{
//
}

/*紧急信息处理*/
void can_exception(void)
{
//
}




/*控制信息处理*/
void can_control_cmd(void)
{
	switch(ExtendID.can_func)
		case(0x3f)://如果是电机实时控制寄存器，进行寄存器值更新
			can_var.Cmd_Iq=((rxmsg.data[6]<<8)&rxmsg.data[7]);
	    can_var.Drive_en=rxmsg.data[5];
	    can_var.Drive_off=(rxmsg.data[5]>>1);
	    can_var.Mweek_en=(rxmsg.data[5]>>2);
		  can_var.MTPA_en=(rxmsg.data[5]>>3);
		  can_var.Security_model_en=(rxmsg.data[5]>>4);
	    can_var.Mweek_deep=rxmsg.data[4];
	    can_var.MTPA_deep=rxmsg.data[3];
	    can_var.Security_model_current=rxmsg.data[2];
//	  break;
}

/*驱动状态信息处理*/
void can_drive_staus(void)
{
//
}
/*设备信息处理*/
void can_device_message(void)
{
//
}
/*上传下载信息处理*/
void can_up_download(void)
{
//
}
/*升级信息处理*/
void can_upgrade(void)
{
//
}

