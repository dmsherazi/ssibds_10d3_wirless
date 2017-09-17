
/********************************************************************
 *                Copyright SSI(wuhan)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   Data_Send_Handle.c
 * version       :   0.20
 * Description   :   process the data of 808 protocol;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.20  2015/9/19 11:32:01 (markdown time) , j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/



 /****************************************************
 * Include Files
 ****************************************************/ 
 #include "location_data_send_handle.h"

 /****************************************************
* External variables declaration
*****************************************************/
System_Flag_Struct	System_Flag; //系统标志位域结构体	
Location_Queue_Struct Location_Queue ;//第一级缓存

Location_Queue_Struct Second_Location_Queue;//位置信息队列


/*****************************************************
* Local variables declaration(Static)
******************************************************/
static BaseLocationSave_Struct Last_Location_Inf_Data;
static BaseLocationSave_Struct Second_Last_Location_Inf_Data;

/*****************************************************************************
 * Function : Location_Inf_Send
 * Description:This function is used to send and save Location data;
 * Parameters : 
 *			Inf_Data:数据储存结构体指针
 * Returns:void;	
 * NOTE
 *     1、是否连接网络连接，
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
void First_Server_Location_Inf_Send(void)
{
    uint16_t servern=0;
	int Location_Queue_State = 0;//队列状态
	uint32_t temp_oim_data;
	BaseLocationSave_Struct Location_Inf_Data;//发发送缓存数据
	temp_oim_data = Real_Time_Location_Inf.oil_first;
    #if 1//file_sytem_debug
    eat_trace("Analysis_Location_Inf --> oil_first:%d !",Real_Time_Location_Inf.oil_first);
    #endif
	Real_Time_Location_Inf.Oil_Mass=(uint16_t)(temp_oim_data/100000);
	Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
    #if 1//file_sytem_debug
    eat_trace("Analysis_Location_Inf --> oim_plus:%d !",Real_Time_Location_Inf.Oil_Mass);
    #endif
    Real_Time_Location_Inf.Oil_Mass=0;
    Real_Time_Location_Inf.oil_first=(Real_Time_Location_Inf.oil_first-temp_oim_data)+(Real_Time_Location_Inf.oil_first%100000);
    #if 1//file_sytem_debug
    eat_trace("Analysis_Location_Inf --> oil_firstsdd:%d !",Real_Time_Location_Inf.oil_first);
    #endif
	if(System_Flag.System_Flag_Bit.MainAutenSucess== EAT_FALSE)//网络连接状态
	{
		Location_Queue_State = Enter_Location_Queue(&Location_Queue,&Last_Location_Inf_Data);
		if(Location_Queue_State < 0)//队列是满
		{
            #if location_data_send_printf 
            eat_trace("First_Server_Location_Inf_Send --> State:queue full.");
            #endif
			Save_Fade_Location_data();//储存数据
			Location_Queue_Init(&Location_Queue);//初始化缓存
			Enter_Location_Queue(&Location_Queue,&Last_Location_Inf_Data);//因满未成功入队的数据入队；
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("First_Server_Location_Inf_Send --> State:first buffer !!! .");
            #endif
		}
        memcpy(&Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Last_Location_Inf_Data));
	}
	else
	{
		if(System_Flag.System_Flag_Bit.Location_Send_State == Idle_State)
		{
			Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&servern,0x0200,Location_Inf_Data.Location_Inf,Location_Inf_Data.Location_Inf_Lens,EAT_FALSE,0,0);
			memcpy(&Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Last_Location_Inf_Data));
			System_Flag.System_Flag_Bit.Location_Send_State = Busy_State;
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("First_Server_Location_Inf_Send --> State:link fail temp.");
            #endif
			Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&servern,0x0200,Last_Location_Inf_Data.Location_Inf,Last_Location_Inf_Data.Location_Inf_Lens,EAT_FALSE,0,0);//发送上一次数据
			Location_Queue_State = Enter_Location_Queue(&Location_Queue,&Last_Location_Inf_Data);
			if(Location_Queue_State < 0)//队列是满
			{
                Save_Fade_Location_data();//储存数据
				Location_Queue_Init(&Location_Queue);//初始化缓存
				Enter_Location_Queue(&Location_Queue,&Location_Inf_Data);//因满未成功入队的数据入队；
			}
            memcpy(&Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Last_Location_Inf_Data));
		}
	}
}

void Second_Server_Location_Inf_Send(void)
{
    
	int Location_Queue_State = 0;//队列状态
    uint16_t servern=0;
	uint32_t temp_oim_data;
	BaseLocationSave_Struct Location_Inf_Data;//发发送缓存数据
	temp_oim_data = Real_Time_Location_Inf.oil_mass_second;
	Real_Time_Location_Inf.Oil_Mass=(uint16_t)(temp_oim_data/100000);
	Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
    #if 1//file_sytem_debug
    eat_trace("second Analysis_Location_Inf --> oim_plus:%d !",Real_Time_Location_Inf.Oil_Mass);
    #endif
    Real_Time_Location_Inf.Oil_Mass=0;
    Real_Time_Location_Inf.oil_mass_second=Real_Time_Location_Inf.oil_mass_second-temp_oim_data+(Real_Time_Location_Inf.oil_mass_second%100000);
	if(System_Flag.System_Flag_Bit.SecondAutenSucess == EAT_FALSE)//网络连接状态
	{
		Location_Queue_State = Enter_Location_Queue(&Second_Location_Queue,&Second_Last_Location_Inf_Data);
		if(Location_Queue_State < 0)//队列是满
		{
			Second_Fade_Area_Location_Save();//存入sd
			Location_Queue_Init(&Second_Location_Queue);//初始化缓存
			Enter_Location_Queue(&Second_Location_Queue,&Second_Last_Location_Inf_Data);//因满未成功入队的数据入队；
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("Second_Server_Location_Inf_Send --> State:second buffer !!! .");
            #endif
		}
        memcpy(&Second_Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Second_Last_Location_Inf_Data));
	}
	else
	{
		if(System_Flag.System_Flag_Bit.Second_Location_Send_State == Idle_State)
		{
			Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&servern,0x0200,Location_Inf_Data.Location_Inf,Location_Inf_Data.Location_Inf_Lens,EAT_FALSE,0,0);
			memcpy(&Second_Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Second_Last_Location_Inf_Data));
			System_Flag.System_Flag_Bit.Second_Location_Send_State = Busy_State;
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("Second_Server_Location_Inf_Send --> State:link fail temp.");
            #endif
			Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&servern,0x0200,Second_Last_Location_Inf_Data.Location_Inf,Second_Last_Location_Inf_Data.Location_Inf_Lens,EAT_FALSE,0,0);//发送上一次数据
			Location_Queue_State = Enter_Location_Queue(&Second_Location_Queue,&Second_Last_Location_Inf_Data);
			if(Location_Queue_State < 0)//队列是满
			{
				Second_Fade_Area_Location_Save();//存入sd
				Location_Queue_Init(&Second_Location_Queue);//初始化缓存
				Enter_Location_Queue(&Second_Location_Queue,&Location_Inf_Data);//因满未成功入队的数据入队；
			}
            memcpy(&Second_Last_Location_Inf_Data,&Location_Inf_Data,sizeof(Second_Last_Location_Inf_Data));
		}
	}
}
void system_location_upadate(void)
{
    First_Server_Location_Inf_Send();
    if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
    {
        Second_Server_Location_Inf_Send();
    }
}

/*****************************************************************************
 * Function : Temp_Location_Inf_Send
 * Description:This function is used to Temp Locatio Inf send;
 * Parameters : 
 *			Inf_Data:数据储存结构体指针
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
static void First_Server_Idle_Location_Inf_Send(void)
{
	if(IS_Location_Queue_Empty(&Location_Queue)>0)
	{
        #if location_data_send_printf 
        eat_trace("First_Server_Idle_Location_Inf_Send --> State:%u.",Location_Queue.Tail);
        #endif
	    Temp_Fade_Area_Location_Send();
		System_Flag.System_Flag_Bit.Queue_Fade_Area = EAT_TRUE;
	}
	else
	{
		if(fade_location_tail != fade_location_head)
		{
            #if location_data_send_printf 
            eat_trace("First_Server_Idle_Location_Inf_Send --> State:blind location data updata.");
            #endif
		    File_Fade_Area_Location_Send();
			System_Flag.System_Flag_Bit.File_Fade_Area = EAT_TRUE;
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("First_Server_Idle_Location_Inf_Send --> State:no locatin data Send.");
            #endif
		}
	}

}
/*****************************************************************************
 * Function : idle_location_updata
 * Description:This function is used to Temp Locatio Inf send;
 * Parameters : 
 *			第一服务器补传信息发送
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/

static void mai_link_idle_location_updata(void)
{
	static uint32_t Fade_Location_Updata_Cnt=0;
	if(System_Flag.System_Flag_Bit.MainAutenSucess== EAT_TRUE)
	{	
	    if(System_Flag.System_Flag_Bit.Location_Send_State==EAT_FALSE)
	    {
    		if((System_Flag.System_Flag_Bit.File_Fade_Area == EAT_FALSE)&&(System_Flag.System_Flag_Bit.Queue_Fade_Area == EAT_FALSE))
    		{
    			if(Fade_Location_Updata_Cnt!=0) Fade_Location_Updata_Cnt = 0;
                First_Server_Idle_Location_Inf_Send();
                
                #if time_process_debug_printf 
                eat_trace("mai_link_idle_location_updata --> 空闲态发送补传数据.");
                #endif
    		}
		}
		else
		{
            #if time_process_debug_printf 
            eat_trace("mai_link_idle_location_updata --> 位置信息发送中.");
            #endif
		}
		if((System_Flag.System_Flag_Bit.File_Fade_Area == EAT_TRUE)||(System_Flag.System_Flag_Bit.Queue_Fade_Area == EAT_TRUE))
		{
			if(Fade_Location_Updata_Cnt == system_param.location_reporting_param.default_frze)
			{
				Fade_Location_Updata_Cnt = 0;
				System_Flag.System_Flag_Bit.File_Fade_Area = EAT_FALSE;
				System_Flag.System_Flag_Bit.Queue_Fade_Area = EAT_FALSE;

				
                #if time_process_debug_printf 
                eat_trace("mai_link_idle_location_updata --> 补传位置位置信息发送超时.");
                #endif
			}
		}
	}
	else
	{
		if(Fade_Location_Updata_Cnt != 0)
		{
			Fade_Location_Updata_Cnt = 0;
		}
		if(System_Flag.System_Flag_Bit.File_Fade_Area == EAT_TRUE)
		{
			System_Flag.System_Flag_Bit.File_Fade_Area = EAT_FALSE;
		}
		else if(System_Flag.System_Flag_Bit.Queue_Fade_Area == EAT_TRUE)
		{
			System_Flag.System_Flag_Bit.Queue_Fade_Area = EAT_FALSE;
		}
	    if(System_Flag.System_Flag_Bit.Location_Send_State==EAT_TRUE)
		{
		    System_Flag.System_Flag_Bit.Location_Send_State=EAT_FALSE;
		}
	}
}
/*****************************************************************************
 * Function : Second_Server_Idle_Location_Inf_Send
 * Description:This function is used to Temp Locatio Inf send;
 * Parameters : 
 *			第二服务器数据发送
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
static void Second_Server_Idle_Location_Inf_Send(void)
{
	if(IS_Location_Queue_Empty(&Second_Location_Queue)>0)
	{
        #if location_data_send_printf 
        eat_trace("Second_Server_Idle_Location_Inf_Send --> State:%u.",Second_Location_Queue.Tail);
        #endif
		Second_Temp_Fade_Area_Location_Send();
		System_Flag.System_Flag_Bit.Second_Queue_Fade_Area = EAT_TRUE;
	}
	else
	{
		if(second_fade_location_tail !=second_fade_location_head)
		{
            #if location_data_send_printf 
            eat_trace("Second_Server_Idle_Location_Inf_Send --> State:blind location data updata.");
            #endif
		    Second_File_Fade_Area_Location_Send();
			System_Flag.System_Flag_Bit.Second_File_Fade_Area = EAT_TRUE;
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("Second_Server_Idle_Location_Inf_Send --> State:no locatin data Send.");
            #endif
		}
	}
}
/*****************************************************************************
 * Function : second_link_idle_location_updata
 * Description:This function is used to Temp Locatio Inf send;
 * Parameters : 
 *			第一服务器补传信息发送
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
static void second_link_idle_location_updata(void)
{
	static uint32_t Fade_Location_Updata_Cnt=0;
	if(System_Flag.System_Flag_Bit.SecondAutenSucess== EAT_TRUE)
	{	
        Fade_Location_Updata_Cnt++;
	    if(System_Flag.System_Flag_Bit.Second_Location_Send_State==Idle_State)
	    {
        	if((System_Flag.System_Flag_Bit.Second_File_Fade_Area == EAT_FALSE)&&(System_Flag.System_Flag_Bit.Second_Queue_Fade_Area == EAT_FALSE))
        	{
        		if(Fade_Location_Updata_Cnt!=0)
        		{
        			Fade_Location_Updata_Cnt = 0;
        		}
                Second_Server_Idle_Location_Inf_Send();
        	}
		}
		else
		{
            #if location_data_send_printf 
            eat_trace("Second_Server_Idle_Location_Inf_Send --> State:no locatin data Send.");
            #endif
		}
        if((System_Flag.System_Flag_Bit.Second_File_Fade_Area == EAT_TRUE)||(System_Flag.System_Flag_Bit.Second_Queue_Fade_Area == EAT_TRUE))
        {
            if(Fade_Location_Updata_Cnt == (system_param.location_reporting_param.default_frze/2))
            {
                Fade_Location_Updata_Cnt = 0;
                System_Flag.System_Flag_Bit.Second_File_Fade_Area = EAT_FALSE;
                System_Flag.System_Flag_Bit.Second_Queue_Fade_Area = EAT_FALSE;
                
                #if location_data_send_printf 
                eat_trace("Second_Server_Idle_Location_Inf_Send --> State:fade send time out !.");
                #endif
            }
        }
	}
	else
	{
		if(Fade_Location_Updata_Cnt != 0)
		{
			Fade_Location_Updata_Cnt = 0;
		}
		if(System_Flag.System_Flag_Bit.Second_File_Fade_Area == EAT_TRUE)
		{
			System_Flag.System_Flag_Bit.Second_File_Fade_Area = EAT_FALSE;
		}
		else if(System_Flag.System_Flag_Bit.Second_Queue_Fade_Area == EAT_TRUE)
		{
			System_Flag.System_Flag_Bit.Second_Queue_Fade_Area = EAT_FALSE;
		}
	    if(System_Flag.System_Flag_Bit.Second_Location_Send_State==EAT_TRUE)
		{
		    System_Flag.System_Flag_Bit.Second_Location_Send_State=EAT_FALSE;
		}
	}
}
/*****************************************************************************
 * Function : system_idle_location_upadate
 * Description:This function is used to Temp Locatio Inf send;
 * Parameters : 
 *			补传信息发送
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
static void system_idle_location_upadate(void)
{
    mai_link_idle_location_updata();
    if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
    {
        second_link_idle_location_updata();
    }
}
/*****************************************************************************
 * Function : timeout_location_report
 * Description:位置信息发送，包括补传
 * Parameters : 
 *			补传信息发送
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
void timeout_location_report(void)
{
    static uint32_t timout_count=0;
    location_report_policy_proc();
    system_idle_location_upadate();
}

/*****************************************************************************
 * Function : Analysis_Location_Inf
 * Description:This function is used to Analysis struct to byte;
 * Parameters : 
 *			Inf_Data:数据储存结构体指针
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
void Analysis_Location_Inf(Location_Inf_Struct* scr_struct,BaseLocationSave_Struct *Inf_Data)
{
	Inf_Data->Location_Inf_Lens = 0;
	Big_End_DwordtoByte(scr_struct->Alarm_Inf.Alarm_Original,Inf_Data->Location_Inf);
	Inf_Data->Location_Inf_Lens += 4;
	
	Big_End_DwordtoByte(scr_struct->VDR_State_Inf.State_Inf,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;
	
	Big_End_DwordtoByte(scr_struct->Location_Latitude,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;
	
	Big_End_DwordtoByte(scr_struct->Location_Longitude,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;
	
	Big_End_WordtoByte(scr_struct->Location_Altitude,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;

	Big_End_WordtoByte(scr_struct->Location_Speed,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;

	Big_End_WordtoByte(scr_struct->Location_Direction,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;
	
	if(scr_struct->IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)
	{
        get_sim800_store_timer();
	    memcpy(Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens,Real_Time_Location_Inf.Curren_Time,6);
	}
	else
	{
	    memcpy(Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens,scr_struct->Curren_Time,6);
	}
	Inf_Data->Location_Inf_Lens += 6;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x01;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x04;
	Inf_Data->Location_Inf_Lens +=1;

	Big_End_DwordtoByte(scr_struct->Vehicle_Odometer,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
	Inf_Data->Location_Inf_Lens +=1;

	Big_End_WordtoByte(scr_struct->Oil_Mass,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x03;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
	Inf_Data->Location_Inf_Lens +=1;
	
	Big_End_WordtoByte(scr_struct->Travel_Speed,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;	

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x04;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
	Inf_Data->Location_Inf_Lens +=1;
	
	Big_End_WordtoByte(scr_struct->Manual_Work_ID,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 2;

	if(scr_struct->Alarm_Inf.Alarm_Bit.Speed_Out == EAT_TRUE)
	{
		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x11;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Over_Speed_Alarm_Add.Over_Speed_Location_Type;
		Inf_Data->Location_Inf_Lens +=1;

		if(scr_struct->Over_Speed_Alarm_Add.Over_Speed_Location_Type != 0x00)
		{
			Big_End_DwordtoByte(scr_struct->Over_Speed_Alarm_Add.Area_ID,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
			Inf_Data->Location_Inf_Lens +=4;
		}
	}
	if(scr_struct->Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
	{
		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x12;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x06;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Area_InorOut_Inf_Add.InorOut_Area_Type;
		Inf_Data->Location_Inf_Lens +=1;

		Big_End_DwordtoByte(scr_struct->Area_InorOut_Inf_Add.InorOut_Area_ID,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
		Inf_Data->Location_Inf_Lens += 4;
			
		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Area_InorOut_Inf_Add.InorOut_Area_Action;
		Inf_Data->Location_Inf_Lens +=1;

	}
	if(scr_struct->Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort == EAT_TRUE)
	{
		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x13;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x07;
		Inf_Data->Location_Inf_Lens +=1;
				
		Big_End_DwordtoByte(scr_struct->Route_Driv_OutorShort_Add.Route_Driv_OutorShort_ID,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
		Inf_Data->Location_Inf_Lens += 4;

		Big_End_DwordtoByte(scr_struct->Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Time,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
		Inf_Data->Location_Inf_Lens += 2;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Reslut;
		Inf_Data->Location_Inf_Lens +=1;
	}
	
	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x25;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x04;
	Inf_Data->Location_Inf_Lens +=1;

	Big_End_DwordtoByte(scr_struct->Extend_Signle_Add.Extend_Signle,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;

	if(scr_struct->IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)
	{
		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x2A;
		Inf_Data->Location_Inf_Lens +=1;

		Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
		Inf_Data->Location_Inf_Lens +=1;

		Big_End_WordtoByte(scr_struct->IO_State_Add.IO_State,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
		Inf_Data->Location_Inf_Lens += 2;
	}

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x2B;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x04;
	Inf_Data->Location_Inf_Lens +=1;

	Big_End_DwordtoByte(scr_struct->Analog_Signal,Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
	Inf_Data->Location_Inf_Lens += 4;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x30;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x01;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Sing_strength;
	Inf_Data->Location_Inf_Lens +=1;
	
	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x31;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x01;
	Inf_Data->Location_Inf_Lens +=1;

	Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = scr_struct->Satelite_Amount;
	Inf_Data->Location_Inf_Lens +=1;
	
    Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0xE1;
    Inf_Data->Location_Inf_Lens +=1;
    
    Inf_Data->Location_Inf[Inf_Data->Location_Inf_Lens] = 0x02;
    Inf_Data->Location_Inf_Lens +=1;
    
    Big_End_WordtoByte((scr_struct->Power_volate/10),Inf_Data->Location_Inf + Inf_Data->Location_Inf_Lens);
    Inf_Data->Location_Inf_Lens += 2;
	
}
/*****************************************************************************
 * Function : Location_Queue_Init
 * Description:This function is used to initialize struct Location_Queue
 * Parameters : void
 * Returns:void;	
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
void Location_Queue_Init(Location_Queue_Struct *Pscr)
{
	Pscr->Head = 0;
	Pscr->Tail = 0;
	Pscr->Location_Queue_Len = Location_Queue_Max;
}
/*****************************************************************************
 * Function : Location_Queue_Init
 * Description:This function is used to Enter Queue
 * Parameters : 
 *				Pscr:队列结构体指针
 *				inf_point :原始位置数据
 *				Inf_Len:信息长度
 * Returns:
 *			-1:队列满;
 *			 1:队列正常
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
 int Enter_Location_Queue(Location_Queue_Struct *Dest,BaseLocationSave_Struct *Inf_point)
 {
  	int Ret;
 	u32 New_Tail=0;
 	New_Tail = (Dest->Tail + 1)%Dest->Location_Queue_Len;
 	if(New_Tail == Dest->Head)
 	{
 		Ret= -1;
 	}
 	else
 	{
 		memcpy(Dest->Location_Save_Buffer[Dest->Tail].Location_Inf,Inf_point->Location_Inf,Inf_point->Location_Inf_Lens);
 		Dest->Location_Save_Buffer[Dest->Tail].Location_Inf_Lens = Inf_point->Location_Inf_Lens;

 		Dest->Tail = New_Tail;

 		Ret= 1;
 	}
 	return Ret;
 }
 
 /*****************************************************************************
 * Function : Exit_Location_Queue
 * Description:This function is used to exit queue
 * Parameters : 
 *				Pscr:队列结构体指针
 * Returns:
 *			-1:队列空;
 *			 1:队列正常
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/
 int Exit_Location_Queue(Location_Queue_Struct *Pscr)
 {
 	int Ret;
 	if(Pscr->Tail == Pscr->Head)
 	{
 		Ret= -1;
 	}
 	else
 	{
 		Pscr->Head = (Pscr->Head + 1) % Pscr->Location_Queue_Len;

 		Ret= 1;
 	}
 	return Ret;
 }

  /*****************************************************************************
 * Function : IS_Location_Queue_Empty
 * Description:This function is used to check wether the queue is empty
 * Parameters : 
 *				Pscr:队列结构体指针
 * Returns:
 *			-1:队列空;
 *			 1:队列正常
 * NOTE
 *     EVERY_STURUCT need to be initalized;
 *****************************************************************************/

 int IS_Location_Queue_Empty(Location_Queue_Struct *Pscr)
 {
 	int Ret;
 	if(Pscr->Tail == Pscr->Head)
 	{
 		Ret= -1;
 	}
 	else	
 	{
 		Ret= 1;
 	}
 	return Ret;
 }
 
