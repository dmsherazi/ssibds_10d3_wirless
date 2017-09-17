/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "system_param_proc.h"


 /****************************************************
* External variables declaration
*****************************************************/
system_param_struct system_param;

/*****************************************************
* Local variables declaration(Static)
******************************************************/
extern const uint8_t system_vison[32]="1D03-Sv1.4_1707111647";
/*********���ٶѿռ�********/
uint8_t app_dynic_mem_test[DYNAMIC_MEM_SIZE]={0};
/* 
* ===  FUNCTION  ======================================================================
*         Name: parm_init_proc
*  Description: ������ʼ��
*        Input:            
*       Output:
*       Return:               
*       author:  j.x.b create at 2016-7-16
* =====================================================================================
*/
static void system_mem_creat_proc(void)
{    
    void* mem_prt=EAT_NULL;
    eat_bool mem_ini_flag=EAT_FALSE;
	if( EAT_FALSE == mem_ini_flag)    
	{            
		mem_ini_flag = eat_mem_init(app_dynic_mem_test, sizeof(app_dynic_mem_test));        
		if(EAT_TRUE == mem_ini_flag)        
		{            
		    eat_trace("system_mem_creat_proc--> memary creat seucess !");         
		}
		else        
		{            
			eat_trace("system_mem_creat_proc--> memary creat fail !");        
		}            
	}
    else    
    {        
    	eat_trace("system_mem_creat_proc--> memary already creat sucess !");    
    } 
}

/* 
* ===  FUNCTION  ======================================================================
*         Name: parm_init_proc
*  Description: ������ʼ��
*        Input:            
*       Output:
*       Return:               
*       author:  j.x.b create at 2016-7-16
* =====================================================================================
*/
static void parm_init_proc(void)
{
    /*��Ȩ��*/
    uint8_t default_num[6]={0x01,0x59,0x07,0x15,0x71,0x13};
    uint8_t sleep_time[4]={0x06,0x30,0x18,0x30};
    
    if(system_param.param_save.statue.wether_first_power_on==EAT_FALSE)
    {
        system_param.param_save.statue.secondip_open_switch=EAT_TRUE;
        system_param.lock_car_save.lock_ip=0;
        system_param.terminl_registe.provincial_id = 42;
        system_param.terminl_registe.county_id = 100;
        memcpy(system_param.terminl_registe.terminl_id,"1000700",7);//G401448
        system_param.terminl_registe.plate_coror = 1;
        memcpy(system_param.terminl_registe.vehicle_vin,"LGAX2A136G1006927",strlen("LGAX2A136G1006927"));
        memcpy(system_param.terminl_registe.plate_number,"��1000700",strlen("��1000700"));
        
        memcpy(system_param.terminl_registe.vehicle_classification,"���ͻ���",strlen("���ͻ���"));
        memcpy(system_param.terminl_registe.manufacture_id,"70315",strlen("70315"));
        memcpy(system_param.terminl_registe.terminl_model,"NERTVM02",strlen("NERTVM02"));
        memset(system_param.terminl_registe.iccd,0,sizeof(system_param.terminl_registe.iccd));
        memcpy(system_param.terminl_registe.hardware_vison,"SSI1D03-V1.4",strlen("SSI1D03-V1.4"));
        memcpy(system_param.terminl_registe.softerware_vison,system_vison,strlen(system_vison));
        
        system_param.terminl_registe.vehicle_puse_num = 0;
        memset(system_param.terminl_registe.first_authentication_id,0,sizeof(system_param.terminl_registe.first_authentication_id));
        memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));

        memcpy(system_param.SaveTerminlPhone.terminl_phone_nums,"15907157113",strlen("15907157113"));
        memcpy(system_param.SaveTerminlPhone.terminl_phone_numd,default_num,6);
       
                                        
        /*��ʱ����*/
        system_param.HeartBeatUpdataFre = 180; //Ĭ������Ƶ�� 60 
        system_param.SaveTcpRetransTimes.waiting_time = 30;//Ĭ��TCP��ʱʱ�� 5s ���� 3
        system_param.SaveTcpRetransTimes.timeout_times= 3;
        
        system_param.SaveUdpRetransTimes.waiting_time = 30;//Ĭ��UDP��ʱʱ�� 5s ���� 
        system_param.SaveUdpRetransTimes.timeout_times= 3;

        
        system_param.SaveSmsRetransTimes.waiting_time = 30;//Ĭ��SMS��ʱʱ�� 5s ���� 3
        system_param.SaveSmsRetransTimes.timeout_times= 3;
        
        /*����APN����*/
        memcpy(system_param.SaveMainBeareParameter.beare_apn,"CMNET",strlen("CMNET"));
        memset(system_param.SaveMainBeareParameter.beare_user,0,sizeof(system_param.SaveMainBeareParameter.beare_user));
        memset(system_param.SaveMainBeareParameter.beare_password,0,strlen(system_param.SaveMainBeareParameter.beare_password));

        memcpy(system_param.SaveBackupBeareParameter.beare_apn,"CMNET",strlen("CMNET"));
        memset(system_param.SaveBackupBeareParameter.beare_user,0,sizeof(system_param.SaveMainBeareParameter.beare_user));
        memset(system_param.SaveBackupBeareParameter.beare_password,0,strlen(system_param.SaveMainBeareParameter.beare_password));

        /*�������������Ӳ���*/
        memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
        memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,"61.183.229.50",strlen("61.183.229.50"));
        system_param.SaveServerParameterArray[MAIN_SERVER].server_socket = 9010;
        system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket=0;
        
        memset(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
        memcpy(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,"",strlen(""));
        system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket = 0;
        system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].udp_server_socket=0;
        
        memset(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
        memcpy(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,"61.183.229.50",strlen("61.183.229.50"));
        system_param.SaveServerParameterArray[SECOND_SERVER].server_socket = 60002;
        system_param.SaveServerParameterArray[SECOND_SERVER].udp_server_socket=0;
        
        memset(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin));
        memcpy(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,"",strlen(""));
        system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_socket = 0;
        system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].udp_server_socket=0;

        /*IC����֤���������Ӳ���*/
        
        
        memset(system_param.ic_server_parameter.main_server_domin,0,sizeof(system_param.ic_server_parameter.backup_server_domin));
        memcpy(system_param.ic_server_parameter.main_server_domin,"",strlen(""));

        memset(system_param.ic_server_parameter.backup_server_domin,0,sizeof(system_param.ic_server_parameter.backup_server_domin));
        memcpy(system_param.ic_server_parameter.backup_server_domin,"",strlen(""));
        system_param.ic_server_parameter.tcp_server_socket = 0;
        system_param.ic_server_parameter.udp_server_socket = 0;
        /*λ���ϱ�����*/ //0x0020--->
        system_param.location_reporting_param.policy = 0; 
        system_param.location_reporting_param.scheme = 0;
        
        system_param.location_reporting_param.not_signed_frze = 30;
        system_param.location_reporting_param.sleeping_frze = 1800;
        system_param.location_reporting_param.emergency_alarm_frze = 10;
        system_param.location_reporting_param.default_frze = 30;
        system_param.location_reporting_param.night_sleep_frze = 7200;
        memcpy(system_param.location_reporting_param.time_night,sleep_time,4);
        system_param.location_reporting_param.sleep_delay= 0;
        
        system_param.location_reporting_param.default_distance= 500;
        system_param.location_reporting_param.not_signed_distance= 500;
        system_param.location_reporting_param.sleeping_distance= 500;
        system_param.location_reporting_param.emergency_alarm_distance = 200;

        
        /*�յ����Χ������*/ //0x0030--->0x0031
        system_param.inflection_angle_passup = 45;
        system_param.electronic_fence_radius = 500;
        
        /*�ֻ�������ز���*/ 
        
        memset(system_param.phone_number_param.monitor_platform,0,sizeof(system_param.phone_number_param.monitor_platform));
        memcpy(system_param.phone_number_param.monitor_platform,"",strlen(""));

        
        memset(system_param.phone_number_param.terminal_restart,0,sizeof(system_param.phone_number_param.terminal_restart));
        memcpy(system_param.phone_number_param.terminal_restart,"",strlen(""));

        
        memset(system_param.phone_number_param.terminal_format,0,sizeof(system_param.phone_number_param.terminal_format));
        memcpy(system_param.phone_number_param.terminal_format,"",strlen(""));

        
        memset(system_param.phone_number_param.monitor_platform_sms,0,sizeof(system_param.phone_number_param.monitor_platform_sms));
        memcpy(system_param.phone_number_param.monitor_platform_sms,"",strlen(""));

        memset(system_param.phone_number_param.terminal_sms_send,0,sizeof(system_param.phone_number_param.terminal_sms_send));
        memcpy(system_param.phone_number_param.terminal_sms_send,"",strlen(""));
        
        system_param.phone_number_param.answer_policy = 0;
        system_param.phone_number_param.every_longest_call_time = 120;
        system_param.phone_number_param.mounth_longest_call_time = 36000;

        
        memset(system_param.phone_number_param.monitor_telephone,0,sizeof(system_param.phone_number_param.monitor_telephone));
        memcpy(system_param.phone_number_param.monitor_telephone,"",strlen(""));

        memset(system_param.phone_number_param.supervision_platform,0,sizeof(system_param.phone_number_param.supervision_platform));
        memcpy(system_param.phone_number_param.supervision_platform,"",strlen(""));

        memset(system_param.phone_number_param.sos_telephone,0,sizeof(system_param.phone_number_param.sos_telephone));
        memcpy(system_param.phone_number_param.sos_telephone,"",strlen(""));

        /*�������β���*/
        system_param.alarm_mask.Alarm_Original = 0x00000000;
        
        /*���ű������ز���*/ 
        system_param.alarm_sms_swtich.Alarm_Original =0x00000000;
        
        /*�������մ������*/ 
        system_param.alarm_camer_switch.Alarm_Original =0x00000000;
        system_param.alarm_camer_upload.Alarm_Original =0x00000000;
        system_param.linchpin_alarm.Alarm_Original = 0x00000000;

       /*���ٲ���*/ 
       system_param.max_speed = 100;       
       system_param.speeding_duration = 5;

       /*ƣ�ͼ�ʻ����*/ 
       system_param.continuous_driving_threshold = 14400;
       system_param.day_continuous_driving_threshold = 57600;
       system_param.min_rest_period =1200;
       system_param.max_parking_time = 3600;

       /*Ԥ������*/ 
       system_param.speed_warning_dif = 100; 
       system_param.fatigue_dring_warning_dif = 1200; 

       /*��ײ����*/ 
       system_param.collision_warning_param = 17926;
       system_param.rollover_param = 30;//0x005D WORD0x005E WORD�෭�����������ã��෭�Ƕȣ���λ 1 �ȣ�Ĭ��Ϊ 30 �ȡ�

       /*���ղ���*/
       system_param.time_photo_param.photo_data = 0x00000000;//��ʱ����
       system_param.distance_photo_param.photo_data = 0x00000000;//��������

       /*ͼ�����*/
       system_param.image_quality = 1;    
       system_param.image_brightness = 225; 
       system_param.image_contrast = 125;  
       system_param.image_saturation = 8; 
       system_param.image_chroma = 225;     
       
       /*��̲���*/
       system_param.mileage = 0;//0x0080 DWORD ������̱������1/10km
       
       /*��λ����*/
       system_param.location_mode = 0x03;
       system_param.baud_rate =0x05;
       system_param.location_out_frze = 0x01;
       system_param.detail_location_get_frze = 1;
       system_param.detail_location_upload_mode = 0;
       system_param.detail_location_upload_setting = 0;

       /*CAN����*/                                                
       memset(&system_param.can_collect_param,0,sizeof(system_param.can_collect_param));
   }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: system_para_save_at_once
 *  Description: �����������
 *        Input:            
 *       Output:
 *       Return:               
 *       author:  j.x.b create at 2013-12-16
 * =====================================================================================
 */
void system_para_save_at_once(void)
{
    file_write(File_SystemParaSave,0,&system_param,sizeof(system_param));
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: system_param_save
 *  Description: ������ȡ������
 *        Input:            
 *       Output:
 *       Return:               
 *       author:  j.x.b create at 2013-12-16
 * =====================================================================================
 */
static void system_param_save(void)
{
    int32_t file_read_result;
    file_create(File_SystemParaSave);
    file_read_result=file_read(File_SystemParaSave,0,&system_param,sizeof(system_param));
    if(file_read_result==OPERATION_NO_ERROR)
    {
        #if system_param_debug
        eat_trace("system_param_save --> file_read ok��");
        #endif
        parm_init_proc();//�������
        
        /**ä��������ʼ��**/
        fade_data_init();
        
        /**txt�ļ��д���**/
        txt_data_record_init();
        
        /**�����ļ�����**/
        regional_file_init();
        
        if(system_param.param_save.statue.wether_first_power_on==EAT_FALSE)
        {
            system_param.param_save.statue.wether_first_power_on=EAT_TRUE;
            system_para_save_at_once();
        }
        else
        {
            Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Kg = system_param.param_save.statue.viech_kg;
            memcpy(system_param.terminl_registe.softerware_vison,system_vison,strlen(system_vison));
        }
    }
    else
    {
        #if system_param_debug
        eat_trace("system_param_save --> file_read error id:%d !",file_read_result);
        #endif
    }
}

 /* 
  * ===  FUNCTION  ======================================================================
  *         Name: sys_param_init_proc
  *  Description: ����������ʼ��
  *        Input:            
  *       Output:
  *       Return:               
  *       author:  j.x.b create at 2013-12-16
  * =====================================================================================
  */
 void sys_param_init_proc(void)
 {
    #if system_param_debug 
    eat_trace("/***************************************************/");
    eat_trace("�������:%s","ssibds_1d03 3870010-C0102");
    eat_trace("Ӳ���汾:%s","SSI1D03-V1.4");
    eat_trace("����汾:%s",system_vison);
    eat_trace("��������:%s","201704101908");
    eat_trace("������:%s","j.x.b");
    eat_trace("/***************************************************/");
    #endif
    /**����memloc�ռ�**/
    system_mem_creat_proc();
    
    /**����ʹ��**/
    system_param_save();
    /**���������ʼ��**/
    regional_file_init(); 
    
    read_txt_data_record();
    
    /**���ڷ��ͽ��ս��������ʼ��**/
    uart1_send_list_init();//������������
    uart1_anaylsis_pro_list_init();//���ڽ������������ʼ��
    
    /**���մ���CAN���������ʼ��**/
    receive_can_pro_list_init();//����CAN��Ϣ
    can_send_pro_list_init();

    /**���մ���TCP���������ʼ��**/
    receive_pro_list_init();//TCP���������ʼ��
    Inf_Struct_Init();//λ����Ϣ��ʼ��
    
    /**���ŷ������������ʼ��**/
    init_sms_setting();
    sms_send_list_init();
    
    /**��ý��ͼƬ��Ϣ��ʼ��**/
    take_picture_system_add_init();
    
    /**������Ϣ��ʼ��**/
    take_pictures_pro_list_init();

    /**��ý��¼����Ϣ��ʼ��**/
    voice_record_system_add_init();
    
    /**��ý���ϴ������ʼ��**/
    media_update_pro_list_init();

    /**��ʻ��¼���ݲɼ�����**/
    inquire_dring_record_queue_init();

 }
