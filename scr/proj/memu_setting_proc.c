
#include "memu_setting_proc.h"
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: memu_setting_return(uint8_t data)
 *  Description: 
 *  FileName： 菜单设置返回    // 内部
 *  return: null
 *  author: J.x.b  create at 2016-5-21
 * =====================================================================================
 */
static memu_setting_return(uint8_t data)
{
    uart1_send_data_frame(0x89,&data,1);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  void vehicle_infor_seting(uint8_t data_id)
 *  Description: 设置车辆信息
 *  FileName：vehicle_infor_seting
 *  return: null
 *  author: J.x.b  create at 2016-5-21
 * =====================================================================================
 */
void vehicle_infor_seting(uint8_t data_id)
{
	uint8_t Temp[20]={0};
	Temp[0]=data_id;
	switch(data_id)
	{
		case 0x01:
		memcpy(Temp+1,system_param.terminl_registe.plate_number,12);
		uart1_send_data_frame(0x69,Temp,13);
		break;
		
		case 0x02:
		memcpy(Temp+1,system_param.terminl_registe.vehicle_classification,12);
		uart1_send_data_frame(0x69,Temp,13);
		break;
		
		case 0x03:
		memcpy(Temp+1,system_param.terminl_registe.vehicle_vin,17);
		uart1_send_data_frame(0x69,Temp,18);
		break;
		
		case 0x04:
		Big_End_WordtoByte(system_param.terminl_registe.vehicle_puse_num,Temp+1);
		uart1_send_data_frame(0x69,Temp,3);
		break;

		case 0x05:
        memcpy(Temp+1,system_param.terminl_registe.terminl_id,7);
        uart1_send_data_frame(0x69,Temp,8);
        break;
        
		case 0x06:
        memcpy(Temp+1,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
        uart1_send_data_frame(0x69,Temp,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
        break;
		default:
		break;	
	}
}
static void log_off_settings(void)
{
    uint16_t msg_svn;
    #if memu_debug_printf
    eat_trace("memu_setting_data_pro-->> log_off_settings 终端注销 !");
    #endif
    Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_svn,0x0003,0,0,0,0,0);
    if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
    {
        Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&msg_svn,0x0003,0,0,0,0,0);
    }
}

/* 
* ===  FUNCTION  ======================================================================
*  void memu_setting_data_pro(uint8_t *data)
*  Description: 设置菜单信息
*  FileName：memu_setting_data_pro
*  return: null
*  author: J.x.b  create at 2016-5-21
* =====================================================================================
*/
void memu_setting_data_pro(uint8_t *data)
{
    uint8_t datat_cmd;
    uint16_t data_len;
    uint8_t open_ipdata;
    uint8_t data_buffer[32]={0};
    uint8_t Re_Link_Min=EAT_FALSE;
    uint8_t Re_Regist_main=EAT_FALSE;
    
    uint8_t Re_Link_second=EAT_FALSE;
    uint8_t Re_Regist_second=EAT_FALSE;
    
    datat_cmd =*data;
    Big_End_BytetoWord(data+1, &data_len);
    #if memu_debug_printf
    eat_trace("memu_setting_data_pro-->> memu set cmd:%0x,len:%d",datat_cmd,data_len);
    #endif
    switch(datat_cmd)
    {
        /*****vin设置*****/
        case 0x01://vin
             memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));
             memcpy(system_param.terminl_registe.vehicle_vin,data+3,data_len);
             memu_setting_return(0x00);
             vehicle_infor_seting(0x03);
             Re_Regist_main=EAT_TRUE;
             Re_Regist_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set vin:%s",system_param.terminl_registe.vehicle_vin);
             #endif
             break;
        /*****车牌号设置*****/
        case 0x02://
             memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
             memcpy(system_param.terminl_registe.plate_number,data+3,data_len);
             memu_setting_return(0x00);
             vehicle_infor_seting(0x01);
             Re_Regist_main=EAT_TRUE;
             Re_Regist_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 车牌:%s",system_param.terminl_registe.plate_number);
             #endif
             break;
        /*****车牌颜色设置*****/
        case 0x03:
             system_param.terminl_registe.plate_coror =*(data+3);
             memu_setting_return(0x00);
             Re_Regist_main=EAT_TRUE;
             Re_Regist_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 车牌颜色:%d",system_param.terminl_registe.plate_coror);
             #endif
             break;
        /*****车辆分类*****/
        case 0x04:
             memset(system_param.terminl_registe.vehicle_classification,0,sizeof(system_param.terminl_registe.vehicle_classification));
             if(*(data+3)==0x00)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"微型货车",strlen("微型货车"));
             }
             else if(*(data+3)==0x01)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"轻型货车",strlen("轻型货车"));
             }
             else if(*(data+3)==0x02)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"中型货车",strlen("中型货车"));
             }
             else if(*(data+3)==0x03)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"重型货车",strlen("重型货车"));
             }
             else if(*(data+3)==0x03)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"重型货车",strlen("重型货车"));
             }
             else if(*(data+3)==0x04)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"危险运输车",strlen("危险运输车"));
             }
             else if(*(data+3)==0x05)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"轻型半挂车",strlen("轻型半挂车"));
             }
             else if(*(data+3)==0x06)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"中型半挂车",strlen("中型半挂车"));
             }
             else if(*(data+3)==0x07)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"重型半挂车",strlen("重型半挂车"));
             }
             else if(*(data+3)==0x08)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"超重型半挂车",strlen("超重型半挂车"));
             }
             if(*(data+3)==0x09)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"微型客车",strlen("微型客车"));
             }
             else if(*(data+3)==0x0A)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"轻型客车",strlen("轻型客车"));
             }
             else if(*(data+3)==0x0B)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"中型客车",strlen("中型客车"));
             }
             else if(*(data+3)==0x0C)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"大型客车",strlen("重型客车"));
             }
             else if(*(data+3)==0x0D)
             {
                 memcpy(system_param.terminl_registe.vehicle_classification,"特大型客车",strlen("特大型客车"));
             }
             memu_setting_return(0x00);
             vehicle_infor_seting(0x02);
             
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 车辆分类:%s",system_param.terminl_registe.vehicle_classification);
             #endif
             break;
        /*****省ID*****/
        case 0x05:
             memset(data_buffer,0,sizeof(data_buffer));
             memcpy(data_buffer,data+3,data_len);
             system_param.terminl_registe.provincial_id=(uint16_t)atoi(data_buffer);
             if(system_param.terminl_registe.provincial_id>0xFFFF)
             {
                 memu_setting_return(0x01);
             }
             else
             {
                memu_setting_return(0x00);
                
                #if memu_debug_printf
                eat_trace("memu_setting_data_pro-->> memu set sheng:%d",system_param.terminl_registe.provincial_id);
                #endif
             }
             break;
             
        /*****市ID*****/
        case 0x06:
             memset(data_buffer,0,sizeof(data_buffer));
             memcpy(data_buffer,data+3,data_len);
             system_param.terminl_registe.county_id=(u16)atoi(data_buffer);
             if(system_param.terminl_registe.county_id>0xFFFF)
             {
                 memu_setting_return(0x01);
             }
             else
             {
                memu_setting_return(0x00);
                #if memu_debug_printf
                eat_trace("memu_setting_data_pro-->> memu set shi:%d",system_param.terminl_registe.county_id);
                #endif
             }
             break;
        /*****手机号码*****/
        case 0x07:
             memset(system_param.SaveTerminlPhone.terminl_phone_nums,0,sizeof(system_param.SaveTerminlPhone.terminl_phone_nums));
             memcpy(system_param.SaveTerminlPhone.terminl_phone_nums,data+3,data_len);
             memset(system_param.SaveTerminlPhone.terminl_phone_numd,0,6);
             if(strlen(system_param.SaveTerminlPhone.terminl_phone_nums)!=0)
             {
                Porotorl_Phone_Num_Conver(system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums),system_param.SaveTerminlPhone.terminl_phone_numd);
                memu_setting_return(0x00);
                Re_Regist_main=EAT_TRUE;
                Re_Regist_second=EAT_TRUE;
                #if memu_debug_printf
                eat_trace("memu_setting_data_pro-->> memu set terminl_phone_nums:%s",system_param.SaveTerminlPhone.terminl_phone_nums);
                #endif
             }
            break;     
        /*****SOS号码*****/
        case 0x08:
             memset(system_param.phone_number_param.sos_telephone,0,sizeof(system_param.phone_number_param.sos_telephone));
             memcpy(system_param.phone_number_param.sos_telephone,data+3,data_len);
             memu_setting_return(0x00);
             
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set sos_telephone:%s",system_param.phone_number_param.sos_telephone);
             #endif
             break;    
        /*****第一服务器APN*****/
        case 0x09:
            memset(system_param.SaveMainBeareParameter.beare_apn,0,sizeof(system_param.SaveMainBeareParameter.beare_apn));
            memcpy(system_param.SaveMainBeareParameter.beare_apn,data+3,data_len);
            memu_setting_return(0x00);
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set beare_apn:%s",system_param.SaveMainBeareParameter.beare_apn);
            #endif
            break;  
        /*****第一服务器 TCP UDP*****/
        case 0x0A:
            memset(data_buffer,0,sizeof(data_buffer));
            memcpy(data_buffer,data+3,data_len);
            memu_setting_return(0x00);
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set tcp or udp");
            #endif
            break;  
        /*****第一服务器 域名*****/
        case 0x0B:
            memset(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
            memcpy(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,data+3,data_len);
            memu_setting_return(0x00);
            vehicle_infor_seting(0x06);
            if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->>第一服务器地址:%s",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
            #endif
            break;  
        /*****第一服务器 端口号*****/
        case 0x0C:
            memset(data_buffer,0,sizeof(data_buffer));
            memcpy(data_buffer,data+3,data_len);
            system_param.SaveServerParameterArray[MAIN_SERVER].server_socket = (u32)atoi(data_buffer);
            memu_setting_return(0x00);
            if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set [MAIN_SERVER].server_socket:%d",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
            #endif
            break;    
        /*****第二服务器APN*****/
        case 0x0D:
            memset(system_param.SaveBackupBeareParameter.beare_apn,0,sizeof(system_param.SaveBackupBeareParameter.beare_apn));
            memcpy(system_param.SaveBackupBeareParameter.beare_apn,data+3,data_len);
            memu_setting_return(0x00);
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set fu beare_apn:%s",system_param.SaveBackupBeareParameter.beare_apn);
            #endif
            break;  
        /*****第二服务器 TCP UDP*****/
        case 0x0E:
            memset(data_buffer,0,sizeof(data_buffer));
            memcpy(data_buffer,data+3,data_len);
            memu_setting_return(0x00);
            break;  
        /*****第二服务器 域名*****/
        case 0x0F:
             memset(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin));
             memcpy(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,data+3,data_len);
             memu_setting_return(0x00);
             if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set [BACKUP_MAIN_SEVER].server_domin:%s",system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin);
             #endif
             break;  
        /*****第二服务器 端口号*****/
        case 0x10:
             memset(data_buffer,0,sizeof(data_buffer));
             memcpy(data_buffer,data+3,data_len);
             system_param.SaveServerParameterArray[SECOND_SERVER].server_socket=(u32)atoi(data_buffer);
             memu_setting_return(0x00);
             if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 第二服务器 server_socket:%d",system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
             #endif
             break;  
        /*****服务开启设置*****/
        case 0x11:
             open_ipdata = *(data+3);
             if(open_ipdata != system_param.param_save.statue.secondip_open_switch)
             {
                 #if memu_debug_printf
                 eat_trace("memu_setting_data_pro-->> 第二服务器开启状态发生改变 ! ");
                 #endif
                 system_param.param_save.statue.secondip_open_switch=open_ipdata;
                 if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
                 {
                     #if memu_debug_printf
                     eat_trace("memu_setting_data_pro-->> 第二服务器开启 ! ");
                     #endif
                     task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                 }
                 else
                 {
                     #if memu_debug_printf
                     eat_trace("memu_setting_data_pro-->> 第二服务器关闭 ! ");
                     #endif
                     close_secondip_link();
                     eat_timer_stop(EAT_TIMER_4);
                     eat_timer_stop(EAT_TIMER_6);
                     
                 }
                 system_para_save_at_once();
             }
             else
             {
                 #if memu_debug_printf
                 eat_trace("memu_setting_data_pro-->> 第二服务器状态未改变 ! ");
                 #endif
             }
             memu_setting_return(0x00);
             break;    
        /*****脉冲系数设置*****/
        case 0x12:
             memset(data_buffer,0,sizeof(data_buffer));
             memcpy(data_buffer,data+3,data_len);
             system_param.terminl_registe.vehicle_puse_num = (u16)atoi(data_buffer);
             memu_setting_return(0x00);
             vehicle_infor_seting(0x04);
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 脉冲系数:%d",system_param.terminl_registe.vehicle_puse_num);
             #endif
             break; 
        /*****终端ID*****/
        case 0x13:
             memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
             memcpy(system_param.terminl_registe.terminl_id,data+3,data_len);
             Re_Regist_main=EAT_TRUE;
             Re_Regist_second=EAT_TRUE;
             
             vehicle_infor_seting(0x05);
             memu_setting_return(0x00);
             break;  
        /*****鉴权码*****/
        case 0x14:
             memcpy(system_param.terminl_registe.first_authentication_id,data+3,data_len);//记录鉴权码
             Re_Regist_main=EAT_TRUE;
             memu_setting_return(0x00);
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 鉴权码:%d",system_param.terminl_registe.first_authentication_id);
             #endif
             break;      
        /*****初始里程*****/
        case 0x15:
             memset(data_buffer,0,sizeof(data_buffer));
             memcpy(data_buffer,data+3,data_len);
             system_param.mileage=(u32)atoi(data_buffer);
             memu_setting_return(0x00);
             vehicle_infor_seting(0x02);
             
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set 里程:%d",system_param.terminl_registe.first_authentication_id);
             #endif
             break;  
         /*****第一IP设置*****/
        case 0x16:
             memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
             memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,data+3,data_len);
              if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set server_domin:%s",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
             #endif
             memu_setting_return(0x00);
             vehicle_infor_seting(0x06);
             Re_Link_Min=EAT_TRUE;
             break;    
        /*****第二IP设置*****/
        case 0x17:
             memset(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
             memcpy(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,data+3,data_len);
             if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
             #if memu_debug_printf
             eat_trace("memu_setting_data_pro-->> memu set [BACKUP_MAIN_SEVER].server_domin:%s",system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
             #endif
             memu_setting_return(0x00);
             break;      
        /*****主机注册*****/
        case 0x18:
             if(wehther_launch_tcp_conect(MAIN_SERVER))
             {
                 Re_Regist_main=EAT_TRUE;
                 Re_Regist_second=EAT_TRUE;
                 memu_setting_return(0x00);
                 Re_Regist_main =EAT_TRUE;
             }
             else
             {
                 memu_setting_return(0x01);
             }
             break;      
        /*****主机注销*****/
        case 0x19:
             log_off_settings();
             close_frist_iplink();
             close_secondip_link();
             memu_setting_return(0x00);
             break;
        /*****载重状态*****/
        case 0x1a:
             Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Kg = *(data+3);
             system_param.param_save.statue.viech_kg = *(data+3);
             system_location_upadate();
             memu_setting_return(0x00);
             break;
        /*****主备份端口*****/
        case 0x1b:
            memset(data_buffer,0,sizeof(data_buffer));
            memcpy(data_buffer,data+3,data_len);
            system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket = (u32)atoi(data_buffer);
            memu_setting_return(0x00);
            if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set [BACKUP_MAIN_SEVER].server_socket:%d",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket);
            #endif
            break;
        /*****主备份端口*****/
        case 0x1c:
            memset(data_buffer,0,sizeof(data_buffer));
            memcpy(data_buffer,data+3,data_len);
            system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_socket = (u32)atoi(data_buffer);
            memu_setting_return(0x00);
            if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
            #if memu_debug_printf
            eat_trace("memu_setting_data_pro-->> memu set [BACKUP_SECOND_SERVER].server_socket:%d",system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_socket);
            #endif
            break;
            
        default:
            memu_setting_return(0x01);
            break;
    }
    /*****参数保存*****/
    system_para_save_at_once();
    if(Re_Link_Min == EAT_TRUE)//第一服务器重连接
	{
	    /*擦除第一服务器鉴权码*/
        memset(system_param.terminl_registe.first_authentication_id,0,sizeof(system_param.terminl_registe.first_authentication_id));//记录鉴权码
        system_para_save_at_once();
        
        Main_Link.link_staue=EAT_FALSE;
        Main_Link.get_ip_statue=EAT_FALSE;
		System_Flag.System_Flag_Bit.MainAutenSucess = EAT_FALSE;
		System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
	}
	if(Re_Regist_main ==EAT_TRUE)//重注册第一IP
	{
	    /*擦除鉴第一IP权码*/
        memset(system_param.terminl_registe.first_authentication_id,0,sizeof(system_param.terminl_registe.first_authentication_id));//记录鉴权码
        system_para_save_at_once();
		if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
		{
            /*若已经鉴权成功重新注册*/
            System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
		}
		else
		{
		    if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
		    {
                /*若未来鉴权成功但，激活成功，则连接(解决信息确实情况)*/
                
                Main_Link.link_staue=EAT_FALSE;
                Main_Link.get_ip_statue=EAT_FALSE;
                task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
		    }
		}
	}
	if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
    {
        if(Re_Link_second == EAT_TRUE)//重连接第二IP
        {
            /*擦除第二服务器鉴权码*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//记录鉴权码
            system_para_save_at_once();
        
            Second_Link.link_staue=EAT_FALSE;
            Second_Link.get_ip_statue=EAT_FALSE;
            System_Flag.System_Flag_Bit.SecondAutenSucess = EAT_FALSE;
            System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
        }
        if(Re_Regist_second == EAT_TRUE)//重注册第二IP
        {
            /*擦除鉴第二IP权码*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//记录鉴权码
            system_para_save_at_once();
            if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
            {
                /*若已经鉴权成功重新注册*/
                System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
                task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
            }
            else
            {
                if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
                {
                    /*若未来鉴权成功但，激活成功，则连接(解决信息缺失情况)*/
                    
                    Second_Link.link_staue=EAT_FALSE;
                    Second_Link.get_ip_statue=EAT_FALSE;
                    task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                }
            }
        }
    }
}

