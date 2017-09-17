

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
   *  Parameters : 设备参数标定
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
	parm_num = *Temp_data_buff;//参数个数
	Base_Data = Temp_data_buff+1;
    #if APP_TRACE_DBG 
    eat_trace("demarcate_data_proc --> param_num:%d.",parm_num);
    #endif
	for(param_cnt=0;param_cnt<parm_num;param_cnt++)
	{
	   Big_End_BytetoDword(Base_Data,&seting_id);
	   switch(seting_id)
	   {
			case 0x0010://主服务器 APN，无线通信拨号访问点
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc --> id:0x0010.");
                #endif
    			memset(system_param.SaveMainBeareParameter.beare_apn,0,strlen(system_param.SaveMainBeareParameter.beare_apn));
    			memcpy(system_param.SaveMainBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));	

                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0010-->主服务器 APN:%s.",system_param.SaveMainBeareParameter.beare_apn);
                #endif
    			break;
			case 0x0013://主服务器地址,IP 或域名			
                memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
                memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,Base_Data+5,*(Base_Data+4));

                if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
                vehicle_infor_seting(0x06);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0013-->主服务器地址,IP 或域名:%s.",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
                #endif
    			break;
			    
			case 0x0018://服务器 TCP 端口
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
    			Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket);
                if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_Min=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0018-->服务器 TCP 端口:%d.",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
                #endif
    			break;
			
		    case 0x0027://休眠时汇报时间间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.sleeping_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0027-->休眠时汇报时间间隔:%d.",system_param.location_reporting_param.sleeping_frze);
                #endif
			    break;
						
		    
		    case 0x0029://缺省时间汇报间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.default_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0028-->缺省时间汇报间隔:%d.",system_param.location_reporting_param.default_frze);
                #endif
		        break;  
		    
		    case 0x002A://晚上休眠时间间隔
		        Big_End_BytetoDword(Base_Data+5,&system_param.location_reporting_param.night_sleep_frze);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x002A-->晚上休眠时间间隔:%d.",system_param.location_reporting_param.night_sleep_frze);
                #endif
		        break;

		    case 0x002B://晚上休眠时间
                memset(system_param.location_reporting_param.time_night,0,sizeof(system_param.location_reporting_param.time_night));
                memcpy(system_param.location_reporting_param.time_night,Base_Data+5,*(Base_Data+4));
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x002B-->:晚上时间:%0x:%0x--%0x:%0x.",system_param.location_reporting_param.time_night[0],\
                system_param.location_reporting_param.time_night[1],system_param.location_reporting_param.time_night[2],system_param.location_reporting_param.time_night[3]);
                #endif
		        break;
		        
           case 0x0055://最高速度
                Big_End_BytetoDword(Base_Data+5,&system_param.max_speed);
                sp_seting=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id::0x0055-->最高速度:%d.",system_param.max_speed);
                #endif
                break;
                
			case 0x0056://超速持续时间
                Big_End_BytetoDword(Base_Data+5,&system_param.speeding_duration);
                sp_seting=EAT_TRUE;
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0056-->超速持续时间:%d.",system_param.speeding_duration);
                #endif
			    break;

			case 0x0081://车辆所在的省域 ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.provincial_id);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0081-->车辆所在的省域 ID:%d.",system_param.terminl_registe.provincial_id);
                #endif
			    break;

			case 0x0082:// 车辆所在的市域 ID
                Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.county_id);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0082-->车辆所在的市域 ID:%d.",system_param.terminl_registe.county_id);
                #endif
			    break;

			case 0x0083://公安交通管理部门颁发的机动车号牌
                memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
                memcpy(system_param.terminl_registe.plate_number,Base_Data+5,*(Base_Data+4));
                Re_Link_second = EAT_TRUE;
                Re_Regist_second = EAT_TRUE;
                
                vehicle_infor_seting(0x01);
                #if demarcate_debug_printf 
                eat_trace("demarcate_data_proc id:0x0083-->公安交通管理部门颁发的机动车号牌:%s.",system_param.terminl_registe.plate_number);
                #endif
			    break;

			case 0x0084://车牌颜色
 			     system_param.terminl_registe.plate_coror = *(Base_Data+5);
                 Re_Link_second = EAT_TRUE;
                 Re_Regist_second = EAT_TRUE;
                 #if demarcate_debug_printf 
                 eat_trace("demarcate_data_proc id:0x0084-->车牌颜色:%d.",system_param.terminl_registe.plate_coror);
                 #endif
 			     break;
			
			case 0x1001://主机号码
     			 memset(system_param.SaveTerminlPhone.terminl_phone_nums,0,sizeof(system_param.SaveTerminlPhone.terminl_phone_nums));
                 memcpy(system_param.SaveTerminlPhone.terminl_phone_nums,Base_Data+5,*(Base_Data+4));
                 memset(system_param.SaveTerminlPhone.terminl_phone_numd,0,6);
                 if(strlen(system_param.SaveTerminlPhone.terminl_phone_nums)!=0)
                 {
                     Porotorl_Phone_Num_Conver(system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums),system_param.SaveTerminlPhone.terminl_phone_numd);
                     Re_Regist_main=EAT_TRUE;
                     Re_Regist_second=EAT_TRUE;
                     #if demarcate_debug_printf
                     eat_trace("demarcate_data_proc id:0x1001-->> 终端手机号码:%s",system_param.SaveTerminlPhone.terminl_phone_nums);
                     #endif
                 }
 			     break;	

			case 0x1002://sos号码
     			 memset(system_param.phone_number_param.sos_telephone,0,sizeof(system_param.phone_number_param.sos_telephone));
                 memcpy(system_param.phone_number_param.sos_telephone,Base_Data+5,*(Base_Data+4));
                  
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1002-->> S0S手机号码:%s",system_param.phone_number_param.sos_telephone);
                 #endif
 			     break;
			
			case 0x1003://终端ID
                 memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
                 memcpy(system_param.terminl_registe.terminl_id,Base_Data+5,*(Base_Data+4));
                 Re_Regist_main=EAT_TRUE;
                 Re_Regist_second=EAT_TRUE;
                 
                 vehicle_infor_seting(0x05);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1003-->> 终端ID:%s",system_param.terminl_registe.terminl_id);
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
			
			case 0x1005://脉冲系数
                 Big_End_BytetoWord(Base_Data+5,&system_param.terminl_registe.vehicle_puse_num);
                 vehicle_infor_seting(0x04);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1005-->>  set 脉冲系数:%d",system_param.terminl_registe.vehicle_puse_num);
                 #endif
     			 break;	

			case 0x1006://车辆分类
     			 memset(system_param.terminl_registe.vehicle_classification,0,sizeof(system_param.terminl_registe.vehicle_classification));
                 memcpy(system_param.terminl_registe.vehicle_classification,Base_Data+5,*(Base_Data+4));
                 vehicle_infor_seting(0x02);
                  
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1006-->>  set 车辆分类:%s",system_param.terminl_registe.vehicle_classification);
                 #endif
     			 break;	

			case 0x1007://休眠延时
       		     Big_End_BytetoWord(Base_Data+5,&system_param.location_reporting_param.sleep_delay);
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1007-->>  set 休眠延时:%d",system_param.terminl_registe.vehicle_classification);
                 #endif
       			 break;	
			
			case 0x1010:
                 memset(system_param.SaveBackupBeareParameter.beare_apn,0,sizeof(system_param.SaveBackupBeareParameter.beare_apn));
                 memcpy(system_param.SaveBackupBeareParameter.beare_apn,Base_Data+5,*(Base_Data+4));
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1010-->> 第二服务器_APN:%s",system_param.SaveBackupBeareParameter.beare_apn);
                 #endif
     			 break;			

			case 0x1013:
                 memset(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
                 memcpy(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,Base_Data+5,*(Base_Data+4));
                 if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1013-->> 第二服务器域名:%s",system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
                 #endif
			     break;

			case 0x1018:
                 Big_End_BytetoDword(Base_Data+5,&system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
                 if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE) Re_Link_second=EAT_TRUE;
                 #if demarcate_debug_printf
                 eat_trace("demarcate_data_proc id:0x1014-->> 第二服务器端口:%d",system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
                 #endif
     			 break;
    	    
			default:
			    break;	
		}
	    Base_Data = Base_Data + *(Base_Data+4)+5;//下一次的首地址
	}
	/**********保存参数***************/
	 system_para_save_at_once();
	 if(sp_seting==EAT_TRUE)
	 {
         seting_data_to_board(0x00);
	 }
	/**********重连接设置***************/
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
    else
    {
        #if demarcate_debug_printf
        eat_trace("demarcate_data_proc id:0x1013-->> 第二服务器未开启，无动作  !");
        #endif
    }
 }
 
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void Setting_Data_Handle(unsigned char *Temp_data_buff)//USER3
 *  Description:  出厂数据标定查询 808 8103 协议相同
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
static void read_demarcate_data_proc(uint8_t *Temp_data_buff)
{
	
	uint8_t *P;//位置指针
	uint16_t Cnt;
	uint8_t Str_Lens;
	uint32_t Data_ID;
	uint8_t Send_Data[1056];
	uint16_t Cnt_Num=0; //数据个数
	*(Send_Data) = *Temp_data_buff;//应答个数
	Cnt_Num +=1; //数据个数
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
     			 P=P+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//地址偏移
     			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveMainBeareParameter.beare_apn);//数据长度
     			 break;

			case 0x0013:
    			 Big_End_DwordtoByte(0x0013,P);
    			 *(P+4) = strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
    			 memcpy(P+5,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
    			 P=P+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//地址偏移
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);//数据长度
    			 break;
			
			case 0x0018:
     			 Big_End_DwordtoByte(0x0018,P);
     			 *(P+4)=4;
     			 Big_End_DwordtoByte(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,P+5);
     			 P=P+5+4;//地址偏移
     			 Cnt_Num=Cnt_Num+5+4;//数据长度
     			 break;

			
		    case 0x0027:
    		     Big_End_DwordtoByte(0x0027,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.sleeping_frze,P+5);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    			 break;			
						
		    case 0x0029:
    		     Big_End_DwordtoByte(0x0029,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.default_frze,P+5);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    		     break;  
		    
		    case 0x002A:
    		     Big_End_DwordtoByte(0x002A,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.location_reporting_param.night_sleep_frze,P+5);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    			 break;

			case 0x002B:
    		     Big_End_DwordtoByte(0x002B,P);
    			 *(P+4)=4;
    			 memcpy(P+5,system_param.location_reporting_param.time_night,4);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    			 break;

			case 0x0055:
    			 Big_End_DwordtoByte(0x0055,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.max_speed,P+5);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    			 break;

			case 0x0056:
    			 Big_End_DwordtoByte(0x0056,P);
    			 *(P+4)=4;
    			 Big_End_DwordtoByte(system_param.speeding_duration,P+5);
    			 P=P+5+4;//地址偏移
    			 Cnt_Num=Cnt_Num+5+4;//数据长度
    			 break;

			case 0x0081:
    			 Big_End_DwordtoByte(0x0081,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.provincial_id,P+5);
    			 P=P+5+2;//地址偏移
    			 Cnt_Num=Cnt_Num+5+2;//数据长度
    			 break;

			case 0x0082:
    			 Big_End_DwordtoByte(0x0082,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.county_id,P+5);
    			 P=P+5+2;//地址偏移
    			 Cnt_Num=Cnt_Num+5+2;//数据长度
    			 break;

			case 0x0083:
    			 Big_End_DwordtoByte(0x0083,P);
    			 *(P+4)=strlen(system_param.terminl_registe.plate_number);
    			 memcpy(P+5,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
    			 P=P+5+strlen(system_param.terminl_registe.plate_number);//地址偏移
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.plate_number);//数据长度
    			 break;

			case 0x0084:
    			 Big_End_DwordtoByte(0x0084,P);
    			 *(P+4)=1;
    			 *(P+5) = system_param.terminl_registe.plate_coror;
    			 P=P+5+1;//地址偏移
    			 Cnt_Num=Cnt_Num+5+1;//数据长度
    			 break;	
			
			case 0x1001:
      			 Big_End_DwordtoByte(0x1001,P);
      			 *(P+4)=strlen(system_param.SaveTerminlPhone.terminl_phone_nums);
      			 memcpy(P+5,system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums));
      			 P=P+5+strlen(system_param.SaveTerminlPhone.terminl_phone_nums);//地址偏移
      			 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveTerminlPhone.terminl_phone_nums);//数据长度
      			 break;

			case 0x1002:
    			 Big_End_DwordtoByte(0x1002,P);
    			 *(P+4)=strlen(system_param.phone_number_param.sos_telephone);
    			 memcpy(P+5,system_param.phone_number_param.sos_telephone,strlen(system_param.phone_number_param.sos_telephone));
    			 P=P+5+strlen(system_param.phone_number_param.sos_telephone);//地址偏移
    			 Cnt_Num=Cnt_Num+5+strlen(system_param.phone_number_param.sos_telephone);//数据长度
    			 break;


			case 0x1003:
    			 Big_End_DwordtoByte(0x1003,P);
    			 *(P+4)=7;
    			 memcpy(P+5,system_param.terminl_registe.terminl_id,7);
    			 P=P+5+7;//地址偏移
    			 Cnt_Num=Cnt_Num+5+7;//数据长度
    			 break;

			case 0x1004:
       			 Big_End_DwordtoByte(0x1004,P);
       			 *(P+4)=17;
       			 memcpy(P+5,system_param.terminl_registe.vehicle_vin,17);
       			 P=P+5+17;//地址偏移
       			 Cnt_Num=Cnt_Num+5+17;//数据长度
       			 break;

			case 0x1005:
    			 Big_End_DwordtoByte(0x1005,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.terminl_registe.vehicle_puse_num,P+5);
    			 P=P+5+2;//地址偏移
    			 Cnt_Num=Cnt_Num+5+2;//数据长度
    			 break;

			case 0x1006:
      			 Big_End_DwordtoByte(0x1006,P);
      			 *(P+4)=strlen(system_param.terminl_registe.vehicle_classification);
      			 memcpy(P+5,system_param.terminl_registe.vehicle_classification,strlen(system_param.terminl_registe.vehicle_classification));
      			 P=P+5+strlen(system_param.terminl_registe.vehicle_classification);//地址偏移
      			 Cnt_Num=Cnt_Num+5+strlen(system_param.terminl_registe.vehicle_classification);//数据长度
      			 break;
			
			case 0x1007:
    			 Big_End_DwordtoByte(0x1007,P);
    			 *(P+4)=2;
    			 Big_End_WordtoByte(system_param.location_reporting_param.sleep_delay,P+5);
    			 P=P+5+2;//地址偏移
    			 Cnt_Num=Cnt_Num+5+2;//数据长度
    			 break;	
			
			case 0x1010:
				 Big_End_DwordtoByte(0x1010,P);
				 *(P+4)=strlen(system_param.SaveBackupBeareParameter.beare_apn);
				 memcpy(P+5,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveBackupBeareParameter.beare_apn));
				 P=P+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//地址偏移
				 Cnt_Num=Cnt_Num+5+strlen(system_param.SaveBackupBeareParameter.beare_apn);//数据长度
			     break;			

			
			case 0x1013:
			{
				Big_End_DwordtoByte(0x1013,P);
				*(P+4)=strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
				memcpy(P+5,system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
				P=P+5+strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);//地址偏移
				Cnt_Num=Cnt_Num+5+strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);//数据长度
			}
			break;

			case 0x1018:
			Big_End_DwordtoByte(0x1018,P);
			*(P+4)=4;
			Big_End_DwordtoByte(system_param.SaveServerParameterArray[SECOND_SERVER].server_socket,P+5);
			P=P+5+4;//地址偏移
			Cnt_Num=Cnt_Num+5+4;//数据长度
			break;
			
			default:
			break;	
		}
	}
	/******完成组包发送*******/
	uart1_send_data_frame(demarcate_ask,Send_Data,Cnt_Num);
	
	#if demarcate_debug_printf 
	//HexToAsicc(Send_Data,Cnt_Num);
	#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void Setting_Data_Handle(unsigned char *Temp_data_buff)//USER3
 *  Description:  出厂数据标定查询 808 8103 协议相同
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
        eat_trace("board_demarcate_data_pro-->> 收到标定指令:0x95.");
        #endif
   }
   else
   {
        read_demarcate_data_proc(param_point+1);
        #if demarcate_debug_printf
        eat_trace("board_demarcate_data_pro-->> 收到标定查询指令:0x85.");
        #endif
   }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void test_result_send
 *  Description: 测试返回
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
 *  Description:   通信帧封装 
 *  Parameters :  *ID           :发送数据ID
 *				  *Temp_buffer  :发送数据的缓存
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
     eat_uart_write(EAT_UART_2,Data_buffer,3);//发送数据
     uart_485_set_rev_porc();
 }

 /* 
* ===  FUNCTION  ======================================================================
*  Name       :  void Loca_Record_Test(u8 *pscr)
*  Description: 开机提示音  
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
       eat_modem_write("AT+CREC=4,1,0,99\r\n",strlen("AT+CREC=4,1,0,99\r\n"));//录音
   }
   else
   {
       buf_ptr = strstrLast((const char *)pscr,"+CREC: 0");
       if(buf_ptr!=NULL)
       {
           test_result_send(0x03,0x00);
           eat_modem_write("AT+CREC=3,1\r\n",strlen("AT+CREC=3,1\r\n"));//录音
           record_test_flag = EAT_FALSE;
       }
   }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:   void system_prouct_test()//USER3
 *  Description:  系统测试
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
        create_send_rs485_frame();//485测试
        break;

        case 0x02://清除VIN
        memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));//储存VIN
        system_para_save_at_once();
        vehicle_infor_seting(0x03);
        break;
        case 0x03:
        record_test_flag = EAT_TRUE; 
        eat_modem_write("At+CREC=1,1,0,5\r\n",strlen("At+CREC=1,1,0,5\r\n"));//录音
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
 *  Description:  数据标定
 *  author     :  J.x.b create at SSI_1D02 at 2015-9-1		 
 * =====================================================================================
 */
void system_prouct_demate(uint8_t *data)
{
    switch(*data)
    {
    	case 0x00://终端ID
    	memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
    	memcpy(system_param.terminl_registe.terminl_id,data+1,7);
    	break;
    	
    	case 0x01:
    	memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));
    	memcpy(system_param.terminl_registe.vehicle_vin,data+1,17);//储存VIN
    	break;
    	
    	case 0x02 :
        memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
    	memcpy(system_param.terminl_registe.plate_number,data+1,12);//车牌号
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

