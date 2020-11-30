/*2020��10��15�գ�can_bsp.h
ͷ�ļ�

*/

#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__

#include <rtthread.h>


void can_emergency(void);//������Ϣ����
void can_exception(void);//������Ϣ����
void can_control_cmd(void);//���������
void can_drive_staus(void);//����״̬��Ϣ����
void can_device_message(void);//�豸��Ϣ����
void can_up_download(void);//�ϴ��������ݺ���
void can_upgrade(void);//������������


//int timer1_sample(void);

typedef struct 
{
    rt_uint8_t can_cmd  : 7;//���ȼ�
    rt_uint8_t can_kbit : 1;//����λ
    rt_uint8_t can_fflag : 3;//can֡��־
    rt_uint8_t can_func : 6;//����ӹ���
    rt_uint8_t can_dest : 6;//Ŀ��ڵ�
    rt_uint8_t can_addr : 6;//Դ�ڵ��ַ
   
}rt_can_ExtendID;


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







#endif


