/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "terminal_register.h"

 /****************************************************
* External variables declaration
*****************************************************/

/*****************************************************
* Local variables declaration(Static)
******************************************************/


/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Protocol_Init
 *  Description:  注册数据初始化
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
static uint8_t  register_protocol_init(uint8_t *Pdest)
{
    uint8_t data_cnt = 0;
	Big_End_WordtoByte(system_param.terminl_registe.provincial_id,Pdest);//进入目标数组
	data_cnt += 2;
	
	Big_End_WordtoByte(system_param.terminl_registe.county_id,Pdest+data_cnt);//进入目标数组
	data_cnt += 2;
	
	memcpy(Pdest+data_cnt,system_param.terminl_registe.manufacture_id,strlen(system_param.terminl_registe.manufacture_id));//制造商ID
	data_cnt += strlen(system_param.terminl_registe.manufacture_id);
	
	memcpy(Pdest+data_cnt,system_param.terminl_registe.terminl_model,20);//终端型号
	data_cnt +=20; 
	
	memcpy(Pdest+data_cnt,system_param.terminl_registe.terminl_id,7);//终端ID
	data_cnt +=7; 
	
	*(Pdest+data_cnt)=system_param.terminl_registe.plate_coror;
	data_cnt +=1;
	if(system_param.terminl_registe.plate_coror==0)
	{
		memcpy(Pdest + data_cnt,system_param.terminl_registe.vehicle_vin,17);//VIN
		data_cnt +=17;
	}
	else
	{
		memcpy(Pdest+data_cnt,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));//VIN
		data_cnt += strlen(system_param.terminl_registe.plate_number);
	}
    return data_cnt;
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: platform_certification
 *  Description:  车辆注册
 *	              车辆是否上牌牌，未上牌VIN ,上牌 车牌号
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void platform_certification(char_t mode_socket)
{
	uint8_t Lens;
	uint16_t ret_sn=0;
	uint8_t Temp_buf[128]={0};
	if(mode_socket == Main_Link.mode_Socket)
	{
        if(strlen(system_param.terminl_registe.first_authentication_id)!=0)
        {
             #if APP_TRACE_DBG 
    		 eat_trace("PlatformCertification--> 鉴权.");
        	 #endif
            Tcp_Message_PacKage_And_Send(mode_socket,&ret_sn,Tcp_authentication_ID,system_param.terminl_registe.first_authentication_id,strlen(system_param.terminl_registe.first_authentication_id),EAT_FALSE,0,0);
        }
        else
        {
            #if APP_TRACE_DBG 
    		eat_trace("PlatformCertification--> 注册.");
        	#endif       
        	Lens = register_protocol_init(Temp_buf);
            Tcp_Message_PacKage_And_Send(mode_socket,&ret_sn,Tcp_Send_Register_ID,Temp_buf,Lens,EAT_FALSE,0,0);    
        }
    }
    else if(mode_socket==Second_Link.mode_Socket)
    {
        if(strlen(system_param.terminl_registe.second_authentication_id)!=0)
        {
             #if APP_TRACE_DBG 
             eat_trace("PlatformCertification--> 鉴权.");
             #endif
            Tcp_Message_PacKage_And_Send(mode_socket,&ret_sn,Tcp_authentication_ID,system_param.terminl_registe.second_authentication_id,strlen(system_param.terminl_registe.second_authentication_id),EAT_FALSE,0,0);
        }
        else
        {
            #if APP_TRACE_DBG 
            eat_trace("PlatformCertification--> 注册.");
            #endif       
            Lens = register_protocol_init(Temp_buf);
            Tcp_Message_PacKage_And_Send(mode_socket,&ret_sn,Tcp_Send_Register_ID,Temp_buf,Lens,EAT_FALSE,0,0);    
        }
    }

}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: analyze_tcp_registe
 *  Description:  解析注册信息应答            
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7	
 *  modefy     : J.x.b 增加对应ID的解析
 * =====================================================================================
 */
void analyze_tcp_registe(int16_t local_socket, uint8_t *Temp_data_buff,uint16_t tempLens,uint16_t Sn)
{
    if(local_socket == Main_Link.mode_Socket)
    {
        switch(*(Temp_data_buff+2))
    	{
    		case 0x00://注册成功 解析鉴权码 发起鉴权
    		{
               #if APP_TRACE_DBG 
               eat_trace("analyze_tcp_registe--> main_link ok.");
               #endif
               memcpy(system_param.terminl_registe.first_authentication_id,Temp_data_buff+3,tempLens-3);//记录鉴权码
               system_para_save_at_once();//保存鉴权码
               first_server_conect_time_out(EAT_FALSE);//连接超时
               task_msg_send(EAT_USER_1,EAT_USER_1,FirstPlatformReg);
                
    		}	
    		break;
    		case 0x01://车辆被注册
    		{
                #if APP_TRACE_DBG 
                eat_trace("analyze_tcp_registe--> Main_Link:车辆被注册.");
                #endif

    		}	
    		break;
    		case 0x02://数据库中无该车
    		{
                #if APP_TRACE_DBG 
                eat_trace("analyze_tcp_registe--> Main_Link:数据库中无该车.");
                #endif
    		}
    		break;
    		case 0x03://终端以经被注册
    		{
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_registe--> Main_Link:终端被注册.");
                 #endif
    		}
    		break;
    		case 0x04://数据库中无改终端
    		{
                #if APP_TRACE_DBG 
                eat_trace("analyze_tcp_registe--> Main_Link:数据库中无该终端.");
                #endif
    		}
    		break;
    		default:
    		break;
    	}
	}
	else if(local_socket == Second_Link.mode_Socket)
	{
        switch(*(Temp_data_buff+2))
    	{
    		case 0x00://注册成功 解析鉴权码 发起鉴权
    		{

               #if APP_TRACE_DBG 
               eat_trace("analyze_tcp_registe--> Second_Link: sucess.");
               #endif
               memcpy(system_param.terminl_registe.second_authentication_id,Temp_data_buff+3,tempLens-3);//记录鉴权码
               system_para_save_at_once();//保存鉴权码
               second_server_conect_time_out(EAT_FALSE);//连接超时
               task_msg_send(EAT_USER_2,EAT_USER_2,SecondPlatformReg);
    		}	
    		break;
    		case 0x01://车辆被注册
    		{
               #if APP_TRACE_DBG 
               eat_trace("analyze_tcp_registe--> Second_Link:车辆被注册.");
               #endif
    		}	
    		break;
    		case 0x02://数据库中无该车
    		{
               #if APP_TRACE_DBG 
               eat_trace("analyze_tcp_registe--> Second_Link:数据库中无该车..");
               #endif
    		}
    		break;
    		case 0x03://终端以经被注册
    		{
               #if APP_TRACE_DBG 
               eat_trace("analyze_tcp_registe--> Second_Link:终端被注册.");
               #endif
    		}
    		break;
    		case 0x04://数据库中无改终端
    		{
                #if APP_TRACE_DBG 
                eat_trace("analyze_tcp_registe--> Second_Link:数据库中无该终端.");
                #endif
    		}
    		break;
    		default:
    		break;
    	}
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: vison_infor_update
 *  Description:  软件版本上报           
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7	
 * =====================================================================================
 */
void vison_infor_update(uint16_t l_socket)
{
    uint8_t infor_buffer[100]={0};
    uint8_t length=0;
    uint16_t ret_sn=0;
    infor_buffer[0]=0x00;
    infor_buffer[1]=0x87;
    length+=2;
    memcpy(infor_buffer+length,system_param.terminl_registe.manufacture_id,5);
    length+=5;
    memcpy(infor_buffer+length,system_param.terminl_registe.terminl_model,20);
    length+=20;
    memcpy(infor_buffer+length,system_param.terminl_registe.iccd,10);
    length+=10;
    *(infor_buffer+length)=strlen(system_param.terminl_registe.hardware_vison);
    length+=1;
    memcpy(infor_buffer+length,system_param.terminl_registe.hardware_vison,strlen(system_param.terminl_registe.hardware_vison));
    length+=strlen(system_param.terminl_registe.hardware_vison);
    *(infor_buffer+length)=strlen(system_param.terminl_registe.softerware_vison);
     length+=1;
    memcpy(infor_buffer+length,system_param.terminl_registe.softerware_vison,strlen(system_param.terminl_registe.softerware_vison));
    length+=strlen(system_param.terminl_registe.softerware_vison);
    Tcp_Message_PacKage_And_Send(l_socket,&ret_sn,0x0107,infor_buffer,length,EAT_FALSE,0,0);    
}

