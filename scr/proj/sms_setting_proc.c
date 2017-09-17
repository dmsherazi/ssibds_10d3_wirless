/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   sms_setting_proc.c.c
 * version       :   0.1
 * Description   :   Founction of this Source of C;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2016/3/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 * ʵ�ֶ������ã���ѯ����
 *-------------------------------------------------------------------- */


#include "sms_setting_proc.h"

uint8_t sms_setting_key[7]="123456";
uint8_t sms_cmd_rest_cmd=EAT_FALSE;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qver_msg_pro
 *  Description: ���Ų�ѯ����
 *        Input:            
 *       Output:  *��������ssbds-1d03:�ն�ID,�汾��* 
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void qver_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   /**********��������ssbds-1d03:�ն�ID,�汾��************/
   memset(sms_envelope,0,sizeof(sms_envelope));
   sprintf(sms_envelope,"ssi-terminal:%s,%s",system_param.terminl_registe.terminl_id,system_param.terminl_registe.softerware_vison);
   #if sms_setting_debug_printf
   eat_trace("qver_msg_pro-->>sprintf sms_envelope:%s,data_len:%d.",sms_envelope,strlen(sms_envelope));
   #endif
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qgps_sate_msg_pro
 *  Description: ���Ų�ѯ����
 *        Input:            
 *       Output:  *��������ssbds-1d03:�ն�ID,gps״̬* 
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void qgps_sate_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   /**********��������ssbds-1d03:�ն�ID,gps״̬************/
   if(Real_Time_Location_Inf.VDR_State_Inf.State_Bit.Wether_Location)
   {
       sprintf(sms_envelope,"ssi-terminal:%s,gps positioned",system_param.terminl_registe.terminl_id);
   }
   else
   {
      sprintf(sms_envelope,"ssi-terminal:%s,gps not positioned",system_param.terminl_registe.terminl_id);
   }
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qgprs_sate_msg_pro
 *  Description: ���Ų�ѯgprs����
 *        Input:            
 *       Output:  *��������ssbds-1d03:�ն�ID,gprs״̬* 
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void qgprs_sate_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   /**********��������ssbds-1d03:�ն�ID,gprs״̬************/

   /**********SIM���ǲ���************/
   if(Wether_SIM_DEF==EAT_TRUE)
   {
        /**********SIM���Ƿ����************/
       if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
       {
           /**********SIM��δ����************/
           if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
           {
               /**********gprs���ӳɹ�************/
               sprintf(sms_envelope,"ssi-terminal:%s,gprs link sucess !!",system_param.terminl_registe.terminl_id);
           }
           else
           {
               /**********gprsδ���ӳɹ�************/
               sprintf(sms_envelope,"ssi-terminal:%s,gprs link fail !!",system_param.terminl_registe.terminl_id);
           }
       }
       else
       {
           /**********gprsδ����************/
           sprintf(sms_envelope,"ssi-terminal:%s,gprs active is fail !!",system_param.terminl_registe.terminl_id);
       }
   }
   else
   {
       /**********SIM��δ����************/
       sprintf(sms_envelope,"ssi-terminal:%s,sim card is not insight!!",system_param.terminl_registe.terminl_id);
   }
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: quip_sate_msg_pro
 *  Description: ���Ӳ�����ѯ
 *        Input:            
 *       Output:  ��һ���������ݣ��������룬��һ������APN��TCP IP��TCP�˿ڣ�UDP IP��UDP�˿ڣ�GPRS����ģʽ��
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void quip_sate_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   uint8_t int_to_ascall[32]={0};
   uint8_t offest_len=0;
   /**********��һ����������************/
   offest_len+=sprintf(sms_envelope,"ssi-terminal:%s,",system_param.terminl_registe.terminl_id);

   memcpy(sms_envelope+offest_len,system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums));
   offest_len+=strlen(system_param.SaveTerminlPhone.terminl_phone_nums);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   memcpy(sms_envelope+offest_len,system_param.SaveMainBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
   offest_len+=strlen(system_param.SaveMainBeareParameter.beare_apn);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   memcpy(sms_envelope+offest_len,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
   offest_len+=strlen(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   itoa(system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   memcpy(sms_envelope+offest_len,"TCP",strlen("TCP"));
   offest_len += strlen("TCP");
   
   memcpy(sms_envelope+offest_len,";",strlen(";"));
   offest_len += strlen(";");
   
   send_sms_data_pro(sms_envelope,scr_num);

   
   /**********�ڶ�����������************/
   memset(sms_envelope,0,sizeof(sms_envelope));
   offest_len=0;
   
   offest_len+=sprintf(sms_envelope,"ssi-terminal:%s,",system_param.terminl_registe.terminl_id);

   memcpy(sms_envelope+offest_len,system_param.SaveTerminlPhone.terminl_phone_nums,strlen(system_param.SaveTerminlPhone.terminl_phone_nums));
   offest_len+=strlen(system_param.SaveTerminlPhone.terminl_phone_nums);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   memcpy(sms_envelope+offest_len,system_param.SaveBackupBeareParameter.beare_apn,strlen(system_param.SaveMainBeareParameter.beare_apn));
   offest_len+=strlen(system_param.SaveBackupBeareParameter.beare_apn);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   memcpy(sms_envelope+offest_len,system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));
   offest_len+=strlen(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);

   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.SaveServerParameterArray[SECOND_SERVER].server_socket,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   itoa(system_param.SaveServerParameterArray[SECOND_SERVER].udp_server_socket,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   memcpy(sms_envelope+offest_len,"TCP",strlen("TCP"));
   offest_len += strlen("TCP");
   
   memcpy(sms_envelope+offest_len,";",strlen(";"));
   offest_len += strlen(";");
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qgprs_sate_msg_pro
 *  Description: ���Ų�ѯgprs����
 *        Input:            
 *       Output:  ��һ���������Ӳ�������
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static uint8_t first_link_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("link_param_setting_msg_pro--> SZCS:%s",buffer);
   #endif
   start=0;
   
   end=GetComma(1, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveMainBeareParameter.beare_apn,0,sizeof(system_param.SaveMainBeareParameter.beare_apn));
   memcpy(system_param.SaveMainBeareParameter.beare_apn,buffer+start,end-start-1);
   start=end;
   
   end=GetComma(2, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin));
   memcpy(system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,buffer+start,end-start-1);
   start=end;

   
   end=GetComma(3,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   start=end;
   system_param.SaveServerParameterArray[MAIN_SERVER].server_socket=atoi(assical_buf);
   start=end;
   
   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memset(assical_buf,0,sizeof(assical_buf));
   memcpy(assical_buf,buffer+start,strlen(buffer)-start);
   system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket=atoi(assical_buf);

   #if sms_setting_debug_printf
   eat_trace("link_param_setting_msg_pro--> apn:%s",system_param.SaveMainBeareParameter.beare_apn);
   eat_trace("link_param_setting_msg_pro--> main:%s",system_param.SaveServerParameterArray[MAIN_SERVER].server_domin);
   eat_trace("link_param_setting_msg_pro--> stsok:%d",system_param.SaveServerParameterArray[MAIN_SERVER].server_socket);
   eat_trace("link_param_setting_msg_pro--> utsok:%d",system_param.SaveServerParameterArray[MAIN_SERVER].udp_server_socket);
   #endif
   system_para_save_at_once();
   
   return 0x01;
}
static uint8_t first_link_buckup_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("first_link_buckup_param_setting_msg_pro--> SZBF:%s",buffer);
   #endif
   start=0;

   end=GetComma(1, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveMainBeareParameter.beare_apn,0,sizeof(system_param.SaveMainBeareParameter.beare_apn));
   memcpy(system_param.SaveBackupBeareParameter.beare_apn,buffer+start,end-start-1);
   start=end;
   
   end=GetComma(2, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin));
   memcpy(system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,buffer+start,end-start-1);
   start=end;

   
   end=GetComma(3,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_socket=atoi(assical_buf);
   start=end;

   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memset(assical_buf,0,sizeof(assical_buf));
   memcpy(assical_buf,buffer+start,strlen(buffer)-start);
   system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].udp_server_socket=atoi(assical_buf);

   #if sms_setting_debug_printf
   eat_trace("first_link_buckup_param_setting_msg_pro--> apn:%s",system_param.SaveMainBeareParameter.beare_apn);
   eat_trace("first_link_buckup_param_setting_msg_pro--> main:%s",system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
   eat_trace("first_link_buckup_param_setting_msg_pro--> stsok:%d",system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
   eat_trace("first_link_buckup_param_setting_msg_pro--> utsok:%d",system_param.SaveServerParameterArray[SECOND_SERVER].udp_server_socket);
   #endif
   system_para_save_at_once();
   
   return 0x01;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: second_link_param_setting_msg_pro
 *  Description: ���Ų�ѯgprs����
 *        Input:            
 *       Output:  �ڶ����������Ӳ�������
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static uint8_t second_link_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("second_link_param_setting_msg_pro--> SZEF:%s",buffer);
   #endif
   start=0;
   memset(system_param.SaveBackupBeareParameter.beare_apn,0,sizeof(system_param.SaveBackupBeareParameter.beare_apn));
   memset(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin));

   end=GetComma(1, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(system_param.SaveBackupBeareParameter.beare_apn,buffer+start,end-start-1);
   start=end;
   
   end=GetComma(2, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,buffer+start,end-start-1);
   start=end;

   
   end=GetComma(3,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.SaveServerParameterArray[SECOND_SERVER].server_socket=atoi(assical_buf);
   start=end;

   memset(assical_buf,0,sizeof(assical_buf));
   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,strlen(buffer)-start);
   system_param.SaveServerParameterArray[SECOND_SERVER].udp_server_socket=atoi(assical_buf);

   #if sms_setting_debug_printf
   eat_trace("second_link_param_setting_msg_pro--> apn:%s",system_param.SaveBackupBeareParameter.beare_apn);
   eat_trace("second_link_param_setting_msg_pro--> main:%s",system_param.SaveServerParameterArray[SECOND_SERVER].server_domin);
   eat_trace("second_link_param_setting_msg_pro--> stsok:%d",system_param.SaveServerParameterArray[SECOND_SERVER].server_socket);
   eat_trace("second_link_param_setting_msg_pro--> utsok:%d",system_param.SaveServerParameterArray[SECOND_SERVER].udp_server_socket);
   #endif
   system_para_save_at_once();
   
   return 0x01;
}
static uint8_t second_link_backup_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("second_link_backup_param_setting_msg_pro--> SZDF:%s",buffer);
   #endif
   start=0;

   end=GetComma(1, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveBackupBeareParameter.beare_apn,0,sizeof(system_param.SaveBackupBeareParameter.beare_apn));
   memcpy(system_param.SaveBackupBeareParameter.beare_apn,buffer+start,end-start-1);
   start=end;
   
   end=GetComma(2, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,0,sizeof(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin));
   memcpy(system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,buffer+start,end-start-1);
   start=end;

   
   end=GetComma(3,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   start=end;
   system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_socket=atoi(assical_buf);
   start=end;

   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memset(assical_buf,0,sizeof(assical_buf));
   memcpy(assical_buf,buffer+start,strlen(buffer)-start);
   system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].udp_server_socket=atoi(assical_buf);

   #if sms_setting_debug_printf
   eat_trace("second_link_backup_param_setting_msg_pro--> apn:%s",system_param.SaveBackupBeareParameter.beare_apn);
   eat_trace("second_link_backup_param_setting_msg_pro--> main:%s",system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin);
   eat_trace("second_link_backup_param_setting_msg_pro--> stsok:%d",system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_socket);
   eat_trace("second_link_backup_param_setting_msg_pro--> utsok:%d",system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].udp_server_socket);
   #endif
   system_para_save_at_once();
   return 0x01;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: reg_param_setting_msg_pro
 *  Description: ע���������
 *        Input: ����ע��������ò�����VIN      SREG      
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static uint8_t sreg_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("sreg_param_setting_msg_pro--> SREG:%s",buffer);
   #endif

   /*****ʡ��*****/
   start=0;
   end=GetComma(1,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.provincial_id=(uint16_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;

   /*****����*****/
   start=0;
   end=GetComma(2,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.county_id=(uint16_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;
   
   /*****������ID*****/
   end=GetComma(3, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.manufacture_id,0,sizeof(system_param.terminl_registe.manufacture_id));
   memcpy(system_param.terminl_registe.manufacture_id,buffer+start,end-start-1);
   start=end;
   
   /*****�ն��ͺ�*****/
   end=GetComma(4, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.terminl_model,0,sizeof(system_param.terminl_registe.terminl_model));
   memcpy(system_param.terminl_registe.terminl_model,buffer+start,end-start-1);
   start=end;
   
   /*****�ն�ID*****/
   end=GetComma(5, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
   memcpy(system_param.terminl_registe.terminl_id,buffer+start,end-start-1);
   start=end;

   /*****������ɫ*****/
   end=GetComma(6,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.plate_coror =(uint8_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;
   
   /*****���ƺ�*****/
   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
   memcpy(system_param.terminl_registe.plate_number,buffer+start,strlen(buffer)-start);
   
   #if sms_setting_debug_printf
   eat_trace("sreg_param_setting_msg_pro--> terminl_id:%s",system_param.terminl_registe.terminl_id);
   eat_trace("sreg_param_setting_msg_pro--> plate_coror:%d",system_param.terminl_registe.plate_coror);
   eat_trace("sreg_param_setting_msg_pro--> plate_number:%s",system_param.terminl_registe.plate_number);
   #endif
   system_para_save_at_once();
   
   return 0x01;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qreg_sate_msg_pro
 *  Description: ע���������
 *        Input: ����ע�������ѯ������VIN      QREG     
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static void qreg_sate_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   uint8_t int_to_ascall[32]={0};
   uint8_t offest_len=0;
   /**********ʡ��ID************/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.provincial_id,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /**********����ID************/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.county_id,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****������ID*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.manufacture_id,strlen(system_param.terminl_registe.manufacture_id));
   offest_len += strlen(system_param.terminl_registe.manufacture_id);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****�ն��ͺ�*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.terminl_model,strlen(system_param.terminl_registe.terminl_model));
   offest_len += strlen(system_param.terminl_registe.terminl_model);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****�ն�ID*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.terminl_id,strlen(system_param.terminl_registe.terminl_id));
   offest_len += strlen(system_param.terminl_registe.terminl_id);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   /*****������ɫ*****/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.plate_coror,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += sizeof(system_param.terminl_registe.plate_coror);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****���ƺ�*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
   offest_len += strlen(system_param.terminl_registe.plate_number);
   
   memcpy(sms_envelope+offest_len,";",strlen(";"));
   offest_len += strlen(";");
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: ssisreg_param_setting_msg_pro
 *  Description: ע��������ð���VIN
 *        Input: ����ע�������ѯ������VIN      SSISREG     
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static uint8_t ssisreg_param_setting_msg_pro(uint8_t *buffer)
{
   uint8_t start,end;
   uint8_t assical_buf[32]={0};
   
   #if sms_setting_debug_printf
   eat_trace("ssisreg_param_setting_msg_pro--> SSISREG:%s",buffer);
   #endif

   /*****ʡ��*****/
   start=0;
   end=GetComma(1,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.provincial_id=(uint16_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;

   /*****����*****/
   start=0;
   end=GetComma(2,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.county_id=(uint16_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;
   
   /*****�ն�ID*****/
   end=GetComma(3, buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.terminl_id,0,sizeof(system_param.terminl_registe.terminl_id));
   memcpy(system_param.terminl_registe.terminl_id,buffer+start,end-start-1);
   start=end;

   /*****������ɫ*****/
   end=GetComma(4,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memcpy(assical_buf,buffer+start,end-start-1);
   system_param.terminl_registe.plate_coror =(uint8_t)atoi(assical_buf);
   memset(assical_buf,0,sizeof(assical_buf));
   start=end;
   
   /*****���ƺ�*****/
   end=GetComma(5,buffer,',');
   if(end==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.plate_number,0,sizeof(system_param.terminl_registe.plate_number));
   memcpy(system_param.terminl_registe.plate_number,buffer+start,end-start-1);
   start=end;

   /*****VIN*****/
   
   if((strlen(buffer)-start)==0)
   {
       return 0x00;
   }
   memset(system_param.terminl_registe.vehicle_vin,0,sizeof(system_param.terminl_registe.vehicle_vin));
   memcpy(system_param.terminl_registe.vehicle_vin,buffer+start,strlen(buffer)-start);
   system_para_save_at_once();
   
   #if sms_setting_debug_printf
   eat_trace("ssisreg_param_setting_msg_pro--> terminl_id:%s",system_param.terminl_registe.terminl_id);
   eat_trace("ssisreg_param_setting_msg_pro--> plate_coror:%d",system_param.terminl_registe.plate_coror);
   eat_trace("ssisreg_param_setting_msg_pro--> plate_number:%s",system_param.terminl_registe.plate_number);
   eat_trace("ssisreg_param_setting_msg_pro--> vehicle_vin:%s",system_param.terminl_registe.vehicle_vin);
   #endif
   
   return 0x01;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qssisreg_param_msg_pro
 *  Description: ע�������ѯ����VIN
 *        Input: ����ע�������ѯ������VIN      SSISREG     
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static void qssisreg_param_msg_pro(uint8_t *scr_num)
{
   uint8_t sms_envelope[128]={0};
   uint8_t int_to_ascall[32]={0};
   uint8_t offest_len=0;
   /**********ʡ��ID************/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.provincial_id,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /**********����ID************/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.county_id,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += strlen(int_to_ascall);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   /*****�ն�ID*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.terminl_id,strlen(system_param.terminl_registe.terminl_id));
   offest_len += strlen(system_param.terminl_registe.terminl_id);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");

   /*****������ɫ*****/
   memset(int_to_ascall,0,sizeof(int_to_ascall));
   itoa(system_param.terminl_registe.plate_coror,int_to_ascall);
   memcpy(sms_envelope+offest_len,int_to_ascall,strlen(int_to_ascall));
   offest_len += sizeof(system_param.terminl_registe.plate_coror);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****���ƺ�*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.plate_number,strlen(system_param.terminl_registe.plate_number));
   offest_len += strlen(system_param.terminl_registe.plate_number);
   memcpy(sms_envelope+offest_len,",",strlen(","));
   offest_len += strlen(",");
   
   /*****VIN*****/
   memcpy(sms_envelope+offest_len,system_param.terminl_registe.vehicle_vin,strlen(system_param.terminl_registe.vehicle_vin));
   offest_len += strlen(system_param.terminl_registe.vehicle_vin);

   memcpy(sms_envelope+offest_len,";",strlen(";"));
   offest_len += strlen(";");
   send_sms_data_pro(sms_envelope,scr_num);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: lock_car_sms_cmd_proc
 *  Description: ���ſ���
 *        Input: ����������         
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static uint8_t lock_car_sms_cmd_proc(uint8_t cmd_data)
{
    uint8_t offest=0;
    uint8_t data_buffer[8]={0};
    data_buffer[offest]=cmd_data;
    offest+=sizeof(uint8_t);
    data_buffer[offest]=0xFF;
    offest+=sizeof(uint8_t);
    data_buffer[offest]=0xFF;
    offest+=sizeof(uint8_t);
    lock_car_strategy_send_proc(0xF001,data_buffer,offest);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: qssisreg_param_msg_pro
 *  Description: ע�������ѯ����VIN
 *        Input: ����ע�������ѯ������VIN      SSISREG     
 *       Output:  
 *       Return:               
 *       author: jxb.2016.11.20
 * =====================================================================================
 */
static uint8_t lock_car_sms_proc(uint8_t *buffer,uint8_t *scr_num)
{
    uint8_t start,end;
    uint8_t index_cmd;
    int check_result;
    uint8_t assical_buf[32]={0};
    
    #if sms_setting_debug_printf
    eat_trace("lock_car_sms_proc--> lock_car data :%s",buffer);
    #endif
    
    /*****������*****/
    start=0;
    end=GetComma(1,buffer,',');
    if(end==0)
    {
        return 0x00;
    }
    memcpy(assical_buf,buffer+start,end-start-1);
    index_cmd=(uint8_t)atoi(assical_buf);
    memset(assical_buf,0,sizeof(assical_buf));
    start=end;
    
    /*****������*****/
    end=GetComma(1,buffer,'!');
    if((strlen(buffer)-start)==0)
    {
        return 0x00;
    }
    memcpy(assical_buf,buffer+start,end-start-1);
    check_result=check_md5_encryption(assical_buf,scr_num);
    if(check_result==0)
    {
        switch(index_cmd)
        {
            case 0x01:
                 lock_car_sms_cmd_proc(0x02);
                 break;
            case 0x02:
                 lock_car_sms_cmd_proc(0x05);
                 break;
            case 0x03:
                 lock_car_sms_cmd_proc(0x07);
                 break;
            case 0x04:
                 lock_car_sms_cmd_proc(0x0A);
                 break;
            default:
                 return 0x00;
                 break;
        }
        return 0x01;
    }
    else
    {
        return 0x00;
    }
    
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: sms_setting_proc
 *  Description: *sms_buffe:�������� sms_len :���ų��ȣ�scr_num:Ŀ�����
 *        Input:            
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
uint8_t sms_action_proc(uint8_t *sms_buffe,uint16_t sms_len,uint8_t *scr_num)
{
	uint8_t ill_key_ascall[7]={0};
    uint8_t *phead=NULL;
	int check_md5_ret=0;
    phead=(char *)strstrLast(sms_buffe,"99123456INTS");
    if(phead!=NULL)
    {
         if(lock_car_sms_proc(phead,scr_num))
         {
             send_sms_data_pro("INTS ok !",scr_num);
            return set_ok;
         }
         else
         {
             send_sms_data_pro("INTS error !",scr_num);
             set_format_error;
         }
    }
    else
    {
        /**********У���Ƿ�Ϊ�Ϸ�����*************/
        if(strncmp(sms_buffe,"99",2)==0)
        {
            /**********У���������************/
            if(strncmp(sms_buffe+2,sms_setting_key,6)==0)
            {
                /**********�汾�Ų�ѯ************/
                if(strncmp(sms_buffe+8,"QVER",4)==0) //�汾��ѯ
                {
                    qver_msg_pro(scr_num);
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_setting-->>QVER");
                    #endif
                    return set_ok;
                } /**********gps״̬��ѯ************/
                else if(strncmp(sms_buffe+8,"QGPS",4)==0)//GPS״̬��ѯ
                {
                    qgps_sate_msg_pro(scr_num);
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_setting-->>QGPS");
                    #endif
                    return set_ok;
                }/**********gprs״̬��ѯ************/
                else if(strncmp(sms_buffe+8,"QNET",4)==0)//����״̬��ѯ
                {
                    qgprs_sate_msg_pro(scr_num);
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_setting-->>QNET");
                    #endif
                    return set_ok;
                }/**********�ն˸�λ����************/
                else if(strncmp(sms_buffe+8,"QREST",strlen("QREST"))==0)//�ն˸�λ
                {
                    sms_cmd_rest_cmd=EAT_TRUE;
                    send_sms_data_pro("restart ok",scr_num);
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_setting-->>QREST");
                    #endif
                    return set_ok;
                }/**********��ѯIP��Ϣ************/
                else if(strncmp(sms_buffe+8,"QUIP",strlen("QUIP"))==0)//�ն˸�λ
                {
                    quip_sate_msg_pro(scr_num);
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_setting-->>QUIP");
                    #endif
                    return set_ok;
                }/**********���õ�һ������Ϣ************/
                else if(strncmp(sms_buffe+8,"SZCS",strlen("SZCS"))==0)
                {
                    if(first_link_param_setting_msg_pro(sms_buffe+12))
                    {
                        send_sms_data_pro("SZCS ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SZCS sms format error please check !",scr_num);
                        return set_format_error;
                    }
                }/**********���õ�һ����������Ϣ************/
                else if(strncmp(sms_buffe+8,"SZBF",strlen("SZBF"))==0)
                {
                    if(first_link_buckup_param_setting_msg_pro(sms_buffe+12))
                    {
                        send_sms_data_pro("SZBF ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SZBF sms format error please check !",scr_num);
                        return set_format_error;
                    }
                }/**********���õڶ�������Ϣ************/
                else if(strncmp(sms_buffe+8,"SZEF",strlen("SZEF"))==0)
                {
                    if(second_link_param_setting_msg_pro(sms_buffe+12))
                    {
                        send_sms_data_pro("SZEF ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SZEF sms format error please check !",scr_num);
                        return set_format_error;
                    }
                }/**********���õڶ�����������Ϣ************/
                else if(strncmp(sms_buffe+8,"SZDF",strlen("SZDF"))==0)
                {
                    if(second_link_backup_param_setting_msg_pro(sms_buffe+12))
                    {
                        send_sms_data_pro("SZDF ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SZDF sms format error please check !",scr_num);
                        return set_format_error;
                    }
                }/**********����ע����Ϣ************/
                else if(strncmp(sms_buffe+8,"SREG",strlen("SREG"))==0)
                {
                    if(sreg_param_setting_msg_pro(sms_buffe+12))
                    {
                        send_sms_data_pro("SREG ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SREG sms format error please check !",scr_num);
                        return set_format_error;
                    }
                }/**********��ѯ��׼ע����Ϣ************/
                else if(strncmp(sms_buffe+8,"QREG",strlen("QREG"))==0)
                {
                    qreg_sate_msg_pro(scr_num);
                    return set_ok;
                }/**********����SSISREG��Ϣ************/
                else if(strncmp(sms_buffe+8,"SSISREG",strlen("SSISREG"))==0)
                {
                    if(ssisreg_param_setting_msg_pro(sms_buffe+15))
                    {
                        send_sms_data_pro("SSISREG ok !",scr_num);
                        return set_ok;
                    }
                    else
                    {
                        send_sms_data_pro("SSISREG sms format error please check !",scr_num);
                        return set_format_error;
                    }
                    return set_ok;
                }/**********��ѯSSISREG��Ϣ************/
                else if(strncmp(sms_buffe+8,"SSISREG",strlen("SSISREG"))==0)
                {
                    qssisreg_param_msg_pro(scr_num);
                    return set_ok;
                }
                else
                {
                    #if sms_setting_debug_printf
                    eat_trace("sms_action_proc_settin-->>mo define message!!");
                    #endif
                    return set_no_define;
                }
            }
            else
            {
               memset(ill_key_ascall,0,sizeof(ill_key_ascall));
			   memcpy(ill_key_ascall,sms_buffe+2,6);
               #if sms_setting_debug_printf
               eat_trace("sms_action_proc_settin-->>not illegal key:%s!!",ill_key_ascall);
               #endif
			   check_md5_ret=check_md5_encryption_sms1(ill_key_ascall,&(Real_Time_Location_Inf.Curren_Time[1]));
			   if(check_md5_ret==0)
			   {
				   if(strncmp(sms_buffe+8,"SZCS",strlen("SZCS"))==0)
				   {
					   if(first_link_param_setting_msg_pro(sms_buffe+12))
					   {
						   send_sms_data_pro("SZCS ok !",scr_num);
						   return set_ok;
					   }
					   else
					   {
						   send_sms_data_pro("SZCS sms format error please check !",scr_num);
						   return set_format_error;
					   }
				   }
				   else
				   {
					   send_sms_data_pro("SZCS sms format error please check !",scr_num);
					   return set_format_error;
				   }
			   }
			   else
			   {
				   check_md5_ret = check_md5_encryption_sms2(ill_key_ascall,&(Real_Time_Location_Inf.Curren_Time[1]));
				   if(check_md5_ret==0)
				   	{
					   if(strncmp(sms_buffe+8,"SZEF",strlen("SZEF"))==0)
					   {
						   if(second_link_param_setting_msg_pro(sms_buffe+12))
						   {
							   send_sms_data_pro("SZEF ok !",scr_num);
							   return set_ok;
						   }
						   else
						   {
							   send_sms_data_pro("SZEF sms format error please check !",scr_num);
							   return set_format_error;
						   }
					   }
					   else
					   {
						   send_sms_data_pro("SZEF sms format error please check !",scr_num);
						   return set_format_error;
					   }
				   	}
				    else
				    {
						send_sms_data_pro("sms key error !",scr_num);
						return set_format_error;
				    }
			   }
            }
        }
        else
        {
            #if sms_setting_debug_printf
            eat_trace("sms_action_proc_settin-->>illegal short message!!");
            #endif
            return set_head_error;
        }
    }
}

