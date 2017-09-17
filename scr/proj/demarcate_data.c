

/****************************************************
 * Include Files
****************************************************/ 
 
 #include "demarcate_data.h"
 #include <stdlib.h>
 #include "math.h"
 /****************************************************
* External variables declaration
*****************************************************/
uint8_t record_test_flag=EAT_FALSE;


/*****************************************************
* Local variables declaration(Static)
******************************************************/

 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: demarcate_data_proc
   *  Description:  
   *  Parameters : �豸�����궨
   *  Return     :  void 
   *  author     :  J.x.b create at SSI_TVM02 at 2016-4-15
   * =====================================================================================
   */ 
static void demarcate_data_proc(uint8_t *Temp_data_buff)//USER3
{
	uint8_t *Base_Data;
	uint32_t seting_id=0;
	uint8_t re_conect=EAT_FALSE;
	uint8_t param_cnt=0,parm_num=0;
    uint8_t data_buffer[32]={0};
    uint8_t sp_seting=EAT_FALSE;
    uint8_t Re_Link_Min=EAT_FALSE;
    uint8_t Re_Regist_main=EAT_FALSE;
    
    uint8_t Re_Link_second=EAT_FALSE;
    uint8_t Re_Regist_second=EAT_FALSE;
	parm_num = *Temp_data_buff;//��������
	Base_Data = Temp_data_buff+1;
    #if APP_TRACE_DBG 
    eat_trace("demarcate_data_proc --> param_num:%d.",parm_num);
    #endif
	for(param_cnt=0;param_cnt<parm_num;param_cnt++)
	{
	   Big_End_BytetoDword(Base_Data,&seting_id);
	   switch(seting_id)
	   {
			case 0x0010://�������� APN������ͨ�Ų��ŷ��ʵ�
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc --> id:0x0010.");
                #endif
    			memset(system_param.SaveMainBeareParameter.beare_apn,0,strlen(system_param.SaveMainBeareParameter.beare_apn));
    			memcpy(system_param.SaveMainBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));	

                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0010-->�������� APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                #endif
    			break;
			case 0x0013://����������ַ,IP ������			
                memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,Base_Data+5,*(Base_Data+4));

                if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
                vehicle_infor_seting(0x06);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0013-->����������ַ,IP ������:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                #endif
    			break;
			    
			case 0x0018://������ TCP �˿�
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket);
                if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0018-->������ TCP �˿�:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                #endif
    			break;
			
		    case 0x0027://����ʱ�㱨ʱ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0027-->����ʱ�㱨ʱ����:%d.",system_param.location_reporting_param.sleeping_frze);
                #endif
			    break;
						
		    
		    case 0x0029://ȱʡʱ��㱨���
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0028-->ȱʡʱ��㱨���:%d.",system_param.location_reporting_param.default_frze);
                #endif
		        break;  
		    
		    case 0x002A://��������ʱ����
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.night_sleep_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x002A-->��������ʱ����:%d.",system_param.location_reporting_param.night_sleep_frze);
                #endif
		        break;

		    case 0x002B://��������ʱ��
                memset(system_param.location_reporting_param.time_night,0,sizeof(system_param.location_reporting_param.time_night));
                memcpy(system_param.location_reporting_param.time_night,Base_Data+5,*(Base_Data+4));
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x002B-->:����ʱ��:%0x:%0x--%0x:%0x.",system_param.location_reporting_param.time_night[0],\
                system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3]);
                #endif
		        break;
		        
           case 0x0055://����ٶ�
                Big_End_BytetoDword(Base_Data+5,&system_param.max_speed);
                sp_seting=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id::0x0055-->����ٶ�:%d.",system_param.max_speed);
                #endif
                break;
                
			case 0x0056://���ٳ���ʱ��
                Big_End_BytetoDword(Base_Data+5,&system_param.speeding_duration);
                sp_seting=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0056-->���ٳ���ʱ��:%d.",system_param.speeding_duration);
                #endif
			    break;

			case 0x0081://�������ڵ�ʡ�� ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.provincial_id);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0081-->�������ڵ�ʡ�� ID:%d.",system_param.terminl_registe.provincial_id);
                #endif
			    break;

			case 0x0082:// �������ڵ����� ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.county_id);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0082-->�������ڵ����� ID:%d.",system_param.terminl_registe.county_id);
                #endif
			    break;

			case 0x0083://������ͨ�����Ű䷢�Ļ���������
                memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
                memcpy(system_param.terminl_registe.plate_number,Base_Data+5,*(Base_Data+4));
                Re_Link_second = EAT_TRUE;
                Re_Regist_second = EAT_TRUE;
                
                vehicle_infor_seting(0x01);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0083-->������ͨ�����Ű䷢�Ļ���������:%s.",system_param.terminl_registe.plate_number);
                #endif
			    break;

			case 0x0084://������ɫ
 			     system_param.terminl_registe.plate_coror = *(Base_Data+5);
                 Re_Link_second = EAT_TRUE;
                 Re_Regist_second = EAT_TRUE;
                 #if demarcate_debug_printf 
                 eat_trace("demarcate_data_proc id:0x0084-->������ɫ:%d.",system_param.terminl_registe.plate_coror);
                 #endif
 			     break;
			
			case 0x1001://��������
     			 memset(system_param.SaveTerminlPhone.terminl_phone_nums,0,sizeof(system_param.SaveTerminlPhone.terminl_phone_nums));
                 memcpy(system_param.SaveTerminlPhone.terminl_phone_nums,Base_Data+5,*(Base_Data+4));
                 memset(system_param.SaveTerminlPhone.terminl_phone_numd,0,6);
                 if(strlen(system_param.SaveTerminlPhone.terminl_phone_nums)!=0)
                 {
                     Porotorl_Phone_Num_Conver(system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums),system_param.SaveTerminlPhone.terminl_phone_numd);
                     Re_Regist_main=EAT_TRUE;
                     Re_Regist_second=EAT_TRUE;
                     #if demarcate_debug_printf
                     eat_trace("demarcate_data_proc id:0x1001-->> �ն��ֻ�����:%s",system_param.SaveTerminlPhone.terminl_phone_nums);
                     #endif
                 }
 			     break;	

			case 0x1002://sos����
     			 memset(system_param.phone_number_param.sos_telephone,0,sizeof(system_param.phone_number_param.sos_telephone));
                 memcpy(system_param.phone_number_param.sos_telephone,Base_Data+5,*(Base_Data+4));
                  
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1002-->> S0S�ֻ�����:%s",system_param.phone_number_param.sos_telephone);
                 #endif
 			     break;
			
			case 0x1003://�ն�ID
                 memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
                 memcpy(system_param.terminl_registe.terminl_id,Base_Data+5,*(Base_Data+4));
                 Re_Regist_main=EAT_TRUE;
                 Re_Regist_second=EAT_TRUE;
                 
                 vehicle_infor_seting(0x05);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1003-->> �ն�ID:%s",system_param.terminl_registe.terminl_id);
                 #endif
     			 break;	

			case 0x1004://vVIN
                 memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));
                 memcpy(system_param.terminl_registe.vehicle_vin,Base_Data+5,*(Base_Data+4));
                 vehicle_infor_seting(0x03);
                 if(system_param.terminl_registe.plate_coror==EAT_FALSE)
                 {
                     Re_Regist_main=EAT_TRUE;
                     Re_Regist_second=EAT_TRUE;
                 } 
                 
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1004-->>  set vin:%s",system_param.terminl_registe.vehicle_vin);
                 #endif 
                 
                 break;
			
			case 0x1005://����ϵ��
                 Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.vehicle_puse_num);
                 vehicle_infor_seting(0x04);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1005-->>  set ����ϵ��:%d",system_param.terminl_registe.vehicle_puse_num);
                 #endif
     			 break;	

			case 0x1006://��������
     			 memset(system_param.terminl_registe.vehicle_classification,0,sizeof(system_param.terminl_registe.vehicle_classification));
                 memcpy(system_param.terminl_registe.vehicle_classification,Base_Data+5,*(Base_Data+4));
                 vehicle_infor_seting(0x02);
                  
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1006-->>  set ��������:%s",system_param.terminl_registe.vehicle_classification);
                 #endif
     			 break;	

			case 0x1007://������ʱ
       		     Big_End_BytetoWord(Base_Data+5,&system_param.location_reporting_param.sleep_delay);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1007-->>  set ������ʱ:%d",system_param.terminl_registe.vehicle_classification);
                 #endif
       			 break;	
			
			case 0x1010:
                 memset(system_param.SaveBackupBeareParameter.beare_apn,0,sizeof(system_param.SaveBackupBeareParameter.beare_apn));
                 memcpy(system_param.SaveBackupBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1010-->> �ڶ�������_APN:%s",system_param.SaveBackupBeareParameter.beare_apn);
                 #endif
     			 break;			

			case 0x1013:
                 memset(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
                 memcpy(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,Base_Data+5,*(Base_Data+4));
                 if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1013-->> �ڶ�����������:%s",system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
                 #endif
			     break;

			case 0x1018:
                 Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
                 if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1014-->> �ڶ��������˿�:%d",system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
                 #endif
     			 break;
    	    
			default:
			    break;	
		}
	    Base_Data = Base_Data + *(Base_Data+4)+5;//��һ�ε��׵�ַ
	}
	/**********�������***************/
	 system_para_save_at_once();
	 if(sp_seting==EAT_TRUE)
	 {
         seting_data_to_board(0x00);
	 }
	/**********����������***************/
	if(Re_Link_Min == EAT_TRUE)//��һ������������
	{
	    /*������һ��������Ȩ��*/
        memset(system_param.terminl_registe.first_authentication_id,0,sizeof(system_param.terminl_registe.first_authentication_id));//��¼��Ȩ��
        system_para_save_at_once();
        
        Main_Link.link_staue=EAT_FALSE;
        Main_Link.get_ip_statue=EAT_FALSE;
		System_Flag.System_Flag_Bit.MainAutenSucess = EAT_FALSE;
		System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
	}
	if(Re_Regist_main ==EAT_TRUE)//��ע���һIP
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
	}
	if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
    {
        if(Re_Link_second == EAT_TRUE)//�����ӵڶ�IP
        {
            /*�����ڶ���������Ȩ��*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//��¼��Ȩ��
            system_para_save_at_once();

            Second_Link.link_staue=EAT_FALSE;
            Second_Link.get_ip_statue=EAT_FALSE;
        	System_Flag.System_Flag_Bit.SecondAutenSucess = EAT_FALSE;
        	System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
            task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
        }
        if(Re_Regist_second == EAT_TRUE)//��ע��ڶ�IP
        {
            /*�������ڶ�IPȨ��*/
            memset(system_param.terminl_registe.second_authentication_id,0,sizeof(system_param.terminl_registe.second_authentication_id));//��¼��Ȩ��
            system_para_save_at_once();
        	if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
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
    else
    {
        #if demarcate_debug_printf
        eat_trace("demarcate_data_proc id:0x1013-->> �ڶ�������δ�������޶���  !");
        #endif
    }
 }
 
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void Setting_Data_Handle(unsigned char *Temp_data_buff)//USER3
 *  Description:  �������ݱ궨��ѯ 808 8103 Э����ͬ
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
static void read_demarcate_data_proc(uint8_t *Temp_data_buff)
{
	
	uint8_t *P;//λ��ָ��
	uint16_t Cnt;
	uint8_t Str_Lens;
	uint32_t Data_ID;
	uint8_t Send_Data[1056];
	uint16_t Cnt_Num=0; //���ݸ���
	*(Send_Data) = *Temp_data_buff;//Ӧ�����
	Cnt_Num +=1; //���ݸ���
	P=Send_Data+1;
	for(Cnt=0;Cnt<*Temp_data_buff;Cnt++)
	{	
		Big_End_BytetoDword(Temp_data_buff+1+4*Cnt,&Data_ID);
		switch(Data_ID)
		{
			case 0x0010:
     			 Big_End_DwordtoByte(0x0010,P);
     			 *(P+4)=strlen(system_param.SaveMainBeareParameter.beare_apn);
     			 memcpy(P+5,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
     			 P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//��ַƫ��
     			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//���ݳ���
     			 break;

			case 0x0013:
    			 Big_End_DwordtoByte(0x0013,P);
    			 *(P+4) = strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
    			 memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
    			 P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//���ݳ���
    			 break;
			
			case 0x0018:
     			 Big_End_DwordtoByte(0x0018,P);
     			 *(P+4)=4;
     			 Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
     			 P=P+5+4;//��ַƫ��
     			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
     			 break;

			
		    case 0x0027:
    		     Big_End_DwordtoByte(0x0027,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    			 break;			
						
		    case 0x0029:
    		     Big_End_DwordtoByte(0x0029,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    		     break;  
		    
		    case 0x002A:
    		     Big_End_DwordtoByte(0x002A,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    			 break;

			case 0x002B:
    		     Big_End_DwordtoByte(0x002B,P);
    			 *(P+4)=4;
    			 memcpy(P+5,system_param.location_reporting_param.time_night,4);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    			 break;

			case 0x0055:
    			 Big_End_DwordtoByte(0x0055,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.max_speed,P+5);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    			 break;

			case 0x0056:
    			 Big_End_DwordtoByte(0x0056,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.speeding_duration,P+5);
    			 P=P+5+4;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+4;//���ݳ���
    			 break;

			case 0x0081:
    			 Big_End_DwordtoByte(0x0081,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
    			 P=P+5+2;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+2;//���ݳ���
    			 break;

			case 0x0082:
    			 Big_End_DwordtoByte(0x0082,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
    			 P=P+5+2;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+2;//���ݳ���
    			 break;

			case 0x0083:
    			 Big_End_DwordtoByte(0x0083,P);
    			 *(P+4)=strlen(system_param.terminl_registe.plate_number);
    			 memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
    			 P=P+5+strlen(system_param.terminl_registe.plate_number);//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);//���ݳ���
    			 break;

			case 0x0084:
    			 Big_End_DwordtoByte(0x0084,P);
    			 *(P+4)=1;
    			 *(P+5) = system_param.terminl_registe.plate_coror;
    			 P=P+5+1;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+1;//���ݳ���
    			 break;	
			
			case 0x1001:
      			 Big_End_DwordtoByte(0x1001,P);
      			 *(P+4)=strlen(system_param.SaveTerminlPhone.terminl_phone_nums);
      			 memcpy(P+5,system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums));
      			 P=P+5+strlen(system_param.SaveTerminlPhone.terminl_phone_nums);//��ַƫ��
      			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveTerminlPhone.terminl_phone_nums);//���ݳ���
      			 break;

			case 0x1002:
    			 Big_End_DwordtoByte(0x1002,P);
    			 *(P+4)=strlen(system_param.phone_number_param.sos_telephone);
    			 memcpy(P+5,system_param.phone_number_param.sos_telephone,strlen(system_param.phone_number_param.sos_telephone));
    			 P=P+5+strlen(system_param.phone_number_param.sos_telephone);//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.sos_telephone);//���ݳ���
    			 break;


			case 0x1003:
    			 Big_End_DwordtoByte(0x1003,P);
    			 *(P+4)=7;
    			 memcpy(P+5,system_param.terminl_registe.terminl_id,7);
    			 P=P+5+7;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+7;//���ݳ���
    			 break;

			case 0x1004:
       			 Big_End_DwordtoByte(0x1004,P);
       			 *(P+4)=17;
       			 memcpy(P+5,system_param.terminl_registe.vehicle_vin,17);
       			 P=P+5+17;//��ַƫ��
       			 Cnt_Num=Cnt_Num+5+17;//���ݳ���
       			 break;

			case 0x1005:
    			 Big_End_DwordtoByte(0x1005,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.vehicle_puse_num,P+5);
    			 P=P+5+2;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+2;//���ݳ���
    			 break;

			case 0x1006:
      			 Big_End_DwordtoByte(0x1006,P);
      			 *(P+4)=strlen(system_param.terminl_registe.vehicle_classification);
      			 memcpy(P+5,system_param.terminl_registe.vehicle_classification,strlen(system_param.terminl_registe.vehicle_classification));
      			 P=P+5+strlen(system_param.terminl_registe.vehicle_classification);//��ַƫ��
      			 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.vehicle_classification);//���ݳ���
      			 break;
			
			case 0x1007:
    			 Big_End_DwordtoByte(0x1007,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.location_reporting_param.sleep_delay,P+5);
    			 P=P+5+2;//��ַƫ��
    			 Cnt_Num=Cnt_Num+5+2;//���ݳ���
    			 break;	
			
			case 0x1010:
				 Big_End_DwordtoByte(0x1010,P);
				 *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
				 memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
				 P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//��ַƫ��
				 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//���ݳ���
			     break;			

			
			case 0x1013:
			{
				Big_End_DwordtoByte(0x1013,P);
				*(P+4)=strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
				memcpy(P+5,system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
				P=P+5+strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);//��ַƫ��
				Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);//���ݳ���
			}
			break;

			case 0x1018:
			Big_End_DwordtoByte(0x1018,P);
			*(P+4)=4;
			Big_End_DwordtoByte(system_param.SaveServerParameterArray[SECOND_SERVER].server_socket,P+5);
			P=P+5+4;//��ַƫ��
			Cnt_Num=Cnt_Num+5+4;//���ݳ���
			break;
			
			default:
			break;	
		}
	}
	/******����������*******/
	uart1_send_data_frame(demarcate_ask,Send_Data,Cnt_Num);
	
	#if demarcate_debug_printf 
	//HexToAsicc(Send_Data,Cnt_Num);
	#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void Setting_Data_Handle(unsigned char *Temp_data_buff)//USER3
 *  Description:  �������ݱ궨��ѯ 808 8103 Э����ͬ
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
void board_demarcate_data_pro(uint8_t *param_point,uint16_t data_len)
{
   if(*param_point==0x00) 
   {
        demarcate_data_proc(param_point+1);
        uart1_send_data_frame(demarcate_ask,param_point+1,data_len-1);
        #if demarcate_debug_printf
        eat_trace("board_demarcate_data_pro-->> �յ��궨ָ��:0x95.");
        #endif
   }
   else
   {
        read_demarcate_data_proc(param_point+1);
        #if demarcate_debug_printf
        eat_trace("board_demarcate_data_pro-->> �յ��궨��ѯָ��:0x85.");
        #endif
   }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void test_result_send
 *  Description: ���Է���
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
 void test_result_send(uint8_t test_id,uint8_t reslut)
{
	uint8_t temp_buffer[2]=0;
	temp_buffer[0]=test_id;
	temp_buffer[1]=reslut;
    uart1_send_data_frame(test_data_ret,temp_buffer,2);
}
static void flow_program_delay(uint32_t delay)
{
    delay=delay/10;
    if(delay==0)delay=1;
    while(delay--);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Create__Send_Communication_Frame
 *  Description:   ͨ��֡��װ 
 *  Parameters :  *ID           :��������ID
 *				  *Temp_buffer  :�������ݵĻ���
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-9			 
 * =====================================================================================
 */ 
void create_send_rs485_frame(void)
 {
     uint8_t Data_buffer[3]={0x7a,0x7a,0x7a};
     eat_trace("create_send_rs485_frame-->> send 7a.");
     uart_485_set_send_porc();
     eat_uart_write(EAT_UART_2,Data_buffer,3);//��������
     uart_485_set_rev_porc();
 }

 /* 
* ===  FUNCTION  ======================================================================
*  Name       :  void Loca_Record_Test(u8 *pscr)
*  Description: ������ʾ��  
*  Parameters : void
*       author: J.x.b  create at 2014-5-28
* =====================================================================================
*/
void loca_record_test(uint8_t *pscr)
{
   char* buf_ptr = NULL;
   buf_ptr = strstrLast((const char *)pscr,"+CREC: 2,1,0,5");
   if(buf_ptr!=NULL)
   {
       eat_modem_write("AT+CREC=4,1,0,99\r\n",strlen("AT+CREC=4,1,0,99\r\n"));//¼��
   }
   else
   {
       buf_ptr = strstrLast((const char *)pscr,"+CREC: 0");
       if(buf_ptr!=NULL)
       {
           test_result_send(0x03,0x00);
           eat_modem_write("AT+CREC=3,1\r\n",strlen("AT+CREC=3,1\r\n"));//¼��
           record_test_flag = EAT_FALSE;
       }
   }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void system_prouct_test()//USER3
 *  Description:  ϵͳ����
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
void system_prouct_test(uint8_t data)
{   
    uint8_t test_tf_data=0x09;
    switch(data)
    {
        case 0x00:
        file_create(test_tf_file);
        file_write(test_tf_file,0,&test_tf_data,sizeof(test_tf_data));
        test_tf_data=0;
        file_read(test_tf_file,0,&test_tf_data,1);
        if(test_tf_data==0x09)
        {
            test_result_send(0x00,0x00);
        }
        else
        {
            test_result_send(0x00,0x01);
        }
        eat_fs_Delete(test_tf_file);
        break;

        case 0x01:
        create_send_rs485_frame();//485����
        break;

        case 0x02://���VIN
        memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));//����VIN
        system_para_save_at_once();
        vehicle_infor_seting(0x03);
        break;
        case 0x03:
        record_test_flag = EAT_TRUE; 
        eat_modem_write("At+CREC=1,1,0,5\r\n",strlen("At+CREC=1,1,0,5\r\n"));//¼��
        break;
        case 0x04:
        
            break;
        default:
            break;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void system_prouct_demate()//USER3
 *  Description:  ���ݱ궨
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
void system_prouct_demate(uint8_t *data)
{
    switch(*data)
    {
    	case 0x00://�ն�ID
    	memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
    	memcpy(system_param.terminl_registe.terminl_id,data+1,7);
    	break;
    	
    	case 0x01:
    	memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));
    	memcpy(system_param.terminl_registe.vehicle_vin,data+1,17);//����VIN
    	break;
    	
    	case 0x02 :
        memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
    	memcpy(system_param.terminl_registe.plate_number,data+1,12);//���ƺ�
    	break;

    	case 0x03:
    	Big_End_BytetoWord(data+1,&system_param.terminl_registe.vehicle_puse_num);
    	break;

    	case 0x04:
        memset(system_param.terminl_registe.vehicle_classification,0,sizeof(system_param.terminl_registe.vehicle_classification));
    	memcpy(system_param.terminl_registe.vehicle_classification,data+1,12);
    	break;
    	default:
    	break;
    }
    system_para_save_at_once();
}

