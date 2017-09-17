
#ifndef _DATA_SEND_HANDLE_H
	#define _DATA_SEND_HANDLE_H  


#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define location_data_send_printf 0

#define Busy_State  1   //状态空空闲
#define Idle_State  0	//状态忙
/*位置信息发送队列*/
#define Location_Queue_Max 6


/*系统标志位位域结构体*/
typedef union _System_Flag_Struct_
{
	uint32_t System_Flag_Integral;
	struct
	{
	    uint32_t Mode_Active_ing:1;//正在激活中
		uint32_t Mode_Active:1;				 //激活标志
		
		uint32_t MainAutenSucess:1;               //鉴权成功
		uint32_t SecondAutenSucess:1;               //鉴权成功
		
		uint32_t Hart_Beat_Send: 1;			 //心跳发送
		uint32_t Second_Hart_Beat_Send: 1;			 //第二服务器心跳发送
		
		uint32_t Location_Send_State : 1 ;	 //发送状态
		uint32_t Second_Location_Send_State : 1 ;	 //第二服务器发送状态
		
		uint32_t Queue_Fade_Area: 1;		     //第一服务器队列中的盲区数据
		uint32_t Second_Queue_Fade_Area: 1;		     //第二服务器队列中的盲区数据
		
		uint32_t File_Fade_Area: 1;			 //文件中盲区数据发送
		uint32_t Second_File_Fade_Area: 1;		     //第二服务器队列中的盲区数据
		
		uint32_t Location_Tracking: 1;        // 1位置跟踪
		uint32_t Second_Location_Tracking: 1;        // 2位置跟踪

		uint32_t can_data_send_state :1;
		uint32_t second_can_data_send_state :1;
		
		uint32_t Sms_Send_Time: 1;
		uint32_t Rout_Tim_Over: 1;//路线行驶时间过长
		uint32_t Rout_Tim_Short : 1;//路线行驶时间不足
        uint32_t ic_inout :1; //ic卡插入拔出 插入 TRUE 拔出FALSE;
        uint32_t record_voice:1;//录音状态
        uint32_t media_update:1;//多媒体上传
        uint32_t take_picture:1;//拍照
        uint32_t tcp_dail_call:1;//平台拨打电话
        uint32_t tcp_listen_call:1;//远程监听
        uint32_t detail_gnss_switch_off:1;//关闭detail gnss
        uint32_t record_data:1;//记录数据
        uint32_t fade_detail_gnss:1;//GNSS盲区
		u32 Flag_Rev :4 ;
	}System_Flag_Bit;
}System_Flag_Struct;


/*位置信息保存结构体*/
typedef struct 
{
	u8 Location_Inf[100];//最大数据长度
	u16 Location_Inf_Lens;
}BaseLocationSave_Struct;



typedef  struct 
{
  u32 Location_Queue_Len;
  u32 Tail,Head;
  BaseLocationSave_Struct Location_Save_Buffer[Location_Queue_Max];
}Location_Queue_Struct;


/*位置信息发送队列*/
extern System_Flag_Struct	System_Flag; //系统标志位域结构体	
extern Location_Queue_Struct Location_Queue;//位置信息队列

extern Location_Queue_Struct Second_Location_Queue;//第二服务器

extern void Location_Queue_Init(Location_Queue_Struct *Pscr);
extern int Enter_Location_Queue(Location_Queue_Struct *Dest,BaseLocationSave_Struct *Inf_point);
extern int Exit_Location_Queue(Location_Queue_Struct *Pscr);
extern int IS_Location_Queue_Empty(Location_Queue_Struct *Pscr);



extern void First_Server_Location_Inf_Send(void);
extern void First_Server_Location_Inf_Send(void);

extern void Analysis_Location_Inf(Location_Inf_Struct* scr_struct,BaseLocationSave_Struct *Inf_Data);//解析基础数据
extern void timeout_location_report(void);

#endif
