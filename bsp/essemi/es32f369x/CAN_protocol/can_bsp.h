/*2020��10��15�գ�can_bsp.h
ͷ�ļ�

*/

#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__

#include <rtthread.h>



#define RX_BUF_SIZE		    30
#define TX_BUF_SIZE		   128

/*CAN���ݽṹ�壬Ϊ������������׼��*/
typedef struct 
{
    rt_uint8_t rx_buffer[RX_BUF_SIZE];//
    rt_uint8_t rx_count ;//
  
	
	  rt_uint8_t tx_buffer[TX_BUF_SIZE];//
    rt_uint8_t tx_count ;//
}rt_can_data;



/*��չ֡ID�ֽ�*/
typedef struct 
{
    rt_uint8_t can_cmd  : 3;//���ȼ�
//    rt_uint8_t can_kbit : 1;//����λ
    rt_uint8_t can_fflag : 2;//can֡��־
    rt_uint16_t can_func : 12;//����ӹ���
    rt_uint8_t can_dest : 6;//Ŀ��ڵ�
    rt_uint8_t can_addr : 6;//Դ�ڵ��ַ
   
}rt_can_ExtendID;



/*�������Ĵ���������*/
typedef struct 
{
  
/*������Ϣ   */
  rt_uint16_t Cmd_Iq;
	rt_uint8_t Drive_en:	1;
	rt_uint8_t Drive_off:	1;
	rt_uint8_t Mweek_en:	1;
	rt_uint8_t Mweek_deep:	3;
	rt_uint8_t MTPA_en:	1;
	rt_uint8_t MTPA_deep:	4;
	rt_uint8_t Security_model_en:1;
	rt_uint8_t Security_model_current:6;
	
/*״̬��Ϣ   */ 
	rt_uint16_t Motor_speed;
	rt_uint8_t Drive_temp;
	rt_uint8_t Drive_Ibus;
	rt_uint8_t Cmd_Vq;
	rt_uint8_t Drive_Vbus;
	rt_uint8_t Drive_V5v;

}rt_can_var;


void can_emergency(int);//������Ϣ����
//void can_exception(int);//������Ϣ����
void can_control_cmd(int);//���������
void can_drive_staus(int);//����״̬��Ϣ����
//void can_device_message(void);//�豸��Ϣ����
//void can_up_download(void);//�ϴ��������ݺ���
void can_upgrade(int,int);//������������
void can_diagnose(int);//���

//int timer1_sample(void);

//int can_sample(int argc, char *argv[]);






void can_rx_deal(rt_can_ExtendID *);//������Ϣ֡ID������






//int can_var_read(rt_can_var *);//�����������������������豸״̬��Ϣ����
//int can_var_write(rt_can_var *);//����д���������豸������Ϣ����

void can_send(int cmd,int func,int dest,int len ,int data[]);//�������ͺ���












#endif


