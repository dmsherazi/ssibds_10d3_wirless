
#include "regional_judgment.h"

static u8 Ju_Sp_Set = EAT_FALSE;
static u8 Yuan_Sp_Set = EAT_FALSE;
static u8 Duo_Sp_Set = EAT_FALSE;
static u8 LuXian_Sp_Set = EAT_FALSE;

static u8 QuYu_Sp_Set = EAT_FALSE;

void initiative_close_gprs_data(void)
{
    socket_close(Main_Link.mode_Socket);
    socket_close(Second_Link.mode_Socket);
    Main_Link.link_staue = EAT_FALSE;
    Second_Link.link_staue = EAT_FALSE;
    
    Main_Link.get_ip_statue=EAT_FALSE;      
    Second_Link.get_ip_statue=EAT_FALSE;
    
    System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
    System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
    eat_timer_stop(EAT_TIMER_5);
    eat_timer_stop(EAT_TIMER_6);
}
void QuYu_Seting_Speed(u32 Speed,u32 Speed_Time)
{
	u8 Temp[20]={0};
	u32 Data_Way=0;
	u16 TRUE_SEED=0;
	Temp[0]=0x00;
	Big_End_DwordtoByte(Speed, Temp+1);
	Big_End_DwordtoByte(Speed_Time, Temp+5);
	TRUE_SEED=(uint16_t)(system_param.speed_warning_dif/10);//真实速度；
	Big_End_WordtoByte(TRUE_SEED,Temp+9);
	uart1_send_data_frame(0x6C,Temp,11);
}

void Yuan_QuYU_ACTION(u32 Wei,u32 Jing)//圆形区域动作
{
	u8 Cnt;
	u8 Ret;
	static u8 YuanStat[QuYu_Max]={0};//区域状态
	for(Cnt=0;Cnt<Yuan_QuYu_Cnt;Cnt++)
	{
		Ret=Yuan_Judgment(Wei,Jing,QuYu_Yuan[Cnt].Cemt_Point_WeiDu,QuYu_Yuan[Cnt].Cemt_Point_JingDu,QuYu_Yuan[Cnt].Banjing_R);
		if(Ret==EAT_TRUE)//原来外，现在内
		{
			if((QuYu_Yuan[Cnt].Yuan_Shuxing>>3)&0x0001)
			{
				if(YuanStat[Cnt]==EAT_FALSE)
				{
    				#if regional_judment_debug_printf
                    eat_trace("Yuan_QuYU_ACTION--> 进圆形区域  !.");
    				#endif
					YuanStat[Cnt]=EAT_TRUE;
					if((QuYu_Yuan[Cnt].Yuan_Shuxing>>1)&0x0001)
					{
						if(Yuan_Sp_Set == EAT_FALSE)
						{
							Yuan_Sp_Set = EAT_TRUE;
						}
						Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x01;
						Real_Time_Location_Inf.Over_Speed_Alarm_Add.Area_ID = QuYu_Yuan[Cnt].Yuan_ID;
						
						QuYu_Seting_Speed((u32)(QuYu_Yuan[Cnt].Max_Speed),(u32)(QuYu_Yuan[Cnt].Time_O));
					}
					if((QuYu_Yuan[Cnt].Yuan_Shuxing>>2)&0x0001)
					{
						eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
						eat_modem_write("AT+CTTS=2,\"进入圆形区域\"\r\n",strlen("AT+CTTS=2,\"进入圆形区域\"\r\n"));
					}
					if((QuYu_Yuan[Cnt].Yuan_Shuxing>>3)&0x0001)
					{
						if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
						{
							Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
							Real_Time_Location_Inf.Manual_Work_ID +=1;
							Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x01;//圆形区域
							Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Yuan[Cnt].Yuan_ID;//进出区域报警ID
							
							Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x00;//进区域
							if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
							{
								worning_sms_send("进圆形区域");
							}
                            system_location_upadate();
						}
					}
					if((QuYu_Yuan[Cnt].Yuan_Shuxing>>14)&0x0001)
					{
			 
						initiative_close_gprs_data();//关闭通讯
						if((QuYu_Yuan[Cnt].Yuan_Shuxing>>15)&0x0001)
						{
							//Eat_FSFile_Create_and_open(GNSS_Data_Save);
                            #if regional_judment_debug_printf
                            eat_trace("Yuan_QuYU_ACTION --> 进入圆形区域关闭通信记录GNSS数据   !.");
                            #endif
        				 	System_Flag.System_Flag_Bit.record_data=EAT_TRUE;
						}
						else
						{
						    
                            #if regional_judment_debug_printf
                            eat_trace("Yuan_QuYU_ACTION --> 进入圆形区域关闭通信不记录GNSS数据   !.");
                            #endif
        					System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_TRUE;
						}
					}
					else//没有关闭通讯
					{
						if(((QuYu_Yuan[Cnt].Yuan_Shuxing>>15)&0x0001)==0x00)
						{
        					System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_FALSE;
						}
					}
				}
				else
				{
				    break;
				}
			}
		}
		else 
		{
			if(YuanStat[Cnt]==EAT_TRUE)
			{
				YuanStat[Cnt]=EAT_FALSE;
                #if regional_judment_debug_printf
                eat_trace("Yuan_QuYU_ACTION--> 离开圆形区域  !.");
                #endif
				if((QuYu_Yuan[Cnt].Yuan_Shuxing>>4)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"离开圆形区域\"\r\n",strlen("AT+CTTS=2,\"离开圆形区域\"\r\n"));
				}
				if((QuYu_Yuan[Cnt].Yuan_Shuxing>>5)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
								
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x01;//圆形区域
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Yuan[Cnt].Yuan_ID;//进出区域报警ID
						Real_Time_Location_Inf.Manual_Work_ID +=1;	
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x01;//出区域

						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
						{
                            worning_sms_send("出圆形区域");
						}
                        system_location_upadate();
					}
				}
				if((QuYu_Yuan[Cnt].Yuan_Shuxing>>14)&0x0001)
				{
                    close_gprs_bearer();
                    task_msg_send(EAT_USER_1,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
				}
				if(((QuYu_Yuan[Cnt].Yuan_Shuxing>>15)&0x0001)==0x00)
				{
                    System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_FALSE;
				}
			}	
		}
	}
	if(Yuan_Sp_Set == EAT_TRUE)
	{
		for(Cnt=0;Cnt < Yuan_QuYu_Cnt;Cnt++)
		{
			if(YuanStat[Cnt]==EAT_TRUE)
			{
				if((QuYu_Yuan[Cnt].Yuan_Shuxing>>1)&0x0001)
				{
					return;
				}
			}
		}
		Yuan_Sp_Set = EAT_FALSE;
	}
}
void Ju_QuYU_ACTION(u32 Wei,u32 Jing)//矩形形区域动作
{
	u8 Cnt;
	u8 Ret;
	static u8 JUState[QuYu_Max]={0};
	for(Cnt=0;Cnt<Juxing_QuYu_Cnt;Cnt++)
	{
		Ret=JuXing_Judgment(Wei,Jing,QuYu_Juxing[Cnt].ZUO_Point_WeiDu,QuYu_Juxing[Cnt].ZUO_Point_JingDu,QuYu_Juxing[Cnt].YOU_Point_WeiDu,QuYu_Juxing[Cnt].YOU_Point_JingDu);
		if(Ret==EAT_TRUE)
		{
			if(JUState[Cnt]==EAT_FALSE)
			{
				JUState[Cnt]=EAT_TRUE;
                #if regional_judment_debug_printf
                eat_trace("PlatformCertification--> 进入矩形区域  !.");
                #endif
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>1)&0x0001)
				{
					if(Ju_Sp_Set == EAT_FALSE)
					{
						Ju_Sp_Set = EAT_TRUE;
					}
					Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x02;
					Real_Time_Location_Inf.Over_Speed_Alarm_Add.Area_ID = QuYu_Juxing[Cnt].Ju_ID;
					QuYu_Seting_Speed((u32)(QuYu_Juxing[Cnt].Max_Speed),(u32)(QuYu_Juxing[Cnt].Time_O));
				}
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>2)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"进入矩形区域\"\r\n",strlen("AT+CTTS=2,\"进入矩形区域\"\r\n"));
				}
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>3)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x02;//矩形区域
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Juxing[Cnt].Ju_ID;//进出区域报警ID
						
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x00;//进区域
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
						{
							worning_sms_send("进矩形区域");			
						}
                        system_location_upadate();
					}
				}
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>14)&0x0001)//关闭通信
				{		
                    initiative_close_gprs_data();
					if((QuYu_Juxing[Cnt].Ju_Shuxing>>15)&0x0001)//采集GNSS数据
					{
						//Eat_FSFile_Create_and_open(GNSS_Data_Save);
                        System_Flag.System_Flag_Bit.record_data=EAT_TRUE;
					}
					else//不采集GNSS
					{
                        System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_TRUE;
					}
				}
				else//不关闭通信
				{						
					if(((QuYu_Juxing[Cnt].Ju_Shuxing>>15)&0x0001)==0x00)//不采集GNSS
					{
                        System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_TRUE;
					}
				}
			}
			{
			    break;
			}
		}
		else
		{
			if(JUState[Cnt]==EAT_TRUE)
			{	
				JUState[Cnt]=EAT_FALSE;
                #if regional_judment_debug_printf
                eat_trace("PlatformCertification--> 出矩形形区域  !.");
                #endif
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>4)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"离开矩形区域\"\r\n",strlen("AT+CTTS=2,\"离开矩形区域\"\r\n"));
				}
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>5)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
							
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x02;//矩形区域
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Juxing[Cnt].Ju_ID;//进出区域报警ID
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x01;//出区域
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
						{
							worning_sms_send("出矩形区域");	
							
						}
                        system_location_upadate();
					}
				}
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>14)&0x0001)
				{
                    close_gprs_bearer();
                    task_msg_send(EAT_USER_1,EAT_USER_0,StartBearerOpen);
				}
				if((((QuYu_Juxing[Cnt].Ju_Shuxing>>15)&0x0001))==0x00)
				{
                    System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_FALSE;
				}
			}
		}
	}
	if(Ju_Sp_Set == EAT_TRUE)
	{
		for(Cnt=0;Cnt < Juxing_QuYu_Cnt;Cnt++)
		{
			if(JUState[Cnt]==EAT_TRUE)
			{
				if((QuYu_Juxing[Cnt].Ju_Shuxing>>1)&0x0001)
				{
					return;
				}
			}
		}
		Ju_Sp_Set = EAT_FALSE;
	}
}
void LuXian_QuYU_ACTION(u32 Wei_d,u32 Jing_d)//路线区域动作
{
	u8 Cnt=0;
	u8 Ret=0;//返回数据1为在路段内 0为偏离
	u8 Point_Cnt;
	static u8 LuXian_Speed = EAT_FALSE;
	static u8 LuXianSate[2]={0};//路线状态0为偏移1为未偏移
	for(Cnt=0;Cnt<Luxian_QuYu_Cnt;Cnt++)
	{
		Ret=Luxian_Judgmentu(Wei_d,Jing_d,Cnt);
		if(Ret==EAT_TRUE)
		{
			if(LuXianSate[Cnt]==EAT_FALSE)//进路段
			{
				LuXianSate[Cnt]=EAT_TRUE;
                #if regional_judment_debug_printf
                eat_trace("PlatformCertification--> 进入设定路线  !.");
                #endif
				if(((LuXian_Date[Cnt].LuXian_Shuxing)>>2)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"进入设定路线\"\r\n",strlen("AT+CTTS=2,\"进入设定路线\"\r\n"));
				}
				if(((LuXian_Date[Cnt].LuXian_Shuxing)>>3)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Route_In_Out== EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Route_In_Out = EAT_TRUE;
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x04;
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=LuXian_Date[Cnt].LuXian_ID;//进出区域报警ID
							
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x00;//进区域
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Route_In_Out == EAT_TRUE)
						{
							worning_sms_send("进路线");		
						}
                        system_location_upadate();
					}
				}
			}
			else
			{
			    break;
			}
		}
		else 
		{
			if(LuXianSate[Cnt]==EAT_TRUE)
			{
				LuXianSate[Cnt]=EAT_FALSE;
				if(((LuXian_Date[Cnt].LuXian_Shuxing)>>4)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"离开设定路线\"\r\n",strlen("AT+CTTS=2,\"离开设定路线\"\r\n"));
				}
				if(((LuXian_Date[Cnt].LuXian_Shuxing)>>5)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Route_In_Out== EAT_FALSE)
					{
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x04;//路线
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=LuXian_Date[Cnt].LuXian_ID;//进出区域报警ID
							
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x01;
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Route_In_Out == EAT_TRUE)
						{
							worning_sms_send("出路线");		
						}
                        system_location_upadate();
					}
				}
			}
		}
	}
	for(Cnt=0;Cnt<Luxian_QuYu_Cnt;Cnt++)
	{
		if(LuXianSate[Cnt]==EAT_TRUE)
		{
			if(Shield_Alarm_Inf.Alarm_Bit.Off_Route== EAT_FALSE)
			{
				if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Off_Route ==EAT_TRUE)
				{
					Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Off_Route = EAT_FALSE;
				}
			}
			return;
		}
	}
	if(Shield_Alarm_Inf.Alarm_Bit.Off_Route== EAT_FALSE)
	{
		if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Off_Route ==EAT_FALSE)
		{
			Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Off_Route = EAT_TRUE;
			if(Shield_SMS_Alarm_Inf.Alarm_Bit.Off_Route == EAT_TRUE)
			{
				worning_sms_send("偏离路线");		
			}
            system_location_upadate();
		}
        #if regional_judment_debug_printf
        eat_trace("PlatformCertification--> 偏离路线  !.");
        #endif
	}
	if(LuXian_Sp_Set == EAT_TRUE)
	{
		LuXian_Sp_Set = EAT_FALSE;
		Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x00;
		seting_data_to_board(0x00);
	}	
}
void Duo_QuYU_ACTION(u32 Wei,u32 Jing)//多边形形区域动作
{
	u8 Cnt;
	u8 Ret;
	double x,y;
	static u8 DUOState[2]={0};
	x=Jing/1000000.0;
	y=Wei/1000000.0;
	GaussCalc(x,y,&x,&y);// Long 
	for(Cnt=0;Cnt<Duo_QuYu_Cnt;Cnt++)
	{
		Ret=pointInPolygon(x,y,Cnt);
		if(Ret==EAT_TRUE)
		{
			if(DUOState[Cnt]==EAT_FALSE)
			{
				DUOState[Cnt]=EAT_TRUE;
                #if regional_judment_debug_printf
                eat_trace("PlatformCertification--> 进入多边形  !.");
                #endif
				if((QuYu_Duo[Cnt].DUO_Shuxing>>1)&0x0001)
				{
					if(Duo_Sp_Set == EAT_FALSE)
					{
						Duo_Sp_Set = EAT_TRUE;
					}
					Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x03;
					Real_Time_Location_Inf.Over_Speed_Alarm_Add.Area_ID = QuYu_Duo[Cnt].Duo_ID;					
					QuYu_Seting_Speed((u32)(QuYu_Duo[Cnt].Max_Speed),(u32)(QuYu_Duo[Cnt].Time_O));
				}
				if((QuYu_Duo[Cnt].DUO_Shuxing>>2)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"进多边形区域\"\r\n",strlen("AT+CTTS=2,\"进多边形区域\"\r\n"));
				}
				if((QuYu_Duo[Cnt].DUO_Shuxing>>3)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x03;//多边形区域
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Duo[Cnt].Duo_ID;//进出区域报警ID
							
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x00;//进区域
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
						{
							worning_sms_send("进多边形区域");	
						}
                        system_location_upadate();
					}
				}
				if((QuYu_Duo[Cnt].DUO_Shuxing>>14)&0x0001)
				{
                    initiative_close_gprs_data();//关闭通信
					if((QuYu_Duo[Cnt].DUO_Shuxing>>15)&0x0001)
					{
						//Eat_FSFile_Create_and_open(GNSS_Data_Save);
						
                        System_Flag.System_Flag_Bit.record_data=EAT_TRUE;
					}
					else
					{
                        System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_TRUE;
					}
				}
				else
				{
					if(((QuYu_Duo[Cnt].DUO_Shuxing>>15)&0x0001)==0x00)
					{
                        System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_TRUE;
					}
				}
			}
		}
		else
		{
			if(DUOState[Cnt]==EAT_TRUE)
			{
                #if regional_judment_debug_printf
                eat_trace("PlatformCertification--> 出多边形形区域  !.");
                #endif
				DUOState[Cnt]=EAT_FALSE;
				if((QuYu_Duo[Cnt].DUO_Shuxing>>4)&0x0001)
				{
					eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//中断其他语音
					eat_modem_write("AT+CTTS=2,\"离开多边形区域\"\r\n",strlen("AT+CTTS=2,\"离开多边形区域\"\r\n"));
				}
				if((QuYu_Duo[Cnt].DUO_Shuxing>>5)&0x0001)
				{
					if(Shield_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_FALSE)
					{
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_TRUE;
						Real_Time_Location_Inf.Manual_Work_ID +=1;
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Type=0x03;//多边形区域
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_ID=QuYu_Duo[Cnt].Duo_ID;//进出区域报警ID
							
						Real_Time_Location_Inf.Area_InorOut_Inf_Add.InorOut_Area_Action=0x01;//进区域
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Region_In_Out == EAT_TRUE)
						{
							worning_sms_send("出多边形区域");	
						}
                        system_location_upadate();
					}
				}
				if((QuYu_Duo[Cnt].DUO_Shuxing>>14)&0x0001)
				{						
                    close_gprs_bearer();
                    task_msg_send(EAT_USER_1,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
				}
				if(((QuYu_Duo[Cnt].DUO_Shuxing>>15)&0x0001)==0x00)
				{
                    System_Flag.System_Flag_Bit.detail_gnss_switch_off=EAT_FALSE;
				}
			}
		}
	}	
	if(Duo_Sp_Set == EAT_TRUE)
	{
		for(Cnt=0;Cnt < Duo_QuYu_Cnt;Cnt++)
		{
			if(DUOState[Cnt]==EAT_TRUE)
			{
				if((QuYu_Duo[Cnt].DUO_Shuxing>>1)&0x0001)
				{
					return;
				}
			}
		}
		Duo_Sp_Set = EAT_FALSE;
	}
}
/* 圆形区域判断,区域内返回0,区域外返回1 */

u8 Yuan_Judgment(u32 Weidu,u32 JingDu,u32 YWeidu,u32 YJingDu,u32 YBanjING)
{
	double Jing1,Wei1,Jing2,Wei2,distance;
	
	Wei1 = Weidu / 1000000.0;
	Jing1 = JingDu / 1000000.0;
	Wei2 = YWeidu / 1000000.0;
	Jing2 = YJingDu / 1000000.0;
	GaussCalc(Jing1,Wei1,&Jing1,&Wei1);//高斯转换图
	GaussCalc(Jing2,Wei2,&Jing2,&Wei2);//高斯转换图	
	distance = g_Distance(Jing1,Wei1,Jing2,Wei2);
	if(distance > YBanjING)
		return 0;
	else
		return 1;
}

/* 矩形区域判断,区域内返回1,区域外返回0 */
u8 JuXing_Judgment(u32 Weidu,u32 JingDu,u32 LWeidu,u32 LJingDu,u32 RWeidu,u32 RJingDu)
{
	if(Weidu > LWeidu)
		return 0;
	else if(Weidu <  RWeidu)
		return 0;
	else if(JingDu < LJingDu)
		return 0;
	else if(JingDu > RJingDu)
		return 0;
	else
		return 1;
}


/* 线路保持判断,区域内返回1,区域外返回0 */
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Comimt_QuYu_File
 *  Description: 区域路线的文件系统
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
double g_Distance(double doX1, double doY1, double doX2, double doY2)
{
	return sqrt(pow((doX1 - doX2), 2) + pow((doY1 - doY2), 2));
}
double g_PointLine(double xx, double yy, double x1, double y1, double x2, double y2)
{
	double a,b,c,ang1,ang2,ang;
	double pai=3.1415926535898;
    //计算三角形三边
    a = g_Distance(x1,y1,xx,yy);if(a==0.0)return 0.0;
    b = g_Distance(x2,y2,xx,yy);if(b==0.0)return 0.0;
    c = g_Distance(x1,y1,x2,y2);
    //如果 同一个点返回距离
    if(c==0.0) return a;
	if(a<b)//如果(xx,yy)到(x1,y1)边短
	{ 
		if(y1==y2)
		{
			if(x1<x2)
				ang1=0;
			else
				ang1=pai;
		}
		else
		{
			ang1=acos((x2-x1)/c);
			if(y1>y2)ang1=pai*2-ang1;  						
		}
		ang2=acos((xx-x1)/a);
		if(y1>yy)ang2=pai*2-ang2;  							
		ang=ang2-ang1;
		if(ang<0)ang=-ang;
		if(ang>pai) ang=pai*2-ang;  							
		if(ang>pai/2)
			return a;    									
		else 
			return a*sin(ang); 								
	}
	else
	{													
		if(y1==y2)
		{
			if(x1<x2)
				ang1=pai;
			else
				ang1=0;
		}
		else
		{
			ang1=acos((x1-x2)/c);     						
			if(y2>y1)ang1=pai*2-ang1;
		}
		ang2=acos((xx-x2)/b);     							
		if(y2>yy)ang2=pai*2-ang2;
		ang=ang2-ang1;
		if(ang<0) ang=-ang;
		if(ang>pai) ang=pai*2-ang;  							
		if(ang>pai/2)  return b;   							
		else 
			return b*sin(ang);  							
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Comimt_QuYu_File
 *  Description: 区域路线的文件系统
 *  Parameters :  Psource_Struct结构体指针
 *  			Pdest 目标数组
 *				OFest 数组长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-11-11			 
 * =====================================================================================
 */
u8 Luxian_Judgmentu(u32 Weidu,u32 JingDu,u8 Ofest)
{
	u8 Cnt;
	Line_struct line;
	double Jing,Wei;
	double distance;
	u8 Road_InorOut=0;
	u8 Curren_Road = 0;
	static u16 Road_Time[2][20]={0};
	static u8 Road_Statu[2][20]={0};
	double routeDataX[20],routeDataY[20];
	Jing=JingDu/1000000.0;
	Wei=Weidu/1000000.0;
	GaussCalc(Jing,Wei,&Jing,&Wei);
	for(Cnt=0;Cnt<(LuXian_Date[Ofest].Guai_Cnt);Cnt++)
	{
		GaussCalc((LuXian_Date[Ofest].Guaidian[Cnt].GUAI_Point_JingDu)/1000000.0,(LuXian_Date[Ofest].Guaidian[Cnt].GUAI_Point_WeiDu)/1000000.0,routeDataX+Cnt,routeDataY+Cnt);
		if(Cnt>0)
		{
			distance=g_PointLine(Jing,Wei,routeDataX[Cnt-1],routeDataY[Cnt-1],routeDataX[Cnt],routeDataY[Cnt]);
			if(distance <=(LuXian_Date[Ofest].Guaidian[Cnt-1].Lu_Kuan))
			{
				Road_InorOut = EAT_TRUE;//在路线内
				if((LuXian_Date[Ofest].Guaidian[Cnt-1].GUAI_Shuxing >>0)&0x01)
				{
					Road_Time[Ofest][Cnt-1]+=5;//路段内计时
				}
				if(Road_Statu[Ofest][Cnt-1]==EAT_FALSE)
				{

					if((LuXian_Date[Ofest].Guaidian[Cnt-1].GUAI_Shuxing >>1)&0x01)
					{
						if(LuXian_Sp_Set == EAT_FALSE)
						{
							LuXian_Sp_Set = EAT_TRUE;
						}
						Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x04;
						Real_Time_Location_Inf.Over_Speed_Alarm_Add.Area_ID = LuXian_Date[Ofest].Guaidian[Cnt-1].LuDuan_ID;
						QuYu_Seting_Speed((u32)(LuXian_Date[Ofest].Guaidian[Cnt-1].Max_Speed),(u32)(LuXian_Date[Ofest].Guaidian[Cnt-1].ChaoSu_Time));
					}
				}
				Road_Statu[Ofest][Cnt-1] = EAT_TRUE;
			}
			else
			{
				if(Road_Statu[Ofest][Cnt-1]==EAT_TRUE)
				{
					if((LuXian_Date[Ofest].Guaidian[Cnt-1].GUAI_Shuxing >>0)&0x01)
					{
						if(Road_Time[Ofest][Cnt-1]<LuXian_Date[Ofest].Guaidian[Cnt-1].Time_Buzu)//不足 
						{
							if(Shield_Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort == EAT_FALSE)
							{
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_ID =LuXian_Date[Ofest].Guaidian[Cnt-1].LuDuan_ID;
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Time = Road_Time[Ofest][Cnt-1];
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Reslut = 0x00;
								
								if(System_Flag.System_Flag_Bit.Rout_Tim_Short == EAT_FALSE)
								{
									System_Flag.System_Flag_Bit.Rout_Tim_Short = EAT_TRUE;
									Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort = EAT_TRUE;
									if(Shield_SMS_Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort == EAT_TRUE)
									{
										worning_sms_send("路段行驶时间不足");
									}
                                    system_location_upadate();
								}
							}
						}
						else if(Road_Time[Ofest][Cnt-1]>LuXian_Date[Ofest].Guaidian[Cnt-1].Max_Time)//过长
						{
							if(Shield_Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort == EAT_FALSE)
							{
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_ID =LuXian_Date[Ofest].Guaidian[Cnt-1].LuDuan_ID;
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Time = Road_Time[Ofest][Cnt-1];
								Real_Time_Location_Inf.Route_Driv_OutorShort_Add.Route_Driv_OutorShort_Reslut = 0x01;
								if(System_Flag.System_Flag_Bit.Rout_Tim_Over ==EAT_FALSE)
								{	
									System_Flag.System_Flag_Bit.Rout_Tim_Over = EAT_TRUE;
									Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort = EAT_TRUE;
									if(Shield_SMS_Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort == EAT_TRUE)
									{
										worning_sms_send("路段行驶时间过长");	
									}
                                    system_location_upadate();
								}
							}
						}
						Road_Time[Ofest][Cnt-1]=0;
					}
				}
				Road_Statu[Ofest][Cnt-1] = EAT_FALSE;
			}			
		}	
	}
	return Road_InorOut;
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: radian
 *  Name: 
 *  Name: 
 *  Name: 
 *  author     :  c.c.c create at SSI_1D02 at 2014-11-14	edite by J.X.B		 
 * =====================================================================================
 */

 /* 求弧度 */
double radian(double d)
{
	return (d * PI / 180.0);   /* 角度1 = π / 180 */
}

/* 计算距离 */
double GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1=0.0,radLat2=0.0,a=0.0,b=0.0,dst=0.0;
	
	radLat1 = radian(lat1);
	radLat2 = radian(lat2);
	a = radLat1 - radLat2;
	b = radian(lng1) - radian(lng2);
	
	dst = 2*asin(sqrt( pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
	dst = dst * EARTH_RADIUS;
	dst= ((dst * 10000) / 10000)+1;
	
	return dst;
}

/*高斯投影转换*/
void GaussCalc(double longitude, double latitude, double *X, double *Y)
{
	int ProjNo=0; int ZoneWide; ////带宽
	double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
	double a,f, e2,ee, NN, T,C,A, M, iPI;
	iPI = 0.0174532925199433; ////3.1415926535898/180.0;
	ZoneWide = 6; ////6度带宽
	a=6378245.0; 
	f=1.0/298.3; //54年北京坐标系参数
	////a=6378140.0; f=1/298.257; //80年西安坐标系参数
	ProjNo = (int)(longitude / ZoneWide) ;
	
	longitude0 = ProjNo * ZoneWide + ZoneWide / 2;
	
	longitude0 = longitude0 * iPI ;
	
	latitude0=0;
	
	longitude1 = longitude * iPI ; //经度转换为弧度
	latitude1 = latitude * iPI ; //纬度转换为弧度
	e2=2*f-f*f;
	ee=e2*(1.0-e2);
	NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
	T=tan(latitude1)*tan(latitude1);
	C=ee*cos(latitude1)*cos(latitude1);
	A=(longitude1-longitude0)*cos(latitude1);
	M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1-(3*e2/8+3*e2*e2/32+45*e2*e2*e2/1024)*sin(2*latitude1)\
	+(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)-(35*e2*e2*e2/3072)*sin(6*latitude1));
	xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
	yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24 \
	+(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
	X0 = 1000000L*(ProjNo+1)+500000L;
	Y0 = 0;
	xval = xval+X0; yval = yval+Y0;
	*X = xval;
	*Y = yval;
}

/* 多边形区域判断,区域内返回1,区域外返回0 */
u8 pointInPolygon(double x,double y,u8 Offest)
{
u16 i,j;
double PX=0;
u16 nCross = 0;
double polyX1=0;
double polyX2=0;
double polyY1=0;
double polyY2=0;
for (i=0, j=QuYu_Duo[Offest].Point_Cnt-1; i<QuYu_Duo[Offest].Point_Cnt; j=i++) 
{
	// 求解 y=p.y 与 p1p2 的交点
	GaussCalc((QuYu_Duo[Offest].Point_Savep[2*i+1])/1000000.0,(QuYu_Duo[Offest].Point_Savep[2*i])/1000000.0,&polyX1,&polyY1);
	GaussCalc((QuYu_Duo[Offest].Point_Savep[2*j+1])/1000000.0,(QuYu_Duo[Offest].Point_Savep[2*j])/1000000.0,&polyX2,&polyY2);

	if (polyY1==polyY2)
	continue;
	
	if (y < min(polyY1,polyY2)) // 交点在p1p2延长线上
	continue;
	
	if ( y >= max(polyY1,polyY2)) // 交点在p1p2延长线上
	continue;

	// 求交点的 X 坐标 --------------------------------------------------------------
	PX =(double)((y - polyY1) * (polyX2 - polyX1) / (polyY2 - polyY1)) + polyX1;
	if (PX >x)
	nCross++; // 只统计单边交点
}
// 单边交点为偶数，点在多边形之外 ---
//eat_trace("交点个数%u】",(nCross % 2));
return (nCross % 2);

}

/*****************************************************************************
 * Function : QuYu_FS_Init
 * Description:This function is used to Init QUyukile
 * Parameters : void
 * Returns:void	
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
 void regional_judgment_pro(void)
{
	u32 Wei,Jing;
	if((Luxian_QuYu_Cnt != 0)||(Duo_QuYu_Cnt != 0)||(Yuan_QuYu_Cnt != 0)||(Juxing_QuYu_Cnt != 0))
	{
		if(Luxian_QuYu_Cnt!=0)//路线
		{
			LuXian_QuYU_ACTION(Real_Time_Location_Inf.Location_Latitude,Real_Time_Location_Inf.Location_Longitude);//路线服务函数	
		}
		if(Yuan_QuYu_Cnt!=0)//圆形区
		{
			Yuan_QuYU_ACTION(Real_Time_Location_Inf.Location_Latitude,Real_Time_Location_Inf.Location_Longitude);//圆形区域动作
		}
		if(Duo_QuYu_Cnt!=0)
		{
			Duo_QuYU_ACTION(Real_Time_Location_Inf.Location_Latitude,Real_Time_Location_Inf.Location_Longitude);//多边形区域动作
		}
		if(Juxing_QuYu_Cnt!=0)//矩形形区	
		{	
			Ju_QuYU_ACTION(Real_Time_Location_Inf.Location_Latitude,Real_Time_Location_Inf.Location_Longitude);
		}
	}
	if((Ju_Sp_Set == EAT_FALSE)&&(Yuan_Sp_Set == EAT_FALSE)
			&&(Duo_Sp_Set == EAT_FALSE)&&(LuXian_Sp_Set == EAT_FALSE))
	{
		if(QuYu_Sp_Set == EAT_TRUE)	
		{
			Real_Time_Location_Inf.Over_Speed_Alarm_Add.Over_Speed_Location_Type= 0x00;
			Real_Time_Location_Inf.Over_Speed_Alarm_Add.Area_ID = 0;
			seting_data_to_board(0x00);
			QuYu_Sp_Set=EAT_FALSE;
		}
	}
	else
	{
		QuYu_Sp_Set=EAT_TRUE;
	}
	#if 0
	if((JUXing_State==EAT_FALSE)&&(Yan_State==EAT_FALSE)&&(DUO_State==EAT_FALSE))
	{
		GPS_Speed_Handle(EAT_TRUE);		
	}
	#endif
	
}
void regional_file_read(void)
{
    Yuan_Comimt_QuYu_File();
    File_Analyze_QuYu_Struct_Ju();
    File_Analyze_QuYu_Struct_Duo();
    File_Luxian_Iint();
}
void regional_file_init(void)
{
    
    if(system_param.param_save.statue.wether_first_power_on==EAT_FALSE)
    {
        file_create(Qu_Yu_Save_File_yuan);
        file_create(Qu_Yu_Save_File_Luxian);
        file_create(Qu_Yu_Save_File_juxing);
        file_create(Qu_Yu_Save_File_duobian);
    }
    else
    {
        regional_file_read();
    }
 }
