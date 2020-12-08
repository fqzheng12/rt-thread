/*2020年10月15日，can_bsp.h
头文件

*/

#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__

#include <rtthread.h>




typedef struct 
{
    rt_uint8_t can_cmd  : 7;//优先级
    rt_uint8_t can_kbit : 1;//保留位
    rt_uint8_t can_fflag : 3;//can帧标志
    rt_uint8_t can_func : 6;//编号子功能
    rt_uint8_t can_dest : 6;//目标节点
    rt_uint8_t can_addr : 6;//源节点地址
   
}rt_can_ExtendID;

void can_emergency(int);//紧急信息处理
//void can_exception(int);//报警信息处理
void can_control_cmd(int);//控制命令处理
void can_drive_staus(int);//驱动状态信息处理
//void can_device_message(void);//设备信息处理
//void can_up_download(void);//上传下载数据函数
void can_upgrade(int);//升级相关命令函数
void can_diagnose(int);//诊断

//int timer1_sample(void);

int can_sample(int argc, char *argv[]);






void can_rx_deal(rt_can_ExtendID *);//接收信息帧ID处理函数


typedef struct 
{
  
/*控制信息   */
  rt_uint16_t Cmd_Iq;
	rt_uint8_t Drive_en:	1;
	rt_uint8_t Drive_off:	1;
	rt_uint8_t Mweek_en:	1;
	rt_uint8_t Mweek_deep:	3;
	rt_uint8_t MTPA_en:	1;
	rt_uint8_t MTPA_deep:	4;
	rt_uint8_t Security_model_en:1;
	rt_uint8_t Security_model_current:6;
	
/*状态信息   */ 
	rt_uint16_t Motor_speed;
	rt_uint8_t Drive_temp;
	rt_uint8_t Drive_Ibus;
	rt_uint8_t Cmd_Vq;
	rt_uint8_t Drive_Vbus;
	rt_uint8_t Drive_V5v;

}rt_can_var;



int can_var_read(rt_can_var *);//变量读处理函数
int can_var_write(rt_can_var *);//变量写处理函数
void can_send(int cmd,int func,int dest,int len ,int data[]);//主动发送函数
#endif


