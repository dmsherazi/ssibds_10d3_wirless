
#include "Eat_App_Protocol_Setting.h"



/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Save_String_Data
 *  Description:  ������յ�������
 *  Parameters : FileName ;�ļ�ָ��
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
		
		TRUE_SEED=(u16)(system_param.speed_warning_dif/10);//��ʵ�ٶȣ�
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
	parm_num = *Temp_data_buff;//��������
	Base_Data = Temp_data_buff+1;
    #if APP_TRACE_DBG 
    eat_trace("tcp_seting_param_proc --> param_num:%d.",parm_num);
    #endif
	for(param_cnt=0;param_cnt<parm_num;param_cnt++)
	{
	   Big_End_BytetoDword(Base_Data,&seting_id);
	   switch(seting_id)
	   {
			case 0x0001://�ն��������ͼ������λΪ�루s��
    			Big_End_BytetoDword(Base_Data+5,&system_param.HeartBeatUpdataFre);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0001-->�ն��������ͼ��:%d��s��",system_param.HeartBeatUpdataFre);
                #endif
    			break;
			
			case 0x0002://TCP ��ϢӦ��ʱʱ�䣬��λΪ�루s��
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveTcpRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0002-->TCP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveTcpRetransTimes.waiting_time);
                #endif
    			break;

			case 0x0003://TCP ��Ϣ�ش�����
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveTcpRetransTimes.timeout_times);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0003-->TCP ��Ϣ�ش�����:%d.",system_param.SaveTcpRetransTimes.timeout_times);
                #endif
    			break;

			case 0x0004://UDP ��ϢӦ��ʱʱ��
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveUdpRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0004-->UDP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveUdpRetransTimes.waiting_time);
                #endif
    			break;
			
			case 0x0005://UDP ��Ϣ�ش�����
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveUdpRetransTimes.timeout_times);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0005-->UDP ��Ϣ�ش�����:%d.",system_param.SaveUdpRetransTimes.timeout_times);
                #endif
    			break;

			case 0x0006://SMS ��ϢӦ��ʱʱ��
			
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveSmsRetransTimes.waiting_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0006-->SMS ��ϢӦ��ʱʱ��%d��s��",system_param.SaveSmsRetransTimes.waiting_time);
                #endif
    			break;

			case 0x0007://SMS ��Ϣ�ش�����
                Big_End_BytetoDword(Base_Data+5,&system_param.SaveSmsRetransTimes.timeout_times);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0007-->SMS ��Ϣ�ش�����:%d.",system_param.SaveSmsRetransTimes.timeout_times);
                #endif
    			break;

            
			case 0x0010://�������� APN������ͨ�Ų��ŷ��ʵ�
                #if APP_TRACE_DBG 
                eat_trace("tcp_seting_param_proc --> id:0x0010.");
                #endif
    			memset(system_param.SaveMainBeareParameter.beare_apn,0,strlen(system_param.SaveMainBeareParameter.beare_apn));
    			memcpy(system_param.SaveMainBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));	

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0010-->�������� APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                #endif
    			break;

			case 0x0011: //������������ͨ�Ų����û���
                 memset(system_param.SaveMainBeareParameter.beare_user,0,strlen(system_param.SaveMainBeareParameter.beare_user));
                 memcpy(system_param.SaveMainBeareParameter.beare_user,Base_Data+5,*(Base_Data+4));   
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0011-->������������ͨ�Ų����û���:%s.",system_param.SaveMainBeareParameter.beare_user);
                 #endif
    			 break;

			case 0x0012://������������ͨ�Ų�������
                memset(system_param.SaveMainBeareParameter.beare_password,0,strlen(system_param.SaveMainBeareParameter.beare_password));
                memcpy(system_param.SaveMainBeareParameter.beare_password,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0012-->������������ͨ�Ų�������:%s.",system_param.SaveMainBeareParameter.beare_password);
                #endif
    			break;

			case 0x0013://����������ַ,IP ������			
                memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,Base_Data+5,*(Base_Data+4));
                re_conect=EAT_TRUE;
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0013-->����������ַ,IP ������:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                #endif
    			break;

			case 0x0014://���ݷ����� APN
                memset(system_param.SaveBackupBeareParameter.beare_apn,0,strlen(system_param.SaveBackupBeareParameter.beare_apn));
                memcpy(system_param.SaveBackupBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));   
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0014-->���ݷ����� APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
                #endif
			    break;

			case 0x0015: //���ݷ���������ͨ�Ų����û���
                memset(system_param.SaveBackupBeareParameter.beare_user,0,strlen(system_param.SaveBackupBeareParameter.beare_user));
                memcpy(system_param.SaveBackupBeareParameter.beare_user,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0015-->���ݷ���������ͨ�Ų����û���:%s.",system_param.SaveBackupBeareParameter.beare_user);
                #endif
			    break;

			case 0x0016://���ݷ���������ͨ�Ų�������
                memset(system_param.SaveBackupBeareParameter.beare_password,0,strlen(system_param.SaveBackupBeareParameter.beare_password));
                memcpy(system_param.SaveBackupBeareParameter.beare_password,Base_Data+5,*(Base_Data+4));   
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0016-->���ݷ���������ͨ�Ų�������:%s.",system_param.SaveBackupBeareParameter.beare_password);
                #endif
    			break;
					
		    case 0x0017://���ݷ�������ַ,IP ������
                memset(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,Base_Data+5,*(Base_Data+4));
                re_conect=EAT_TRUE;
                
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0017-->���ݷ�������ַ,IP ������:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                #endif
			    break;
			    
			case 0x0018://������ TCP �˿�
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket);
                re_conect=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0018-->������ TCP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                #endif
    			break;

			case 0x0019://������ UDP �˿�
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].udp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0019-->������ UDP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                #endif
			    break;

			case 0x001A://��·����֤ IC ����֤�������� IP ��ַ������

                memset(system_param.ic_server_parameter.main_server_domin,0,sizeof(system_param.ic_server_parameter.main_server_domin));
                memcpy(system_param.ic_server_parameter.main_server_domin,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001A-->��·����֤ IC ����֤��������:%s.",system_param.ic_server_parameter.main_server_domin);
                #endif
		        break;

			case 0x001B://��·����֤ IC ����֤�������� TCP �˿�
    			Big_End_BytetoDword(Base_Data+5,&system_param.ic_server_parameter.tcp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001b-->��·����֤ IC ����֤�������� TCP �˿�:%d.",system_param.ic_server_parameter.tcp_server_socket);
                #endif
			    break;
						
			case 0x001C://��·����֤ IC ����֤�������� UDP �˿�
    			Big_End_BytetoDword(Base_Data+5,&system_param.ic_server_parameter.udp_server_socket);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001c-->��·����֤ IC ����֤�������� UDP �˿�:%d.",system_param.ic_server_parameter.udp_server_socket);
                #endif
		        break;

		    case 0x001D://��·����֤ IC ����֤���ݷ����� IP ��ַ������
                memset(system_param.ic_server_parameter.backup_server_domin,0,sizeof(system_param.ic_server_parameter.backup_server_domin));
                memcpy(system_param.ic_server_parameter.backup_server_domin,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x001d-->��·����֤ IC ����֤���ݷ����� IP ��ַ������:%s.",system_param.ic_server_parameter.backup_server_domin);
                #endif
		        break;
		    
 			case 0x0020://λ�û㱨����
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.policy);
                befor_mileage_base=system_param.mileage;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0020-->λ�û㱨����:%d.",system_param.location_reporting_param.policy);
                #endif
			    break;
			
		    case 0x0021://λ�û㱨����
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.scheme);
                acc_off_mileage_base=system_param.mileage;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0021-->λ�û㱨����:%d.",system_param.location_reporting_param.scheme);
                #endif
			    break;

			case 0x0022://��ʻԱδ��¼�㱨ʱ����
    			Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.not_signed_frze);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0022-->��ʻԱδ��¼�㱨ʱ����:%d.",system_param.location_reporting_param.not_signed_frze);
                #endif
                
			    break;
			
		    case 0x0027://����ʱ�㱨ʱ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0027-->����ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.sleeping_frze);
                #endif
			    break;
						
			case 0x0028://��������ʱ�㱨ʱ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.emergency_alarm_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->��������ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.emergency_alarm_frze);
                #endif
		        break; 
		    
		    case 0x0029://ȱʡʱ��㱨���
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->ȱʡʱ��㱨���:%d.",system_param.location_reporting_param.default_frze);
                #endif
		        break;  
		    
		    case 0x002A://��������ʱ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.night_sleep_frze);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x002A-->��������ʱ����:%d.",system_param.location_reporting_param.night_sleep_frze);
                #endif
		        break;

		    case 0x002B://��������ʱ��
                memset(system_param.location_reporting_param.time_night,0,sizeof(system_param.location_reporting_param.time_night));
                memcpy(system_param.location_reporting_param.time_night,Base_Data+5,*(Base_Data+4));
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x002B-->:����ʱ��:%0x:%0x--%0x:%0x",system_param.location_reporting_param.time_night[0],\
                system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3]);
                #endif
		        break;
		    
		    case 0x002C://ȱʡ����㱨���
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->ȱʡ����㱨���:%d.",system_param.location_reporting_param.default_distance);
                #endif
		        break;

		    case 0x002D://��ʻԱδ��¼�㱨������
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.not_signed_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->��ʻԱδ��¼�㱨������:%d.",system_param.location_reporting_param.not_signed_distance);
                #endif
		        break; 
		    
		    case 0x002E://����ʱ�㱨������
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->����ʱ�㱨������:%d.",system_param.location_reporting_param.sleeping_distance);
                #endif
		        break; 

		    case 0x002F://��������ʱ�㱨������
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.emergency_alarm_distance);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->��������ʱ�㱨������:%d.",system_param.location_reporting_param.emergency_alarm_distance);
                #endif
		        break; 

		    case 0x0030://�յ㲹���Ƕ�
		        Big_End_BytetoDword(Base_Data+5,&system_param.inflection_angle_passup);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0028-->�յ㲹���Ƕ�:%d.",system_param.inflection_angle_passup);
                #endif
		    break; 
		    
		    case 0x0031://����Χ���뾶���Ƿ�λ����ֵ��
		        Big_End_BytetoWord(Base_Data+5,&system_param.electronic_fence_radius);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0031-->����Χ���뾶���Ƿ�λ����ֵ��:%d.",system_param.electronic_fence_radius);
                #endif
		        break;

		    case 0x0040://���ƽ̨�绰����	
                memset(system_param.phone_number_param.monitor_platform,0,sizeof(system_param.phone_number_param.monitor_platform));
                memcpy(system_param.phone_number_param.monitor_platform,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0040-->���ƽ̨�绰����:%s.",system_param.phone_number_param.monitor_platform);
                #endif
			    break;
			
			case 0x0041://��λ�绰����
                memset(system_param.phone_number_param.terminal_restart,0,sizeof(system_param.phone_number_param.terminal_restart));
                memcpy(system_param.phone_number_param.terminal_restart,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0041-->��λ�绰����:%s.",system_param.phone_number_param.terminal_restart);
                #endif
			    break;

			case 0x0042://�ָ��������õ绰����
                memset(system_param.phone_number_param.terminal_format,0,sizeof(system_param.phone_number_param.terminal_format));
                memcpy(system_param.phone_number_param.terminal_format,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0042-->�ָ��������õ绰����:%s.",system_param.phone_number_param.terminal_format);
                #endif
			    break;

			case 0x0043://���ƽ̨ SMS �绰����
                memset(system_param.phone_number_param.monitor_platform_sms,0,sizeof(system_param.phone_number_param.monitor_platform_sms));
                memcpy(system_param.phone_number_param.monitor_platform_sms,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0043-->���ƽ̨ SMS �绰����:%s.",system_param.phone_number_param.monitor_platform_sms);
                #endif
			    break;

			case 0x0044://�����ն� SMS �ı���������
                memset(system_param.phone_number_param.terminal_sms_send,0,sizeof(system_param.phone_number_param.terminal_sms_send));
                memcpy(system_param.phone_number_param.terminal_sms_send,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0044-->�����ն� SMS �ı���������:%s.",system_param.phone_number_param.terminal_sms_send);
                #endif
			    break;

			case 0x0045://�ն˵绰�������ԣ�0���Զ�������1��ACC ON ʱ�Զ�������OFF ʱ�ֶ�����
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.answer_policy);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0045-->�ն˵绰��������:%d.",system_param.phone_number_param.answer_policy);
                #endif
			    break;

			case 0x0046://ÿ���ͨ��ʱ��
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.every_longest_call_time);

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0046-->ÿ���ͨ��ʱ��:%d.",system_param.phone_number_param.every_longest_call_time);
                #endif
			    break;

			case 0x0047://�����ͨ��ʱ��
		        Big_End_BytetoDword(Base_Data+5,&system_param.phone_number_param.mounth_longest_call_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0047-->�����ͨ��ʱ��:%d.",system_param.phone_number_param.mounth_longest_call_time);
                #endif
			    break;
			
			case 0x0048://�����绰����
                memset(system_param.phone_number_param.monitor_telephone,0,sizeof(system_param.phone_number_param.monitor_telephone));
                memcpy(system_param.phone_number_param.monitor_telephone,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0048-->�����绰����:%s.",system_param.phone_number_param.monitor_telephone);
                #endif
			    break;
			
			case 0x0049://���ƽ̨��Ȩ���ź���	
                memset(system_param.phone_number_param.supervision_platform,0,sizeof(system_param.phone_number_param.supervision_platform));
                memcpy(system_param.phone_number_param.supervision_platform,Base_Data+5,*(Base_Data+4));

                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0049-->���ƽ̨��Ȩ���ź���:%s.",system_param.phone_number_param.supervision_platform);
                #endif
			    break;

			case 0x0050://����������
		        Big_End_BytetoDword(Base_Data+5,&system_param.alarm_mask.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0050-->����������:%d.",system_param.alarm_mask.Alarm_Original);
                #endif
			    break;

			case 0x0051://���������ı� SMS ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.alarm_sms_swtich.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0051-->���������ı� SMS ����:%d.",system_param.alarm_sms_swtich.Alarm_Original);
                #endif
			    break;

			case 0x0052://�������㿪��
                Big_End_BytetoDword(Base_Data+5,&system_param.alarm_camer_switch.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0052-->�������㿪��:%d.",system_param.alarm_camer_switch.Alarm_Original);
                #endif
    			break;

			case 0x0053://��������洢��־
                Big_End_BytetoDword(Base_Data+5,&system_param.alarm_camer_upload.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0053-->��������洢��־:%d.",system_param.alarm_camer_upload.Alarm_Original);
                #endif
			    break;

			case 0x0054://�ؼ���־
                Big_End_BytetoDword(Base_Data+5,&system_param.linchpin_alarm.Alarm_Original);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0054-->�ؼ���־:%d.",system_param.linchpin_alarm.Alarm_Original);
                #endif
			    break;

			case 0x0055://����ٶ�
                Big_End_BytetoDword(Base_Data+5,&system_param.max_speed);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0055-->����ٶ�:%d.",system_param.max_speed);
                #endif
			    break;

			case 0x0056://���ٳ���ʱ��
                Big_End_BytetoDword(Base_Data+5,&system_param.speeding_duration);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0056-->���ٳ���ʱ��:%d.",system_param.speeding_duration);
                #endif
			    break;

			case 0x0057://������ʻʱ������
                Big_End_BytetoDword(Base_Data+5,&system_param.continuous_driving_threshold);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0057-->������ʻʱ������:%d.",system_param.continuous_driving_threshold);
                #endif
			    break;

			case 0x0058://�����ۼƼ�ʻʱ������
                Big_End_BytetoDword(Base_Data+5,&system_param.day_continuous_driving_threshold);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0058-->�����ۼƼ�ʻʱ������:%d.",system_param.day_continuous_driving_threshold);
                #endif
			    break;

			case 0x0059://��С��Ϣʱ��
                Big_End_BytetoDword(Base_Data+5,&system_param.min_rest_period);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0059-->��С��Ϣʱ��:%d.",system_param.min_rest_period);
                #endif
			    break;

			case 0x005A://�ͣ��ʱ��
                Big_End_BytetoDword(Base_Data+5,&system_param.max_parking_time);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005A-->�ͣ��ʱ��:%d.",system_param.max_parking_time);
                #endif
			    break;

			case 0x005B://���ٱ���Ԥ����ֵ
                Big_End_BytetoWord(Base_Data+5,&system_param.speed_warning_dif);
                sp_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005B-->���ٱ���Ԥ����ֵ:%d.",system_param.speed_warning_dif);
                #endif
			    break;

			case 0x005C://ƣ�ͼ�ʻԤ����ֵ
                Big_End_BytetoWord(Base_Data+5,&system_param.fatigue_dring_warning_dif);
                pl_seting=EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005C-->ƣ�ͼ�ʻԤ����ֵ:%d.",system_param.fatigue_dring_warning_dif);
                #endif
			    break;

			case 0x005D://��ײ�����������ã�
                Big_End_BytetoWord(Base_Data+5,&system_param.collision_warning_param);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005D-->��ײ������������:%d.",system_param.collision_warning_param);
                #endif
			    break;

			case 0x005E://�෭������������
                Big_End_BytetoWord(Base_Data+5,&system_param.rollover_param);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x005E-->�෭������������:%d.",system_param.rollover_param);
                #endif
			    break;
			
			case 0x0064://��ʱ���տ���
                Big_End_BytetoDword(Base_Data+5,&system_param.time_photo_param.photo_data);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0064-->��ʱ���տ���:0x%0x.",system_param.time_photo_param.photo_data);
                #endif
			    break;

			case 0x0065://�������տ���
                Big_End_BytetoDword(Base_Data+5,&system_param.distance_photo_param.photo_data);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0065-->�������տ���:0x%0x.",system_param.distance_photo_param.photo_data);
                #endif
			    break;

			case 0x0070://ͼ��/��Ƶ������1-10��1 ���
                Big_End_BytetoDword(Base_Data+5,&system_param.image_quality);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0070-->ͼ��/��Ƶ����:%d.",system_param.image_quality);
                #endif
			    break;

			case 0x0071://���ȣ�0-255
                Big_End_BytetoDword(Base_Data+5,&system_param.image_brightness);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0071-->����:%d.",system_param.image_brightness);
                #endif
			    break;

			case 0x0072://�Աȶȣ�0-127
                Big_End_BytetoDword(Base_Data+5,&system_param.image_contrast);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0072-->�Աȶ�:%d.",system_param.image_contrast);
                #endif
			    break;

			case 0x0073://���Ͷȣ�0-127
                Big_End_BytetoDword(Base_Data+5,&system_param.image_saturation);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0073-->���Ͷ�:%d.",system_param.image_saturation);
                #endif
			    break;

			case 0x0074://ɫ�ȣ�0-255
                Big_End_BytetoDword(Base_Data+5,&system_param.image_chroma);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0074-->ɫ��:%d.",system_param.image_chroma);
                #endif
			    break;

			case 0x0080://������̱������1/10km
                Big_End_BytetoDword(Base_Data+5,&system_param.mileage);
                seting_data_to_board(0x02);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->������̱����:%d.",system_param.mileage);
                #endif
			    break;

			case 0x0081://�������ڵ�ʡ�� ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.provincial_id);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0081-->�������ڵ�ʡ�� ID:%d.",system_param.terminl_registe.provincial_id);
                #endif
			    break;

			case 0x0082:// �������ڵ����� ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.county_id);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0082-->�������ڵ����� ID:%d.",system_param.terminl_registe.county_id);
                #endif
			    break;

			case 0x0083://������ͨ�����Ű䷢�Ļ���������
                memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
                memcpy(system_param.terminl_registe.plate_number,Base_Data+5,*(Base_Data+4));
                
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0083-->������ͨ�����Ű䷢�Ļ���������:%s.",system_param.terminl_registe.plate_number);
                #endif
			    break;

			case 0x0084://������ɫ
			    system_param.terminl_registe.plate_coror = *(Base_Data+5);
			    re_rgist =EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0084-->������ɫ:%d.",system_param.terminl_registe.plate_coror);
                #endif
			    break;
			
			case 0x0090://GNSS ��λģʽ
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
                eat_trace("seting_param id:0x0090-->��λģʽ:%d.",system_param.location_mode);
                #endif
			    break;

			case 0x0091://GNSS ������
			    system_param.baud_rate= *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0091-->GNSS ������:%d.",system_param.baud_rate);
                #endif
			    break;

			case 0x0092://GNSS ģ����ϸ��λ�������Ƶ��
			    system_param.location_out_frze = *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0092-->ģ����ϸ��λ�������Ƶ��:%d.",system_param.location_out_frze);
                #endif
			    break;

			case 0x0093://GNSS ģ����ϸ��λ���ݲɼ�Ƶ��
			    system_param.detail_location_get_frze = *(Base_Data+5);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0093-->ģ����ϸ��λ���ݲɼ�Ƶ��:%d.",system_param.detail_location_get_frze);
                #endif
			    break;
			
			case 0x0094://GNSS ģ����ϸ��λ�����ϴ���ʽ
			    system_param.detail_location_upload_mode= *(Base_Data+5);
			    /**detail���ݲ���**/
			    detial_data_befor_clean = EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0094-->ģ����ϸ��λ�����ϴ���ʽ:%d.",system_param.detail_location_upload_mode);
                #endif
			    break;

			case 0x0095://GNSS ģ����ϸ��λ�����ϴ�����
			    system_param.detail_location_upload_setting = *(Base_Data+5);
			    /**detail���ݲ���**/
			    detial_data_befor_clean = EAT_TRUE;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0095-->ģ����ϸ��λ�����ϴ�����:%d.",system_param.detail_location_upload_setting);
                #endif
			    break;

			case 0x0100://CAN ����ͨ�� 1 �ɼ�ʱ����
                Big_End_BytetoDword(Base_Data+5,&system_param.can_collect_param.can1_time_interval);
                if(socket==Main_Link.mode_Socket) system_param.can_collect_param.update_socket=0x01;
                if(socket==Second_Link.mode_Socket)system_param.can_collect_param.update_socket=0x02;
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0100-->CAN����ͨ��1�ɼ�ʱ����:%u.",system_param.can_collect_param.can1_time_interval);
                #endif
			    break;
			    
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0100-->CAN����ͨ��1�ɼ�ʱ����4:%u.",system_param.can_collect_param.can1_time_interval);
                #endif

			case 0x0101://CAN ����ͨ�� 1 �ϴ�ʱ����
			
            #if APP_TRACE_DBG 
            eat_trace("seting_param id:0x0100-->CAN����ͨ��1�ɼ�ʱ����4:%u.",system_param.can_collect_param.can1_time_interval);
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
                eat_trace("seting_param id:0x0101-->CAN����ͨ��1�ϴ�ʱ����:%d.",system_param.can_collect_param.can1_upload_interval);
                #endif
			    break;

			case 0x0102://CAN ����ͨ�� 2 �ɼ�ʱ����
                Big_End_BytetoDword(Base_Data+5,&system_param.can_collect_param.can2_time_interval);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->CAN ����ͨ�� 2 �ɼ�ʱ����:%d.",system_param.can_collect_param.can2_time_interval);
                #endif
			    break;

			case 0x0103://CAN����ͨ��2�ϴ�ʱ����
                Big_End_BytetoWord(Base_Data+5,&system_param.can_collect_param.can2_upload_interval);
                #if APP_TRACE_DBG 
                eat_trace("seting_param id:0x0080-->CAN����ͨ��2�ϴ�ʱ����:%d.",system_param.can_collect_param.can2_upload_interval);
                #endif
    			break;
			case 0xF006://IP����
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
	  Base_Data = Base_Data + *(Base_Data+4)+5;//��һ�ε��׵�ַ
	}
	system_para_save_at_once();//�������
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
        task_msg_send(EAT_USER_1,EAT_USER_0,StartBearerOpen);//������Ϣ��USER_2
	}
	else if(re_rgist ==EAT_TRUE)//��ע���һIP
	{
	    /*��������һIPȨ��*/
        memset(system_param.terminl_registe.first_authentication_id,0,sizeof(system_param.terminl_registe.first_authentication_id));//��¼��Ȩ��
        system_para_save_at_once();
		if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
		{
            /*���Ѿ���Ȩ�ɹ�����ע��*/
            System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
		}
		else
		{
		    if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
		    {
                /*��δ����Ȩ�ɹ���������ɹ���������(�����Ϣȷʵ���)*/
                
                Main_Link.link_staue=EAT_FALSE;
                Main_Link.get_ip_statue=EAT_FALSE;
                task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
		    }
		}
		
        if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
        {
            /*�������ڶ�IPȨ��*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//��¼��Ȩ��
            system_para_save_at_once();
    		if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
    		{
                /*���Ѿ���Ȩ�ɹ�����ע��*/
                System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
                task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
    		}
    		else
    		{
    		    if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
    		    {
                    /*��δ����Ȩ�ɹ���������ɹ���������(�����Ϣȱʧ���)*/
                    
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
  *  Description:  ��ѯ����
  *  Parameters : FileName ;�ļ�ָ��
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-16              
  * =====================================================================================
  */
 void tcp_request_param_serven(int16_t l_socket,uint8_t *Temp_data_buff,uint16_t Servn)
 {
     
     uint8_t *P;//λ��ָ��
     uint16_t Cnt = 0;
     uint16_t Cnt_Num = 3; //���ݸ���
     uint32_t Data_ID = 0;
     uint16_t msgs_sern = 0;
     uint8_t Send_Data[1056]={0};
	 uint16_t ip1_lock_state = 0;  //ip1����״̬
	 uint16_t ip2_lock_state = 0;  //ip2����״̬
     Big_End_WordtoByte(Servn,Send_Data);
     *(Send_Data+2) = *Temp_data_buff;//Ӧ�����
     P=Send_Data+3;
     for(Cnt=0;Cnt<*Temp_data_buff;Cnt++)
     {  
         Big_End_BytetoDword(Temp_data_buff+1+4*Cnt,&Data_ID);
         switch(Data_ID)
         {
             case 0x0001://�ն��������ͼ������λΪ�루s��
                  Big_End_DwordtoByte(0x0001,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.HeartBeatUpdataFre,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;//���ݳ���= ������+3(ID+����) + ���ݳ���
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0001-->�ն��������ͼ��:%d��s��",system_param.HeartBeatUpdataFre);
                  #endif
                  break;
             
             case 0x0002://TCP ��ϢӦ��ʱʱ�䣬��λΪ�루s��
                  Big_End_DwordtoByte(0x0002,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.waiting_time,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;//���ݳ���= ������+3(ID+����) + ���ݳ���
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0002-->TCP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveTcpRetransTimes.waiting_time);
                  #endif
                  break;
             
             case 0x0003://TCP ��Ϣ�ش�����
                  Big_End_DwordtoByte(0x0003,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.timeout_times,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0003-->TCP ��Ϣ�ش�����:%d.",system_param.SaveTcpRetransTimes.timeout_times);
                  #endif
                  break;
             
             case 0x0004://UDP ��ϢӦ��ʱʱ��
                  Big_End_DwordtoByte(0x0004,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.waiting_time,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0004-->UDP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveUdpRetransTimes.waiting_time);
                  #endif
                  break;
                         
             case 0x0005://UDP ��Ϣ�ش�����
                  Big_End_DwordtoByte(0x0005,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.timeout_times,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;

                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0005-->UDP ��Ϣ�ش�����:%d.",system_param.SaveUdpRetransTimes.timeout_times);
                  #endif
                  break;
             
             case 0x0006://SMS ��ϢӦ��ʱʱ��
                  Big_End_DwordtoByte(0x0006,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.waiting_time,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0006-->SMS ��ϢӦ��ʱʱ��%d��s��",system_param.SaveSmsRetransTimes.waiting_time);
                  #endif
                  break;
             
             case 0x0007://SMS ��Ϣ�ش�����
                  Big_End_DwordtoByte(0x0007,P);
                  *(P+4)=4;
                  Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.timeout_times,P+5);
                  P=P+5+4;//��ַƫ��
                  Cnt_Num=Cnt_Num+5+4;
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0007-->SMS ��Ϣ�ش�����:%d.",system_param.SaveSmsRetransTimes.timeout_times);
                  #endif
                 
                  break;
             
                         
             case 0x0010://�������� APN������ͨ�Ų��ŷ��ʵ�
                  Big_End_DwordtoByte(0x0010,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_apn);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);

                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0010-->�������� APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                  #endif
                  break;
             
             case 0x0011: //������������ͨ�Ų����û���
                  Big_End_DwordtoByte(0x0011,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_user);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_user,strlen(system_param.SaveMainBeareParameter.beare_user));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_user);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_user);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0011-->������������ͨ�Ų����û���:%s.",system_param.SaveMainBeareParameter.beare_user);
                  #endif
                  break;
             
             case 0x0012://������������ͨ�Ų�������
                  Big_End_DwordtoByte(0x0012,P);
                  *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_password);
                  memcpy(P+5,system_param.SaveMainBeareParameter.beare_password,strlen(system_param.SaveMainBeareParameter.beare_password));
                  P=P+5+strlen(system_param.SaveMainBeareParameter.beare_password);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_password);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0012-->������������ͨ�Ų�������:%s.",system_param.SaveMainBeareParameter.beare_password);
                  #endif
                  break;
             
             case 0x0013://����������ַ,IP ������            
                  Big_End_DwordtoByte(0x0013,P);
                  *(P+4)=strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                  P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0013-->����������ַ,IP ������:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                  #endif
                  break;
             
             case 0x0014://���ݷ����� APN
                  Big_End_DwordtoByte(0x0014,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0014-->���ݷ����� APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
                  #endif
                  break;
             
             case 0x0015: //���ݷ���������ͨ�Ų����û���
                  Big_End_DwordtoByte(0x0015,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_user);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_user,strlen(system_param.SaveBackupBeareParameter.beare_user));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_user);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_user);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0015-->���ݷ���������ͨ�Ų����û���:%s.",system_param.SaveBackupBeareParameter.beare_user);
                  #endif
                  break;
             
             case 0x0016://���ݷ���������ͨ�Ų�������
                  Big_End_DwordtoByte(0x0016,P);
                  *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_password);
                  memcpy(P+5,system_param.SaveBackupBeareParameter.beare_password,strlen(system_param.SaveBackupBeareParameter.beare_password));
                  P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_password);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_password);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0016-->���ݷ���������ͨ�Ų�������:%s.",system_param.SaveBackupBeareParameter.beare_password);
                  #endif
                  break;
                                 
             case 0x0017://���ݷ�������ַ,IP ������
                  Big_End_DwordtoByte(0x0017,P);
                  *(P+4)=strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  memcpy(P+5,system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
                  P=P+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  #if APP_TRACE_DBG 
                  eat_trace("ask_param id:0x0017-->���ݷ�������ַ,IP ������:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
                  #endif
                  break;
                             
             case 0x0018://������ TCP �˿�
                  Big_End_DwordtoByte(0x0018,P);
                  *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
                  P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0018-->������ TCP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                  #endif
                  break;
             
             case 0x0019://������ UDP �˿�
                  Big_End_DwordtoByte(0x0019,P);
                  *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket,P+5);
                  P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0019-->������ UDP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
                  #endif
                  break;
             
             case 0x001A://��·����֤ IC ����֤�������� IP ��ַ������
                  Big_End_DwordtoByte(0x001A,P);
                  *(P+4)=strlen(system_param.ic_server_parameter.main_server_domin);
                  memcpy(P+5,system_param.ic_server_parameter.main_server_domin,strlen(system_param.ic_server_parameter.main_server_domin));
                  P=P+5+strlen(system_param.ic_server_parameter.main_server_domin);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.main_server_domin);
                    
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001A-->��·����֤ IC ����֤��������:%s.",system_param.ic_server_parameter.main_server_domin);
                  #endif
                  break;
             
             case 0x001B://��·����֤ IC ����֤�������� TCP �˿�
                  Big_End_DwordtoByte(0x001B,P);
                  *(P+4)=sizeof(system_param.ic_server_parameter.tcp_server_socket);
                  Big_End_DwordtoByte(system_param.ic_server_parameter.tcp_server_socket,P+5);
                  P=P+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);
                    
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001b-->��·����֤ IC ����֤�������� TCP �˿�:%d.",system_param.ic_server_parameter.tcp_server_socket);
                  #endif
                  break;
                                     
             case 0x001C://��·����֤ IC ����֤�������� UDP �˿�
                  Big_End_DwordtoByte(0x001C,P);
                  *(P+4)=sizeof(system_param.ic_server_parameter.udp_server_socket);
                  Big_End_DwordtoByte(system_param.ic_server_parameter.udp_server_socket,P+5);
                  P=P+5+sizeof(system_param.ic_server_parameter.udp_server_socket);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.udp_server_socket);
                     
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001c-->��·����֤ IC ����֤�������� UDP �˿�:%d.",system_param.ic_server_parameter.udp_server_socket);
                  #endif
                  break;
             
             case 0x001D://��·����֤ IC ����֤���ݷ����� IP ��ַ������
                  Big_End_DwordtoByte(0x001D,P);
                  *(P+4)=strlen(system_param.ic_server_parameter.backup_server_domin);
                  memcpy(P+5,system_param.ic_server_parameter.backup_server_domin,strlen(system_param.ic_server_parameter.backup_server_domin));
                  P=P+5+strlen(system_param.ic_server_parameter.backup_server_domin);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.backup_server_domin);
                      
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x001d-->��·����֤ IC ����֤���ݷ����� IP ��ַ������:%s.",system_param.ic_server_parameter.backup_server_domin);
                  #endif
                  break;
                         
             case 0x0020://λ�û㱨����
                  Big_End_DwordtoByte(0x0020,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.policy);
                  Big_End_DwordtoByte(system_param.location_reporting_param.policy,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.policy);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.policy);
         
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0020-->λ�û㱨����:%d.",system_param.location_reporting_param.policy);
                  #endif
                  break;
                         
             case 0x0021://λ�û㱨����
                  Big_End_DwordtoByte(0x0021,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.scheme);
                  Big_End_DwordtoByte(system_param.location_reporting_param.scheme,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.scheme);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.scheme);
         
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0021-->λ�û㱨����:%d.",system_param.location_reporting_param.scheme);
                  #endif
                  break;
             
             case 0x0022://��ʻԱδ��¼�㱨ʱ����
                  Big_End_DwordtoByte(0x0022,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.not_signed_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.not_signed_frze);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0022-->��ʻԱδ��¼�㱨ʱ����:%d.",system_param.location_reporting_param.not_signed_frze);
                  #endif
                  break;
                         
             case 0x0027://����ʱ�㱨ʱ����
                  Big_End_DwordtoByte(0x0027,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.sleeping_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.sleeping_frze);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0027-->����ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.sleeping_frze);
                  #endif
                  break;
                                     
             case 0x0028://��������ʱ�㱨ʱ����
                  Big_End_DwordtoByte(0x0028,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->��������ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.emergency_alarm_frze);
                  #endif
                  break; 
                         
             case 0x0029://ȱʡʱ��㱨���
                  Big_End_DwordtoByte(0x0029,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.default_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.default_frze);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->ȱʡʱ��㱨���:%d.",system_param.location_reporting_param.default_frze);
                  #endif
                  break;  
                         
             case 0x002A://��������ʱ����
                  Big_End_DwordtoByte(0x002A,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.night_sleep_frze);
                  Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.night_sleep_frze);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.night_sleep_frze);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x002A-->��������ʱ����:%d.",system_param.location_reporting_param.night_sleep_frze);
                  #endif
                  break;
             
             case 0x002B://��������ʱ��
                  Big_End_DwordtoByte(0x002B,P);
                  *(P+4) = sizeof(system_param.location_reporting_param.time_night) ;
                  memcpy(P+5,system_param.location_reporting_param.time_night,sizeof(system_param.location_reporting_param.time_night));
                  P=P+5+sizeof(system_param.location_reporting_param.time_night);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.time_night);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x002B-->:����ʱ��:%0x:%0x:%0x--%0x:%0x:%0x:%0x.",system_param.location_reporting_param.time_night[0],\
                  system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3],\
                                                                          system_param.location_reporting_param.time_night[4],system_param.location_reporting_param.time_night[5]);
                  #endif
                  break;
                         
             case 0x002C://ȱʡ����㱨���
                  Big_End_DwordtoByte(0x002C,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.default_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.default_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.default_distance);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_distance);
  
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->ȱʡ����㱨���:%d.",system_param.location_reporting_param.default_distance);
                  #endif
                  break;
             
             case 0x002D://��ʻԱδ��¼�㱨������
                  Big_End_DwordtoByte(0x002D,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.not_signed_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.not_signed_distance);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->��ʻԱδ��¼�㱨������:%d.",system_param.location_reporting_param.not_signed_distance);
                  #endif
                  break; 
                                   
             case 0x002E://����ʱ�㱨������
                  Big_End_DwordtoByte(0x002E,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.sleeping_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.sleeping_distance);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->����ʱ�㱨������:%d.",system_param.location_reporting_param.sleeping_distance);
                  #endif
                  break; 
             
             case 0x002F://��������ʱ�㱨������
                  Big_End_DwordtoByte(0x002F,P);
                  *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_distance);
                  Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_distance,P+5);
                  P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->��������ʱ�㱨������:%d.",system_param.location_reporting_param.emergency_alarm_distance);
                  #endif
                  break; 
             
             case 0x0030://�յ㲹���Ƕ�
                  Big_End_DwordtoByte(0x0030,P);
                  *(P+4)=sizeof(system_param.inflection_angle_passup);
                  Big_End_DwordtoByte(system_param.inflection_angle_passup,P+5);
                  P=P+5+sizeof(system_param.inflection_angle_passup);//��ַƫ��
                  Cnt_Num=Cnt_Num+5+sizeof(system_param.inflection_angle_passup);
                  #if APP_TRACE_DBG 
                  eat_trace("seting_param id:0x0028-->�յ㲹���Ƕ�:%d.",system_param.inflection_angle_passup);
                  #endif
                  break; 
                         
             case 0x0031://����Χ���뾶���Ƿ�λ����ֵ��
                 Big_End_DwordtoByte(0x0031,P);
                 *(P+4)=sizeof(system_param.electronic_fence_radius);
                 Big_End_WordtoByte(system_param.electronic_fence_radius,P+5);
                 P=P+5+sizeof(system_param.electronic_fence_radius);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.electronic_fence_radius);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0031-->����Χ���뾶���Ƿ�λ����ֵ��:%d.",system_param.electronic_fence_radius);
                 #endif
                 break;
             
            case 0x0040://���ƽ̨�绰����  
                 Big_End_DwordtoByte(0x0040,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_platform);
                 memcpy(P+5,system_param.phone_number_param.monitor_platform,strlen(system_param.phone_number_param.monitor_platform));
                 P=P+5+strlen(system_param.phone_number_param.monitor_platform);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform);
         
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0040-->���ƽ̨�绰����:%s.",system_param.phone_number_param.monitor_platform);
                 #endif
                 break;
                         
            case 0x0041://��λ�绰����
                 Big_End_DwordtoByte(0x0041,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_restart);
                 memcpy(P+5,system_param.phone_number_param.terminal_restart,strlen(system_param.phone_number_param.terminal_restart));
                 P=P+5+strlen(system_param.phone_number_param.terminal_restart);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_restart);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0041-->��λ�绰����:%s.",system_param.phone_number_param.terminal_restart);
                 #endif
                 break;
             
            case 0x0042://�ָ��������õ绰����
                 Big_End_DwordtoByte(0x0042,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_format);
                 memcpy(P+5,system_param.phone_number_param.terminal_format,strlen(system_param.phone_number_param.terminal_format));
                 P=P+5+strlen(system_param.phone_number_param.terminal_format);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_format);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0042-->�ָ��������õ绰����:%s.",system_param.phone_number_param.terminal_format);
                 #endif
                 break;
             
            case 0x0043://���ƽ̨ SMS �绰����
                 Big_End_DwordtoByte(0x0043,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_platform_sms);
                 memcpy(P+5,system_param.phone_number_param.monitor_platform_sms,strlen(system_param.phone_number_param.monitor_platform_sms));
                 P=P+5+strlen(system_param.phone_number_param.monitor_platform_sms);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform_sms);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0043-->���ƽ̨ SMS �绰����:%s.",system_param.phone_number_param.monitor_platform_sms);
                 #endif
                 break;
             
            case 0x0044://�����ն� SMS �ı���������
                 Big_End_DwordtoByte(0x0044,P);
                 *(P+4)=strlen(system_param.phone_number_param.terminal_sms_send);
                 memcpy(P+5,system_param.phone_number_param.terminal_sms_send,strlen(system_param.phone_number_param.terminal_sms_send));
                 P=P+5+strlen(system_param.phone_number_param.terminal_sms_send);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_sms_send);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0044-->�����ն� SMS �ı���������:%s.",system_param.phone_number_param.terminal_sms_send);
                 #endif
                 break;
             
            case 0x0045://�ն˵绰�������ԣ�0���Զ�������1��ACC ON ʱ�Զ�������OFF ʱ�ֶ�����
                 Big_End_DwordtoByte(0x0045,P);
                 *(P+4)=sizeof(system_param.phone_number_param.answer_policy);
                 Big_End_DwordtoByte(system_param.phone_number_param.answer_policy,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.answer_policy);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.answer_policy);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0045-->�ն˵绰��������:%d.",system_param.phone_number_param.answer_policy);
                 #endif
                 break;
             
            case 0x0046://ÿ���ͨ��ʱ��
                 Big_End_DwordtoByte(0x0046,P);
                 *(P+4)=sizeof(system_param.phone_number_param.every_longest_call_time);
                 Big_End_DwordtoByte(system_param.phone_number_param.every_longest_call_time,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.every_longest_call_time);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.every_longest_call_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0046-->ÿ���ͨ��ʱ��:%d.",system_param.phone_number_param.every_longest_call_time);
                 #endif
                 break;
             
            case 0x0047://�����ͨ��ʱ��
                 Big_End_DwordtoByte(0x0047,P);
                 *(P+4)=sizeof(system_param.phone_number_param.mounth_longest_call_time);
                 Big_End_DwordtoByte(system_param.phone_number_param.mounth_longest_call_time,P+5);
                 P=P+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0047-->�����ͨ��ʱ��:%d.",system_param.phone_number_param.mounth_longest_call_time);
                 #endif
                 break;
                         
            case 0x0048://�����绰����
                 Big_End_DwordtoByte(0x0048,P);
                 *(P+4)=strlen(system_param.phone_number_param.monitor_telephone);
                 memcpy(P+5,system_param.phone_number_param.monitor_telephone,strlen(system_param.phone_number_param.monitor_telephone));
                 P=P+5+strlen(system_param.phone_number_param.monitor_telephone);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_telephone);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0048-->�����绰����:%s.",system_param.phone_number_param.monitor_telephone);
                 #endif
                 break;
                         
            case 0x0049://���ƽ̨��Ȩ���ź���  
                 Big_End_DwordtoByte(0x0049,P);
                 *(P+4)=strlen(system_param.phone_number_param.supervision_platform);
                 memcpy(P+5,system_param.phone_number_param.supervision_platform,strlen(system_param.phone_number_param.supervision_platform));
                 P=P+5+strlen(system_param.phone_number_param.supervision_platform);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.supervision_platform);
                      
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0049-->���ƽ̨��Ȩ���ź���:%s.",system_param.phone_number_param.supervision_platform);
                 #endif
                 break;
             
            case 0x0050://����������
                 Big_End_DwordtoByte(0x0050,P);
                 *(P+4)=sizeof(system_param.alarm_mask.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_mask.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_mask.Alarm_Original);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_mask.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0050-->����������:%d.",system_param.alarm_mask.Alarm_Original);
                 #endif
                 break;
             
            case 0x0051://���������ı� SMS ����
                 Big_End_DwordtoByte(0x0051,P);
                 *(P+4)=sizeof(system_param.alarm_sms_swtich.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_sms_swtich.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0051-->���������ı� SMS ����:%d.",system_param.alarm_sms_swtich.Alarm_Original);
                 #endif
                 break;
             
            case 0x0052://�������㿪��
                 Big_End_DwordtoByte(0x0052,P);
                 *(P+4)=sizeof(system_param.alarm_camer_switch.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_camer_switch.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0052-->�������㿪��:%d.",system_param.alarm_camer_switch.Alarm_Original);
                 #endif
                 break;
             
            case 0x0053://��������洢��־
                 Big_End_DwordtoByte(0x0053,P);
                 *(P+4)=sizeof(system_param.alarm_camer_upload.Alarm_Original);
                 Big_End_DwordtoByte(system_param.alarm_camer_upload.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0053-->��������洢��־:%d.",system_param.alarm_camer_upload.Alarm_Original);
                 #endif
                 break;
             
            case 0x0054://�ؼ���־
                 Big_End_DwordtoByte(0x0054,P);
                 *(P+4)=sizeof(system_param.linchpin_alarm.Alarm_Original);
                 Big_End_DwordtoByte(system_param.linchpin_alarm.Alarm_Original,P+5);
                 P=P+5+sizeof(system_param.linchpin_alarm.Alarm_Original);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.linchpin_alarm.Alarm_Original);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0054-->�ؼ���־:%d.",system_param.linchpin_alarm.Alarm_Original);
                 #endif
                 break;
             
            case 0x0055://����ٶ�
                 Big_End_DwordtoByte(0x0055,P);
                 *(P+4)=sizeof(system_param.max_speed);
                 Big_End_DwordtoByte(system_param.max_speed,P+5);
                 P=P+5+sizeof(system_param.max_speed);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.max_speed);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0055-->����ٶ�:%d.",system_param.max_speed);
                 #endif
                 break;
             
            case 0x0056://���ٳ���ʱ��
                 Big_End_DwordtoByte(0x0056,P);
                 *(P+4)=sizeof(system_param.speeding_duration);
                 Big_End_DwordtoByte(system_param.speeding_duration,P+5);
                 P=P+5+sizeof(system_param.speeding_duration);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.speeding_duration);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0056-->���ٳ���ʱ��:%d.",system_param.speeding_duration);
                 #endif
                 break;
             
            case 0x0057://������ʻʱ������
                 Big_End_DwordtoByte(0x0057,P);
                 *(P+4)=sizeof(system_param.continuous_driving_threshold);
                 Big_End_DwordtoByte(system_param.continuous_driving_threshold,P+5);
                 P=P+5+sizeof(system_param.continuous_driving_threshold);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.continuous_driving_threshold);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0057-->������ʻʱ������:%d.",system_param.continuous_driving_threshold);
                 #endif
                 break;
             
            case 0x0058://�����ۼƼ�ʻʱ������
                 Big_End_DwordtoByte(0x0058,P);
                 *(P+4)=sizeof(system_param.day_continuous_driving_threshold);
                 Big_End_DwordtoByte(system_param.day_continuous_driving_threshold,P+5);
                 P=P+5+sizeof(system_param.day_continuous_driving_threshold);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.day_continuous_driving_threshold);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0058-->�����ۼƼ�ʻʱ������:%d.",system_param.day_continuous_driving_threshold);
                 #endif
                 break;
             
            case 0x0059://��С��Ϣʱ��
                 Big_End_DwordtoByte(0x0059,P);
                 *(P+4)=sizeof(system_param.min_rest_period);
                 Big_End_DwordtoByte(system_param.min_rest_period,P+5);
                 P=P+5+sizeof(system_param.min_rest_period);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.min_rest_period);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0059-->��С��Ϣʱ��:%d.",system_param.min_rest_period);
                 #endif
                 break;
             
            case 0x005A://�ͣ��ʱ��
                 Big_End_DwordtoByte(0x005A,P);
                 *(P+4)=sizeof(system_param.max_parking_time);
                 Big_End_DwordtoByte(system_param.max_parking_time,P+5);
                 P=P+5+sizeof(system_param.max_parking_time);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.max_parking_time);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005A-->�ͣ��ʱ��:%d.",system_param.max_parking_time);
                 #endif
                 break;
             
            case 0x005B://���ٱ���Ԥ����ֵ
                 Big_End_DwordtoByte(0x005B,P);
                 *(P+4)=sizeof(system_param.speed_warning_dif);
                 Big_End_DwordtoByte(system_param.speed_warning_dif,P+5);
                 P=P+5+sizeof(system_param.speed_warning_dif);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.speed_warning_dif);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005B-->���ٱ���Ԥ����ֵ:%d.",system_param.speed_warning_dif);
                 #endif
                 break;
             
            case 0x005C://ƣ�ͼ�ʻԤ����ֵ
                 Big_End_DwordtoByte(0x005C,P);
                 *(P+4)=sizeof(system_param.fatigue_dring_warning_dif);
                 Big_End_DwordtoByte(system_param.fatigue_dring_warning_dif,P+5);
                 P=P+5+sizeof(system_param.fatigue_dring_warning_dif);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.fatigue_dring_warning_dif);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005C-->ƣ�ͼ�ʻԤ����ֵ:%d.",system_param.fatigue_dring_warning_dif);
                 #endif
                 break;
             
            case 0x005D://��ײ�����������ã�
                 Big_End_DwordtoByte(0x005D,P);
                 *(P+4)=sizeof(system_param.collision_warning_param);
                 Big_End_DwordtoByte(system_param.collision_warning_param,P+5);
                 P=P+5+sizeof(system_param.collision_warning_param);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.collision_warning_param);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005D-->��ײ������������:%d.",system_param.collision_warning_param);
                 #endif
                 break;
             
            case 0x005E://�෭������������
                 Big_End_DwordtoByte(0x005E,P);
                 *(P+4)=sizeof(system_param.rollover_param);
                 Big_End_DwordtoByte(system_param.rollover_param,P+5);
                 P=P+5+sizeof(system_param.rollover_param);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.rollover_param);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x005E-->�෭������������:%d.",system_param.rollover_param);
                 #endif
                 break;
                         
            case 0x0064://��ʱ���տ���
                 Big_End_DwordtoByte(0x0064,P);
                 *(P+4)=sizeof(system_param.time_photo_param.photo_data);
                 Big_End_DwordtoByte(system_param.time_photo_param.photo_data,P+5);
                 P=P+5+sizeof(system_param.time_photo_param.photo_data);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.time_photo_param.photo_data);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0064-->��ʱ���տ���:0x%0x.",system_param.time_photo_param.photo_data);
                 #endif
                 break;
             
            case 0x0065://�������տ���
                 Big_End_DwordtoByte(0x0065,P);
                 *(P+4)=sizeof(system_param.distance_photo_param.photo_data);
                 Big_End_DwordtoByte(system_param.distance_photo_param.photo_data,P+5);
                 P=P+5+sizeof(system_param.distance_photo_param.photo_data);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.distance_photo_param.photo_data);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0065-->�������տ���:0x%0x.",system_param.distance_photo_param.photo_data);
                 #endif
                 break;
             
            case 0x0070://ͼ��/��Ƶ������1-10��1 ���
                 Big_End_DwordtoByte(0x0070,P);
                 *(P+4)=sizeof(system_param.image_quality);
                 Big_End_DwordtoByte(system_param.image_quality,P+5);
                 P=P+5+sizeof(system_param.image_quality);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_quality);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0070-->ͼ��/��Ƶ����:%d.",system_param.image_quality);
                 #endif
                 break;
             
            case 0x0071://���ȣ�0-255
                 Big_End_DwordtoByte(0x0071,P);
                 *(P+4)=sizeof(system_param.image_brightness);
                 Big_End_DwordtoByte(system_param.image_brightness,P+5);
                 P=P+5+sizeof(system_param.image_brightness);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_brightness);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0071-->����:%d.",system_param.image_brightness);
                 #endif
                 break;
             
            case 0x0072://�Աȶȣ�0-127
                 Big_End_DwordtoByte(0x0072,P);
                 *(P+4)=sizeof(system_param.image_contrast);
                 Big_End_DwordtoByte(system_param.image_contrast,P+5);
                 P=P+5+sizeof(system_param.image_contrast);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_contrast);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0072-->�Աȶ�:%d.",system_param.image_contrast);
                 #endif
                 break;
             
            case 0x0073://���Ͷȣ�0-127
                 Big_End_DwordtoByte(0x0073,P);
                 *(P+4)=sizeof(system_param.image_saturation);
                 Big_End_DwordtoByte(system_param.image_saturation,P+5);
                 P=P+5+sizeof(system_param.image_saturation);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_saturation);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0073-->���Ͷ�:%d.",system_param.image_saturation);
                 #endif
                 break;
             
            case 0x0074://ɫ�ȣ�0-255
                 Big_End_DwordtoByte(0x0074,P);
                 *(P+4)=sizeof(system_param.image_chroma);
                 Big_End_DwordtoByte(system_param.image_chroma,P+5);
                 P=P+5+sizeof(system_param.image_chroma);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.image_chroma);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0074-->ɫ��:%d.",system_param.image_chroma);
                 #endif
                 break;
             
            case 0x0080://������̱������1/10km
                 Big_End_DwordtoByte(0x0080,P);
                 *(P+4)=sizeof(system_param.mileage);
                 Big_End_DwordtoByte(system_param.mileage,P+5);
                 P=P+5+sizeof(system_param.mileage);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.mileage);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->������̱����:%d.",system_param.mileage);
                 #endif
                 break;
             
            case 0x0081://�������ڵ�ʡ�� ID
                 Big_End_DwordtoByte(0x0081,P);
                 *(P+4)=sizeof(system_param.terminl_registe.provincial_id);
                 Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
                 P=P+5+sizeof(system_param.terminl_registe.provincial_id);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.provincial_id);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0081-->�������ڵ�ʡ�� ID:%d.",system_param.terminl_registe.provincial_id);
                 #endif
                 break;
             
            case 0x0082:// �������ڵ����� ID
                 Big_End_DwordtoByte(0x0082,P);
                 *(P+4)=sizeof(system_param.terminl_registe.county_id);
                 Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
                 P=P+5+sizeof(system_param.terminl_registe.county_id);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.county_id);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0082-->�������ڵ����� ID:%d.",system_param.terminl_registe.county_id);
                 #endif
                 break;
             
            case 0x0083://������ͨ�����Ű䷢�Ļ���������
                 Big_End_DwordtoByte(0x0083,P);
                 *(P+4)=strlen(system_param.terminl_registe.plate_number);
                 memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
                 P=P+5+strlen(system_param.terminl_registe.plate_number);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0083-->������ͨ�����Ű䷢�Ļ���������:%s.",system_param.terminl_registe.plate_number);
                 #endif
                 break;
             
            case 0x0084://������ɫ
                 Big_End_DwordtoByte(0x0084,P);
                 *(P+4) = sizeof(system_param.terminl_registe.plate_coror);
                 *(P+5) = system_param.terminl_registe.plate_coror;
                 P=P+5+sizeof(system_param.terminl_registe.plate_coror);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.plate_coror);
                 
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0084-->������ɫ:%d.",system_param.terminl_registe.plate_coror);
                 #endif
                 break;
                         
            case 0x0090://GNSS ��λģʽ
                 Big_End_DwordtoByte(0x0090,P);
                 *(P+4)=sizeof(system_param.location_mode);
                 *(P+5) = system_param.location_mode;
                 P=P+5+sizeof(system_param.location_mode);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.location_mode);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0090-->��λģʽ:%d.",system_param.location_mode);
                 #endif
                 break;
             
            case 0x0091://GNSS ������
                 Big_End_DwordtoByte(0x0091,P);
                 *(P+4)=sizeof(system_param.baud_rate);
                 *(P+5) = system_param.baud_rate;
                 P=P+5+sizeof(system_param.baud_rate);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.baud_rate);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0091-->GNSS ������:%d.",system_param.baud_rate);
                 #endif
                 break;
             
            case 0x0092://GNSS ģ����ϸ��λ�������Ƶ��
                 Big_End_DwordtoByte(0x0092,P);
                 *(P+4)=sizeof(system_param.location_out_frze);
                 *(P+5) = system_param.location_out_frze;
                 P=P+5+sizeof(system_param.location_out_frze);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.location_out_frze);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0092-->ģ����ϸ��λ�������Ƶ��:%d.",system_param.location_out_frze);
                 #endif
                 break;
             
            case 0x0093://GNSS ģ����ϸ��λ���ݲɼ�Ƶ��
                 Big_End_DwordtoByte(0x0093,P);
                 *(P+4)=sizeof(system_param.detail_location_get_frze);
                 Big_End_DwordtoByte(system_param.detail_location_get_frze,P+5);
                 P=P+5+sizeof(system_param.detail_location_get_frze);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_get_frze);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0093-->ģ����ϸ��λ���ݲɼ�Ƶ��:%d.",system_param.detail_location_get_frze);
                 #endif
                 break;
                         
            case 0x0094://GNSS ģ����ϸ��λ�����ϴ���ʽ
                 Big_End_DwordtoByte(0x0094,P);
                 *(P+4)=sizeof(system_param.detail_location_upload_mode);
                 *(P+5) = system_param.detail_location_upload_mode;
                 P=P+5+sizeof(system_param.detail_location_upload_mode);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_mode);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0094-->ģ����ϸ��λ���ݲɼ�Ƶ��:%d.",system_param.detail_location_upload_mode);
                 #endif
                 break;
             
            case 0x0095://GNSS ģ����ϸ��λ�����ϴ�����
                 Big_End_DwordtoByte(0x0095,P);
                 *(P+4)=sizeof(system_param.detail_location_upload_setting);
                 *(P+5) = system_param.detail_location_upload_setting;
                 P=P+5+sizeof(system_param.detail_location_upload_setting);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_setting);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0095-->ģ����ϸ��λ�����ϴ�����:%d.",system_param.detail_location_upload_setting);
                 #endif
                 break;
             
            case 0x0100://CAN ����ͨ�� 1 �ɼ�ʱ����
                 Big_End_DwordtoByte(0x0100,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can1_time_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can1_time_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can1_time_interval);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_time_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0100-->CAN����ͨ��1�ɼ�ʱ����:%d.",system_param.can_collect_param.can1_time_interval);
                 #endif
                 break;
             
            case 0x0101://CAN ����ͨ�� 1 �ϴ�ʱ����
                 Big_End_DwordtoByte(0x0101,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can1_upload_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can1_upload_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can1_upload_interval);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_upload_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0101-->CAN����ͨ��1�ϴ�ʱ����:%d.",system_param.can_collect_param.can1_upload_interval);
                 #endif
                 break;
             
            case 0x0102://CAN ����ͨ�� 2 �ɼ�ʱ����
                 Big_End_DwordtoByte(0x0102,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can2_time_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can2_time_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can2_time_interval);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_time_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->CAN ����ͨ�� 2 �ɼ�ʱ����:%d.",system_param.can_collect_param.can2_time_interval);
                 #endif
                 break;
             
            case 0x0103://CAN����ͨ��2�ϴ�ʱ����
                 Big_End_DwordtoByte(0x0103,P);
                 *(P+4)=sizeof(system_param.can_collect_param.can2_upload_interval);
                 Big_End_DwordtoByte(system_param.can_collect_param.can2_upload_interval,P+5);
                 P=P+5+sizeof(system_param.can_collect_param.can2_upload_interval);//��ַƫ��
                 Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_upload_interval);
                 #if APP_TRACE_DBG 
                 eat_trace("seting_param id:0x0080-->CAN����ͨ��2�ϴ�ʱ����:%d.",system_param.can_collect_param.can2_upload_interval);
                 #endif
                 break;
			case 0xF006://IP����
				 ip1_lock_state = system_param.param_save.statue.ip1_lock;  //ip1����״̬
				 ip2_lock_state = system_param.param_save.statue.ip2_lock;  //ip2����״̬
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
              P=P+5+8;//��ַƫ��
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
  *  Description:  ��ѯȫ������
  *  Parameters : FileName ;�ļ�ָ��
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-16              
  * =====================================================================================
  */
 void tcp_all_data_request(int16_t l_socket,u16 Servn)
 {
     uint8_t *P;//λ��ָ��
     uint16_t Cnt=0;
     uint16_t Cnt_Num = 3; //���ݸ���
     uint16_t msgs_sern = 0;
     uint8_t Send_Data[1056]={0};
     Big_End_WordtoByte(Servn,Send_Data);
     P=Send_Data+3;

     //case 0x0001://�ն��������ͼ������λΪ�루s��
     Big_End_DwordtoByte(0x0001,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.HeartBeatUpdataFre,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;//���ݳ���= ������+3(ID+����) + ���ݳ���
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0001-->�ն��������ͼ��:%d��s��",system_param.HeartBeatUpdataFre);
     #endif
                  
     //case 0x0002://TCP ��ϢӦ��ʱʱ�䣬��λΪ�루s��
     Big_End_DwordtoByte(0x0002,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.waiting_time,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;//���ݳ���= ������+3(ID+����) + ���ݳ���
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0002-->TCP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveTcpRetransTimes.waiting_time);
     #endif
                  
     //case 0x0003://TCP ��Ϣ�ش�����
     Big_End_DwordtoByte(0x0003,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveTcpRetransTimes.timeout_times,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0003-->TCP ��Ϣ�ش�����:%d.",system_param.SaveTcpRetransTimes.timeout_times);
     #endif
                  
     //case 0x0004://UDP ��ϢӦ��ʱʱ��
     Big_End_DwordtoByte(0x0004,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.waiting_time,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0004-->UDP ��ϢӦ��ʱʱ��:%d��s��",system_param.SaveUdpRetransTimes.waiting_time);
     #endif
                              
     //case 0x0005://UDP ��Ϣ�ش�����
     Big_End_DwordtoByte(0x0005,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveUdpRetransTimes.timeout_times,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0005-->UDP ��Ϣ�ش�����:%d.",system_param.SaveUdpRetransTimes.timeout_times);
     #endif
                  
     //case 0x0006://SMS ��ϢӦ��ʱʱ��
     Big_End_DwordtoByte(0x0006,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.waiting_time,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0006-->SMS ��ϢӦ��ʱʱ��%d��s��",system_param.SaveSmsRetransTimes.waiting_time);
     #endif
                  
     //case 0x0007://SMS ��Ϣ�ش�����
     Big_End_DwordtoByte(0x0007,P);
     *(P+4)=4;
     Big_End_DwordtoByte(system_param.SaveSmsRetransTimes.timeout_times,P+5);
     P=P+5+4;//��ַƫ��
     Cnt_Num=Cnt_Num+5+4;
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0007-->SMS ��Ϣ�ش�����:%d.",system_param.SaveSmsRetransTimes.timeout_times);
     #endif
                              
     //case 0x0010://�������� APN������ͨ�Ų��ŷ��ʵ�
     Big_End_DwordtoByte(0x0010,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_apn);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0010-->�������� APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
     #endif
                  
     //case 0x0011: //������������ͨ�Ų����û���
     Big_End_DwordtoByte(0x0011,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_user);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_user,strlen(system_param.SaveMainBeareParameter.beare_user));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_user);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_user);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0011-->������������ͨ�Ų����û���:%s.",system_param.SaveMainBeareParameter.beare_user);
     #endif
                 
     //case 0x0012://������������ͨ�Ų�������
     Big_End_DwordtoByte(0x0012,P);
     *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_password);
     memcpy(P+5,system_param.SaveMainBeareParameter.beare_password,strlen(system_param.SaveMainBeareParameter.beare_password));
     P=P+5+strlen(system_param.SaveMainBeareParameter.beare_password);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_password);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0012-->������������ͨ�Ų�������:%s.",system_param.SaveMainBeareParameter.beare_password);
     #endif
                  
     //case 0x0013://����������ַ,IP ������            
     Big_End_DwordtoByte(0x0013,P);
     *(P+4)=strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
     P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0013-->����������ַ,IP ������:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
     #endif
                  
     //case 0x0014://���ݷ����� APN
     Big_End_DwordtoByte(0x0014,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0014-->���ݷ����� APN:%s.",system_param.SaveBackupBeareParameter.beare_apn);
     #endif
                  
     //case 0x0015: //���ݷ���������ͨ�Ų����û���
     Big_End_DwordtoByte(0x0015,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_user);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_user,strlen(system_param.SaveBackupBeareParameter.beare_user));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_user);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_user);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0015-->���ݷ���������ͨ�Ų����û���:%s.",system_param.SaveBackupBeareParameter.beare_user);
     #endif
     
     //case 0x0016://���ݷ���������ͨ�Ų�������
     Big_End_DwordtoByte(0x0016,P);
     *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_password);
     memcpy(P+5,system_param.SaveBackupBeareParameter.beare_password,strlen(system_param.SaveBackupBeareParameter.beare_password));
     P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_password);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_password);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0016-->���ݷ���������ͨ�Ų�������:%s.",system_param.SaveBackupBeareParameter.beare_password);
     #endif
     
     //case 0x0017://���ݷ�������ַ,IP ������
     Big_End_DwordtoByte(0x0017,P);
     *(P+4)=strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     memcpy(P+5,system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
     P=P+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     #if APP_TRACE_DBG 
     eat_trace("ask_param id:0x0017-->���ݷ�������ַ,IP ������:%s.",system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin);
     #endif
                                  
     //case 0x0018://������ TCP �˿�
     Big_End_DwordtoByte(0x0018,P);
     *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
     P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0018-->������ TCP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
     #endif
                  
     //case 0x0019://������ UDP �˿�
     Big_End_DwordtoByte(0x0019,P);
     *(P+4)=sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket,P+5);
     P=P+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0019-->������ UDP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
     #endif
                  
     //case 0x001A://��·����֤ IC ����֤�������� IP ��ַ������
     Big_End_DwordtoByte(0x001A,P);
     *(P+4)=strlen(system_param.ic_server_parameter.main_server_domin);
     memcpy(P+5,system_param.ic_server_parameter.main_server_domin,strlen(system_param.ic_server_parameter.main_server_domin));
     P=P+5+strlen(system_param.ic_server_parameter.main_server_domin);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.main_server_domin);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001A-->��·����֤ IC ����֤��������:%s.",system_param.ic_server_parameter.main_server_domin);
     #endif
                  
     //case 0x001B://��·����֤ IC ����֤�������� TCP �˿�
     Big_End_DwordtoByte(0x001B,P);
     *(P+4)=sizeof(system_param.ic_server_parameter.tcp_server_socket);
     Big_End_DwordtoByte(system_param.ic_server_parameter.tcp_server_socket,P+5);
     P=P+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.tcp_server_socket);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001b-->��·����֤ IC ����֤�������� TCP �˿�:%d.",system_param.ic_server_parameter.tcp_server_socket);
     #endif
                                          
     //case 0x001C://��·����֤ IC ����֤�������� UDP �˿�
     Big_End_DwordtoByte(0x001C,P);
     *(P+4)=sizeof(system_param.ic_server_parameter.udp_server_socket);
     Big_End_DwordtoByte(system_param.ic_server_parameter.udp_server_socket,P+5);
     P=P+5+sizeof(system_param.ic_server_parameter.udp_server_socket);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.ic_server_parameter.udp_server_socket);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001c-->��·����֤ IC ����֤�������� UDP �˿�:%d.",system_param.ic_server_parameter.udp_server_socket);
     #endif
                  
     //case 0x001D://��·����֤ IC ����֤���ݷ����� IP ��ַ������
     Big_End_DwordtoByte(0x001D,P);
     *(P+4)=strlen(system_param.ic_server_parameter.backup_server_domin);
     memcpy(P+5,system_param.ic_server_parameter.backup_server_domin,strlen(system_param.ic_server_parameter.backup_server_domin));
     P=P+5+strlen(system_param.ic_server_parameter.backup_server_domin);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.ic_server_parameter.backup_server_domin);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x001d-->��·����֤ IC ����֤���ݷ����� IP ��ַ������:%s.",system_param.ic_server_parameter.backup_server_domin);
     #endif
                              
     //case 0x0020://λ�û㱨����
     Big_End_DwordtoByte(0x0020,P);
     *(P+4)=sizeof(system_param.location_reporting_param.policy);
     Big_End_DwordtoByte(system_param.location_reporting_param.policy,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.policy);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.policy);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0020-->λ�û㱨����:%d.",system_param.location_reporting_param.policy);
     #endif
                              
     //case 0x0021://λ�û㱨����
     Big_End_DwordtoByte(0x0021,P);
     *(P+4)=sizeof(system_param.location_reporting_param.scheme);
     Big_End_DwordtoByte(system_param.location_reporting_param.scheme,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.scheme);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.scheme);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0021-->λ�û㱨����:%d.",system_param.location_reporting_param.scheme);
     #endif
                  
     //case 0x0022://��ʻԱδ��¼�㱨ʱ����
     Big_End_DwordtoByte(0x0022,P);
     *(P+4)=sizeof(system_param.location_reporting_param.not_signed_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.not_signed_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0022-->��ʻԱδ��¼�㱨ʱ����:%d.",system_param.location_reporting_param.not_signed_frze);
     #endif
                              
     //case 0x0027://����ʱ�㱨ʱ����
     Big_End_DwordtoByte(0x0027,P);
     *(P+4)=sizeof(system_param.location_reporting_param.sleeping_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.sleeping_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0027-->����ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.sleeping_frze);
     #endif
                                          
     //case 0x0028://��������ʱ�㱨ʱ����
     Big_End_DwordtoByte(0x0028,P);
     *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->��������ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.emergency_alarm_frze);
     #endif
                              
     //case 0x0029://ȱʡʱ��㱨���
     Big_End_DwordtoByte(0x0029,P);
     *(P+4)=sizeof(system_param.location_reporting_param.default_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.default_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->ȱʡʱ��㱨���:%d.",system_param.location_reporting_param.default_frze);
     #endif
                              
     //case 0x002A://��������ʱ����
     Big_End_DwordtoByte(0x002A,P);
     *(P+4)=sizeof(system_param.location_reporting_param.night_sleep_frze);
     Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.night_sleep_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.night_sleep_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x002A-->��������ʱ����:%d.",system_param.location_reporting_param.night_sleep_frze);
     #endif
                  
     //case 0x002B://��������ʱ��
     Big_End_DwordtoByte(0x002B,P);
     *(P+4) = sizeof(system_param.location_reporting_param.time_night) ;
     memcpy(P+5,system_param.location_reporting_param.time_night,sizeof(system_param.location_reporting_param.time_night));
     P=P+5+sizeof(system_param.location_reporting_param.time_night);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.time_night);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x002B-->:����ʱ��:%0x:%0x:%0x--%0x:%0x:%0x:%0x.",system_param.location_reporting_param.time_night[0],\
     system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3],\
                                                          system_param.location_reporting_param.time_night[4],system_param.location_reporting_param.time_night[5]);
     #endif
                              
     //case 0x002C://ȱʡ����㱨���
     Big_End_DwordtoByte(0x002C,P);
     *(P+4)=sizeof(system_param.location_reporting_param.default_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.default_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.default_distance);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.default_distance);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->ȱʡ����㱨���:%d.",system_param.location_reporting_param.default_distance);
     #endif
                  
     //case 0x002D://��ʻԱδ��¼�㱨������
     Big_End_DwordtoByte(0x002D,P);
     *(P+4)=sizeof(system_param.location_reporting_param.not_signed_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.not_signed_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.not_signed_distance);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.not_signed_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->��ʻԱδ��¼�㱨������:%d.",system_param.location_reporting_param.not_signed_distance);
     #endif
                              
     //case 0x002E://����ʱ�㱨������
     Big_End_DwordtoByte(0x002E,P);
     *(P+4)=sizeof(system_param.location_reporting_param.sleeping_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.sleeping_distance);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.sleeping_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->����ʱ�㱨������:%d.",system_param.location_reporting_param.sleeping_distance);
     #endif
                  
     //case 0x002F://��������ʱ�㱨������
     Big_End_DwordtoByte(0x002F,P);
     *(P+4)=sizeof(system_param.location_reporting_param.emergency_alarm_distance);
     Big_End_DwordtoByte(system_param.location_reporting_param.emergency_alarm_distance,P+5);
     P=P+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_reporting_param.emergency_alarm_distance);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->��������ʱ�㱨������:%d.",system_param.location_reporting_param.emergency_alarm_distance);
     #endif
                  
     //case 0x0030://�յ㲹���Ƕ�
     Big_End_DwordtoByte(0x0030,P);
     *(P+4)=sizeof(system_param.inflection_angle_passup);
     Big_End_DwordtoByte(system_param.inflection_angle_passup,P+5);
     P=P+5+sizeof(system_param.inflection_angle_passup);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.inflection_angle_passup);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0028-->�յ㲹���Ƕ�:%d.",system_param.inflection_angle_passup);
     #endif
                              
     //case 0x0031://����Χ���뾶���Ƿ�λ����ֵ��
     Big_End_DwordtoByte(0x0031,P);
     *(P+4)=sizeof(system_param.electronic_fence_radius);
     Big_End_WordtoByte(system_param.electronic_fence_radius,P+5);
     P=P+5+sizeof(system_param.electronic_fence_radius);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.electronic_fence_radius);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0031-->����Χ���뾶���Ƿ�λ����ֵ��:%d.",system_param.electronic_fence_radius);
     #endif
                  
     //case 0x0040://���ƽ̨�绰����  
     Big_End_DwordtoByte(0x0040,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_platform);
     memcpy(P+5,system_param.phone_number_param.monitor_platform,strlen(system_param.phone_number_param.monitor_platform));
     P=P+5+strlen(system_param.phone_number_param.monitor_platform);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform);
     
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0040-->���ƽ̨�绰����:%s.",system_param.phone_number_param.monitor_platform);
     #endif
                              
     //case 0x0041://��λ�绰����
     Big_End_DwordtoByte(0x0041,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_restart);
     memcpy(P+5,system_param.phone_number_param.terminal_restart,strlen(system_param.phone_number_param.terminal_restart));
     P=P+5+strlen(system_param.phone_number_param.terminal_restart);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_restart);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0041-->��λ�绰����:%s.",system_param.phone_number_param.terminal_restart);
     #endif
                  
     //case 0x0042://�ָ��������õ绰����
     Big_End_DwordtoByte(0x0042,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_format);
     memcpy(P+5,system_param.phone_number_param.terminal_format,strlen(system_param.phone_number_param.terminal_format));
     P=P+5+strlen(system_param.phone_number_param.terminal_format);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_format);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0042-->�ָ��������õ绰����:%s.",system_param.phone_number_param.terminal_format);
     #endif
                  
     //case 0x0043://���ƽ̨ SMS �绰����
     Big_End_DwordtoByte(0x0043,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_platform_sms);
     memcpy(P+5,system_param.phone_number_param.monitor_platform_sms,strlen(system_param.phone_number_param.monitor_platform_sms));
     P=P+5+strlen(system_param.phone_number_param.monitor_platform_sms);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_platform_sms);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0043-->���ƽ̨ SMS �绰����:%s.",system_param.phone_number_param.monitor_platform_sms);
     #endif
                  
     //case 0x0044://�����ն� SMS �ı���������
     Big_End_DwordtoByte(0x0044,P);
     *(P+4)=strlen(system_param.phone_number_param.terminal_sms_send);
     memcpy(P+5,system_param.phone_number_param.terminal_sms_send,strlen(system_param.phone_number_param.terminal_sms_send));
     P=P+5+strlen(system_param.phone_number_param.terminal_sms_send);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.terminal_sms_send);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0044-->�����ն� SMS �ı���������:%s.",system_param.phone_number_param.terminal_sms_send);
     #endif
                  
     //case 0x0045://�ն˵绰�������ԣ�0���Զ�������1��ACC ON ʱ�Զ�������OFF ʱ�ֶ�����
     Big_End_DwordtoByte(0x0045,P);
     *(P+4)=sizeof(system_param.phone_number_param.answer_policy);
     Big_End_DwordtoByte(system_param.phone_number_param.answer_policy,P+5);
     P=P+5+sizeof(system_param.phone_number_param.answer_policy);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.answer_policy);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0045-->�ն˵绰��������:%d.",system_param.phone_number_param.answer_policy);
     #endif
                  
     //case 0x0046://ÿ���ͨ��ʱ��
     Big_End_DwordtoByte(0x0046,P);
     *(P+4)=sizeof(system_param.phone_number_param.every_longest_call_time);
     Big_End_DwordtoByte(system_param.phone_number_param.every_longest_call_time,P+5);
     P=P+5+sizeof(system_param.phone_number_param.every_longest_call_time);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.every_longest_call_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0046-->ÿ���ͨ��ʱ��:%d.",system_param.phone_number_param.every_longest_call_time);
     #endif
                  
     //case 0x0047://�����ͨ��ʱ��
     Big_End_DwordtoByte(0x0047,P);
     *(P+4)=sizeof(system_param.phone_number_param.mounth_longest_call_time);
     Big_End_DwordtoByte(system_param.phone_number_param.mounth_longest_call_time,P+5);
     P=P+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.phone_number_param.mounth_longest_call_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0047-->�����ͨ��ʱ��:%d.",system_param.phone_number_param.mounth_longest_call_time);
     #endif
                              
     //case 0x0048://�����绰����
     Big_End_DwordtoByte(0x0048,P);
     *(P+4)=strlen(system_param.phone_number_param.monitor_telephone);
     memcpy(P+5,system_param.phone_number_param.monitor_telephone,strlen(system_param.phone_number_param.monitor_telephone));
     P=P+5+strlen(system_param.phone_number_param.monitor_telephone);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.monitor_telephone);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0048-->�����绰����:%s.",system_param.phone_number_param.monitor_telephone);
     #endif
                              
     //case 0x0049://���ƽ̨��Ȩ���ź���  
     Big_End_DwordtoByte(0x0049,P);
     *(P+4)=strlen(system_param.phone_number_param.supervision_platform);
     memcpy(P+5,system_param.phone_number_param.supervision_platform,strlen(system_param.phone_number_param.supervision_platform));
     P=P+5+strlen(system_param.phone_number_param.supervision_platform);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.supervision_platform);
                  
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0049-->���ƽ̨��Ȩ���ź���:%s.",system_param.phone_number_param.supervision_platform);
     #endif
                  
     //case 0x0050://����������
     Big_End_DwordtoByte(0x0050,P);
     *(P+4)=sizeof(system_param.alarm_mask.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_mask.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_mask.Alarm_Original);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_mask.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0050-->����������:%d.",system_param.alarm_mask.Alarm_Original);
     #endif
                  
     //case 0x0051://���������ı� SMS ����
     Big_End_DwordtoByte(0x0051,P);
     *(P+4)=sizeof(system_param.alarm_sms_swtich.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_sms_swtich.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_sms_swtich.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0051-->���������ı� SMS ����:%d.",system_param.alarm_sms_swtich.Alarm_Original);
     #endif
                  
     //case 0x0052://�������㿪��
     Big_End_DwordtoByte(0x0052,P);
     *(P+4)=sizeof(system_param.alarm_camer_switch.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_camer_switch.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_switch.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0052-->�������㿪��:%d.",system_param.alarm_camer_switch.Alarm_Original);
     #endif
                  
     //case 0x0053://��������洢��־
     Big_End_DwordtoByte(0x0053,P);
     *(P+4)=sizeof(system_param.alarm_camer_upload.Alarm_Original);
     Big_End_DwordtoByte(system_param.alarm_camer_upload.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.alarm_camer_upload.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0053-->��������洢��־:%d.",system_param.alarm_camer_upload.Alarm_Original);
     #endif
                  
     //case 0x0054://�ؼ���־
     Big_End_DwordtoByte(0x0054,P);
     *(P+4)=sizeof(system_param.linchpin_alarm.Alarm_Original);
     Big_End_DwordtoByte(system_param.linchpin_alarm.Alarm_Original,P+5);
     P=P+5+sizeof(system_param.linchpin_alarm.Alarm_Original);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.linchpin_alarm.Alarm_Original);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0054-->�ؼ���־:%d.",system_param.linchpin_alarm.Alarm_Original);
     #endif
                  
     //case 0x0055://����ٶ�
     Big_End_DwordtoByte(0x0055,P);
     *(P+4)=sizeof(system_param.max_speed);
     Big_End_DwordtoByte(system_param.max_speed,P+5);
     P=P+5+sizeof(system_param.max_speed);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.max_speed);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0055-->����ٶ�:%d.",system_param.max_speed);
     #endif
                  
     //case 0x0056://���ٳ���ʱ��
     Big_End_DwordtoByte(0x0056,P);
     *(P+4)=sizeof(system_param.speeding_duration);
     Big_End_DwordtoByte(system_param.speeding_duration,P+5);
     P=P+5+sizeof(system_param.speeding_duration);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.speeding_duration);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0056-->���ٳ���ʱ��:%d.",system_param.speeding_duration);
     #endif
                  
     //case 0x0057://������ʻʱ������
     Big_End_DwordtoByte(0x0057,P);
     *(P+4)=sizeof(system_param.continuous_driving_threshold);
     Big_End_DwordtoByte(system_param.continuous_driving_threshold,P+5);
     P=P+5+sizeof(system_param.continuous_driving_threshold);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.continuous_driving_threshold);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0057-->������ʻʱ������:%d.",system_param.continuous_driving_threshold);
     #endif
                  
     //case 0x0058://�����ۼƼ�ʻʱ������
     Big_End_DwordtoByte(0x0058,P);
     *(P+4)=sizeof(system_param.day_continuous_driving_threshold);
     Big_End_DwordtoByte(system_param.day_continuous_driving_threshold,P+5);
     P=P+5+sizeof(system_param.day_continuous_driving_threshold);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.day_continuous_driving_threshold);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0058-->�����ۼƼ�ʻʱ������:%d.",system_param.day_continuous_driving_threshold);
     #endif
                  
     //case 0x0059://��С��Ϣʱ��
     Big_End_DwordtoByte(0x0059,P);
     *(P+4)=sizeof(system_param.min_rest_period);
     Big_End_DwordtoByte(system_param.min_rest_period,P+5);
     P=P+5+sizeof(system_param.min_rest_period);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.min_rest_period);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0059-->��С��Ϣʱ��:%d.",system_param.min_rest_period);
     #endif
                  
     //case 0x005A://�ͣ��ʱ��
     Big_End_DwordtoByte(0x005A,P);
     *(P+4)=sizeof(system_param.max_parking_time);
     Big_End_DwordtoByte(system_param.max_parking_time,P+5);
     P=P+5+sizeof(system_param.max_parking_time);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.max_parking_time);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005A-->�ͣ��ʱ��:%d.",system_param.max_parking_time);
     #endif
                  
     //case 0x005B://���ٱ���Ԥ����ֵ
     Big_End_DwordtoByte(0x005B,P);
     *(P+4)=sizeof(system_param.speed_warning_dif);
     Big_End_DwordtoByte(system_param.speed_warning_dif,P+5);
     P=P+5+sizeof(system_param.speed_warning_dif);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.speed_warning_dif);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005B-->���ٱ���Ԥ����ֵ:%d.",system_param.speed_warning_dif);
     #endif
                  
     //case 0x005C://ƣ�ͼ�ʻԤ����ֵ
     Big_End_DwordtoByte(0x005C,P);
     *(P+4)=sizeof(system_param.fatigue_dring_warning_dif);
     Big_End_DwordtoByte(system_param.fatigue_dring_warning_dif,P+5);
     P=P+5+sizeof(system_param.fatigue_dring_warning_dif);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.fatigue_dring_warning_dif);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005C-->ƣ�ͼ�ʻԤ����ֵ:%d.",system_param.fatigue_dring_warning_dif);
     #endif
                  
     //case 0x005D://��ײ�����������ã�
     Big_End_DwordtoByte(0x005D,P);
     *(P+4)=sizeof(system_param.collision_warning_param);
     Big_End_DwordtoByte(system_param.collision_warning_param,P+5);
     P=P+5+sizeof(system_param.collision_warning_param);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.collision_warning_param);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005D-->��ײ������������:%d.",system_param.collision_warning_param);
     #endif
                  
     //case 0x005E://�෭������������
     Big_End_DwordtoByte(0x005E,P);
     *(P+4)=sizeof(system_param.rollover_param);
     Big_End_DwordtoByte(system_param.rollover_param,P+5);
     P=P+5+sizeof(system_param.rollover_param);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.rollover_param);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x005E-->�෭������������:%d.",system_param.rollover_param);
     #endif
                              
     //case 0x0064://��ʱ���տ���
     Big_End_DwordtoByte(0x0064,P);
     *(P+4)=sizeof(system_param.time_photo_param.photo_data);
     Big_End_DwordtoByte(system_param.time_photo_param.photo_data,P+5);
     P=P+5+sizeof(system_param.time_photo_param.photo_data);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.time_photo_param.photo_data);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0064-->��ʱ���տ���:0x%0x.",system_param.time_photo_param.photo_data);
     #endif
                  
     //case 0x0065://�������տ���
     Big_End_DwordtoByte(0x0065,P);
     *(P+4)=sizeof(system_param.distance_photo_param.photo_data);
     Big_End_DwordtoByte(system_param.distance_photo_param.photo_data,P+5);
     P=P+5+sizeof(system_param.distance_photo_param.photo_data);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.distance_photo_param.photo_data);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0065-->�������տ���:0x%0x.",system_param.distance_photo_param.photo_data);
     #endif
                  
     //case 0x0070://ͼ��/��Ƶ������1-10��1 ���
     Big_End_DwordtoByte(0x0070,P);
     *(P+4)=sizeof(system_param.image_quality);
     Big_End_DwordtoByte(system_param.image_quality,P+5);
     P=P+5+sizeof(system_param.image_quality);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_quality);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0070-->ͼ��/��Ƶ����:%d.",system_param.image_quality);
     #endif
                  
     //case 0x0071://���ȣ�0-255
     Big_End_DwordtoByte(0x0071,P);
     *(P+4)=sizeof(system_param.image_brightness);
     Big_End_DwordtoByte(system_param.image_brightness,P+5);
     P=P+5+sizeof(system_param.image_brightness);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_brightness);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0071-->����:%d.",system_param.image_brightness);
     #endif
                  
     //case 0x0072://�Աȶȣ�0-127
     Big_End_DwordtoByte(0x0072,P);
     *(P+4)=sizeof(system_param.image_contrast);
     Big_End_DwordtoByte(system_param.image_contrast,P+5);
     P=P+5+sizeof(system_param.image_contrast);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_contrast);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0072-->�Աȶ�:%d.",system_param.image_contrast);
     #endif
                  
     //case 0x0073://���Ͷȣ�0-127
     Big_End_DwordtoByte(0x0073,P);
     *(P+4)=sizeof(system_param.image_saturation);
     Big_End_DwordtoByte(system_param.image_saturation,P+5);
     P=P+5+sizeof(system_param.image_saturation);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_saturation);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0073-->���Ͷ�:%d.",system_param.image_saturation);
     #endif
                  
     //case 0x0074://ɫ�ȣ�0-255
     Big_End_DwordtoByte(0x0074,P);
     *(P+4)=sizeof(system_param.image_chroma);
     Big_End_DwordtoByte(system_param.image_chroma,P+5);
     P=P+5+sizeof(system_param.image_chroma);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.image_chroma);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0074-->ɫ��:%d.",system_param.image_chroma);
     #endif
                  
     //case 0x0080://������̱������1/10km
     Big_End_DwordtoByte(0x0080,P);
     *(P+4)=sizeof(system_param.mileage);
     Big_End_DwordtoByte(system_param.mileage,P+5);
     P=P+5+sizeof(system_param.mileage);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.mileage);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->������̱����:%d.",system_param.mileage);
     #endif
                  
     //case 0x0081://�������ڵ�ʡ�� ID
     Big_End_DwordtoByte(0x0081,P);
     *(P+4)=sizeof(system_param.terminl_registe.provincial_id);
     Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
     P=P+5+sizeof(system_param.terminl_registe.provincial_id);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.provincial_id);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0081-->�������ڵ�ʡ�� ID:%d.",system_param.terminl_registe.provincial_id);
     #endif
                  
     //case 0x0082:// �������ڵ����� ID
     Big_End_DwordtoByte(0x0082,P);
     *(P+4)=sizeof(system_param.terminl_registe.county_id);
     Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
     P=P+5+sizeof(system_param.terminl_registe.county_id);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.county_id);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0082-->�������ڵ����� ID:%d.",system_param.terminl_registe.county_id);
     #endif
                  
     //case 0x0083://������ͨ�����Ű䷢�Ļ���������
     Big_End_DwordtoByte(0x0083,P);
     *(P+4)=strlen(system_param.terminl_registe.plate_number);
     memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
     P=P+5+strlen(system_param.terminl_registe.plate_number);//��ַƫ��
     Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0083-->������ͨ�����Ű䷢�Ļ���������:%s.",system_param.terminl_registe.plate_number);
     #endif
                  
     //case 0x0084://������ɫ
     Big_End_DwordtoByte(0x0084,P);
     *(P+4) = sizeof(system_param.terminl_registe.plate_coror);
     *(P+5) = system_param.terminl_registe.plate_coror;
     P=P+5+sizeof(system_param.terminl_registe.plate_coror);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.terminl_registe.plate_coror);
             
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0084-->������ɫ:%d.",system_param.terminl_registe.plate_coror);
     #endif
                              
     //case 0x0090://GNSS ��λģʽ
     Big_End_DwordtoByte(0x0090,P);
     *(P+4)=sizeof(system_param.location_mode);
     *(P+5) = system_param.location_mode;
     P=P+5+sizeof(system_param.location_mode);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_mode);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0090-->��λģʽ:%d.",system_param.location_mode);
     #endif
                  
     //case 0x0091://GNSS ������
     Big_End_DwordtoByte(0x0091,P);
     *(P+4)=sizeof(system_param.baud_rate);
     *(P+5) = system_param.baud_rate;
     P=P+5+sizeof(system_param.baud_rate);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.baud_rate);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0091-->GNSS ������:%d.",system_param.baud_rate);
     #endif
                  
     //case 0x0092://GNSS ģ����ϸ��λ�������Ƶ��
     Big_End_DwordtoByte(0x0092,P);
     *(P+4)=sizeof(system_param.location_out_frze);
     *(P+5) = system_param.location_out_frze;
     P=P+5+sizeof(system_param.location_out_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.location_out_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0092-->ģ����ϸ��λ�������Ƶ��:%d.",system_param.location_out_frze);
     #endif
                  
     //case 0x0093://GNSS ģ����ϸ��λ���ݲɼ�Ƶ��
     Big_End_DwordtoByte(0x0093,P);
     *(P+4)=sizeof(system_param.detail_location_get_frze);
     Big_End_DwordtoByte(system_param.detail_location_get_frze,P+5);
     P=P+5+sizeof(system_param.detail_location_get_frze);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_get_frze);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0093-->ģ����ϸ��λ���ݲɼ�Ƶ��:%d.",system_param.detail_location_get_frze);
     #endif
                              
     //case 0x0094://GNSS ģ����ϸ��λ�����ϴ���ʽ
     Big_End_DwordtoByte(0x0094,P);
     *(P+4)=sizeof(system_param.detail_location_upload_mode);
     *(P+5) = system_param.detail_location_upload_mode;
     P=P+5+sizeof(system_param.detail_location_upload_mode);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_mode);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0094-->ģ����ϸ��λ�����ϴ���ʽ:%d.",system_param.detail_location_upload_mode);
     #endif
                  
     //case 0x0095://GNSS ģ����ϸ��λ�����ϴ�����
     Big_End_DwordtoByte(0x0095,P);
     *(P+4)=sizeof(system_param.detail_location_upload_setting);
     *(P+5) = system_param.detail_location_upload_setting;
     P=P+5+sizeof(system_param.detail_location_upload_setting);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.detail_location_upload_setting);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0095-->ģ����ϸ��λ�����ϴ�����:%d.",system_param.detail_location_upload_setting);
     #endif
                  
     //case 0x0100://CAN ����ͨ�� 1 �ɼ�ʱ����
     Big_End_DwordtoByte(0x0100,P);
     *(P+4)=sizeof(system_param.can_collect_param.can1_time_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can1_time_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can1_time_interval);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_time_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0100-->CAN����ͨ��1�ɼ�ʱ����:%d.",system_param.can_collect_param.can1_time_interval);
     #endif
                  
     //case 0x0101://CAN ����ͨ�� 1 �ϴ�ʱ����
     Big_End_DwordtoByte(0x0101,P);
     *(P+4)=sizeof(system_param.can_collect_param.can1_upload_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can1_upload_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can1_upload_interval);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can1_upload_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0101-->CAN����ͨ��1�ϴ�ʱ����:%d.",system_param.can_collect_param.can1_upload_interval);
     #endif
                  
     //case 0x0102://CAN ����ͨ�� 2 �ɼ�ʱ����
     Big_End_DwordtoByte(0x0102,P);
     *(P+4)=sizeof(system_param.can_collect_param.can2_time_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can2_time_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can2_time_interval);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_time_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->CAN ����ͨ�� 2 �ɼ�ʱ����:%d.",system_param.can_collect_param.can2_time_interval);
     #endif
                  
     //case 0x0103://CAN����ͨ��2�ϴ�ʱ����
     Big_End_DwordtoByte(0x0103,P);
     *(P+4)=sizeof(system_param.can_collect_param.can2_upload_interval);
     Big_End_DwordtoByte(system_param.can_collect_param.can2_upload_interval,P+5);
     P=P+5+sizeof(system_param.can_collect_param.can2_upload_interval);//��ַƫ��
     Cnt_Num=Cnt_Num+5+sizeof(system_param.can_collect_param.can2_upload_interval);
     #if APP_TRACE_DBG 
     eat_trace("seting_param id:0x0080-->CAN����ͨ��2�ϴ�ʱ����:%d.",system_param.can_collect_param.can2_upload_interval);
     #endif
     for(Cnt=0;Cnt<system_param.can_collect_param.canid_num;Cnt++)
     {
         Big_End_DwordtoByte(0x0110+Cnt,P);
         *(P+4) = 8;
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[Cnt].can_id,P+5);
         Big_End_DwordtoByte(system_param.can_collect_param.can_id_param_array[Cnt].can_id_collect_frze,P+9);
         P=P+5+8;//��ַƫ��
         Cnt_Num=Cnt_Num+5+8;
     }
     *(Send_Data+2) = 81+Cnt;//��������
     Tcp_Message_PacKage_And_Send(l_socket,&msgs_sern,tcp_request_param_replay,Send_Data,Cnt_Num,0,0,0);
 }


 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: Tcp_VDR_Request_Sate
  *  Description: �ն˷����Բ�ѯ����
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
 *  Description: ��������
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
 	Big_End_WordtoByte(msg_svn,Loca_Temp); /*SN��ˮ��ת��*/	
 	memcpy(Loca_Temp+2,Location_Inf_Data.Location_Inf,Location_Inf_Data.Location_Inf_Lens);//λ��
    Tcp_Message_PacKage_And_Send(socket,&msgs_sern,tcp_car_contrl_id_replay_id,temp_buf,Location_Inf_Data.Location_Inf_Lens + 2,0,0,0);
  }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: void tcp_car_ctrl_proc(uint8_t socket,uint16_t msg_svn,int8_t *temp_buf)
 *  Description: �ն˿���
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
              eat_trace("remote_vdr_ctrl_proc--> �ն���ָ��������:%s",data_rev+1);
              #endif
              break;
              
          case 0x03:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> ���ն˹ػ���!!!");
              #endif
              break;
          case 0x04:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> ���ն˸�λ��!!!");
              #endif
              
              board_mode_seting(0x05);
              break;
          case 0x05:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> �������ն��ն˻ָ��������á�!!!");
              #endif
              system_param.param_save.statue.wether_first_power_on=EAT_FALSE;
              system_para_save_at_once();
              board_mode_seting(0x06);
              break;
          case 0x06:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> �������ն˹ر�����ͨ�š�!!!");
              #endif
              
              break;
          case 0x07:
              #if remote_data_setting
              eat_trace("remote_vdr_ctrl_proc--> �������ն˹ر���������ͨ�š�!!!");
              #endif
              break;
          default:
          break;  
      }
  
  }

