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


/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;//����ѭ����ʱ������ѭ�����͵���ٶ���Ϣ
static struct rt_semaphore rx_sem;     /* �����ź��� */
static struct rt_semaphore tx_sem;     /* ��ʱ�����ź��� */
static rt_device_t can_dev;  //can���
struct rt_can_msg rxmsg = {0};
struct rt_can_msg txmsg = {0};//����֡


rt_can_ExtendID rxExtendID = {0};
rt_can_ExtendID txExtendID = {0};
//rt_can_ExtendID *pExtendID = &ExtendID;//����һ��ָ��

rt_can_var can_var={0};
rt_can_var *pcan_var  = &can_var;


/* ��ʱ���Ŀ��ƿ� */
static void timeout1(void *parameter)//��ʱ�ͷŷ����ź���
{
//	int count = 1;
	  
	  rt_sem_release(&tx_sem);
   
//      return RT_EOK;
// 
}


/* can������ں���
{ */
static void can_tx_thread(void *parameter)//
{
  while(1)
	{
	rxmsg.hdr = -1;
        /* �ȴ�������ɺ�ص������ͷŵ��ź��� */
    rt_sem_take(&tx_sem, RT_WAITING_FOREVER);//��ȡһ���ź������ź�����һ
	  rt_size_t  size;
//    rt_kprintf("periodic timer is timeout %d\n", cnt);
/*111000(56��״̬��Ϣ)|0��Ԥ����|011����֡��|111111�������룩|000001��Ŀ��ڵ��ַ��|000010|��Դ�ڵ��ַ��=0x70ff042*/
    txmsg.id = 0xE0FF042;              /* ID ? 0x78 */
//    msg.ide = RT_CAN_EXTID;     /* ��չ֡ */
	  txmsg.ide = 1;     /* ��չ֡ */
    txmsg.rtr = RT_CAN_DTR;       /* ����֡ */
    txmsg.len = 8;                /* */
    /* */
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
    /* CAN */
    size = rt_device_write(can_dev, 0, &txmsg, sizeof(txmsg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }
		
		int data[8]={0xff,0xee,12,55,33,76,23,45};
	  can_send(2,1,5,8,data);
	}	

}





static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* ���ջص�����*///�������ͷ��ź���
    rt_sem_release(&rx_sem);

    return RT_EOK;
}


/*can������ں���*/

static void can_rx_thread(void *parameter)//can������ں���
{
	
//    int i;
    rt_err_t res;
    
	rt_size_t  size;	

//	rt_can_ExtendID *pExtendID = &ExtendID;//����һ��ָ��
    /* ���ûص����� */
    rt_device_set_rx_indicate(can_dev, can_rx_call);

#ifdef RT_CAN_USING_HDR//Ӳ����������
    struct rt_can_filter_item items[1] =
    {
        {0x02, 1, 0, 0, 0x3f<<3, 0} //��ʹ��Ĭ�Ϲ��˱�����������0�Ź��˱����һλ��mask����λ3λ���ײ�����û����                                 
    };//���Ϲ��˱�ʵ����չ֡��6λ����Ϊ0x02��֡�Żᱻ���գ����������Ŀ���ַΪ0x02����֡Ŀ���ַΪ��������֡���ܱ����ա�
    struct rt_can_filter_config cfg = {1, 1, items}; /*  */
    /* */
    res = rt_device_control(can_dev, RT_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);
#endif

    while (1)
    {
        /* hdr - 1, uselist �����ȡ���� */
        rxmsg.hdr = -1;
        /* �ȴ�������ɺ�ص������ͷŵ��ź��� */
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);//��ȡһ���ź������ź�����һ
        /* CAN ���ź�����֮�󣬶�ȡһ֡���� */
        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
			if(rxmsg.ide==1)//�������չ֡�Ļ��Ͱ�֡ID��Э��ֽ�
			{
				rxExtendID.can_cmd=((rxmsg.id>>22)&0x7f);//����22λ���¸�7λ��E0FF042
				rxExtendID.can_kbit=((rxmsg.id>>21)&0x01);//����21λ���¸�1λ��
				rxExtendID.can_fflag=((rxmsg.id>>18)&0x7);//����18λ���¸�3λ��
				rxExtendID.can_func=((rxmsg.id>>12)&0x3f);//����12λ���¸�6λ�� 
				rxExtendID.can_dest=((rxmsg.id>>6)&0x3f);//����6λ���¸�6λ��
				rxExtendID.can_addr=(rxmsg.id&0x3f);//���¸�6λ��

					  
					 
			
        /* �����յ���can ID��ӡ����*/
           rt_kprintf("ID:%x\n", rxmsg.id);
					 rt_kprintf("\n");

           rt_kprintf("%2x ", rxExtendID.can_cmd);//�������֤�·ֽ����û������
				   rt_kprintf("%2x ", rxExtendID.can_kbit);
				   rt_kprintf("%2x ", rxExtendID.can_fflag);
				   rt_kprintf("%2x ", rxExtendID.can_func);
				   rt_kprintf("%2x ", rxExtendID.can_dest);
				   rt_kprintf("%2x ", rxExtendID.can_addr);
           rt_kprintf("%2x ", rxmsg.data[7]);

           rt_kprintf("\n");
					 can_rx_deal(&rxExtendID);//��id�ֽ����д���

					 }
			rxmsg.id=rxExtendID.can_cmd<<22|rxExtendID.can_kbit<<21|(rxExtendID.can_fflag|0x04)<<18|rxExtendID.can_func<<12|rxExtendID.can_addr<<6|rxExtendID.can_dest;
			 size = rt_device_write(can_dev, 0, &rxmsg, sizeof(txmsg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
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
    /* ע���豸�����ҵ�*/
    can_dev = rt_device_find(can_name);
    if (!can_dev)
    {
        rt_kprintf("find %s failed!\n", can_name);
        return RT_ERROR;
    }

    /* ��ʼ��can�����ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
		/* ��ʼ��can�����ź��� */
    rt_sem_init(&tx_sem, "tx_sem", 0, RT_IPC_FLAG_FIFO);
		

    /* CAN */
    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
		res = rt_device_control(can_dev, RT_CAN_CMD_SET_BAUD, (void *)CAN500kBaud);
    RT_ASSERT(res == RT_EOK);
    /*  */
    can_rx = rt_thread_create("can_rx", can_rx_thread, RT_NULL, 1024, 25, 10);//��̬�߳�
    if (can_rx != RT_NULL)
    {
        rt_thread_startup(can_rx);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }
/*can�����߳�*/	
		can_tx = rt_thread_create("can_tx", can_tx_thread, RT_NULL, 1024, 25, 10);//��̬�߳�
    if (can_tx != RT_NULL)
    {
        rt_thread_startup(can_tx);
    }
    else
    {
        rt_kprintf("create can_tx thread failed!\n");
    }
		
/*����ѭ����ʱ������������״̬��Ϣ����*/
timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 200,//3�����ͷ�һ���ź������������ݡ�
                             RT_TIMER_FLAG_PERIODIC);		
    /* ������ʱ��1 */
    if (timer1 != RT_NULL) rt_timer_start(timer1);		
		

    return res;
}
MSH_CMD_EXPORT(can_sample, can device sample);






/*������Ϣ����*/
void can_emergency(int func)
{
//
}



/*������Ϣ����*/
//void can_exception(void)
//{
////
//}






/*������Ϣ����*/
void can_control_cmd(int func)
{
	switch(func)
		case(0x3f)://����ǵ��ʵʱ���ƼĴ��������мĴ���ֵ����
//		can_var.Cmd_Iq=(((rxmsg.data[6]&0xff)<<8)&(rxmsg.data[7]&0xff));
			can_var.Cmd_Iq=(rxmsg.data[6]<<8)|rxmsg.data[7];
	    can_var.Drive_en=rxmsg.data[5];
	    can_var.Drive_off=(rxmsg.data[5]>>1);
	    can_var.Mweek_en=(rxmsg.data[5]>>2);
		can_var.MTPA_en=(rxmsg.data[5]>>3);
		can_var.Security_model_en=(rxmsg.data[5]>>4);
	    can_var.Mweek_deep=rxmsg.data[4];
	    can_var.MTPA_deep=rxmsg.data[3];
	    can_var.Security_model_current=rxmsg.data[2];
		rt_kprintf("Cmd_Iq=%2x ", can_var.Cmd_Iq);
		rt_kprintf("%2x ", can_var.Drive_en);
		rt_kprintf("%2x ", can_var.Drive_off);
		rt_kprintf("%2x ", can_var.Mweek_en);
		rt_kprintf("%2x ", can_var.MTPA_en);
		rt_kprintf("%2x ", can_var.Security_model_en);
		rt_kprintf("%2x ", can_var.Mweek_deep);
		rt_kprintf("%2x ", can_var.MTPA_deep);
		rt_kprintf("%2x ", can_var.Security_model_current);

		
//	  break;
}



/*����״̬��Ϣ����*/
void can_drive_staus(int func)
{
//	 rt_err_t res;
    
	rt_size_t  size;	
  can_var.Drive_Ibus = 0xff;
	struct rt_can_msg ack_txmsg = rxmsg;//�ظ�֡
	ack_txmsg.id = rxExtendID.can_cmd<<22|rxExtendID.can_kbit<<21|(rxExtendID.can_fflag|0x04)<<18|rxExtendID.can_func<<12|rxExtendID.can_addr<<6|rxExtendID.can_dest;
	switch(func)
	{
		case(0x01):
			
		ack_txmsg.data[7]=can_var.Drive_Ibus;
		
	
	
	}
		
	
	
	
	
	size = rt_device_write(can_dev, 0, &ack_txmsg, sizeof(ack_txmsg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }			
//
}
///*�豸��Ϣ����*/
//void can_device_message(void)
//{
////
//}
///*�ϴ�������Ϣ����*/
//void can_up_download(void)
//{
////
//}
/*������Ϣ����*/
void can_upgrade(int func)
{
//
	
	
	
	
	
}

void can_diagnose(int func)
{
//
}

/*��չ֡ID������*/
void can_rx_deal(rt_can_ExtendID *rxExtendID)//
{
	
      switch(rxExtendID->can_cmd)
					   {
						 case(11):
						 can_emergency(rxExtendID->can_func);
						 break;
						 
//						 case(26):
//						 can_exception();
//						 break;
						 
						 case(41):
						 can_control_cmd(rxExtendID->can_func);
						 break;
						 
						 case(56):
						 can_drive_staus(rxExtendID->can_func);
						 break;
						 
//						 case(71):
//						 can_device_message();
//						 break;
//						 
//						 case(86):
//						 can_up_download();
//						 break;
						 
						 case(101):
						 can_upgrade(rxExtendID->can_func);
						 break;
						 
						 case(86):
						 can_diagnose(rxExtendID->can_func);
						 break;
						 
              }
}





int can_var_read(rt_can_var *can_var)//������������
{
	
	
	
	return 0;
}


int can_var_write(rt_can_var *can_var)//����д������
{
	
	
	return 0;
}




void can_send(int cmd,int func,int dest,int len,int data[])
{
int i;
//	int len = 8;
  rt_size_t  size;	
  can_var.Drive_Ibus = 0xff;
	struct rt_can_msg txmsg=rxmsg;//�ظ�֡
	txmsg.ide=1;
	txmsg.len=len;
  rt_can_ExtendID ExtendID={cmd,1,0,func,2,dest};
	txmsg.id = ExtendID.can_cmd<<22|ExtendID.can_kbit<<21|(ExtendID.can_fflag|0x04)<<18|ExtendID.can_func<<12|ExtendID.can_addr<<6|ExtendID.can_dest;
	for(i=0;i<=len;i++) txmsg.data[i]=data[i];
//     { 
	
//			 txmsg.data[0]=0;
//			 txmsg.data[1]=0xff;
//     };
	
	
	
	size = rt_device_write(can_dev, 0, &txmsg, sizeof(txmsg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }			

}	

