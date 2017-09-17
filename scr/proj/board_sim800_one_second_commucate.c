
#include "board_sim800_one_second_commucate.h"

state_record_struct state_record;

 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: void Commucation_Board_Location(unsigned char *Psource)
  *  Description:  跟新位置，时间，警报信息
  *  Parameters :  无                
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-31             
  * =====================================================================================
  */
static void sys_gps_rmc_parse(char *line,Location_orgin_data *GPS)
 {
     int  degree_int=0;
     double cent=0.0;
     float gps_tmp;
     unsigned short  gps_short;
     unsigned char status,we_statue;
     char *buf = line;
     status = buf[GetComma(2, buf,',')];
     if (status == 'A')  //
     {
       GPS->status=0x01;//定位
       tool_str2double(&buf[GetComma(3, buf,',')],&(GPS->latitude));  
       tool_str2double(&buf[GetComma(5, buf,',')],&(GPS->longitude));
 
       GPS->latitude_Degree  = (int)GPS->latitude / 100;       //维度
             degree_int  = GPS->latitude_Degree;//
             cent = GPS->latitude - degree_int*100;//
             cent/=60.0;//
             GPS->latitude=cent+degree_int;//
             degree_int=(int)(GPS->latitude*1000000.0); 
             GPS->jt_latitude = (uint32_t)degree_int;
             //--------------------------------------------------    
                                               
             //---------------------------
       GPS->longitude_Degree  = (int)GPS->longitude / 100;       //经度
             degree_int=GPS->longitude_Degree;
             cent = GPS->longitude - degree_int*100;
             cent /= 60.0;
             GPS->longitude=cent+degree_int;
             degree_int=(int)(GPS->longitude*1000000.0); 
             
             GPS->jt_longitude= (uint32_t)degree_int;
             //--------------------------------------------------    
        
       we_statue = buf[GetComma(4, buf,',')];
            if(we_statue=='S')
            {   
                GPS->latitude_drect =0x01;
            }
            else
            {
                GPS->latitude_drect =0x00;
            }
            
       we_statue = buf[GetComma(4, buf,',')];
            if(we_statue=='W')
            {   
                GPS->longitude_drect=0x01;
            }
            else
            {
                 GPS->longitude_drect =0x00;
            }
             //-----------------------------------
       tool_str2float(&buf[GetComma(7, buf,',')],&gps_tmp);   
             gps_short  = (unsigned short)(gps_tmp*1.85*10.0);
             GPS->gps_speed= (uint16_t)gps_short;
             
             //------------------------------------     
      tool_str2float(&buf[GetComma(8, buf,',')],&gps_tmp); 
             GPS->gps_drection= (uint16_t)gps_tmp;
     } 
     else
     {
         GPS->status=0x00;//定位
     }
 }
 
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: static uint8_t find_one_second_data_offest(uint8_t *pscr)
 *  Description:  内部函数 查找1s信息偏移量
 *  Parameters :  无			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
static uint8_t find_one_second_data_offest(uint8_t *pscr,uint16_t offest)
{
    uint16_t ii=0;
    for(ii=0;ii<512;ii++)
    {
      if((*(pscr+offest+ii)=='$')&&(*(pscr+offest+ii+1)=='G')&&\
        (*(pscr+offest+ii+3)=='R')&&(*(pscr+offest+ii+4)=='M')&&\
                                    (*(pscr+offest+ii+5)=='C')) 
      {
            return ii;
      }
    }
}
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sim800_board_commucation(unsigned char *Psource)
 *  Description:  更新位置，时间，警报信息
 *  Parameters :  无			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
void sim800_board_commucation_proc(uint8_t *Psouce)
{
	Alarm_Inf_Struct Temp_Alarm_Inf;
	State_Inf_Struct Temp_State_Inf;
	Location_orgin_data Positon;//位置信息
	uint16_t data_offest=0;
	uint8_t nvagation_offest = 0;
	uint32_t temp_oil=0;
	static uint32_t plus=0;
	uint8_t temp_bcd_gps_time[6]={0};
	uint8_t temp_bcd_rtc_time[6]={0};
	static uint8_t firs_board_farme=EAT_FALSE;
	Big_End_BytetoDword(Psouce,&(Temp_Alarm_Inf.Alarm_Original));
	data_offest +=4;
	Big_End_BytetoDword(Psouce+data_offest,&(Temp_State_Inf.State_Inf));
	data_offest +=4;
	
	data_offest +=8;//经度维度
	
    Big_End_BytetoWord(Psouce + data_offest,&(Real_Time_Location_Inf.Location_Altitude));//高程
	data_offest +=2;//高程
	
	data_offest +=4;//速度方向
	
	memcpy(temp_bcd_gps_time,Psouce + data_offest,6);//gps时间
	data_offest +=6;
	
	memcpy(temp_bcd_rtc_time,Psouce + data_offest,6);//RTC时间
	data_offest +=6;

	Big_End_BytetoDword(Psouce + data_offest,&(Real_Time_Location_Inf.Vehicle_Odometer));//里程
    data_offest+=4;
    system_param.mileage = Real_Time_Location_Inf.Vehicle_Odometer;//里程
    
	Big_End_BytetoWord(Psouce + data_offest,&(Real_Time_Location_Inf.Power_volate));//电压
    data_offest+=2;
    
	Big_End_BytetoDword(Psouce + data_offest,&temp_oil);//油耗
	Real_Time_Location_Inf.oil_mass_second += temp_oil;
	Real_Time_Location_Inf.oil_first += temp_oil;
	plus += temp_oil;
	
    #if debuge_printf_switch
    eat_trace("second Analysis_Location_Inf --> temp oimplu:%u,plus1:%u,second:%u !",plus,Real_Time_Location_Inf.oil_first,Real_Time_Location_Inf.oil_mass_second);
    #endif
    data_offest+=4;
    
	Big_End_BytetoWord(Psouce + data_offest,&(Real_Time_Location_Inf.Travel_Speed));//脉冲速度
    data_offest+=2;
    
	Real_Time_Location_Inf.Travel_Speed = Real_Time_Location_Inf.Travel_Speed*10;
	Big_End_BytetoDword(Psouce + data_offest,&(Real_Time_Location_Inf.Extend_Signle_Add.Extend_Signle));
    data_offest+=4;
    data_offest+=2;//扩展状态
    
	Big_End_BytetoDword(Psouce + data_offest,&(Real_Time_Location_Inf.Analog_Signal));
    data_offest+=4;
    
    Real_Time_Location_Inf.Satelite_Amount = *(Psouce + data_offest);
    data_offest+=1;
    
	/**********位置信息处理************/
    /*nvagation_offest=find_one_second_data_offest(Psouce,data_offest);
    #if debuge_printf_switch 
    eat_trace("sys_gps_rmc_parse --> GNRMC DATA:data_len:%d,%s",data_offest,Psouce + data_offest+nvagation_offest);
    #endif*/
    sys_gps_rmc_parse(Psouce + data_offest,&Positon);
    if(Positon.status==EAT_TRUE)
    {
        Real_Time_Location_Inf.Location_Latitude = Positon.jt_latitude;
        Real_Time_Location_Inf.Location_Longitude = Positon.jt_longitude;
        Real_Time_Location_Inf.Location_Speed = Positon.gps_speed;
        Real_Time_Location_Inf.Location_Direction = Positon.gps_drection;
        memcpy(Real_Time_Location_Inf.Curren_Time,temp_bcd_gps_time,6);//RTC时间
        #if debuge_printf_switch 
        eat_trace("sys_gps_rmc_parse --> inflecting:%u,%u",Real_Time_Location_Inf.Location_Latitude,Real_Time_Location_Inf.Location_Longitude);
        #endif
        /**********拐点处理*********/
	    inflect_pass_up_proc();
        /**********区域处理*********/
	    regional_judgment_pro();
    }
    else
    {
        memcpy(Real_Time_Location_Inf.Curren_Time,temp_bcd_rtc_time,6);//RTC时间
        Real_Time_Location_Inf.Satelite_Amount = 0;
        Real_Time_Location_Inf.Location_Speed = 0;
        #if debuge_printf_switch 
        eat_trace("sys_gps_rmc_parse --> rtc_time:20%0x-%0x-%0x %0x:%0x:%0x",temp_bcd_rtc_time[0],temp_bcd_rtc_time[1],temp_bcd_rtc_time[2],temp_bcd_rtc_time[3],temp_bcd_rtc_time[4],temp_bcd_rtc_time[5]);
        #endif
    }
	/**********第一次通信成功处理************/
	if(firs_board_farme==EAT_FALSE)
	{
        rtc_system_redress_time(temp_bcd_rtc_time);
        vehicle_infor_seting(0x01);
        vehicle_infor_seting(0x02);
        vehicle_infor_seting(0x03);
        vehicle_infor_seting(0x04);
        vehicle_infor_seting(0x05);
        vehicle_infor_seting(0x06);
        befor_mileage_base=system_param.mileage;
	    firs_board_farme=EAT_TRUE;
	}
	/**********报警处理************/
	State_Inf_Process(&Temp_State_Inf);
	Alarm_Inf_Process(&Temp_Alarm_Inf);

}
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: Send_Reqest_Farm
  *  Description:  发送应答帧 
  *  Parameters :  ID           :应答对应的ID
  *                data         :应答内容
  *  
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
  * =====================================================================================
  */ 
  void uart1_sim800_send_1s_proc(void)
  {
      uint8_t data_send[6]={0};
      uint8_t data_offest=0;
      *(data_send+data_offest) = System_Flag.System_Flag_Bit.Mode_Active;
      data_offest += 1;
      *(data_send+data_offest) = (System_Flag.System_Flag_Bit.MainAutenSucess|System_Flag.System_Flag_Bit.SecondAutenSucess);
      data_offest += 1;
      if(Wether_SIM_DEF==EAT_FALSE)
      {
          Real_Time_Location_Inf.Sing_strength=0;
      }
      else
      {
          if(Real_Time_Location_Inf.Sing_strength<5)
          {
            Real_Time_Location_Inf.Sing_strength=5;
          }
      }
      *(data_send+data_offest) = Real_Time_Location_Inf.Sing_strength;
      data_offest += 1;
      *(data_send+data_offest) = Wether_SIM_DEF;
      data_offest += 1;
      *(data_send+data_offest) = Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep;
      data_offest += 1;
	  
      state_record.state_bit.sate_ip1_lock = system_param.param_save.statue.ip1_lock;
	  state_record.state_bit.sate_ip2_lock = system_param.param_save.statue.ip2_lock;
	  
      *(data_send+data_offest) = state_record.state_data;
      data_offest += 1;
	  
      #if debuge_printf_switch 
      eat_trace("uart1_sim800_send_1s_proc--> send");
      HexToAsicc(data_send,data_offest);
      #endif
      uart1_send_data_frame(SIM_Card_Signal_Strength_ID,data_send,data_offest);
  }


