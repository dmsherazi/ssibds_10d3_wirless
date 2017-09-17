
#include "call_ring_pro.h"
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: tcp_vdr_dail_call
 *  Description: TCP拨打语音电话
 *  Parameters :(u8 *pscr)源数据指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */
void tcp_vdr_dail_call(uint8_t l_socket,uint16_t msg_svn,uint8_t *tcp_scr)
{
    if((System_Flag.System_Flag_Bit.tcp_dail_call==EAT_FALSE)&&\
        (System_Flag.System_Flag_Bit.tcp_listen_call==EAT_FALSE)&&\
            (Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone==EAT_FALSE))
    {
    	if(*tcp_scr==0x01)
    	{
            #if call_ring_pro_debug_printf
            eat_trace("tcp_vdr_dail_call --> 监听!");
            #endif
    		eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);
    		eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//设置默认LOW		
            System_Flag.System_Flag_Bit.tcp_listen_call = EAT_TRUE;
    	}
    	else
    	{
            #if call_ring_pro_debug_printf
            eat_trace("tcp_vdr_dail_call --> 回拨 !");
            #endif
            System_Flag.System_Flag_Bit.tcp_dail_call = EAT_TRUE;
    	}
        Tcp_VDR_Common_Replay(l_socket,msg_svn,tcp_dail_vdr_call_id,0x00);
    	dail_voice_call(tcp_scr+1);
	}
	else
	{
        #if call_ring_pro_debug_printf
        eat_trace("tcp_vdr_dail_call --> 回拨或者监听忙 !");
        #endif
        Tcp_VDR_Common_Replay(l_socket,msg_svn,tcp_dail_vdr_call_id,0x01);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  void dail_voice_call(uint8_t *point_num)
 *  Description: 拨打语音电话
 *  Parameters :(u8 *pscr)源数据指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */

void dail_voice_call(uint8_t *point_num)
{
	uint8_t phone_num[32]={0};
	if(strlen(point_num)!=0)
	{
        sprintf(phone_num,"ATD%s;\r\n",point_num);
        eat_modem_write(phone_num,strlen(phone_num));
        #if call_ring_pro_debug_printf
        eat_trace("dail_voice_call --> phone_num:%s !",phone_num);
        #endif
    }
    else
    {
        #if call_ring_pro_debug_printf
        eat_trace("dail_voice_call --> 电话号码为空 !");
        #endif
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  Calling_Handle(u8 *pscr)
 *  Description:  解析AT来电数据
 *  Parameters :(u8 *pscr)源数据指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2015-6-16 			 
 * =====================================================================================
 */
void call_ring_data_num_proc(uint8_t *pscr)
{
    char* buf_ptr = NULL;
    uint8_t *peck_point=NULL;
    uint8_t Phone_Num[16]={0};
    peck_point=Phone_Num;
    buf_ptr = strstrLast((const char *)pscr,"+CLIP: \"");
    if( buf_ptr != NULL)
    {
    	while(*buf_ptr!='\"')
    	{
    		*peck_point++=*buf_ptr++;
    	}
    	*peck_point='\0';
    	if(strlen(Phone_Num)!=0)
    	{
	        if((strcmp(Phone_Num,system_param.phone_number_param.terminal_format)==0x00)&&(strlen(system_param.phone_number_param.terminal_format)!=0))//恢复出厂设置
	        {
	             #if call_ring_pro_debug_printf
	        	 eat_trace("call_ring_data_num_proc --> 恢复出厂设置电话%s",Phone_Num);
	        	 #endif
                 system_param.param_save.statue.wether_first_power_on=EAT_FALSE;
                 system_para_save_at_once();
                 
	        	 eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
                 board_mode_seting(0x06);
	        }
	        else if((strcmp(Phone_Num,system_param.phone_number_param.terminal_restart)==0x00)&&(strlen(system_param.phone_number_param.terminal_restart)!=0))
	        {
                 #if call_ring_pro_debug_printf
                 eat_trace("call_ring_data_num_proc --> 复位电话:%s",Phone_Num);
	        	 #endif
	        	 eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
                 board_mode_seting(0x05);
	        }
	        else if((strcmp(Phone_Num,system_param.phone_number_param.monitor_telephone)==0x00)&&(strlen(system_param.phone_number_param.monitor_telephone)!=0))//监听电话
	        {
				eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//设置为GPIO模式
				eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//设置默认LOW
				
				eat_modem_write("ATA\r\n",strlen("ATA\r\n"));
                #if call_ring_pro_debug_printf
                eat_trace("call_ring_data_num_proc --> 监听电话:%s",Phone_Num);
                #endif
	        }
	        else
	        {
                #if call_ring_pro_debug_printf
                eat_trace("call_ring_data_num_proc --> 其他非指定电话:%s",Phone_Num);
                #endif
	        	eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
	        }
        }
        else
        {
        
            #if call_ring_pro_debug_printf
            eat_trace("call_ring_data_num_proc --> 设备不具备来电显示:%s",Phone_Num);
            #endif
	       	eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : phone_over_proc
 *  Description: 电话相关参数获取
 *  Parameters : void
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */
 void phone_over_proc(u8 *at_buffer)
 {
    static uint8_t dail_sos_cnt=0;
    if((Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone==EAT_TRUE)||(System_Flag.System_Flag_Bit.tcp_listen_call==EAT_TRUE)||(System_Flag.System_Flag_Bit.tcp_dail_call==EAT_TRUE))
    {
    	if((strstrLast((const char *)at_buffer,"BUSY")!=NULL)||(strstrLast((const char *)at_buffer,"NO ANSWER")!=NULL))
    	{
            if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone==EAT_TRUE)
            {
                dail_sos_cnt++;
                if(dail_sos_cnt==3)
                {
                    dail_sos_cnt=0;
                    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone=EAT_FALSE;
                }
                else
                {
                    dail_voice_call(system_param.phone_number_param.sos_telephone);
                }
            }
    	    else if(System_Flag.System_Flag_Bit.tcp_listen_call== EAT_TRUE)
    	    {
                eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//设置为GPIO模式
                eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//设置默认LOW   
                
                #if call_ring_pro_debug_printf
                eat_trace("phone_over_proc --> 电话接监听成 !");
                #endif
                System_Flag.System_Flag_Bit.tcp_listen_call= EAT_FALSE;
    	    }
    	    else if(System_Flag.System_Flag_Bit.tcp_dail_call== EAT_TRUE)
    	    {
                System_Flag.System_Flag_Bit.tcp_dail_call= EAT_FALSE;
    	    }
     	}
     	else if((strstrLast((const char *)at_buffer,"NO CARRIER")!=NULL))
     	{
            if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone==EAT_TRUE)
            {
               Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.SOS_Phone=EAT_FALSE;
            }
    	    else if(System_Flag.System_Flag_Bit.tcp_listen_call== EAT_TRUE)
    	    {
                #if call_ring_pro_debug_printf
                eat_trace("phone_over_proc --> 电话CARRIER监听成 !");
                #endif
                eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//设置为GPIO模式
                eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//设置默认LOW   
                System_Flag.System_Flag_Bit.tcp_listen_call= EAT_FALSE;
    	    }
    	    else if(System_Flag.System_Flag_Bit.tcp_dail_call== EAT_TRUE)
    	    {
                System_Flag.System_Flag_Bit.tcp_dail_call= EAT_FALSE;
    	    }
     	}
 	}
 	else
 	{
 	    if(dail_sos_cnt!=0)dail_sos_cnt=0;
 	}
 }

