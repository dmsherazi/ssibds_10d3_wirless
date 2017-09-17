

#include "detailed_gnss_date.h"

uint8_t detial_data_befor_clean=EAT_FALSE;
uint16_t detai_data_file_offest = 0;
uint16_t detail_gnss_cnt = 0;

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  ask_board_gnss_data(uint8_t data_cmd)
 *  Description:请求GANSS数据
 *  Input:   cmd 
 *  Output:void
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
void ask_board_gnss_data(uint8_t data_cmd)
{
	uart1_send_data_frame(ask_all_gness_data,&data_cmd,1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  GNSS_updata_handle
 *  Description:GNSS数据上传
 *  Input:   speed 
 *  Output: 1
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
 void detail_gnss_data_updata_ctrl(void)
 {
 	static uint8_t start_flag=EAT_FALSE;
 	static uint8_t over_flag=EAT_FALSE;
 	static uint32_t upate_data_plus=0;
 	if(detial_data_befor_clean==EAT_TRUE)
 	{
	 	if(start_flag==EAT_TRUE)
	 	{
		 	upate_data_plus=0;
		 	over_flag=EAT_FALSE;
		 	start_flag=EAT_FALSE;
	 	}
	 	detial_data_befor_clean=EAT_FALSE;
	 	
        #if detial_gnss_data_debug_printf
        eat_trace("detail_gnss_data_updata_ctrl --> 清理数据参数 !!");
        #endif
 	}
 	if(system_param.detail_location_upload_mode != 0)
	 {
           if(System_Flag.System_Flag_Bit.MainAutenSucess == EAT_TRUE)
           {
    		 	switch(system_param.detail_location_upload_mode)
    		 	{
    		 		case 0x01://时间间隔
        				upate_data_plus++;
        				if(upate_data_plus==system_param.detail_location_upload_setting)
        				{
        					if(System_Flag.System_Flag_Bit.detail_gnss_switch_off==EAT_FALSE)
        				 	{ 
                                ask_board_gnss_data(0x01);//开启
                            }
        					upate_data_plus=0;
        				}
        		 		break;
        		 		
    		 		case 0x02://距离间隔
        		 		if(start_flag==EAT_FALSE)
        		 		{
        		 			upate_data_plus = system_param.mileage;
        		 			start_flag=EAT_TRUE;
        		 		}
        				if((system_param.mileage-upate_data_plus)*100>=system_param.detail_location_upload_setting)
        				{
        					if(System_Flag.System_Flag_Bit.detail_gnss_switch_off==EAT_FALSE)
        				 	{ 
                                ask_board_gnss_data(0x01);//开启
        				 	}
        				 	upate_data_plus=system_param.mileage;
        				}
    		 		    break;

    		 		case 0x0B://累计时间
        		 		if(start_flag==EAT_FALSE)
        		 		{
        		 			start_flag=EAT_TRUE;
        		 			ask_board_gnss_data(0x01);
        		 		}
        		 		if(over_flag==EAT_FALSE)
        			 	{
        					upate_data_plus++;
        					if(upate_data_plus==system_param.detail_location_upload_setting)
        					{
        						ask_board_gnss_data(0x00);
        						over_flag=EAT_TRUE;
        						upate_data_plus=0;
                                #if detial_gnss_data_debug_printf
                                eat_trace("detail_gnss_data_updata_ctrl --> 0x0B:累计时间:%d !!",upate_data_plus);
                                #endif
        					}
        				}
        		 		break;
    		 		case 0x0C:
        		 		if(start_flag==EAT_FALSE)
        		 		{
                            upate_data_plus = system_param.mileage;
        		 			ask_board_gnss_data(0x01);
        		 			start_flag=EAT_TRUE;
        		 		}
        		 		if(over_flag==EAT_FALSE)
        			 	{
        					if((system_param.mileage-upate_data_plus)*100>=system_param.detail_location_upload_setting)
        					{
        						ask_board_gnss_data(0x00);
        						over_flag=EAT_TRUE;
        					}
        				}
        		 		break;
    		 		case 0x0D:
        		 		if(start_flag==EAT_FALSE)
        		 		{
        		 			ask_board_gnss_data(0x01);
        		 			start_flag=EAT_TRUE;
        		 		}
        				if(over_flag==EAT_FALSE)
        				{
        			 		upate_data_plus++;
        			 		if(upate_data_plus==system_param.detail_location_upload_setting)
        			 		{
        			 			ask_board_gnss_data(0x00);
        			 			over_flag=EAT_TRUE;
                                #if detial_gnss_data_debug_printf
                                eat_trace("detail_gnss_data_updata_ctrl --> 0x0d:累计条数:%d !!",upate_data_plus);
                                #endif
                                
        			 			upate_data_plus=0;
        			 		}
        		 		}
        		 		break;
    		 		default:
    				break;
    		 	}
		 	}
	 }
	 else
	 {
	 	if(start_flag==EAT_TRUE)
	 	{
		 	upate_data_plus=0;
		 	over_flag=EAT_FALSE;
		 	start_flag=EAT_FALSE;
	 	}
	 }
 }
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name:   void tcp_data_penetrate(uint8_t socket,uint8_t data,uint8_t *penetrate_data,uint16_t data_len)
  *  Description: 数据透传
  *  Input:   speed 
  *  Output: 1
  *  author: J create at 2015-4-24
  * =====================================================================================
  */
 void tcp_data_penetrate(uint8_t socket,uint8_t data,uint8_t *penetrate_data,uint16_t data_len)
 {
    uint16_t msgs_sern=0;
    uint8_t send_data[560]={0};
    send_data[0] = data;
    memcpy(send_data + 1,penetrate_data,data_len);
    Tcp_Message_PacKage_And_Send(socket,&msgs_sern,data_penetrate_id,send_data,data_len+1,0,0,0);
 }
 
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name:   void penetrate_record_gnss_data(uint8_t socket)  
  *  Description: GNSS数据透传
  *  Input:   speed 
  *  Output: 1
  *  author: J create at 2015-4-24
  * =====================================================================================
  */
 void penetrate_record_gnss_data(uint8_t socket)
 {
    uint16_t data_len=0;
    static uint16_t data_offest=0;
	uint8_t data_send[512]={0};
	if(detail_gnss_cnt != 0)
	{
        if(System_Flag.System_Flag_Bit.MainAutenSucess == EAT_TRUE)
        {
    	    if(System_Flag.System_Flag_Bit.fade_detail_gnss==EAT_FALSE)
    	    {
                file_read(detail_gnss_file,data_offest,&data_len,sizeof(uint16_t));//长度
                data_offest += sizeof(data_offest);
                
                file_read(detail_gnss_file,data_offest,data_send,data_len);
                data_offest += data_len;
                
                tcp_data_penetrate(socket,0x00,data_send,data_len);
                System_Flag.System_Flag_Bit.fade_detail_gnss=EAT_TRUE;
        		detail_gnss_cnt --;
    		}
		}
		else
		{
		    if(System_Flag.System_Flag_Bit.fade_detail_gnss==EAT_TRUE)
		    {
		        System_Flag.System_Flag_Bit.fade_detail_gnss=EAT_FALSE;
		    }
		}
	}
	else
	{
	    if(detai_data_file_offest!=0)  detai_data_file_offest = 0;
		if(data_offest!=0)  data_offest = 0;
        System_Flag.System_Flag_Bit.fade_detail_gnss=EAT_FALSE;
	}
 }
 
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name:   void record_fade_gnss_data(uint8_t *tempbuf,uint16_t data_len)
  *  Description: GNSS数据保存
  *  Input:   speed 
  *  Output: 1
  *  author: J create at 2015-4-24
  * =====================================================================================
  */
 static void record_fade_gnss_data(uint8_t *tempbuf,uint16_t data_len)
 {
     uint8_t send_data[560]={0};
     detail_gnss_cnt++;
     file_write(detail_gnss_file,detai_data_file_offest,&data_len,sizeof(data_len));//长度
     detai_data_file_offest += sizeof(int16_t);
     
     file_write(detail_gnss_file,detai_data_file_offest,tempbuf,data_len);
     detai_data_file_offest +=data_len;
 }
 
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name:   void gnss_updata_handle(uint8_t *penetrate_data,uint16_t tempLens) 
  *  Description: GNSS数据保存
  *  Input:   speed 
  *  Output: 1
  *  author: J create at 2015-4-24
  * =====================================================================================
  */
 void gnss_updata_handle(uint8_t *penetrate_data,uint16_t tempLens)
 {
 	if(system_param.detail_location_upload_mode != 0)
 	{
 		switch(system_param.detail_location_upload_mode)
 		{
			case 0x01 :
    			ask_board_gnss_data(0x00);
    		 	if(System_Flag.System_Flag_Bit.record_data==EAT_TRUE)
    			{
    				record_fade_gnss_data(penetrate_data,tempLens);
    			}
    			else
    			{
    				tcp_data_penetrate(Main_Link.mode_Socket,0x00,penetrate_data,tempLens);
    			}
    			break;
			case 0x02:
    			ask_board_gnss_data(0x00);
                if(System_Flag.System_Flag_Bit.record_data==EAT_TRUE)
    			{
    				record_fade_gnss_data(penetrate_data,tempLens);
    			}
    			else
    			{
                    tcp_data_penetrate(Main_Link.mode_Socket,0x00,penetrate_data,tempLens);
    			}
    			break;

			default:
    			if(System_Flag.System_Flag_Bit.record_data==EAT_TRUE)
    			{
    				record_fade_gnss_data(penetrate_data,tempLens);
    			}
    			else
    			{
                    tcp_data_penetrate(Main_Link.mode_Socket,0x00,penetrate_data,tempLens);
    			}
    			break;		
		}
	}
 }


