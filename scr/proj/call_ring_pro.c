
#include "call_ring_pro.h"
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: tcp_vdr_dail_call
 *  Description: TCP���������绰
 *  Parameters :(u8 *pscr)Դ����ָ��
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
            eat_trace("tcp_vdr_dail_call --> ����!");
            #endif
    		eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);
    		eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����Ĭ��LOW		
            System_Flag.System_Flag_Bit.tcp_listen_call = EAT_TRUE;
    	}
    	else
    	{
            #if call_ring_pro_debug_printf
            eat_trace("tcp_vdr_dail_call --> �ز� !");
            #endif
            System_Flag.System_Flag_Bit.tcp_dail_call = EAT_TRUE;
    	}
        Tcp_VDR_Common_Replay(l_socket,msg_svn,tcp_dail_vdr_call_id,0x00);
    	dail_voice_call(tcp_scr+1);
	}
	else
	{
        #if call_ring_pro_debug_printf
        eat_trace("tcp_vdr_dail_call --> �ز����߼���æ !");
        #endif
        Tcp_VDR_Common_Replay(l_socket,msg_svn,tcp_dail_vdr_call_id,0x01);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  void dail_voice_call(uint8_t *point_num)
 *  Description: ���������绰
 *  Parameters :(u8 *pscr)Դ����ָ��
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
        eat_trace("dail_voice_call --> �绰����Ϊ�� !");
        #endif
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  Calling_Handle(u8 *pscr)
 *  Description:  ����AT��������
 *  Parameters :(u8 *pscr)Դ����ָ��
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
	        if((strcmp(Phone_Num,system_param.phone_number_param.terminal_format)==0x00)&&(strlen(system_param.phone_number_param.terminal_format)!=0))//�ָ���������
	        {
	             #if call_ring_pro_debug_printf
	        	 eat_trace("call_ring_data_num_proc --> �ָ��������õ绰%s",Phone_Num);
	        	 #endif
                 system_param.param_save.statue.wether_first_power_on=EAT_FALSE;
                 system_para_save_at_once();
                 
	        	 eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
                 board_mode_seting(0x06);
	        }
	        else if((strcmp(Phone_Num,system_param.phone_number_param.terminal_restart)==0x00)&&(strlen(system_param.phone_number_param.terminal_restart)!=0))
	        {
                 #if call_ring_pro_debug_printf
                 eat_trace("call_ring_data_num_proc --> ��λ�绰:%s",Phone_Num);
	        	 #endif
	        	 eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
                 board_mode_seting(0x05);
	        }
	        else if((strcmp(Phone_Num,system_param.phone_number_param.monitor_telephone)==0x00)&&(strlen(system_param.phone_number_param.monitor_telephone)!=0))//�����绰
	        {
				eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
				eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����Ĭ��LOW
				
				eat_modem_write("ATA\r\n",strlen("ATA\r\n"));
                #if call_ring_pro_debug_printf
                eat_trace("call_ring_data_num_proc --> �����绰:%s",Phone_Num);
                #endif
	        }
	        else
	        {
                #if call_ring_pro_debug_printf
                eat_trace("call_ring_data_num_proc --> ������ָ���绰:%s",Phone_Num);
                #endif
	        	eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
	        }
        }
        else
        {
        
            #if call_ring_pro_debug_printf
            eat_trace("call_ring_data_num_proc --> �豸���߱�������ʾ:%s",Phone_Num);
            #endif
	       	eat_modem_write("ATH\r\n",strlen("ATH\r\n"));
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : phone_over_proc
 *  Description: �绰��ز�����ȡ
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
                eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
                eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����Ĭ��LOW   
                
                #if call_ring_pro_debug_printf
                eat_trace("phone_over_proc --> �绰�Ӽ����� !");
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
                eat_trace("phone_over_proc --> �绰CARRIER������ !");
                #endif
                eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
                eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����Ĭ��LOW   
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

