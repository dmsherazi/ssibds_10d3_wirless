/********************************************************************
 *                Copyright SSI(wuhan)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   Base_Process_Handle.c
 * version       :   0.20
 * Description   :   optimize the data communication program
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.20  2015-09-19, maobin, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/


/****************************************************
 * Include Files
 ****************************************************/ 
#include "Base_Process_Handle.h"




/****************************************************
* External variables declaration
*****************************************************/
Location_Inf_Struct Real_Time_Location_Inf; //实时位置信息结构体

Alarm_Inf_Struct Shield_SMS_Alarm_Inf;//短信报警屏蔽	
Alarm_Inf_Struct Shield_Alarm_Inf;//报警屏蔽


/*****************************************************
* Local variables declaration(Static)
******************************************************/

/*****************************************************************************
 * Function :Alarm_Inf_Process
 * Description:This function is used to process Alram_Inf.
 * Parameters : Alarm_Inf_Struct :报警结构体
 * Returns:
 *  	Void 	
 * NOTE
 *     Nothing 
 *****************************************************************************/
void Alarm_Inf_Process(Alarm_Inf_Struct *Temp_Alarm_Inf)
{
	/***************************SOS报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Sos == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos == EAT_FALSE)
				&&(Temp_Alarm_Inf->Alarm_Bit.Sos == EAT_TRUE))
		{
			Real_Time_Location_Inf.Manual_Work_ID += 1;
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Sos == EAT_TRUE)
			{
				worning_sms_send("紧急");
			}
            system_location_upadate();
		}
	}
	/***************************超速报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Speed_Out == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Out = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Out == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Speed_Out == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Out = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Speed_Out == EAT_TRUE)
			{
				worning_sms_send("超速");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Out == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Speed_Out == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Out = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************疲劳驾驶报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Fatigue_Driv == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Fatigue_Driv == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Fatigue_Driv == EAT_TRUE)
			{
				worning_sms_send("疲劳驾驶");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Fatigue_Driv == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************天线断开报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Antenna_Discon == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Discon = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Discon == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Antenna_Discon == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Discon = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Antenna_Discon == EAT_TRUE)
			{
				worning_sms_send("天线断开");
			}
            /*天线断开语音报警*/
            //antena_indef_alarm_tone();
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Discon == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Antenna_Discon == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Discon = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************天线短路报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Antenna_Short_Circuit == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit == EAT_TRUE)
			{
				worning_sms_send("天线短路");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Antenna_Short_Circuit == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Antenna_Short_Circuit = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************欠压报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Undervoltage == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
			{
				worning_sms_send("欠压");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Undervoltage == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************掉电报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Power_Down == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Power_Down == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Power_Down == EAT_TRUE)
			{
				worning_sms_send("掉电");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Power_Down == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************超速预警报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Speed_Early_Warning == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Early_Warning = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Early_Warning == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Speed_Early_Warning == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Early_Warning = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Speed_Early_Warning == EAT_TRUE)
			{
				worning_sms_send("超速预警");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Early_Warning == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Speed_Early_Warning == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Speed_Early_Warning = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************疲劳驾驶预警报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_TRUE)
			{
				worning_sms_send("疲劳驾驶预警");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Fatigue_Driv_Early_Warning == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Fatigue_Driv_Early_Warning = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************SOS电话**********************************/	
	if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.SOS_Phone == EAT_TRUE))
	{
        Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone = EAT_TRUE;
        dail_voice_call(system_param.phone_number_param.sos_telephone);
	}
	/***************************碰撞报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Crashed == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Crashed = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Crashed == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Crashed == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Crashed = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Crashed == EAT_TRUE)
			{
				worning_sms_send("碰撞");
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Crashed == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Crashed == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Crashed = EAT_FALSE;
            system_location_upadate();
		}
	}
	/***************************侧翻报警**********************************/	
	if(Shield_Alarm_Inf.Alarm_Bit.Rolloved == EAT_TRUE)
	{
		Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Rolloved = EAT_FALSE;
	}
	else
	{
		if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Rolloved == EAT_FALSE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Rolloved == EAT_TRUE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Rolloved = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Rolloved == EAT_TRUE)
			{
				worning_sms_send("侧翻");	
			}
            system_location_upadate();
		}
		else if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Rolloved == EAT_TRUE)
			&&(Temp_Alarm_Inf->Alarm_Bit.Rolloved == EAT_FALSE))
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Rolloved = EAT_FALSE;
            system_location_upadate();
		}
	}
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Danger = Temp_Alarm_Inf->Alarm_Bit.Danger;
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.GNSS_Mode = Temp_Alarm_Inf->Alarm_Bit.GNSS_Mode;
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.TTS_Failure = Temp_Alarm_Inf->Alarm_Bit.TTS_Failure;
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Camer_Failure = Temp_Alarm_Inf->Alarm_Bit.Camer_Failure;
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.IC_Failure = Temp_Alarm_Inf->Alarm_Bit.IC_Failure;
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.VSS_Failure = Temp_Alarm_Inf->Alarm_Bit.VSS_Failure;
}
/*****************************************************************************
 * Function :State_Inf_Process
 * Description:This function is used to process State_Inf.
 * Parameters : Alarm_Inf_Struct :状态结构体
 * Returns:
 *  	Void 	
 * NOTE
 *     Nothing 
 *****************************************************************************/
 void system_sleep_juged(void)
 {
 	static u8 lucv_system_sleep_cnt=0;
    #if location_data_send_printf 
    eat_trace("system_sleep_juged --> acc state:%u,Power_Down:%d,Undervoltage::%d",Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC,Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down,Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage);
    #endif
	if((Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC ==EAT_FALSE)&&(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down==EAT_FALSE)
		&&(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage==EAT_FALSE))
	{
        #if location_data_send_printf 
        eat_trace("system_sleep_juged --> acc state:%u,Power_Down:%d,Undervoltage::%d",Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC,Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Power_Down,Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage);
        #endif
		lucv_system_sleep_cnt++;
		if(3== lucv_system_sleep_cnt)
		{
			lucv_system_sleep_cnt=0;
			Real_Time_Location_Inf.Location_Speed =0;//卫星速度
			Real_Time_Location_Inf.Travel_Speed = 0;//行驶记录速度
			Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep =EAT_TRUE;
            task_msg_send(EAT_USER_0,EAT_USER_0,SystemSleepMsg);
		}
	}
	else
	{
		if(lucv_system_sleep_cnt != 0)
		{
			lucv_system_sleep_cnt = 0;
		}
	}
}
void State_Inf_Process(State_Inf_Struct *Temp_State_Inf)
{
    uint8_t msg_point=sleep_action;
    static uint8_t sleep_remanber=EAT_FALSE;
	if((Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC)!=(Temp_State_Inf->State_Bit.ACC))
	{
		Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC = Temp_State_Inf->State_Bit.ACC;
        system_location_upadate();
	}
	if((Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Wether_Location)!=(Temp_State_Inf->State_Bit.Wether_Location))
	{
		gps_redress_timer();
		Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Wether_Location = Temp_State_Inf->State_Bit.Wether_Location;       
        system_location_upadate();
	}
	if((system_param.param_save.statue.ip1_lock!=(Temp_State_Inf->State_Bit.ip1_lock_board_state))||(system_param.param_save.statue.ip2_lock!=(Temp_State_Inf->State_Bit.ip2_lock_board_state)))
	{
		system_param.param_save.statue.ip1_lock = Temp_State_Inf->State_Bit.ip1_lock_board_state; 
		system_param.param_save.statue.ip2_lock = Temp_State_Inf->State_Bit.ip2_lock_board_state; 
		system_para_save_at_once();//保存参数
	}
}
/*****************************************************************************
 * Function :Location_Inf_Struct_Init
 * Description:This function is used to initialize Location_Inf_Struct.
 * Parameters : void
 * Returns:
 *  	Void 	
 * NOTE
 *     Nothing 
 *****************************************************************************/
void Inf_Struct_Init(void)
{
	System_Flag.System_Flag_Integral = 0;	//系统标志位初始化
	Real_Time_Location_Inf.Alarm_Inf.Alarm_Original =0;
	Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC = EAT_TRUE;
	Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Wether_Location = EAT_FALSE;
    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.GPS = 0x01;
    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.BD = 0x01;
	Shield_SMS_Alarm_Inf.Alarm_Original = 0x00000000;//短信报警默认为关
	Shield_Alarm_Inf.Alarm_Original = 0x00000000;    //普通报警默认为开
	memset(&Shield_Alarm_Inf,0,sizeof(Shield_Alarm_Inf)); //初始化位置储存结构体
	Location_Queue_Init(&Location_Queue);
	Location_Queue_Init(&Second_Location_Queue);
}

