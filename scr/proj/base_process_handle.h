

#ifndef _BASE_PROCESS_HANDLE_H_
	#define _BASE_PROCESS_HANDLE_H_

    #include "eat_interface.h"



extern Alarm_Inf_Struct Shield_SMS_Alarm_Inf;//短信报警屏蔽	
extern Alarm_Inf_Struct Shield_Alarm_Inf;//报警屏蔽
extern Location_Inf_Struct Real_Time_Location_Inf; //实时位置信息结构体

extern Location_Inf_Struct Location_Inf;//位置信息；
extern void Inf_Struct_Init(void);
extern void Alarm_Inf_Process(Alarm_Inf_Struct *Temp_Alarm_Inf);
extern void State_Inf_Process(State_Inf_Struct *Temp_State_Inf);
extern void system_sleep_juged(void);
#endif


