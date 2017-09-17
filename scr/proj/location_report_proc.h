/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Eat_App_Protoco_Locat_Report_.h
 *  Description: The function of Location information reporting
 *        
 *       author: J.x.b  create at 2014-10-14
 * =====================================================================================
 */

 #ifndef EAT_APP_PROTOCOL_LOCATION_REPORT_H
	#define EAT_APP_PROTOCOL_LOCATION_REPORT_H

    #include"eat_interface.h"


#define QuYu_Panduan 5


#define TCP_Location_Update_ID      0x0704
#define Tcp_Location_Ask_Req_ID     0x0201//λ����Ϣ��ѯӦ��

#define Location_Package_Max 12



extern uint16_t tracking_time_interval;//����ʱ���� 0 ֹͣ����
extern uint32_t tracking_validity; //������Ч�� 
extern uint32_t acc_off_mileage_base;//��׼���
extern uint32_t befor_mileage_base;//��һ���ϴ����

extern void get_sim800_store_timer(void);
extern eat_bool gps_redress_timer(void);
extern eat_bool rtc_system_redress_time(uint8_t *Bct_Time);
extern void Tcp_Location_Infor_Ask(uint8_t socket,uint16_t SN);
extern void VDR_Position_Enter_Queue(void);
extern void VDR_Positon_Exit_Queue(void);
extern void TCP_Data_Any_Process(void);
extern void VDR_Position0_Enter_Queue(void);
extern void location_tracking_proc(uint16_t snver,uint8_t *temp_buf);//λ�ø��ٴ���
extern void location_report_policy_proc(void);

#endif

