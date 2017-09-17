
#include "Eat_App_Protocol_Setting.h"



/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Save_String_Data
 *  Description:  保存接收到的数据
 *  Parameters : FileName ;文件指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void seting_data_to_board(uint8_t Data)
{
	uint8_t Temp[20]={0};
	uint32_t Data_Way=0;
	uint16_t TRUE_SEED=0;
	if(Data==0x00)
	{
		Temp[0]=Data;		
		Big_End_DwordtoByte(system_param.max_speed,Temp+1);
		Big_End_DwordtoByte(system_param.speeding_duration,Temp+5);
		
		TRUE_SEED=(u16)(system_param.speed_warning_dif/10);//真实速度；
		Big_End_WordtoByte(TRUE_SEED,Temp+9);
		uart1_send_data_frame(0x6C,Temp,11);
	}
	else if(Data==0x01)
	{
		Temp[0]=Data;		
		Big_End_DwordtoByte(system_param.continuous_driving_threshold,Temp+1);
		Big_End_DwordtoByte(system_param.min_rest_period,Temp+5);		
		Big_End_WordtoByte(system_param.fatigue_dring_warning_dif,Temp+9);
		uart1_send_data_frame(0x6C,Temp,11);

	}
	else if(Data==0x02)
	{
		Temp[0]=Data;
		Big_End_DwordtoByte(system_param.mileage,Temp+1);
		uart1_send_data_frame(0x6C,Temp,5);
	}
	
}
static void send_can_seting(void)
 {
     uint8_t ii;
     uint8_t offest=0;
     uint8_t can_send[256]={0};
     for(ii=0;ii<system_param.can_collect_param.canid_num;ii++)
     {
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[ii].can_id,can_send+offest);
         offest += 4;
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[ii].can_id_collect_frze,can_send+offest);
         offest += 4;
     }
     uart1_send_data_frame(0x64,can_send,offest);
 }
 void tcp_seting_param_proc(uint8_t socket,uint8_t *Temp_data_buff)//USER3
 {
	uint8_t *Base_Data;
	uint32_t seting_id=0;
	uint8_t re_conect=EAT_FALSE,re_rgist=EAT_FALSE;
	uint8_t param_cnt=0,parm_num=0;
	uint8_t can_seting=0,sp_seting=0,pl_seting=0;
	parm_num = *Temp_data_buff;//参数个数
	Base_Data = Temp_data_buff+1;
    #if APP_TRACE_DBG 
    eat_trace("tcp_seting_param_proc --> param_num:%d.",parm_num);
    #endif
	for(param_cnt=0;param_cnt<parm_num;param_cnt++)
	{
	   Big_End_BytetoDword(Base_Data,&seting_id);
	   switch(seting_id)
	   {
			case 0x0001://终端心跳发送间隔，单位为秒（s）
    			Big_End_BytetoDword(Base_Data+5,&system_param.HeartBeatUpdataFre);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0001-->终端心跳发送间隔:%d（s）",system_param.HeartBeatUpdataFre);
                #endif
    			break;
			
			case 0x0002://TCP 消息应答超时时间，单位为秒（s）
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveTcpRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0002-->TCP 消息应答超时时间:%d（s）",system_param.SaveTcpRetransTimes.waiting_time);
                #endif
    			break;

			case 0x0003://TCP 消息重传次数
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveTcpRetransTimes.timeout_times);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0003-->TCP 消息重传次数:%d.",system_param.SaveTcpRetransTimes.timeout_times);
                #endif
    			break;

			case 0x0004://UDP 消息应答超时时间
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveUdpRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0004-->UDP 消息应答超时时间:%d（s）",system_param.SaveUdpRetransTimes.waiting_time);
                #endif
    			break;
			
			case 0x0005://UDP 消息重传次数
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveUdpRetransTimes.timeout_times);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0005-->UDP 消息重传次数:%d.",system_param.SaveUdpRetransTimes.timeout_times);
                #endif
    			break;

			case 0x0006://SMS 消息应答超时时间
			
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveSmsRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0006-->SMS 消息应答超时时间%d（s）",system_param.SaveSmsRetransTimes.waiting_time);
                #endif
    			break;

			case 0x0007://SMS 消息重传次数
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveSmsRetransTimes.timeout_times);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0007-->SMS 消息重传次数:%d.",system_param.SaveSmsRetransTimes.timeout_times);
                #endif
    			break;

            
			case 0x0010://主服务器 APN，无线通信拨号访问点
                #if APP_TRACE_DBG 
                eat_trace("tcp_seting_param_proc --> id:0x0010.");
                #endif
    			memset(system_param.SaveMainBeareParameter.beare_apn,0,strlen(system_param.SaveMainBeareParameter.beare_apn));
    			memcpy(system_param.SaveMainBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));	

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0010-->主服务器 APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                #endif
    			break;

			case 0x0011: //主服务器无线通信拨号用户名
                 memset(system_param.SaveMainBeareParameter.beare_user,0,strlen(system_param.SaveMainBeareParameter.beare_user));
                 memcpy(system_param.SaveMainBeareParameter.beare_user,Base_Data+5,*(Base_Data+4));   
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0011-->主服务器无线通信拨号用户名:%s.",system_param.SaveMainBeareParameter.beare_user);
                 #endif
    			 break;

			case 0x0012://主服务器无线通信拨号密码
                memset(system_param.SaveMainBeareParameter.beare_password,0,strlen(system_param.SaveMainBeareParameter.beare_password));
                memcpy(system_param.SaveMainBeareParameter.beare_password,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0012-->主服务器无线通信拨号密码:%s.",system_param.SaveMainBeareParameter.beare_password);
                #endif
    			break;

			case 0x0013://主服务器地址,IP 或域名			
                memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,Base_Data+5,*(Base_Data+4));
                re_conect=EAT_TRUE;
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0013-->主服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                #endif
    			break;

			case 0x0014://备份服务器 APN
                memset(system_param.SaveBackupBeareParameter.beare_apn,0,strlen(system_param.SaveBackupBeareParameter.beare_apn));
                memcpy(system_param.SaveBackupBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));   
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0014-->备份服务器 APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
                #endif
			    break;

			case 0x0015: //备份服务器无线通信拨号用户名
                memset(system_param.SaveBackupBeareParameter.beare_user,0,strlen(system_param.SaveBackupBeareParameter.beare_user));
                memcpy(system_param.SaveBackupBeareParameter.beare_user,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0015-->备份服务器无线通信拨号用户名:%s.",system_param.SaveBackupBeareParameter.beare_user);
                #endif
			    break;

			case 0x0016://备份服务器无线通信拨号密码
                memset(system_param.SaveBackupBeareParameter.beare_password,0,strlen(system_param.SaveBackupBeareParameter.beare_password));
                memcpy(system_param.SaveBackupBeareParameter.beare_password,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0016-->备份服务器无线通信拨号密码:%s.",system_param.SaveBackupBeareParameter.beare_password);
                #endif
    			break;
					
		    case 0x0017://备份服务器地址,IP 或域名
                memset(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,Base_Data+5,*(Base_Data+4));
                re_conect=EAT_TRUE;
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0017-->备份服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                #endif
			    break;
			    
			case 0x0018://服务器 TCP 端口
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket);
                re_conect=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0018-->服务器 TCP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                #endif
    			break;

			case 0x0019://服务器 UDP 端口
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].udp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0019-->服务器 UDP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                #endif
			    break;

			case 0x001A://道路运输证 IC 卡认证主服务器 IP 地址或域名

                memset(system_param.ic_server_parameter.main_server_domin,0,sizeof(system_param.ic_server_parameter.main_server_domin));
                memcpy(system_param.ic_server_parameter.main_server_domin,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001A-->道路运输证 IC 卡认证主服务器:%s.",system_param.ic_server_parameter.main_server_domin);
                #endif
		        break;

			case 0x001B://道路运输证 IC 卡认证主服务器 TCP 端口
    			Big_End_BytetoDword(Base_Data+5,&system_param.ic_server_parameter.tcp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001b-->道路运输证 IC 卡认证主服务器 TCP 端口:%d.",system_param.ic_server_parameter.tcp_server_socket);
                #endif
			    break;
						
			case 0x001C://道路运输证 IC 卡认证主服务器 UDP 端口
    			Big_End_BytetoDword(Base_Data+5,&system_param.ic_server_parameter.udp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001c-->道路运输证 IC 卡认证主服务器 UDP 端口:%d.",system_param.ic_server_parameter.udp_server_socket);
                #endif
		        break;

		    case 0x001D://道路运输证 IC 卡认证备份服务器 IP 地址或域名
                memset(system_param.ic_server_parameter.backup_server_domin,0,sizeof(system_param.ic_server_parameter.backup_server_domin));
                memcpy(system_param.ic_server_parameter.backup_server_domin,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001d-->道路运输证 IC 卡认证备份服务器 IP 地址或域名:%s.",system_param.ic_server_parameter.backup_server_domin);
                #endif
		        break;
		    
 			case 0x0020://位置汇报策略
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.policy);
                befor_mileage_base=system_param.mileage;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0020-->位置汇报策略:%d.",system_param.location_reporting_param.policy);
                #endif
			    break;
			
		    case 0x0021://位置汇报方案
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.scheme);
                acc_off_mileage_base=system_param.mileage;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0021-->位置汇报方案:%d.",system_param.location_reporting_param.scheme);
                #endif
			    break;

			case 0x0022://驾驶员未登录汇报时间间隔
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.not_signed_frze);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0022-->驾驶员未登录汇报时间间隔:%d.",system_param.location_reporting_param.not_signed_frze);
                #endif
                
			    break;
			
		    case 0x0027://休眠时汇报时间间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0027-->休眠时汇报时间间隔:%d.",system_param.location_reporting_param.sleeping_frze);
                #endif
			    break;
						
			case 0x0028://紧急报警时汇报时间间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.emergency_alarm_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->紧急报警时汇报时间间隔:%d.",system_param.location_reporting_param.emergency_alarm_frze);
                #endif
		        break; 
		    
		    case 0x0029://缺省时间汇报间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->缺省时间汇报间隔:%d.",system_param.location_reporting_param.default_frze);
                #endif
		        break;  
		    
		    case 0x002A://晚上休眠时间间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.night_sleep_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x002A-->晚上休眠时间间隔:%d.",system_param.location_reporting_param.night_sleep_frze);
                #endif
		        break;

		    case 0x002B://晚上休眠时间
                memset(system_param.location_reporting_param.time_night,0,sizeof(system_param.location_reporting_param.time_night));
                memcpy(system_param.location_reporting_param.time_night,Base_Data+5,*(Base_Data+4));
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x002B-->:晚上时间:%0x:%0x--%0x:%0x",system_param.location_reporting_param.time_night[0],\
                system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3]);
                #endif
		        break;
		    
		    case 0x002C://缺省距离汇报间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->缺省距离汇报间隔:%d.",system_param.location_reporting_param.default_distance);
                #endif
		        break;

		    case 0x002D://驾驶员未登录汇报距离间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.not_signed_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->驾驶员未登录汇报距离间隔:%d.",system_param.location_reporting_param.not_signed_distance);
                #endif
		        break; 
		    
		    case 0x002E://休眠时汇报距离间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->休眠时汇报距离间隔:%d.",system_param.location_reporting_param.sleeping_distance);
                #endif
		        break; 

		    case 0x002F://紧急报警时汇报距离间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.emergency_alarm_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->紧急报警时汇报距离间隔:%d.",system_param.location_reporting_param.emergency_alarm_distance);
                #endif
		        break; 

		    case 0x0030://拐点补传角度
		        Big_End_BytetoDword(Base_Data+5,&system_param.inflection_angle_passup);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->拐点补传角度:%d.",system_param.inflection_angle_passup);
                #endif
		    break; 
		    
		    case 0x0031://电子围栏半径（非法位移阈值）
		        Big_End_BytetoWord(Base_Data+5,&system_param.electronic_fence_radius);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0031-->电子围栏半径（非法位移阈值）:%d.",system_param.electronic_fence_radius);
                #endif
		        break;

		    case 0x0040://监控平台电话号码	
                memset(system_param.phone_number_param.monitor_platform,0,sizeof(system_param.phone_number_param.monitor_platform));
                memcpy(system_param.phone_number_param.monitor_platform,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0040-->监控平台电话号码:%s.",system_param.phone_number_param.monitor_platform);
                #endif
			    break;
			
			case 0x0041://复位电话号码
                memset(system_param.phone_number_param.terminal_restart,0,sizeof(system_param.phone_number_param.terminal_restart));
                memcpy(system_param.phone_number_param.terminal_restart,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0041-->复位电话号码:%s.",system_param.phone_number_param.terminal_restart);
                #endif
			    break;

			case 0x0042://恢复出厂设置电话号码
                memset(system_param.phone_number_param.terminal_format,0,sizeof(system_param.phone_number_param.terminal_format));
                memcpy(system_param.phone_number_param.terminal_format,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0042-->恢复出厂设置电话号码:%s.",system_param.phone_number_param.terminal_format);
                #endif
			    break;

			case 0x0043://监控平台 SMS 电话号码
                memset(system_param.phone_number_param.monitor_platform_sms,0,sizeof(system_param.phone_number_param.monitor_platform_sms));
                memcpy(system_param.phone_number_param.monitor_platform_sms,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0043-->监控平台 SMS 电话号码:%s.",system_param.phone_number_param.monitor_platform_sms);
                #endif
			    break;

			case 0x0044://接收终端 SMS 文本报警号码
                memset(system_param.phone_number_param.terminal_sms_send,0,sizeof(system_param.phone_number_param.terminal_sms_send));
                memcpy(system_param.phone_number_param.terminal_sms_send,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0044-->接收终端 SMS 文本报警号码:%s.",system_param.phone_number_param.terminal_sms_send);
                #endif
			    break;

			case 0x0045://终端电话接听策略，0：自动接听；1：ACC ON 时自动接听，OFF 时手动接听
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.answer_policy);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0045-->终端电话接听策略:%d.",system_param.phone_number_param.answer_policy);
                #endif
			    break;

			case 0x0046://每次最长通话时间
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.every_longest_call_time);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0046-->每次最长通话时间:%d.",system_param.phone_number_param.every_longest_call_time);
                #endif
			    break;

			case 0x0047://当月最长通话时间
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.mounth_longest_call_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0047-->当月最长通话时间:%d.",system_param.phone_number_param.mounth_longest_call_time);
                #endif
			    break;
			
			case 0x0048://监听电话号码
                memset(system_param.phone_number_param.monitor_telephone,0,sizeof(system_param.phone_number_param.monitor_telephone));
                memcpy(system_param.phone_number_param.monitor_telephone,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0048-->监听电话号码:%s.",system_param.phone_number_param.monitor_telephone);
                #endif
			    break;
			
			case 0x0049://监管平台特权短信号码	
                memset(system_param.phone_number_param.supervision_platform,0,sizeof(system_param.phone_number_param.supervision_platform));
                memcpy(system_param.phone_number_param.supervision_platform,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0049-->监管平台特权短信号码:%s.",system_param.phone_number_param.supervision_platform);
                #endif
			    break;

			case 0x0050://报警屏蔽字
		        Big_End_BytetoDword(Base_Data+5,&system_param.alarm_mask.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0050-->报警屏蔽字:%d.",system_param.alarm_mask.Alarm_Original);
                #endif
			    break;

			case 0x0051://报警发送文本 SMS 开关
		        Big_End_BytetoDword(Base_Data+5,&system_param.alarm_sms_swtich.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0051-->报警发送文本 SMS 开关:%d.",system_param.alarm_sms_swtich.Alarm_Original);
                #endif
			    break;

			case 0x0052://报警拍摄开关
                Big_End_BytetoDword(Base_Data+5,&system_param.alarm_camer_switch.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0052-->报警拍摄开关:%d.",system_param.alarm_camer_switch.Alarm_Original);
                #endif
    			break;

			case 0x0053://报警拍摄存储标志
                Big_End_BytetoDword(Base_Data+5,&system_param.alarm_camer_upload.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0053-->报警拍摄存储标志:%d.",system_param.alarm_camer_upload.Alarm_Original);
                #endif
			    break;

			case 0x0054://关键标志
                Big_End_BytetoDword(Base_Data+5,&system_param.linchpin_alarm.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0054-->关键标志:%d.",system_param.linchpin_alarm.Alarm_Original);
                #endif
			    break;

			case 0x0055://最高速度
                Big_End_BytetoDword(Base_Data+5,&system_param.max_speed);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0055-->最高速度:%d.",system_param.max_speed);
                #endif
			    break;

			case 0x0056://超速持续时间
                Big_End_BytetoDword(Base_Data+5,&system_param.speeding_duration);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0056-->超速持续时间:%d.",system_param.speeding_duration);
                #endif
			    break;

			case 0x0057://连续驾驶时间门限
                Big_End_BytetoDword(Base_Data+5,&system_param.continuous_driving_threshold);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0057-->连续驾驶时间门限:%d.",system_param.continuous_driving_threshold);
                #endif
			    break;

			case 0x0058://当天累计驾驶时间门限
                Big_End_BytetoDword(Base_Data+5,&system_param.day_continuous_driving_threshold);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0058-->当天累计驾驶时间门限:%d.",system_param.day_continuous_driving_threshold);
                #endif
			    break;

			case 0x0059://最小休息时间
                Big_End_BytetoDword(Base_Data+5,&system_param.min_rest_period);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0059-->最小休息时间:%d.",system_param.min_rest_period);
                #endif
			    break;

			case 0x005A://最长停车时间
                Big_End_BytetoDword(Base_Data+5,&system_param.max_parking_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005A-->最长停车时间:%d.",system_param.max_parking_time);
                #endif
			    break;

			case 0x005B://超速报警预警差值
                Big_End_BytetoWord(Base_Data+5,&system_param.speed_warning_dif);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005B-->超速报警预警差值:%d.",system_param.speed_warning_dif);
                #endif
			    break;

			case 0x005C://疲劳驾驶预警差值
                Big_End_BytetoWord(Base_Data+5,&system_param.fatigue_dring_warning_dif);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005C-->疲劳驾驶预警差值:%d.",system_param.fatigue_dring_warning_dif);
                #endif
			    break;

			case 0x005D://碰撞报警参数设置：
                Big_End_BytetoWord(Base_Data+5,&system_param.collision_warning_param);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005D-->碰撞报警参数设置:%d.",system_param.collision_warning_param);
                #endif
			    break;

			case 0x005E://侧翻报警参数设置
                Big_End_BytetoWord(Base_Data+5,&system_param.rollover_param);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005E-->侧翻报警参数设置:%d.",system_param.rollover_param);
                #endif
			    break;
			
			case 0x0064://定时拍照控制
                Big_End_BytetoDword(Base_Data+5,&system_param.time_photo_param.photo_data);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0064-->定时拍照控制:0x%0x.",system_param.time_photo_param.photo_data);
                #endif
			    break;

			case 0x0065://定距拍照控制
                Big_End_BytetoDword(Base_Data+5,&system_param.distance_photo_param.photo_data);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0065-->定距拍照控制:0x%0x.",system_param.distance_photo_param.photo_data);
                #endif
			    break;

			case 0x0070://图像/视频质量，1-10，1 最好
                Big_End_BytetoDword(Base_Data+5,&system_param.image_quality);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0070-->图像/视频质量:%d.",system_param.image_quality);
                #endif
			    break;

			case 0x0071://亮度，0-255
                Big_End_BytetoDword(Base_Data+5,&system_param.image_brightness);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0071-->亮度:%d.",system_param.image_brightness);
                #endif
			    break;

			case 0x0072://对比度，0-127
                Big_End_BytetoDword(Base_Data+5,&system_param.image_contrast);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0072-->对比度:%d.",system_param.image_contrast);
                #endif
			    break;

			case 0x0073://饱和度，0-127
                Big_End_BytetoDword(Base_Data+5,&system_param.image_saturation);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0073-->饱和度:%d.",system_param.image_saturation);
                #endif
			    break;

			case 0x0074://色度，0-255
                Big_End_BytetoDword(Base_Data+5,&system_param.image_chroma);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0074-->色度:%d.",system_param.image_chroma);
                #endif
			    break;

			case 0x0080://车辆里程表读数，1/10km
                Big_End_BytetoDword(Base_Data+5,&system_param.mileage);
                seting_data_to_board(0x02);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->车辆里程表读数:%d.",system_param.mileage);
                #endif
			    break;

			case 0x0081://车辆所在的省域 ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.provincial_id);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0081-->车辆所在的省域 ID:%d.",system_param.terminl_registe.provincial_id);
                #endif
			    break;

			case 0x0082:// 车辆所在的市域 ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.county_id);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0082-->车辆所在的市域 ID:%d.",system_param.terminl_registe.county_id);
                #endif
			    break;

			case 0x0083://公安交通管理部门颁发的机动车号牌
                memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
                memcpy(system_param.terminl_registe.plate_number,Base_Data+5,*(Base_Data+4));
                
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0083-->公安交通管理部门颁发的机动车号牌:%s.",system_param.terminl_registe.plate_number);
                #endif
			    break;

			case 0x0084://车牌颜色
			    system_param.terminl_registe.plate_coror = *(Base_Data+5);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0084-->车牌颜色:%d.",system_param.terminl_registe.plate_coror);
                #endif
			    break;
			
			case 0x0090://GNSS 定位模式
			    system_param.location_mode = *(Base_Data+5);
                if(system_param.location_mode == 0x01)
                {
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.GPS = 0x01;
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.BD = 0x00;
                    board_mode_seting(0x03);
                }
                else if(system_param.location_mode ==0x02)
                {
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.GPS = 0x00;
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.BD = 0x01;
                    board_mode_seting(0x02);
                }
                else if(system_param.location_mode==0x03)
                {
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.GPS = 0x01;
                    Real_Time_Location_Inf.VDR_State_Inf.State_Bit.BD = 0x01;
                    board_mode_seting(0x04);
                }
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0090-->定位模式:%d.",system_param.location_mode);
                #endif
			    break;

			case 0x0091://GNSS 波特率
			    system_param.baud_rate= *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0091-->GNSS 波特率:%d.",system_param.baud_rate);
                #endif
			    break;

			case 0x0092://GNSS 模块详细定位数据输出频率
			    system_param.location_out_frze = *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0092-->模块详细定位数据输出频率:%d.",system_param.location_out_frze);
                #endif
			    break;

			case 0x0093://GNSS 模块详细定位数据采集频率
			    system_param.detail_location_get_frze = *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0093-->模块详细定位数据采集频率:%d.",system_param.detail_location_get_frze);
                #endif
			    break;
			
			case 0x0094://GNSS 模块详细定位数据上传方式
			    system_param.detail_location_upload_mode= *(Base_Data+5);
			    /**detail数据参数**/
			    detial_data_befor_clean = EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0094-->模块详细定位数据上传方式:%d.",system_param.detail_location_upload_mode);
                #endif
			    break;

			case 0x0095://GNSS 模块详细定位数据上传设置
			    system_param.detail_location_upload_setting = *(Base_Data+5);
			    /**detail数据参数**/
			    detial_data_befor_clean = EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0095-->模块详细定位数据上传设置:%d.",system_param.detail_location_upload_setting);
                #endif
			    break;

			case 0x0100://CAN 总线通道 1 采集时间间隔
                Big_End_BytetoDword(Base_Data+5,&system_param.can_collect_param.can1_time_interval);
                if(socket==Main_Link.mode_Socket) system_param.can_collect_param.update_socket=0x01;
                if(socket==Second_Link.mode_Socket)system_param.can_collect_param.update_socket=0x02;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0100-->CAN总线通道1采集时间间隔:%u.",system_param.can_collect_param.can1_time_interval);
                #endif
			    break;
			    
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0100-->CAN总线通道1采集时间间隔4:%u.",system_param.can_collect_param.can1_time_interval);
                #endif

			case 0x0101://CAN 总线通道 1 上传时间间隔
			
            #if APP_TRACE_DBG 
            eat_trace("seting_param id:0x0100-->CAN总线通道1采集时间间隔4:%u.",system_param.can_collect_param.can1_time_interval);
            #endif
                Big_End_BytetoWord(Base_Data+5,&system_param.can_collect_param.can1_upload_interval);
                if(socket==Main_Link.mode_Socket) 
                {
                    system_param.can_collect_param.update_socket=0x01;
                }
                if(socket==Second_Link.mode_Socket)
                {
                    system_param.can_collect_param.update_socket=0x02;
                }
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0101-->CAN总线通道1上传时间间隔:%d.",system_param.can_collect_param.can1_upload_interval);
                #endif
			    break;

			case 0x0102://CAN 总线通道 2 采集时间间隔
                Big_End_BytetoDword(Base_Data+5,&system_param.can_collect_param.can2_time_interval);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->CAN 总线通道 2 采集时间间隔:%d.",system_param.can_collect_param.can2_time_interval);
                #endif
			    break;

			case 0x0103://CAN总线通道2上传时间间隔
                Big_End_BytetoWord(Base_Data+5,&system_param.can_collect_param.can2_upload_interval);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->CAN总线通道2上传时间间隔:%d.",system_param.can_collect_param.can2_upload_interval);
                #endif
    			break;
			case 0xF006://IP锁定
				 ip_lock_data_proc(Base_Data+5);
				 break;
			default:
                if((seting_id >= 0x0110)&&(seting_id <= 0x01FF))
                {
                   can_seting = EAT_TRUE;
                   if(system_param.can_collect_param.canid_num<20)
                   {
                      Big_End_BytetoDword(Base_Data+5,&system_param.can_collect_param.can_id_param_array[system_param.can_collect_param.canid_num].can_id);
                      Big_End_BytetoDword(Base_Data+9,&(system_param.can_collect_param.can_id_param_array[system_param.can_collect_param.canid_num].can_id_collect_frze));
                      system_param.can_collect_param.canid_num++;
                   }
                }
			    break;	
		}
	  Base_Data = Base_Data + *(Base_Data+4)+5;//下一次的首地址
	}
	system_para_save_at_once();//保存参数
	if(sp_seting==EAT_TRUE)
	{
        seting_data_to_board(0x00);
	}
	if(pl_seting==EAT_TRUE)
	{
        seting_data_to_board(0x01);
	}
	if(can_seting==EAT_TRUE)
	{
	    send_can_seting();
	}
	if(re_conect==EAT_TRUE)
	{
        socket_close(Main_Link.mode_Socket);
    	Main_Link.link_staue = EAT_FALSE;
    	Main_Link.get_ip_statue=EAT_FALSE;    	
    	System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
    	System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;

    	
        if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
        {
            socket_close(Second_Link.mode_Socket);
            Second_Link.link_staue = EAT_FALSE;
            Second_Link.get_ip_statue=EAT_FALSE;
            System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
            System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
            
        }
        close_gprs_bearer();
        task_msg_send(EAT_USER_1,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
	}
	else if(re_rgist ==EAT_TRUE)//重注册第一IP
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
		
        if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
        {
            /*擦除鉴第二IP权码*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//记录鉴权码
            system_para_save_at_once();
    		if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
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

 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: tcp_request_param_serven
  *  Description:  查询参数
  *  Parameters : FileName ;文件指针
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-16              
  * =====================================================================================
  */
 void tcp_request_param_serven(int16_t l_socket,uint8_t *Temp_data_buff,uint16_t Servn)
 {
     
     uint8_t *P;//位置指针
     uint16_t Cnt = 0;
     uint16_t Cnt_Num = 3; //数据个数
     uint32_t Data_ID = 0;
     uint16_t msgs_sern = 0;
     uint8_t Send_Data[1056]={0};
	 uint16_t ip1_lock_state = 0;  //ip1锁定状态
	 uint16_t ip2_lock_state = 0;  //ip2锁定状态
     Big_End_WordtoByte(Servn,Send_Data);
     *(Send_Data+2) = *Temp_data_buff;//应答个数
     P=Send_Data+3;
     for(Cnt=0;Cnt<*Temp_data_buff;Cnt++)
     {  
         Big_End_BytetoDword(Temp_data_buff+1+4*Cnt,&Data_ID);
         switch(Data_ID)
         {
             case 0x0001://终端心跳发送间隔，单位为秒（s）
                  Big_End_DwordtoByte(0x0001,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.HeartBeatUpdataFre,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;//数据长度= 基长度+3(ID+长度) + 数据长度
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0001-->终端心跳发送间隔:%d（s）",system_param.HeartBeatUpdataFre);
                  #endif
                  break;
             
             case 0x0002://TCP 消息应答超时时间，单位为秒（s）
                  Big_End_DwordtoByte(0x0002,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.waiting_time,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;//数据长度= 基长度+3(ID+长度) + 数据长度
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0002-->TCP 消息应答超时时间:%d（s）",system_param.SaveTcpRetransTimes.waiting_time);
                  #endif
                  break;
             
             case 0x0003://TCP 消息重传次数
                  Big_End_DwordtoByte(0x0003,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.timeout_times,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0003-->TCP 消息重传次数:%d.",system_param.SaveTcpRetransTimes.timeout_times);
                  #endif
                  break;
             
             case 0x0004://UDP 消息应答超时时间
                  Big_End_DwordtoByte(0x0004,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.waiting_time,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0004-->UDP 消息应答超时时间:%d（s）",system_param.SaveUdpRetransTimes.waiting_time);
                  #endif
                  break;
                         
             case 0x0005://UDP 消息重传次数
                  Big_End_DwordtoByte(0x0005,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.timeout_times,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;

                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0005-->UDP 消息重传次数:%d.",system_param.SaveUdpRetransTimes.timeout_times);
                  #endif
                  break;
             
             case 0x0006://SMS 消息应答超时时间
                  Big_End_DwordtoByte(0x0006,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.waiting_time,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0006-->SMS 消息应答超时时间%d（s）",system_param.SaveSmsRetransTimes.waiting_time);
                  #endif
                  break;
             
             case 0x0007://SMS 消息重传次数
                  Big_End_DwordtoByte(0x0007,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.timeout_times,P+5);
                  P=P+5+4;//地址偏移
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0007-->SMS 消息重传次数:%d.",system_param.SaveSmsRetransTimes.timeout_times);
                  #endif
                 
                  break;
             
                         
             case 0x0010://主服务器 APN，无线通信拨号访问点
                  Big_End_DwordtoByte(0x0010,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_apn);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);

                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0010-->主服务器 APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                  #endif
                  break;
             
             case 0x0011: //主服务器无线通信拨号用户名
                  Big_End_DwordtoByte(0x0011,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_user);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_user,strlen(system_param.SaveMainBeareParameter.beare_user));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_user);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_user);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0011-->主服务器无线通信拨号用户名:%s.",system_param.SaveMainBeareParameter.beare_user);
                  #endif
                  break;
             
             case 0x0012://主服务器无线通信拨号密码
                  Big_End_DwordtoByte(0x0012,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_password);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_password,strlen(system_param.SaveMainBeareParameter.beare_password));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_password);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_password);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0012-->主服务器无线通信拨号密码:%s.",system_param.SaveMainBeareParameter.beare_password);
                  #endif
                  break;
             
             case 0x0013://主服务器地址,IP 或域名            
                  Big_End_DwordtoByte(0x0013,P);
                  *(P+4)=strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                  P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0013-->主服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  #endif
                  break;
             
             case 0x0014://备份服务器 APN
                  Big_End_DwordtoByte(0x0014,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0014-->备份服务器 APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
                  #endif
                  break;
             
             case 0x0015: //备份服务器无线通信拨号用户名
                  Big_End_DwordtoByte(0x0015,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_user);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_user,strlen(system_param.SaveBackupBeareParameter.beare_user));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_user);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_user);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0015-->备份服务器无线通信拨号用户名:%s.",system_param.SaveBackupBeareParameter.beare_user);
                  #endif
                  break;
             
             case 0x0016://备份服务器无线通信拨号密码
                  Big_End_DwordtoByte(0x0016,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_password);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_password,strlen(system_param.SaveBackupBeareParameter.beare_password));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_password);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_password);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0016-->备份服务器无线通信拨号密码:%s.",system_param.SaveBackupBeareParameter.beare_password);
                  #endif
                  break;
                                 
             case 0x0017://备份服务器地址,IP 或域名
                  Big_End_DwordtoByte(0x0017,P);
                  *(P+4)=strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  memcpy(P+5,system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
                  P=P+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0017-->备份服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  #endif
                  break;
                             
             case 0x0018://服务器 TCP 端口
                  Big_End_DwordtoByte(0x0018,P);
                  *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
                  P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0018-->服务器 TCP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  #endif
                  break;
             
             case 0x0019://服务器 UDP 端口
                  Big_End_DwordtoByte(0x0019,P);
                  *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket,P+5);
                  P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0019-->服务器 UDP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  #endif
                  break;
             
             case 0x001A://道路运输证 IC 卡认证主服务器 IP 地址或域名
                  Big_End_DwordtoByte(0x001A,P);
                  *(P+4)=strlen(system_param.ic_server_parameter.main_server_domin);
                  memcpy(P+5,system_param.ic_server_parameter.main_server_domin,strlen(system_param.ic_server_parameter.main_server_domin));
                  P=P+5+strlen(system_param.ic_server_parameter.main_server_domin);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.main_server_domin);
                    
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001A-->道路运输证 IC 卡认证主服务器:%s.",system_param.ic_server_parameter.main_server_domin);
                  #endif
                  break;
             
             case 0x001B://道路运输证 IC 卡认证主服务器 TCP 端口
                  Big_End_DwordtoByte(0x001B,P);
                  *(P+4)=sizeof(system_param.ic_server_parameter.tcp_server_socket);
                  Big_End_DwordtoByte(system_param.ic_server_parameter.tcp_server_socket,P+5);
                  P=P+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);
                    
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001b-->道路运输证 IC 卡认证主服务器 TCP 端口:%d.",system_param.ic_server_parameter.tcp_server_socket);
                  #endif
                  break;
                                     
             case 0x001C://道路运输证 IC 卡认证主服务器 UDP 端口
                  Big_End_DwordtoByte(0x001C,P);
                  *(P+4)=sizeof(system_param.ic_server_parameter.udp_server_socket);
                  Big_End_DwordtoByte(system_param.ic_server_parameter.udp_server_socket,P+5);
                  P=P+5+sizeof(system_param.ic_server_parameter.udp_server_socket);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.udp_server_socket);
                     
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001c-->道路运输证 IC 卡认证主服务器 UDP 端口:%d.",system_param.ic_server_parameter.udp_server_socket);
                  #endif
                  break;
             
             case 0x001D://道路运输证 IC 卡认证备份服务器 IP 地址或域名
                  Big_End_DwordtoByte(0x001D,P);
                  *(P+4)=strlen(system_param.ic_server_parameter.backup_server_domin);
                  memcpy(P+5,system_param.ic_server_parameter.backup_server_domin,strlen(system_param.ic_server_parameter.backup_server_domin));
                  P=P+5+strlen(system_param.ic_server_parameter.backup_server_domin);//地址偏移
                  Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.backup_server_domin);
                      
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001d-->道路运输证 IC 卡认证备份服务器 IP 地址或域名:%s.",system_param.ic_server_parameter.backup_server_domin);
                  #endif
                  break;
                         
             case 0x0020://位置汇报策略
                  Big_End_DwordtoByte(0x0020,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.policy);
                  Big_End_DwordtoByte(system_param.location_reporting_param.policy,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.policy);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.policy);
         
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0020-->位置汇报策略:%d.",system_param.location_reporting_param.policy);
                  #endif
                  break;
                         
             case 0x0021://位置汇报方案
                  Big_End_DwordtoByte(0x0021,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.scheme);
                  Big_End_DwordtoByte(system_param.location_reporting_param.scheme,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.scheme);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.scheme);
         
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0021-->位置汇报方案:%d.",system_param.location_reporting_param.scheme);
                  #endif
                  break;
             
             case 0x0022://驾驶员未登录汇报时间间隔
                  Big_End_DwordtoByte(0x0022,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.not_signed_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.not_signed_frze);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0022-->驾驶员未登录汇报时间间隔:%d.",system_param.location_reporting_param.not_signed_frze);
                  #endif
                  break;
                         
             case 0x0027://休眠时汇报时间间隔
                  Big_End_DwordtoByte(0x0027,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.sleeping_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.sleeping_frze);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0027-->休眠时汇报时间间隔:%d.",system_param.location_reporting_param.sleeping_frze);
                  #endif
                  break;
                                     
             case 0x0028://紧急报警时汇报时间间隔
                  Big_End_DwordtoByte(0x0028,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->紧急报警时汇报时间间隔:%d.",system_param.location_reporting_param.emergency_alarm_frze);
                  #endif
                  break; 
                         
             case 0x0029://缺省时间汇报间隔
                  Big_End_DwordtoByte(0x0029,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.default_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.default_frze);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->缺省时间汇报间隔:%d.",system_param.location_reporting_param.default_frze);
                  #endif
                  break;  
                         
             case 0x002A://晚上休眠时间间隔
                  Big_End_DwordtoByte(0x002A,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.night_sleep_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.night_sleep_frze);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.night_sleep_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x002A-->晚上休眠时间间隔:%d.",system_param.location_reporting_param.night_sleep_frze);
                  #endif
                  break;
             
             case 0x002B://晚上休眠时间
                  Big_End_DwordtoByte(0x002B,P);
                  *(P+4) = sizeof(system_param.location_reporting_param.time_night) ;
                  memcpy(P+5,system_param.location_reporting_param.time_night,sizeof(system_param.location_reporting_param.time_night));
                  P=P+5+sizeof(system_param.location_reporting_param.time_night);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.time_night);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x002B-->:晚上时间:%0x:%0x:%0x--%0x:%0x:%0x:%0x.",system_param.location_reporting_param.time_night[0],\
                  system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3],\
                                                                          system_param.location_reporting_param.time_night[4],system_param.location_reporting_param.time_night[5]);
                  #endif
                  break;
                         
             case 0x002C://缺省距离汇报间隔
                  Big_End_DwordtoByte(0x002C,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.default_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.default_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.default_distance);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_distance);
  
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->缺省距离汇报间隔:%d.",system_param.location_reporting_param.default_distance);
                  #endif
                  break;
             
             case 0x002D://驾驶员未登录汇报距离间隔
                  Big_End_DwordtoByte(0x002D,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.not_signed_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.not_signed_distance);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->驾驶员未登录汇报距离间隔:%d.",system_param.location_reporting_param.not_signed_distance);
                  #endif
                  break; 
                                   
             case 0x002E://休眠时汇报距离间隔
                  Big_End_DwordtoByte(0x002E,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.sleeping_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.sleeping_distance);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->休眠时汇报距离间隔:%d.",system_param.location_reporting_param.sleeping_distance);
                  #endif
                  break; 
             
             case 0x002F://紧急报警时汇报距离间隔
                  Big_End_DwordtoByte(0x002F,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->紧急报警时汇报距离间隔:%d.",system_param.location_reporting_param.emergency_alarm_distance);
                  #endif
                  break; 
             
             case 0x0030://拐点补传角度
                  Big_End_DwordtoByte(0x0030,P);
                  *(P+4)=sizeof(system_param.inflection_angle_passup);
                  Big_End_DwordtoByte(system_param.inflection_angle_passup,P+5);
                  P=P+5+sizeof(system_param.inflection_angle_passup);//地址偏移
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.inflection_angle_passup);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->拐点补传角度:%d.",system_param.inflection_angle_passup);
                  #endif
                  break; 
                         
             case 0x0031://电子围栏半径（非法位移阈值）
                 Big_End_DwordtoByte(0x0031,P);
                 *(P+4)=sizeof(system_param.electronic_fence_radius);
                 Big_End_WordtoByte(system_param.electronic_fence_radius,P+5);
                 P=P+5+sizeof(system_param.electronic_fence_radius);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.electronic_fence_radius);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0031-->电子围栏半径（非法位移阈值）:%d.",system_param.electronic_fence_radius);
                 #endif
                 break;
             
            case 0x0040://监控平台电话号码  
                 Big_End_DwordtoByte(0x0040,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_platform);
                 memcpy(P+5,system_param.phone_number_param.monitor_platform,strlen(system_param.phone_number_param.monitor_platform));
                 P=P+5+strlen(system_param.phone_number_param.monitor_platform);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform);
         
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0040-->监控平台电话号码:%s.",system_param.phone_number_param.monitor_platform);
                 #endif
                 break;
                         
            case 0x0041://复位电话号码
                 Big_End_DwordtoByte(0x0041,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_restart);
                 memcpy(P+5,system_param.phone_number_param.terminal_restart,strlen(system_param.phone_number_param.terminal_restart));
                 P=P+5+strlen(system_param.phone_number_param.terminal_restart);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_restart);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0041-->复位电话号码:%s.",system_param.phone_number_param.terminal_restart);
                 #endif
                 break;
             
            case 0x0042://恢复出厂设置电话号码
                 Big_End_DwordtoByte(0x0042,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_format);
                 memcpy(P+5,system_param.phone_number_param.terminal_format,strlen(system_param.phone_number_param.terminal_format));
                 P=P+5+strlen(system_param.phone_number_param.terminal_format);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_format);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0042-->恢复出厂设置电话号码:%s.",system_param.phone_number_param.terminal_format);
                 #endif
                 break;
             
            case 0x0043://监控平台 SMS 电话号码
                 Big_End_DwordtoByte(0x0043,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_platform_sms);
                 memcpy(P+5,system_param.phone_number_param.monitor_platform_sms,strlen(system_param.phone_number_param.monitor_platform_sms));
                 P=P+5+strlen(system_param.phone_number_param.monitor_platform_sms);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform_sms);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0043-->监控平台 SMS 电话号码:%s.",system_param.phone_number_param.monitor_platform_sms);
                 #endif
                 break;
             
            case 0x0044://接收终端 SMS 文本报警号码
                 Big_End_DwordtoByte(0x0044,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_sms_send);
                 memcpy(P+5,system_param.phone_number_param.terminal_sms_send,strlen(system_param.phone_number_param.terminal_sms_send));
                 P=P+5+strlen(system_param.phone_number_param.terminal_sms_send);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_sms_send);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0044-->接收终端 SMS 文本报警号码:%s.",system_param.phone_number_param.terminal_sms_send);
                 #endif
                 break;
             
            case 0x0045://终端电话接听策略，0：自动接听；1：ACC ON 时自动接听，OFF 时手动接听
                 Big_End_DwordtoByte(0x0045,P);
                 *(P+4)=sizeof(system_param.phone_number_param.answer_policy);
                 Big_End_DwordtoByte(system_param.phone_number_param.answer_policy,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.answer_policy);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.answer_policy);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0045-->终端电话接听策略:%d.",system_param.phone_number_param.answer_policy);
                 #endif
                 break;
             
            case 0x0046://每次最长通话时间
                 Big_End_DwordtoByte(0x0046,P);
                 *(P+4)=sizeof(system_param.phone_number_param.every_longest_call_time);
                 Big_End_DwordtoByte(system_param.phone_number_param.every_longest_call_time,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.every_longest_call_time);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.every_longest_call_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0046-->每次最长通话时间:%d.",system_param.phone_number_param.every_longest_call_time);
                 #endif
                 break;
             
            case 0x0047://当月最长通话时间
                 Big_End_DwordtoByte(0x0047,P);
                 *(P+4)=sizeof(system_param.phone_number_param.mounth_longest_call_time);
                 Big_End_DwordtoByte(system_param.phone_number_param.mounth_longest_call_time,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0047-->当月最长通话时间:%d.",system_param.phone_number_param.mounth_longest_call_time);
                 #endif
                 break;
                         
            case 0x0048://监听电话号码
                 Big_End_DwordtoByte(0x0048,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_telephone);
                 memcpy(P+5,system_param.phone_number_param.monitor_telephone,strlen(system_param.phone_number_param.monitor_telephone));
                 P=P+5+strlen(system_param.phone_number_param.monitor_telephone);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_telephone);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0048-->监听电话号码:%s.",system_param.phone_number_param.monitor_telephone);
                 #endif
                 break;
                         
            case 0x0049://监管平台特权短信号码  
                 Big_End_DwordtoByte(0x0049,P);
                 *(P+4)=strlen(system_param.phone_number_param.supervision_platform);
                 memcpy(P+5,system_param.phone_number_param.supervision_platform,strlen(system_param.phone_number_param.supervision_platform));
                 P=P+5+strlen(system_param.phone_number_param.supervision_platform);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.supervision_platform);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0049-->监管平台特权短信号码:%s.",system_param.phone_number_param.supervision_platform);
                 #endif
                 break;
             
            case 0x0050://报警屏蔽字
                 Big_End_DwordtoByte(0x0050,P);
                 *(P+4)=sizeof(system_param.alarm_mask.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_mask.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_mask.Alarm_Original);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_mask.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0050-->报警屏蔽字:%d.",system_param.alarm_mask.Alarm_Original);
                 #endif
                 break;
             
            case 0x0051://报警发送文本 SMS 开关
                 Big_End_DwordtoByte(0x0051,P);
                 *(P+4)=sizeof(system_param.alarm_sms_swtich.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_sms_swtich.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0051-->报警发送文本 SMS 开关:%d.",system_param.alarm_sms_swtich.Alarm_Original);
                 #endif
                 break;
             
            case 0x0052://报警拍摄开关
                 Big_End_DwordtoByte(0x0052,P);
                 *(P+4)=sizeof(system_param.alarm_camer_switch.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_camer_switch.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0052-->报警拍摄开关:%d.",system_param.alarm_camer_switch.Alarm_Original);
                 #endif
                 break;
             
            case 0x0053://报警拍摄存储标志
                 Big_End_DwordtoByte(0x0053,P);
                 *(P+4)=sizeof(system_param.alarm_camer_upload.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_camer_upload.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0053-->报警拍摄存储标志:%d.",system_param.alarm_camer_upload.Alarm_Original);
                 #endif
                 break;
             
            case 0x0054://关键标志
                 Big_End_DwordtoByte(0x0054,P);
                 *(P+4)=sizeof(system_param.linchpin_alarm.Alarm_Original);
                 Big_End_DwordtoByte(system_param.linchpin_alarm.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.linchpin_alarm.Alarm_Original);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.linchpin_alarm.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0054-->关键标志:%d.",system_param.linchpin_alarm.Alarm_Original);
                 #endif
                 break;
             
            case 0x0055://最高速度
                 Big_End_DwordtoByte(0x0055,P);
                 *(P+4)=sizeof(system_param.max_speed);
                 Big_End_DwordtoByte(system_param.max_speed,P+5);
                 P=P+5+sizeof(system_param.max_speed);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.max_speed);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0055-->最高速度:%d.",system_param.max_speed);
                 #endif
                 break;
             
            case 0x0056://超速持续时间
                 Big_End_DwordtoByte(0x0056,P);
                 *(P+4)=sizeof(system_param.speeding_duration);
                 Big_End_DwordtoByte(system_param.speeding_duration,P+5);
                 P=P+5+sizeof(system_param.speeding_duration);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.speeding_duration);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0056-->超速持续时间:%d.",system_param.speeding_duration);
                 #endif
                 break;
             
            case 0x0057://连续驾驶时间门限
                 Big_End_DwordtoByte(0x0057,P);
                 *(P+4)=sizeof(system_param.continuous_driving_threshold);
                 Big_End_DwordtoByte(system_param.continuous_driving_threshold,P+5);
                 P=P+5+sizeof(system_param.continuous_driving_threshold);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.continuous_driving_threshold);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0057-->连续驾驶时间门限:%d.",system_param.continuous_driving_threshold);
                 #endif
                 break;
             
            case 0x0058://当天累计驾驶时间门限
                 Big_End_DwordtoByte(0x0058,P);
                 *(P+4)=sizeof(system_param.day_continuous_driving_threshold);
                 Big_End_DwordtoByte(system_param.day_continuous_driving_threshold,P+5);
                 P=P+5+sizeof(system_param.day_continuous_driving_threshold);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.day_continuous_driving_threshold);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0058-->当天累计驾驶时间门限:%d.",system_param.day_continuous_driving_threshold);
                 #endif
                 break;
             
            case 0x0059://最小休息时间
                 Big_End_DwordtoByte(0x0059,P);
                 *(P+4)=sizeof(system_param.min_rest_period);
                 Big_End_DwordtoByte(system_param.min_rest_period,P+5);
                 P=P+5+sizeof(system_param.min_rest_period);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.min_rest_period);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0059-->最小休息时间:%d.",system_param.min_rest_period);
                 #endif
                 break;
             
            case 0x005A://最长停车时间
                 Big_End_DwordtoByte(0x005A,P);
                 *(P+4)=sizeof(system_param.max_parking_time);
                 Big_End_DwordtoByte(system_param.max_parking_time,P+5);
                 P=P+5+sizeof(system_param.max_parking_time);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.max_parking_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005A-->最长停车时间:%d.",system_param.max_parking_time);
                 #endif
                 break;
             
            case 0x005B://超速报警预警差值
                 Big_End_DwordtoByte(0x005B,P);
                 *(P+4)=sizeof(system_param.speed_warning_dif);
                 Big_End_DwordtoByte(system_param.speed_warning_dif,P+5);
                 P=P+5+sizeof(system_param.speed_warning_dif);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.speed_warning_dif);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005B-->超速报警预警差值:%d.",system_param.speed_warning_dif);
                 #endif
                 break;
             
            case 0x005C://疲劳驾驶预警差值
                 Big_End_DwordtoByte(0x005C,P);
                 *(P+4)=sizeof(system_param.fatigue_dring_warning_dif);
                 Big_End_DwordtoByte(system_param.fatigue_dring_warning_dif,P+5);
                 P=P+5+sizeof(system_param.fatigue_dring_warning_dif);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.fatigue_dring_warning_dif);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005C-->疲劳驾驶预警差值:%d.",system_param.fatigue_dring_warning_dif);
                 #endif
                 break;
             
            case 0x005D://碰撞报警参数设置：
                 Big_End_DwordtoByte(0x005D,P);
                 *(P+4)=sizeof(system_param.collision_warning_param);
                 Big_End_DwordtoByte(system_param.collision_warning_param,P+5);
                 P=P+5+sizeof(system_param.collision_warning_param);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.collision_warning_param);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005D-->碰撞报警参数设置:%d.",system_param.collision_warning_param);
                 #endif
                 break;
             
            case 0x005E://侧翻报警参数设置
                 Big_End_DwordtoByte(0x005E,P);
                 *(P+4)=sizeof(system_param.rollover_param);
                 Big_End_DwordtoByte(system_param.rollover_param,P+5);
                 P=P+5+sizeof(system_param.rollover_param);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.rollover_param);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005E-->侧翻报警参数设置:%d.",system_param.rollover_param);
                 #endif
                 break;
                         
            case 0x0064://定时拍照控制
                 Big_End_DwordtoByte(0x0064,P);
                 *(P+4)=sizeof(system_param.time_photo_param.photo_data);
                 Big_End_DwordtoByte(system_param.time_photo_param.photo_data,P+5);
                 P=P+5+sizeof(system_param.time_photo_param.photo_data);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.time_photo_param.photo_data);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0064-->定时拍照控制:0x%0x.",system_param.time_photo_param.photo_data);
                 #endif
                 break;
             
            case 0x0065://定距拍照控制
                 Big_End_DwordtoByte(0x0065,P);
                 *(P+4)=sizeof(system_param.distance_photo_param.photo_data);
                 Big_End_DwordtoByte(system_param.distance_photo_param.photo_data,P+5);
                 P=P+5+sizeof(system_param.distance_photo_param.photo_data);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.distance_photo_param.photo_data);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0065-->定距拍照控制:0x%0x.",system_param.distance_photo_param.photo_data);
                 #endif
                 break;
             
            case 0x0070://图像/视频质量，1-10，1 最好
                 Big_End_DwordtoByte(0x0070,P);
                 *(P+4)=sizeof(system_param.image_quality);
                 Big_End_DwordtoByte(system_param.image_quality,P+5);
                 P=P+5+sizeof(system_param.image_quality);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_quality);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0070-->图像/视频质量:%d.",system_param.image_quality);
                 #endif
                 break;
             
            case 0x0071://亮度，0-255
                 Big_End_DwordtoByte(0x0071,P);
                 *(P+4)=sizeof(system_param.image_brightness);
                 Big_End_DwordtoByte(system_param.image_brightness,P+5);
                 P=P+5+sizeof(system_param.image_brightness);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_brightness);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0071-->亮度:%d.",system_param.image_brightness);
                 #endif
                 break;
             
            case 0x0072://对比度，0-127
                 Big_End_DwordtoByte(0x0072,P);
                 *(P+4)=sizeof(system_param.image_contrast);
                 Big_End_DwordtoByte(system_param.image_contrast,P+5);
                 P=P+5+sizeof(system_param.image_contrast);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_contrast);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0072-->对比度:%d.",system_param.image_contrast);
                 #endif
                 break;
             
            case 0x0073://饱和度，0-127
                 Big_End_DwordtoByte(0x0073,P);
                 *(P+4)=sizeof(system_param.image_saturation);
                 Big_End_DwordtoByte(system_param.image_saturation,P+5);
                 P=P+5+sizeof(system_param.image_saturation);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_saturation);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0073-->饱和度:%d.",system_param.image_saturation);
                 #endif
                 break;
             
            case 0x0074://色度，0-255
                 Big_End_DwordtoByte(0x0074,P);
                 *(P+4)=sizeof(system_param.image_chroma);
                 Big_End_DwordtoByte(system_param.image_chroma,P+5);
                 P=P+5+sizeof(system_param.image_chroma);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_chroma);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0074-->色度:%d.",system_param.image_chroma);
                 #endif
                 break;
             
            case 0x0080://车辆里程表读数，1/10km
                 Big_End_DwordtoByte(0x0080,P);
                 *(P+4)=sizeof(system_param.mileage);
                 Big_End_DwordtoByte(system_param.mileage,P+5);
                 P=P+5+sizeof(system_param.mileage);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.mileage);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->车辆里程表读数:%d.",system_param.mileage);
                 #endif
                 break;
             
            case 0x0081://车辆所在的省域 ID
                 Big_End_DwordtoByte(0x0081,P);
                 *(P+4)=sizeof(system_param.terminl_registe.provincial_id);
                 Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
                 P=P+5+sizeof(system_param.terminl_registe.provincial_id);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.provincial_id);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0081-->车辆所在的省域 ID:%d.",system_param.terminl_registe.provincial_id);
                 #endif
                 break;
             
            case 0x0082:// 车辆所在的市域 ID
                 Big_End_DwordtoByte(0x0082,P);
                 *(P+4)=sizeof(system_param.terminl_registe.county_id);
                 Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
                 P=P+5+sizeof(system_param.terminl_registe.county_id);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.county_id);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0082-->车辆所在的市域 ID:%d.",system_param.terminl_registe.county_id);
                 #endif
                 break;
             
            case 0x0083://公安交通管理部门颁发的机动车号牌
                 Big_End_DwordtoByte(0x0083,P);
                 *(P+4)=strlen(system_param.terminl_registe.plate_number);
                 memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
                 P=P+5+strlen(system_param.terminl_registe.plate_number);//地址偏移
                 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0083-->公安交通管理部门颁发的机动车号牌:%s.",system_param.terminl_registe.plate_number);
                 #endif
                 break;
             
            case 0x0084://车牌颜色
                 Big_End_DwordtoByte(0x0084,P);
                 *(P+4) = sizeof(system_param.terminl_registe.plate_coror);
                 *(P+5) = system_param.terminl_registe.plate_coror;
                 P=P+5+sizeof(system_param.terminl_registe.plate_coror);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.plate_coror);
                 
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0084-->车牌颜色:%d.",system_param.terminl_registe.plate_coror);
                 #endif
                 break;
                         
            case 0x0090://GNSS 定位模式
                 Big_End_DwordtoByte(0x0090,P);
                 *(P+4)=sizeof(system_param.location_mode);
                 *(P+5) = system_param.location_mode;
                 P=P+5+sizeof(system_param.location_mode);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.location_mode);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0090-->定位模式:%d.",system_param.location_mode);
                 #endif
                 break;
             
            case 0x0091://GNSS 波特率
                 Big_End_DwordtoByte(0x0091,P);
                 *(P+4)=sizeof(system_param.baud_rate);
                 *(P+5) = system_param.baud_rate;
                 P=P+5+sizeof(system_param.baud_rate);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.baud_rate);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0091-->GNSS 波特率:%d.",system_param.baud_rate);
                 #endif
                 break;
             
            case 0x0092://GNSS 模块详细定位数据输出频率
                 Big_End_DwordtoByte(0x0092,P);
                 *(P+4)=sizeof(system_param.location_out_frze);
                 *(P+5) = system_param.location_out_frze;
                 P=P+5+sizeof(system_param.location_out_frze);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.location_out_frze);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0092-->模块详细定位数据输出频率:%d.",system_param.location_out_frze);
                 #endif
                 break;
             
            case 0x0093://GNSS 模块详细定位数据采集频率
                 Big_End_DwordtoByte(0x0093,P);
                 *(P+4)=sizeof(system_param.detail_location_get_frze);
                 Big_End_DwordtoByte(system_param.detail_location_get_frze,P+5);
                 P=P+5+sizeof(system_param.detail_location_get_frze);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_get_frze);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0093-->模块详细定位数据采集频率:%d.",system_param.detail_location_get_frze);
                 #endif
                 break;
                         
            case 0x0094://GNSS 模块详细定位数据上传方式
                 Big_End_DwordtoByte(0x0094,P);
                 *(P+4)=sizeof(system_param.detail_location_upload_mode);
                 *(P+5) = system_param.detail_location_upload_mode;
                 P=P+5+sizeof(system_param.detail_location_upload_mode);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_mode);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0094-->模块详细定位数据采集频率:%d.",system_param.detail_location_upload_mode);
                 #endif
                 break;
             
            case 0x0095://GNSS 模块详细定位数据上传设置
                 Big_End_DwordtoByte(0x0095,P);
                 *(P+4)=sizeof(system_param.detail_location_upload_setting);
                 *(P+5) = system_param.detail_location_upload_setting;
                 P=P+5+sizeof(system_param.detail_location_upload_setting);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_setting);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0095-->模块详细定位数据上传设置:%d.",system_param.detail_location_upload_setting);
                 #endif
                 break;
             
            case 0x0100://CAN 总线通道 1 采集时间间隔
                 Big_End_DwordtoByte(0x0100,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can1_time_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can1_time_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can1_time_interval);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_time_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0100-->CAN总线通道1采集时间间隔:%d.",system_param.can_collect_param.can1_time_interval);
                 #endif
                 break;
             
            case 0x0101://CAN 总线通道 1 上传时间间隔
                 Big_End_DwordtoByte(0x0101,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can1_upload_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can1_upload_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can1_upload_interval);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_upload_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0101-->CAN总线通道1上传时间间隔:%d.",system_param.can_collect_param.can1_upload_interval);
                 #endif
                 break;
             
            case 0x0102://CAN 总线通道 2 采集时间间隔
                 Big_End_DwordtoByte(0x0102,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can2_time_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can2_time_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can2_time_interval);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_time_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->CAN 总线通道 2 采集时间间隔:%d.",system_param.can_collect_param.can2_time_interval);
                 #endif
                 break;
             
            case 0x0103://CAN总线通道2上传时间间隔
                 Big_End_DwordtoByte(0x0103,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can2_upload_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can2_upload_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can2_upload_interval);//地址偏移
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_upload_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->CAN总线通道2上传时间间隔:%d.",system_param.can_collect_param.can2_upload_interval);
                 #endif
                 break;
			case 0xF006://IP锁定
				 ip1_lock_state = system_param.param_save.statue.ip1_lock;  //ip1锁定状态
				 ip2_lock_state = system_param.param_save.statue.ip2_lock;  //ip2锁定状态
				 Big_End_DwordtoByte(0xF006,P); 
				 *(P+4) = 16;
				 Big_End_WordtoByte(ip1_lock_state,P+5);
				 Big_End_WordtoByte(ip1_lock_state,P+16);
                 Cnt_Num=Cnt_Num+5+16;
				 break;
              default:
              break;  

          }
         if((Data_ID >= 0x0110)&&(Data_ID <= 0x01FF))
      	 {
              
              Big_End_DwordtoByte(Data_ID,P);
              *(P+4) = 8;
              Big_End_WordtoByte(system_param.can_collect_param.can_id_param_array[Data_ID-0x0110].can_id,P+5);
              Big_End_WordtoByte(system_param.can_collect_param.can_id_param_array[Data_ID-0x0110].can_id_collect_frze,P+9);
              P=P+5+8;//地址偏移
              Cnt_Num=Cnt_Num+5+8;
          }     
   }
   Tcp_Message_PacKage_And_Send(l_socket,&msgs_sern,tcp_request_param_replay,Send_Data,Cnt_Num,0,0,0);
   #if APP_TRACE_DBG 
   HexToAsicc(Send_Data,Cnt_Num);
   #endif
 }
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: tcp_all_data_request
  *  Description:  查询全部参数
  *  Parameters : FileName ;文件指针
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-16              
  * =====================================================================================
  */
 void tcp_all_data_request(int16_t l_socket,u16 Servn)
 {
     uint8_t *P;//位置指针
     uint16_t Cnt=0;
     uint16_t Cnt_Num = 3; //数据个数
     uint16_t msgs_sern = 0;
     uint8_t Send_Data[1056]={0};
     Big_End_WordtoByte(Servn,Send_Data);
     P=Send_Data+3;

     //case 0x0001://终端心跳发送间隔，单位为秒（s）
     Big_End_DwordtoByte(0x0001,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.HeartBeatUpdataFre,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;//数据长度= 基长度+3(ID+长度) + 数据长度
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0001-->终端心跳发送间隔:%d（s）",system_param.HeartBeatUpdataFre);
     #endif
                  
     //case 0x0002://TCP 消息应答超时时间，单位为秒（s）
     Big_End_DwordtoByte(0x0002,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.waiting_time,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;//数据长度= 基长度+3(ID+长度) + 数据长度
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0002-->TCP 消息应答超时时间:%d（s）",system_param.SaveTcpRetransTimes.waiting_time);
     #endif
                  
     //case 0x0003://TCP 消息重传次数
     Big_End_DwordtoByte(0x0003,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.timeout_times,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0003-->TCP 消息重传次数:%d.",system_param.SaveTcpRetransTimes.timeout_times);
     #endif
                  
     //case 0x0004://UDP 消息应答超时时间
     Big_End_DwordtoByte(0x0004,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.waiting_time,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0004-->UDP 消息应答超时时间:%d（s）",system_param.SaveUdpRetransTimes.waiting_time);
     #endif
                              
     //case 0x0005://UDP 消息重传次数
     Big_End_DwordtoByte(0x0005,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.timeout_times,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0005-->UDP 消息重传次数:%d.",system_param.SaveUdpRetransTimes.timeout_times);
     #endif
                  
     //case 0x0006://SMS 消息应答超时时间
     Big_End_DwordtoByte(0x0006,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.waiting_time,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0006-->SMS 消息应答超时时间%d（s）",system_param.SaveSmsRetransTimes.waiting_time);
     #endif
                  
     //case 0x0007://SMS 消息重传次数
     Big_End_DwordtoByte(0x0007,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.timeout_times,P+5);
     P=P+5+4;//地址偏移
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0007-->SMS 消息重传次数:%d.",system_param.SaveSmsRetransTimes.timeout_times);
     #endif
                              
     //case 0x0010://主服务器 APN，无线通信拨号访问点
     Big_End_DwordtoByte(0x0010,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_apn);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0010-->主服务器 APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
     #endif
                  
     //case 0x0011: //主服务器无线通信拨号用户名
     Big_End_DwordtoByte(0x0011,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_user);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_user,strlen(system_param.SaveMainBeareParameter.beare_user));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_user);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_user);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0011-->主服务器无线通信拨号用户名:%s.",system_param.SaveMainBeareParameter.beare_user);
     #endif
                 
     //case 0x0012://主服务器无线通信拨号密码
     Big_End_DwordtoByte(0x0012,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_password);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_password,strlen(system_param.SaveMainBeareParameter.beare_password));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_password);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_password);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0012-->主服务器无线通信拨号密码:%s.",system_param.SaveMainBeareParameter.beare_password);
     #endif
                  
     //case 0x0013://主服务器地址,IP 或域名            
     Big_End_DwordtoByte(0x0013,P);
     *(P+4)=strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
     P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0013-->主服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     #endif
                  
     //case 0x0014://备份服务器 APN
     Big_End_DwordtoByte(0x0014,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0014-->备份服务器 APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
     #endif
                  
     //case 0x0015: //备份服务器无线通信拨号用户名
     Big_End_DwordtoByte(0x0015,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_user);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_user,strlen(system_param.SaveBackupBeareParameter.beare_user));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_user);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_user);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0015-->备份服务器无线通信拨号用户名:%s.",system_param.SaveBackupBeareParameter.beare_user);
     #endif
     
     //case 0x0016://备份服务器无线通信拨号密码
     Big_End_DwordtoByte(0x0016,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_password);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_password,strlen(system_param.SaveBackupBeareParameter.beare_password));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_password);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_password);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0016-->备份服务器无线通信拨号密码:%s.",system_param.SaveBackupBeareParameter.beare_password);
     #endif
     
     //case 0x0017://备份服务器地址,IP 或域名
     Big_End_DwordtoByte(0x0017,P);
     *(P+4)=strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     memcpy(P+5,system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
     P=P+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0017-->备份服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     #endif
                                  
     //case 0x0018://服务器 TCP 端口
     Big_End_DwordtoByte(0x0018,P);
     *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
     P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0018-->服务器 TCP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     #endif
                  
     //case 0x0019://服务器 UDP 端口
     Big_End_DwordtoByte(0x0019,P);
     *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket,P+5);
     P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0019-->服务器 UDP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     #endif
                  
     //case 0x001A://道路运输证 IC 卡认证主服务器 IP 地址或域名
     Big_End_DwordtoByte(0x001A,P);
     *(P+4)=strlen(system_param.ic_server_parameter.main_server_domin);
     memcpy(P+5,system_param.ic_server_parameter.main_server_domin,strlen(system_param.ic_server_parameter.main_server_domin));
     P=P+5+strlen(system_param.ic_server_parameter.main_server_domin);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.main_server_domin);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001A-->道路运输证 IC 卡认证主服务器:%s.",system_param.ic_server_parameter.main_server_domin);
     #endif
                  
     //case 0x001B://道路运输证 IC 卡认证主服务器 TCP 端口
     Big_End_DwordtoByte(0x001B,P);
     *(P+4)=sizeof(system_param.ic_server_parameter.tcp_server_socket);
     Big_End_DwordtoByte(system_param.ic_server_parameter.tcp_server_socket,P+5);
     P=P+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001b-->道路运输证 IC 卡认证主服务器 TCP 端口:%d.",system_param.ic_server_parameter.tcp_server_socket);
     #endif
                                          
     //case 0x001C://道路运输证 IC 卡认证主服务器 UDP 端口
     Big_End_DwordtoByte(0x001C,P);
     *(P+4)=sizeof(system_param.ic_server_parameter.udp_server_socket);
     Big_End_DwordtoByte(system_param.ic_server_parameter.udp_server_socket,P+5);
     P=P+5+sizeof(system_param.ic_server_parameter.udp_server_socket);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.udp_server_socket);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001c-->道路运输证 IC 卡认证主服务器 UDP 端口:%d.",system_param.ic_server_parameter.udp_server_socket);
     #endif
                  
     //case 0x001D://道路运输证 IC 卡认证备份服务器 IP 地址或域名
     Big_End_DwordtoByte(0x001D,P);
     *(P+4)=strlen(system_param.ic_server_parameter.backup_server_domin);
     memcpy(P+5,system_param.ic_server_parameter.backup_server_domin,strlen(system_param.ic_server_parameter.backup_server_domin));
     P=P+5+strlen(system_param.ic_server_parameter.backup_server_domin);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.backup_server_domin);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001d-->道路运输证 IC 卡认证备份服务器 IP 地址或域名:%s.",system_param.ic_server_parameter.backup_server_domin);
     #endif
                              
     //case 0x0020://位置汇报策略
     Big_End_DwordtoByte(0x0020,P);
     *(P+4)=sizeof(system_param.location_reporting_param.policy);
     Big_End_DwordtoByte(system_param.location_reporting_param.policy,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.policy);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.policy);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0020-->位置汇报策略:%d.",system_param.location_reporting_param.policy);
     #endif
                              
     //case 0x0021://位置汇报方案
     Big_End_DwordtoByte(0x0021,P);
     *(P+4)=sizeof(system_param.location_reporting_param.scheme);
     Big_End_DwordtoByte(system_param.location_reporting_param.scheme,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.scheme);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.scheme);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0021-->位置汇报方案:%d.",system_param.location_reporting_param.scheme);
     #endif
                  
     //case 0x0022://驾驶员未登录汇报时间间隔
     Big_End_DwordtoByte(0x0022,P);
     *(P+4)=sizeof(system_param.location_reporting_param.not_signed_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.not_signed_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0022-->驾驶员未登录汇报时间间隔:%d.",system_param.location_reporting_param.not_signed_frze);
     #endif
                              
     //case 0x0027://休眠时汇报时间间隔
     Big_End_DwordtoByte(0x0027,P);
     *(P+4)=sizeof(system_param.location_reporting_param.sleeping_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.sleeping_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0027-->休眠时汇报时间间隔:%d.",system_param.location_reporting_param.sleeping_frze);
     #endif
                                          
     //case 0x0028://紧急报警时汇报时间间隔
     Big_End_DwordtoByte(0x0028,P);
     *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->紧急报警时汇报时间间隔:%d.",system_param.location_reporting_param.emergency_alarm_frze);
     #endif
                              
     //case 0x0029://缺省时间汇报间隔
     Big_End_DwordtoByte(0x0029,P);
     *(P+4)=sizeof(system_param.location_reporting_param.default_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.default_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->缺省时间汇报间隔:%d.",system_param.location_reporting_param.default_frze);
     #endif
                              
     //case 0x002A://晚上休眠时间间隔
     Big_End_DwordtoByte(0x002A,P);
     *(P+4)=sizeof(system_param.location_reporting_param.night_sleep_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.night_sleep_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.night_sleep_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x002A-->晚上休眠时间间隔:%d.",system_param.location_reporting_param.night_sleep_frze);
     #endif
                  
     //case 0x002B://晚上休眠时间
     Big_End_DwordtoByte(0x002B,P);
     *(P+4) = sizeof(system_param.location_reporting_param.time_night) ;
     memcpy(P+5,system_param.location_reporting_param.time_night,sizeof(system_param.location_reporting_param.time_night));
     P=P+5+sizeof(system_param.location_reporting_param.time_night);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.time_night);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x002B-->:晚上时间:%0x:%0x:%0x--%0x:%0x:%0x:%0x.",system_param.location_reporting_param.time_night[0],\
     system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3],\
                                                          system_param.location_reporting_param.time_night[4],system_param.location_reporting_param.time_night[5]);
     #endif
                              
     //case 0x002C://缺省距离汇报间隔
     Big_End_DwordtoByte(0x002C,P);
     *(P+4)=sizeof(system_param.location_reporting_param.default_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.default_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.default_distance);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_distance);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->缺省距离汇报间隔:%d.",system_param.location_reporting_param.default_distance);
     #endif
                  
     //case 0x002D://驾驶员未登录汇报距离间隔
     Big_End_DwordtoByte(0x002D,P);
     *(P+4)=sizeof(system_param.location_reporting_param.not_signed_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.not_signed_distance);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->驾驶员未登录汇报距离间隔:%d.",system_param.location_reporting_param.not_signed_distance);
     #endif
                              
     //case 0x002E://休眠时汇报距离间隔
     Big_End_DwordtoByte(0x002E,P);
     *(P+4)=sizeof(system_param.location_reporting_param.sleeping_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.sleeping_distance);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->休眠时汇报距离间隔:%d.",system_param.location_reporting_param.sleeping_distance);
     #endif
                  
     //case 0x002F://紧急报警时汇报距离间隔
     Big_End_DwordtoByte(0x002F,P);
     *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->紧急报警时汇报距离间隔:%d.",system_param.location_reporting_param.emergency_alarm_distance);
     #endif
                  
     //case 0x0030://拐点补传角度
     Big_End_DwordtoByte(0x0030,P);
     *(P+4)=sizeof(system_param.inflection_angle_passup);
     Big_End_DwordtoByte(system_param.inflection_angle_passup,P+5);
     P=P+5+sizeof(system_param.inflection_angle_passup);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.inflection_angle_passup);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->拐点补传角度:%d.",system_param.inflection_angle_passup);
     #endif
                              
     //case 0x0031://电子围栏半径（非法位移阈值）
     Big_End_DwordtoByte(0x0031,P);
     *(P+4)=sizeof(system_param.electronic_fence_radius);
     Big_End_WordtoByte(system_param.electronic_fence_radius,P+5);
     P=P+5+sizeof(system_param.electronic_fence_radius);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.electronic_fence_radius);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0031-->电子围栏半径（非法位移阈值）:%d.",system_param.electronic_fence_radius);
     #endif
                  
     //case 0x0040://监控平台电话号码  
     Big_End_DwordtoByte(0x0040,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_platform);
     memcpy(P+5,system_param.phone_number_param.monitor_platform,strlen(system_param.phone_number_param.monitor_platform));
     P=P+5+strlen(system_param.phone_number_param.monitor_platform);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0040-->监控平台电话号码:%s.",system_param.phone_number_param.monitor_platform);
     #endif
                              
     //case 0x0041://复位电话号码
     Big_End_DwordtoByte(0x0041,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_restart);
     memcpy(P+5,system_param.phone_number_param.terminal_restart,strlen(system_param.phone_number_param.terminal_restart));
     P=P+5+strlen(system_param.phone_number_param.terminal_restart);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_restart);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0041-->复位电话号码:%s.",system_param.phone_number_param.terminal_restart);
     #endif
                  
     //case 0x0042://恢复出厂设置电话号码
     Big_End_DwordtoByte(0x0042,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_format);
     memcpy(P+5,system_param.phone_number_param.terminal_format,strlen(system_param.phone_number_param.terminal_format));
     P=P+5+strlen(system_param.phone_number_param.terminal_format);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_format);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0042-->恢复出厂设置电话号码:%s.",system_param.phone_number_param.terminal_format);
     #endif
                  
     //case 0x0043://监控平台 SMS 电话号码
     Big_End_DwordtoByte(0x0043,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_platform_sms);
     memcpy(P+5,system_param.phone_number_param.monitor_platform_sms,strlen(system_param.phone_number_param.monitor_platform_sms));
     P=P+5+strlen(system_param.phone_number_param.monitor_platform_sms);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform_sms);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0043-->监控平台 SMS 电话号码:%s.",system_param.phone_number_param.monitor_platform_sms);
     #endif
                  
     //case 0x0044://接收终端 SMS 文本报警号码
     Big_End_DwordtoByte(0x0044,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_sms_send);
     memcpy(P+5,system_param.phone_number_param.terminal_sms_send,strlen(system_param.phone_number_param.terminal_sms_send));
     P=P+5+strlen(system_param.phone_number_param.terminal_sms_send);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_sms_send);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0044-->接收终端 SMS 文本报警号码:%s.",system_param.phone_number_param.terminal_sms_send);
     #endif
                  
     //case 0x0045://终端电话接听策略，0：自动接听；1：ACC ON 时自动接听，OFF 时手动接听
     Big_End_DwordtoByte(0x0045,P);
     *(P+4)=sizeof(system_param.phone_number_param.answer_policy);
     Big_End_DwordtoByte(system_param.phone_number_param.answer_policy,P+5);
     P=P+5+sizeof(system_param.phone_number_param.answer_policy);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.answer_policy);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0045-->终端电话接听策略:%d.",system_param.phone_number_param.answer_policy);
     #endif
                  
     //case 0x0046://每次最长通话时间
     Big_End_DwordtoByte(0x0046,P);
     *(P+4)=sizeof(system_param.phone_number_param.every_longest_call_time);
     Big_End_DwordtoByte(system_param.phone_number_param.every_longest_call_time,P+5);
     P=P+5+sizeof(system_param.phone_number_param.every_longest_call_time);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.every_longest_call_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0046-->每次最长通话时间:%d.",system_param.phone_number_param.every_longest_call_time);
     #endif
                  
     //case 0x0047://当月最长通话时间
     Big_End_DwordtoByte(0x0047,P);
     *(P+4)=sizeof(system_param.phone_number_param.mounth_longest_call_time);
     Big_End_DwordtoByte(system_param.phone_number_param.mounth_longest_call_time,P+5);
     P=P+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0047-->当月最长通话时间:%d.",system_param.phone_number_param.mounth_longest_call_time);
     #endif
                              
     //case 0x0048://监听电话号码
     Big_End_DwordtoByte(0x0048,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_telephone);
     memcpy(P+5,system_param.phone_number_param.monitor_telephone,strlen(system_param.phone_number_param.monitor_telephone));
     P=P+5+strlen(system_param.phone_number_param.monitor_telephone);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_telephone);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0048-->监听电话号码:%s.",system_param.phone_number_param.monitor_telephone);
     #endif
                              
     //case 0x0049://监管平台特权短信号码  
     Big_End_DwordtoByte(0x0049,P);
     *(P+4)=strlen(system_param.phone_number_param.supervision_platform);
     memcpy(P+5,system_param.phone_number_param.supervision_platform,strlen(system_param.phone_number_param.supervision_platform));
     P=P+5+strlen(system_param.phone_number_param.supervision_platform);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.supervision_platform);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0049-->监管平台特权短信号码:%s.",system_param.phone_number_param.supervision_platform);
     #endif
                  
     //case 0x0050://报警屏蔽字
     Big_End_DwordtoByte(0x0050,P);
     *(P+4)=sizeof(system_param.alarm_mask.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_mask.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_mask.Alarm_Original);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_mask.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0050-->报警屏蔽字:%d.",system_param.alarm_mask.Alarm_Original);
     #endif
                  
     //case 0x0051://报警发送文本 SMS 开关
     Big_End_DwordtoByte(0x0051,P);
     *(P+4)=sizeof(system_param.alarm_sms_swtich.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_sms_swtich.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0051-->报警发送文本 SMS 开关:%d.",system_param.alarm_sms_swtich.Alarm_Original);
     #endif
                  
     //case 0x0052://报警拍摄开关
     Big_End_DwordtoByte(0x0052,P);
     *(P+4)=sizeof(system_param.alarm_camer_switch.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_camer_switch.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0052-->报警拍摄开关:%d.",system_param.alarm_camer_switch.Alarm_Original);
     #endif
                  
     //case 0x0053://报警拍摄存储标志
     Big_End_DwordtoByte(0x0053,P);
     *(P+4)=sizeof(system_param.alarm_camer_upload.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_camer_upload.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0053-->报警拍摄存储标志:%d.",system_param.alarm_camer_upload.Alarm_Original);
     #endif
                  
     //case 0x0054://关键标志
     Big_End_DwordtoByte(0x0054,P);
     *(P+4)=sizeof(system_param.linchpin_alarm.Alarm_Original);
     Big_End_DwordtoByte(system_param.linchpin_alarm.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.linchpin_alarm.Alarm_Original);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.linchpin_alarm.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0054-->关键标志:%d.",system_param.linchpin_alarm.Alarm_Original);
     #endif
                  
     //case 0x0055://最高速度
     Big_End_DwordtoByte(0x0055,P);
     *(P+4)=sizeof(system_param.max_speed);
     Big_End_DwordtoByte(system_param.max_speed,P+5);
     P=P+5+sizeof(system_param.max_speed);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.max_speed);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0055-->最高速度:%d.",system_param.max_speed);
     #endif
                  
     //case 0x0056://超速持续时间
     Big_End_DwordtoByte(0x0056,P);
     *(P+4)=sizeof(system_param.speeding_duration);
     Big_End_DwordtoByte(system_param.speeding_duration,P+5);
     P=P+5+sizeof(system_param.speeding_duration);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.speeding_duration);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0056-->超速持续时间:%d.",system_param.speeding_duration);
     #endif
                  
     //case 0x0057://连续驾驶时间门限
     Big_End_DwordtoByte(0x0057,P);
     *(P+4)=sizeof(system_param.continuous_driving_threshold);
     Big_End_DwordtoByte(system_param.continuous_driving_threshold,P+5);
     P=P+5+sizeof(system_param.continuous_driving_threshold);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.continuous_driving_threshold);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0057-->连续驾驶时间门限:%d.",system_param.continuous_driving_threshold);
     #endif
                  
     //case 0x0058://当天累计驾驶时间门限
     Big_End_DwordtoByte(0x0058,P);
     *(P+4)=sizeof(system_param.day_continuous_driving_threshold);
     Big_End_DwordtoByte(system_param.day_continuous_driving_threshold,P+5);
     P=P+5+sizeof(system_param.day_continuous_driving_threshold);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.day_continuous_driving_threshold);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0058-->当天累计驾驶时间门限:%d.",system_param.day_continuous_driving_threshold);
     #endif
                  
     //case 0x0059://最小休息时间
     Big_End_DwordtoByte(0x0059,P);
     *(P+4)=sizeof(system_param.min_rest_period);
     Big_End_DwordtoByte(system_param.min_rest_period,P+5);
     P=P+5+sizeof(system_param.min_rest_period);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.min_rest_period);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0059-->最小休息时间:%d.",system_param.min_rest_period);
     #endif
                  
     //case 0x005A://最长停车时间
     Big_End_DwordtoByte(0x005A,P);
     *(P+4)=sizeof(system_param.max_parking_time);
     Big_End_DwordtoByte(system_param.max_parking_time,P+5);
     P=P+5+sizeof(system_param.max_parking_time);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.max_parking_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005A-->最长停车时间:%d.",system_param.max_parking_time);
     #endif
                  
     //case 0x005B://超速报警预警差值
     Big_End_DwordtoByte(0x005B,P);
     *(P+4)=sizeof(system_param.speed_warning_dif);
     Big_End_DwordtoByte(system_param.speed_warning_dif,P+5);
     P=P+5+sizeof(system_param.speed_warning_dif);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.speed_warning_dif);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005B-->超速报警预警差值:%d.",system_param.speed_warning_dif);
     #endif
                  
     //case 0x005C://疲劳驾驶预警差值
     Big_End_DwordtoByte(0x005C,P);
     *(P+4)=sizeof(system_param.fatigue_dring_warning_dif);
     Big_End_DwordtoByte(system_param.fatigue_dring_warning_dif,P+5);
     P=P+5+sizeof(system_param.fatigue_dring_warning_dif);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.fatigue_dring_warning_dif);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005C-->疲劳驾驶预警差值:%d.",system_param.fatigue_dring_warning_dif);
     #endif
                  
     //case 0x005D://碰撞报警参数设置：
     Big_End_DwordtoByte(0x005D,P);
     *(P+4)=sizeof(system_param.collision_warning_param);
     Big_End_DwordtoByte(system_param.collision_warning_param,P+5);
     P=P+5+sizeof(system_param.collision_warning_param);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.collision_warning_param);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005D-->碰撞报警参数设置:%d.",system_param.collision_warning_param);
     #endif
                  
     //case 0x005E://侧翻报警参数设置
     Big_End_DwordtoByte(0x005E,P);
     *(P+4)=sizeof(system_param.rollover_param);
     Big_End_DwordtoByte(system_param.rollover_param,P+5);
     P=P+5+sizeof(system_param.rollover_param);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.rollover_param);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005E-->侧翻报警参数设置:%d.",system_param.rollover_param);
     #endif
                              
     //case 0x0064://定时拍照控制
     Big_End_DwordtoByte(0x0064,P);
     *(P+4)=sizeof(system_param.time_photo_param.photo_data);
     Big_End_DwordtoByte(system_param.time_photo_param.photo_data,P+5);
     P=P+5+sizeof(system_param.time_photo_param.photo_data);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.time_photo_param.photo_data);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0064-->定时拍照控制:0x%0x.",system_param.time_photo_param.photo_data);
     #endif
                  
     //case 0x0065://定距拍照控制
     Big_End_DwordtoByte(0x0065,P);
     *(P+4)=sizeof(system_param.distance_photo_param.photo_data);
     Big_End_DwordtoByte(system_param.distance_photo_param.photo_data,P+5);
     P=P+5+sizeof(system_param.distance_photo_param.photo_data);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.distance_photo_param.photo_data);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0065-->定距拍照控制:0x%0x.",system_param.distance_photo_param.photo_data);
     #endif
                  
     //case 0x0070://图像/视频质量，1-10，1 最好
     Big_End_DwordtoByte(0x0070,P);
     *(P+4)=sizeof(system_param.image_quality);
     Big_End_DwordtoByte(system_param.image_quality,P+5);
     P=P+5+sizeof(system_param.image_quality);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_quality);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0070-->图像/视频质量:%d.",system_param.image_quality);
     #endif
                  
     //case 0x0071://亮度，0-255
     Big_End_DwordtoByte(0x0071,P);
     *(P+4)=sizeof(system_param.image_brightness);
     Big_End_DwordtoByte(system_param.image_brightness,P+5);
     P=P+5+sizeof(system_param.image_brightness);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_brightness);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0071-->亮度:%d.",system_param.image_brightness);
     #endif
                  
     //case 0x0072://对比度，0-127
     Big_End_DwordtoByte(0x0072,P);
     *(P+4)=sizeof(system_param.image_contrast);
     Big_End_DwordtoByte(system_param.image_contrast,P+5);
     P=P+5+sizeof(system_param.image_contrast);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_contrast);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0072-->对比度:%d.",system_param.image_contrast);
     #endif
                  
     //case 0x0073://饱和度，0-127
     Big_End_DwordtoByte(0x0073,P);
     *(P+4)=sizeof(system_param.image_saturation);
     Big_End_DwordtoByte(system_param.image_saturation,P+5);
     P=P+5+sizeof(system_param.image_saturation);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_saturation);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0073-->饱和度:%d.",system_param.image_saturation);
     #endif
                  
     //case 0x0074://色度，0-255
     Big_End_DwordtoByte(0x0074,P);
     *(P+4)=sizeof(system_param.image_chroma);
     Big_End_DwordtoByte(system_param.image_chroma,P+5);
     P=P+5+sizeof(system_param.image_chroma);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_chroma);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0074-->色度:%d.",system_param.image_chroma);
     #endif
                  
     //case 0x0080://车辆里程表读数，1/10km
     Big_End_DwordtoByte(0x0080,P);
     *(P+4)=sizeof(system_param.mileage);
     Big_End_DwordtoByte(system_param.mileage,P+5);
     P=P+5+sizeof(system_param.mileage);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.mileage);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->车辆里程表读数:%d.",system_param.mileage);
     #endif
                  
     //case 0x0081://车辆所在的省域 ID
     Big_End_DwordtoByte(0x0081,P);
     *(P+4)=sizeof(system_param.terminl_registe.provincial_id);
     Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
     P=P+5+sizeof(system_param.terminl_registe.provincial_id);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.provincial_id);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0081-->车辆所在的省域 ID:%d.",system_param.terminl_registe.provincial_id);
     #endif
                  
     //case 0x0082:// 车辆所在的市域 ID
     Big_End_DwordtoByte(0x0082,P);
     *(P+4)=sizeof(system_param.terminl_registe.county_id);
     Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
     P=P+5+sizeof(system_param.terminl_registe.county_id);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.county_id);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0082-->车辆所在的市域 ID:%d.",system_param.terminl_registe.county_id);
     #endif
                  
     //case 0x0083://公安交通管理部门颁发的机动车号牌
     Big_End_DwordtoByte(0x0083,P);
     *(P+4)=strlen(system_param.terminl_registe.plate_number);
     memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
     P=P+5+strlen(system_param.terminl_registe.plate_number);//地址偏移
     Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0083-->公安交通管理部门颁发的机动车号牌:%s.",system_param.terminl_registe.plate_number);
     #endif
                  
     //case 0x0084://车牌颜色
     Big_End_DwordtoByte(0x0084,P);
     *(P+4) = sizeof(system_param.terminl_registe.plate_coror);
     *(P+5) = system_param.terminl_registe.plate_coror;
     P=P+5+sizeof(system_param.terminl_registe.plate_coror);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.plate_coror);
             
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0084-->车牌颜色:%d.",system_param.terminl_registe.plate_coror);
     #endif
                              
     //case 0x0090://GNSS 定位模式
     Big_End_DwordtoByte(0x0090,P);
     *(P+4)=sizeof(system_param.location_mode);
     *(P+5) = system_param.location_mode;
     P=P+5+sizeof(system_param.location_mode);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_mode);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0090-->定位模式:%d.",system_param.location_mode);
     #endif
                  
     //case 0x0091://GNSS 波特率
     Big_End_DwordtoByte(0x0091,P);
     *(P+4)=sizeof(system_param.baud_rate);
     *(P+5) = system_param.baud_rate;
     P=P+5+sizeof(system_param.baud_rate);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.baud_rate);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0091-->GNSS 波特率:%d.",system_param.baud_rate);
     #endif
                  
     //case 0x0092://GNSS 模块详细定位数据输出频率
     Big_End_DwordtoByte(0x0092,P);
     *(P+4)=sizeof(system_param.location_out_frze);
     *(P+5) = system_param.location_out_frze;
     P=P+5+sizeof(system_param.location_out_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_out_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0092-->模块详细定位数据输出频率:%d.",system_param.location_out_frze);
     #endif
                  
     //case 0x0093://GNSS 模块详细定位数据采集频率
     Big_End_DwordtoByte(0x0093,P);
     *(P+4)=sizeof(system_param.detail_location_get_frze);
     Big_End_DwordtoByte(system_param.detail_location_get_frze,P+5);
     P=P+5+sizeof(system_param.detail_location_get_frze);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_get_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0093-->模块详细定位数据采集频率:%d.",system_param.detail_location_get_frze);
     #endif
                              
     //case 0x0094://GNSS 模块详细定位数据上传方式
     Big_End_DwordtoByte(0x0094,P);
     *(P+4)=sizeof(system_param.detail_location_upload_mode);
     *(P+5) = system_param.detail_location_upload_mode;
     P=P+5+sizeof(system_param.detail_location_upload_mode);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_mode);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0094-->模块详细定位数据上传方式:%d.",system_param.detail_location_upload_mode);
     #endif
                  
     //case 0x0095://GNSS 模块详细定位数据上传设置
     Big_End_DwordtoByte(0x0095,P);
     *(P+4)=sizeof(system_param.detail_location_upload_setting);
     *(P+5) = system_param.detail_location_upload_setting;
     P=P+5+sizeof(system_param.detail_location_upload_setting);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_setting);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0095-->模块详细定位数据上传设置:%d.",system_param.detail_location_upload_setting);
     #endif
                  
     //case 0x0100://CAN 总线通道 1 采集时间间隔
     Big_End_DwordtoByte(0x0100,P);
     *(P+4)=sizeof(system_param.can_collect_param.can1_time_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can1_time_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can1_time_interval);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_time_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0100-->CAN总线通道1采集时间间隔:%d.",system_param.can_collect_param.can1_time_interval);
     #endif
                  
     //case 0x0101://CAN 总线通道 1 上传时间间隔
     Big_End_DwordtoByte(0x0101,P);
     *(P+4)=sizeof(system_param.can_collect_param.can1_upload_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can1_upload_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can1_upload_interval);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_upload_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0101-->CAN总线通道1上传时间间隔:%d.",system_param.can_collect_param.can1_upload_interval);
     #endif
                  
     //case 0x0102://CAN 总线通道 2 采集时间间隔
     Big_End_DwordtoByte(0x0102,P);
     *(P+4)=sizeof(system_param.can_collect_param.can2_time_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can2_time_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can2_time_interval);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_time_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->CAN 总线通道 2 采集时间间隔:%d.",system_param.can_collect_param.can2_time_interval);
     #endif
                  
     //case 0x0103://CAN总线通道2上传时间间隔
     Big_End_DwordtoByte(0x0103,P);
     *(P+4)=sizeof(system_param.can_collect_param.can2_upload_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can2_upload_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can2_upload_interval);//地址偏移
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_upload_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->CAN总线通道2上传时间间隔:%d.",system_param.can_collect_param.can2_upload_interval);
     #endif
     for(Cnt=0;Cnt<system_param.can_collect_param.canid_num;Cnt++)
     {
         Big_End_DwordtoByte(0x0110+Cnt,P);
         *(P+4) = 8;
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[Cnt].can_id,P+5);
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[Cnt].can_id_collect_frze,P+9);
         P=P+5+8;//地址偏移
         Cnt_Num=Cnt_Num+5+8;
     }
     *(Send_Data+2) = 81+Cnt;//参数个数
     Tcp_Message_PacKage_And_Send(l_socket,&msgs_sern,tcp_request_param_replay,Send_Data,Cnt_Num,0,0,0);
 }


 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: Tcp_VDR_Request_Sate
  *  Description: 终端服属性查询函数
  *  Parameters : void
  *  author     : J.x.b create at SSI_1D02 at 2014-11-11          
  * =====================================================================================
  */
 void tcp_vdr_attributes_request(uint8_t socket)
 {
     uint8_t offest=0;
     uint16_t msgs_sern=0;
     uint8_t update[128]={0};
     
     Big_End_WordtoByte(0x0087,update);
     offest += sizeof(uint16_t);
     
     memcpy(update+offest,system_param.terminl_registe.manufacture_id,5);
     offest += 5;
     
     memcpy(update+offest,system_param.terminl_registe.terminl_model,20);
     offest += 20;
     
     memcpy(update+offest,system_param.terminl_registe.terminl_id,7);
     offest += 7;
     
     memcpy(update+offest,system_param.terminl_registe.iccd,10);
     offest += 10;

     *(update+offest) = strlen(system_param.terminl_registe.hardware_vison);
     offest+=sizeof(uint8_t);

     memcpy(update+offest,system_param.terminl_registe.hardware_vison,strlen(system_param.terminl_registe.hardware_vison));
     offest += strlen(system_param.terminl_registe.hardware_vison);
     
     *(update+offest) = strlen(system_param.terminl_registe.softerware_vison);
     offest+=sizeof(uint8_t);

     memcpy(update+offest,system_param.terminl_registe.softerware_vison,strlen(system_param.terminl_registe.softerware_vison));
     offest += strlen(system_param.terminl_registe.softerware_vison);
     
     *(update+offest) = 0x03;
     offest+=sizeof(uint8_t);

     *(update+offest) = 0x01;
     offest+=sizeof(uint8_t); 
     
     Tcp_Message_PacKage_And_Send(socket,&msgs_sern,tcp_vdr_attributes_replay_id,update,offest,0,0,0);
 
 }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: void tcp_car_ctrl_proc(uint8_t socket,uint16_t msg_svn,int8_t *temp_buf)
 *  Description: 车辆控制
 *  Parameters : void
 *  author     : J.x.b create at SSI_1D02 at 2014-11-18			 
 * =====================================================================================
 */
 void tcp_car_ctrl_proc(uint8_t socket,uint16_t msg_svn,int8_t *temp_buf)
 {
 	uint8_t Loca_Temp[128]={0};
 	uint16_t msgs_sern=0;
 	BaseLocationSave_Struct Location_Inf_Data;
    Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
 	if((*temp_buf)&0x01)
 	{
 		Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Lock_Door = 0x01;
 	}
 	else
 	{
 		Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Lock_Door = 0x00;

 	}
 	Big_End_WordtoByte(msg_svn,Loca_Temp); /*SN流水号转换*/	
 	memcpy(Loca_Temp+2,Location_Inf_Data.Location_Inf,Location_Inf_Data.Location_Inf_Lens);//位置
    Tcp_Message_PacKage_And_Send(socket,&msgs_sern,tcp_car_contrl_id_replay_id,temp_buf,Location_Inf_Data.Location_Inf_Lens + 2,0,0,0);
  }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: void tcp_car_ctrl_proc(uint8_t socket,uint16_t msg_svn,int8_t *temp_buf)
 *  Description: 终端控制
 *  Parameters : void
 *  author     : J.x.b create at SSI_1D02 at 2014-11-18			 
 * =====================================================================================
 */
  void remote_vdr_ctrl_proc(uint8_t *data_rev)
  {
      static u8 UL_address0[42]={0};
      static u8 Ctrl_NAME0[12]={0};
      static u8 Ctrl_MiM[12]={0};
      static u8 Address[42]={0};
      static u32 TCP_soc=0,UDP_SOC=0;
      static u8 Address0[7]={0};
      static u8 Hard_V[40]={0};
      static u8 Rard_V[40]={0};
      static u32 Ltimer=0;
      switch(*data_rev)
      {
          case 0x01:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> parma string:%s",data_rev+1);
              #endif
              break;
              
          case 0x02:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 终端连指定服务器:%s",data_rev+1);
              #endif
              break;
              
          case 0x03:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 【终端关机】!!!");
              #endif
              break;
          case 0x04:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 【终端复位】!!!");
              #endif
              
              board_mode_seting(0x05);
              break;
          case 0x05:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 【控制终端终端恢复出厂设置】!!!");
              #endif
              system_param.param_save.statue.wether_first_power_on=EAT_FALSE;
              system_para_save_at_once();
              board_mode_seting(0x06);
              break;
          case 0x06:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 【控制终端关闭数据通信】!!!");
              #endif
              
              break;
          case 0x07:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> 【控制终端关闭所有无线通信】!!!");
              #endif
              break;
          default:
          break;  
      }
  
  }

