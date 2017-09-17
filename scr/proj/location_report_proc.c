
#include "location_report_proc.h"

uint16_t tracking_time_interval = 0;//����ʱ���� 0 ֹͣ����
uint32_t tracking_validity = 0; //������Ч��
uint32_t curren_location_report_count = 1;//��Ϣ�ϱ�ʱ��
uint32_t acc_off_mileage_base = 0;//��׼���
uint32_t befor_mileage_base=0;//��һ���ϴ����
 
  /* 
 * ===  FUNCTION  ======================================================================
 *  Name: void Tcp_Location_Infor_Ask(u16 SN)
 *  Description:  λ����Ϣ��ѯ��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
 void Tcp_Location_Infor_Ask(uint8_t socket,uint16_t servn)
 {
 	static uint8_t Loca_Temp[128]={0};
 	BaseLocationSave_Struct Location_Inf_Data;//�����ͻ�������
	Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
 	Big_End_WordtoByte(servn,Loca_Temp); /*SN��ˮ��ת��*/
	memcpy(Loca_Temp + 2,Location_Inf_Data.Location_Inf,Location_Inf_Data.Location_Inf_Lens);
 	Tcp_Message_PacKage_And_Send(socket,&servn,Tcp_Location_Ask_Req_ID,Loca_Temp,Location_Inf_Data.Location_Inf_Lens + 2,EAT_FALSE,0,0);
 	
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Get_Store_Timer
 *  Description:  ��ȡĿǰRTCʱ��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
void get_sim800_store_timer(void)
{
	unsigned char result;
	unsigned char Time[6]={0};
	EatRtc_st rtc = {0};
	result = eat_get_rtc(&rtc);
	if(result)
	{
		Time[0] = rtc.year;
		Time[1] = rtc.mon;
		Time[2] = rtc.day;
		Time[3] = rtc.hour;
		Time[4] = rtc.min;
		Time[5] = rtc.sec;
		Dec_To_BCD(Time,6,Real_Time_Location_Inf.Curren_Time);
	}
	else
	{
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Get_Store_Timer
 *  Description:  ��ȡĿǰRTCʱ��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
 
eat_bool Eat_rtc_Set(u8 year,u8 mon,u8 day,u8 hour ,u8 min,u8 sec)
{
	EatRtc_st rtc = {0};
	eat_bool result = 0;
    rtc.year = year;
    rtc.mon = mon;
    rtc.day = day;
    rtc.hour =hour;
    rtc.min = min;
    rtc.sec = sec;
    result = eat_set_rtc(&rtc);
    if( result )
    {
        #if APP_TRACE_DBG 
        eat_trace("Eat_rtc_Set --> State:ok.");
        #endif
    }
	else
    {
        #if APP_TRACE_DBG 
        eat_trace("Eat_rtc_Set --> State:error.");
        #endif
    }
	return result;
}
eat_bool gps_redress_timer(void)
{
	static u8 RTC_Timers[6] ={0};
	EatRtc_st rtc = {0};
	eat_bool ret;
	BCD_To_Dec(Real_Time_Location_Inf.Curren_Time,RTC_Timers,6);//ת��10����
    ret = Eat_rtc_Set(RTC_Timers[0],RTC_Timers[1],RTC_Timers[2],RTC_Timers[3],RTC_Timers[4],RTC_Timers[5]);
    #if APP_TRACE_DBG 
    eat_trace("Eat_rtc_Set --> Gprs_Redress_Timer:%u.",ret);
    #endif
    return ret;
}
eat_bool rtc_system_redress_time(uint8_t *Bct_Time)
{
	uint8_t RTC_Timers[6] ={0};
	EatRtc_st rtc = {0};
	eat_bool ret;
	BCD_To_Dec(Bct_Time,RTC_Timers,6);//ת��10����
    ret = Eat_rtc_Set(RTC_Timers[0],RTC_Timers[1],RTC_Timers[2],RTC_Timers[3],RTC_Timers[4],RTC_Timers[5]);
    #if APP_TRACE_DBG 
    eat_trace("rtc_system_redress_time --> ret:%u.",ret);
    #endif
    return ret;
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: location_tracking_proc
 *  Description:  ��ʱλ�ø��ٿ��� ID: ��ϢID��0x8202��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
void location_tracking_proc(uint16_t snver,uint8_t *temp_buf)
{
 	Big_End_BytetoWord(temp_buf,&tracking_time_interval);//λ����Ϣ�ϱ�Ƶ��
 	if(tracking_time_interval != 0)
 	{
	 	Big_End_BytetoDword(temp_buf+2,&tracking_validity);/*�ϱ�ʱ��*/
	 	System_Flag.System_Flag_Bit.Location_Tracking = EAT_TRUE;
 	}
 	else
 	{
 		System_Flag.System_Flag_Bit.Location_Tracking = EAT_FALSE;
 	}
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Stop_Time_Out
 *  Description:  ��ʱ��ͣ������			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
 static void vehice_stop_time_out(void)
 {
 	uint16_t speed_curen=0;
	static uint32_t veh_stop_time=0;
	static uint16_t speed_last=0;
	static uint8_t vicher_stop =EAT_FALSE;
	if(Shield_Alarm_Inf.Alarm_Bit.Park_Over_Time == EAT_FALSE)
	{
		speed_curen = Real_Time_Location_Inf.Travel_Speed;
		if(system_param.max_parking_time != 0)
		{		
			if((speed_last!=0)&&(speed_curen==0))
			{
				vicher_stop = EAT_TRUE;
			}
			else if((speed_last!=0)&&(speed_curen!=0))
			{
				if(vicher_stop == EAT_TRUE)
				{
					vicher_stop = EAT_FALSE; 
				}
			}
			if(vicher_stop == EAT_TRUE)
			{
				if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Park_Over_Time == EAT_FALSE)
				{
					veh_stop_time++;
					if(veh_stop_time >= system_param.max_parking_time)
					{
						veh_stop_time=0;
						Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Park_Over_Time = EAT_TRUE;
                        system_location_upadate();
						if(Shield_SMS_Alarm_Inf.Alarm_Bit.Park_Over_Time == EAT_TRUE)
						{
							worning_sms_send("��ʱͣ��");
						}
					}
				}
			}
			else
			{
				if(veh_stop_time !=0 )
				{
					veh_stop_time = 0;
				}
				if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Park_Over_Time == EAT_TRUE)
				{
					Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Park_Over_Time = EAT_FALSE;
				}
			}
		}
		speed_last = speed_curen;//��¼�ٶ�
	}
 }

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Illegality_Vector_Ignited
 *  Description: �Ƿ�λ����Ƿ�������жϡ�
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
static void Illegality_Vector_Ignited(void)
{
    static u32 Illegality_Report_Cnt = 0;
    /***************�㱨����*******************/
    if(system_param.location_reporting_param.scheme== 0x01)
    {
        if(Shield_Alarm_Inf.Alarm_Bit.Illegality_Ignited == EAT_FALSE)
        {
            if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Ignited == EAT_FALSE)
            {
                if((System_Flag.System_Flag_Bit.ic_inout==EAT_FALSE)&&(Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC == EAT_TRUE))
                {
                    Illegality_Report_Cnt++;
                    if(Illegality_Report_Cnt == 600)//600
                    {
                            Real_Time_Location_Inf.Manual_Work_ID += 1;
                            Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Ignited = EAT_TRUE;
                            if(Shield_SMS_Alarm_Inf.Alarm_Bit.Illegality_Ignited == EAT_TRUE)
                            {
                                worning_sms_send("�Ƿ����"); 
                            }
                            system_location_upadate();
                     }
                 }
                 else
                 {
                        Illegality_Report_Cnt = 0;
                 }
            }
        }
        if(Shield_Alarm_Inf.Alarm_Bit.Illegality_Vector == EAT_FALSE)
        {
            if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Vector == EAT_FALSE)
            {
                if((System_Flag.System_Flag_Bit.ic_inout==EAT_FALSE)&&(Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC == EAT_FALSE))
                {
                    if((system_param.mileage - acc_off_mileage_base)*100>=system_param.electronic_fence_radius)
                    {   
                        Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Vector = EAT_TRUE;
                        Real_Time_Location_Inf.Manual_Work_ID +=1;
                            
                        system_param.mileage = (acc_off_mileage_base*100+system_param.electronic_fence_radius)/100;
                        if(Shield_SMS_Alarm_Inf.Alarm_Bit.Illegality_Vector == EAT_TRUE)
                        {
                           worning_sms_send("�Ƿ�λ��"); 
                        }
                        
                        system_location_upadate();
                        acc_off_mileage_base = system_param.mileage;                  
                     }   
                 }
              }
            }
    }
    else
    {
        Illegality_Report_Cnt = 0;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: distance_location_report
 *  Description: λ����Ϣ���������ϱ� ��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
static void distance_location_report(void)
{
    if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos == EAT_TRUE)
    {
    	if((system_param.mileage-befor_mileage_base)*100>=system_param.location_reporting_param.emergency_alarm_distance)
    	{
    		system_param.mileage=(befor_mileage_base*100+system_param.location_reporting_param.emergency_alarm_distance)/100;
            system_location_upadate();
    		befor_mileage_base=system_param.mileage;
    	}
    }
    else
    {
        if(system_param.location_reporting_param.scheme==0x00)
        {
            if((system_param.mileage-befor_mileage_base)*100>=system_param.location_reporting_param.default_distance)
            {
                system_param.mileage=(befor_mileage_base*100+system_param.location_reporting_param.default_distance)/100;
                system_location_upadate();
                befor_mileage_base=system_param.mileage;
            }
        }
        else
        {
			if(System_Flag.System_Flag_Bit.ic_inout==EAT_FALSE)
			{
				if((system_param.mileage-befor_mileage_base)*100>=system_param.location_reporting_param.not_signed_distance)
				{
					system_param.mileage=(befor_mileage_base*100+system_param.location_reporting_param.not_signed_distance)/100;
                    system_location_upadate();
					befor_mileage_base=system_param.mileage;
				}
			}
			else
			{
				if((system_param.mileage-befor_mileage_base)*100>=system_param.location_reporting_param.default_distance)
				{
					system_param.mileage=(befor_mileage_base*100+system_param.location_reporting_param.default_distance)/100;
                    system_location_upadate();
					befor_mileage_base=system_param.mileage;
				}
			}
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: location_report_policy_proc
 *  Description: λ����Ϣ�ϱ� ��
 *  Parameters :  ��			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
void location_report_policy_proc(void)
{
    vehice_stop_time_out();
    Illegality_Vector_Ignited();//�Ƿ�λ����Ƿ������
    curren_location_report_count --;//��Ϣ�ϱ�ʱ��
    if(System_Flag.System_Flag_Bit.Location_Tracking == EAT_TRUE)//λ�ø���
    {
         tracking_validity--;
         if(curren_location_report_count == 0)
         {
             system_location_upadate();
             curren_location_report_count = tracking_time_interval;
         }
         if(tracking_validity == 0)
         {
             System_Flag.System_Flag_Bit.Location_Tracking = EAT_FALSE;
         }
    }
    else/**********************��ͨλ����Ϣ�ϴ�********************/
    {   
         /**********************�����ϴ�********************/
         if(system_param.location_reporting_param.policy == 0x01)
         {
             curren_location_report_count = 1;
             
             distance_location_report();
         }/*****************��ʱ�ϴ�,����****************/
         else if((system_param.location_reporting_param.policy == 0x00)||(system_param.location_reporting_param.policy == 0x02))
         {
             /*��ʱ����*/
             if(curren_location_report_count == 0)
             {
                 system_location_upadate();
                if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos == EAT_TRUE)//���������㱨
                {
                    curren_location_report_count = system_param.location_reporting_param.emergency_alarm_frze;    
                }
                else/************�ǽ��������㱨*******************/
                {
                    if(system_param.location_reporting_param.scheme== 0x00)//ֻ�ж�ACC
                    {
                        curren_location_report_count = system_param.location_reporting_param.default_frze;
                    }
                    else
                    {
                        if(System_Flag.System_Flag_Bit.ic_inout == EAT_FALSE)//δ�忨
                        {
                            curren_location_report_count = system_param.location_reporting_param.not_signed_frze; //δ��¼״̬   
                        }
                        else
                        {
                            curren_location_report_count = system_param.location_reporting_param.default_frze;//��½ʱ����
                        }
                    }
                }   
             }
             
             /*���ദ��*/
             if(system_param.location_reporting_param.policy == 0x02)
             {
                distance_location_report();
             }
         }
    }
}

