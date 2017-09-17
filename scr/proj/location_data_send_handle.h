
#ifndef _DATA_SEND_HANDLE_H
	#define _DATA_SEND_HANDLE_H  


#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define location_data_send_printf 0

#define Busy_State  1   //״̬�տ���
#define Idle_State  0	//״̬æ
/*λ����Ϣ���Ͷ���*/
#define Location_Queue_Max 6


/*ϵͳ��־λλ��ṹ��*/
typedef union _System_Flag_Struct_
{
	uint32_t System_Flag_Integral;
	struct
	{
	    uint32_t Mode_Active_ing:1;//���ڼ�����
		uint32_t Mode_Active:1;				 //�����־
		
		uint32_t MainAutenSucess:1;               //��Ȩ�ɹ�
		uint32_t SecondAutenSucess:1;               //��Ȩ�ɹ�
		
		uint32_t Hart_Beat_Send: 1;			 //��������
		uint32_t Second_Hart_Beat_Send: 1;			 //�ڶ���������������
		
		uint32_t Location_Send_State : 1 ;	 //����״̬
		uint32_t Second_Location_Send_State : 1 ;	 //�ڶ�����������״̬
		
		uint32_t Queue_Fade_Area: 1;		     //��һ�����������е�ä������
		uint32_t Second_Queue_Fade_Area: 1;		     //�ڶ������������е�ä������
		
		uint32_t File_Fade_Area: 1;			 //�ļ���ä�����ݷ���
		uint32_t Second_File_Fade_Area: 1;		     //�ڶ������������е�ä������
		
		uint32_t Location_Tracking: 1;        // 1λ�ø���
		uint32_t Second_Location_Tracking: 1;        // 2λ�ø���

		uint32_t can_data_send_state :1;
		uint32_t second_can_data_send_state :1;
		
		uint32_t Sms_Send_Time: 1;
		uint32_t Rout_Tim_Over: 1;//·����ʻʱ�����
		uint32_t Rout_Tim_Short : 1;//·����ʻʱ�䲻��
        uint32_t ic_inout :1; //ic������γ� ���� TRUE �γ�FALSE;
        uint32_t record_voice:1;//¼��״̬
        uint32_t media_update:1;//��ý���ϴ�
        uint32_t take_picture:1;//����
        uint32_t tcp_dail_call:1;//ƽ̨����绰
        uint32_t tcp_listen_call:1;//Զ�̼���
        uint32_t detail_gnss_switch_off:1;//�ر�detail gnss
        uint32_t record_data:1;//��¼����
        uint32_t fade_detail_gnss:1;//GNSSä��
		u32 Flag_Rev :4 ;
	}System_Flag_Bit;
}System_Flag_Struct;


/*λ����Ϣ����ṹ��*/
typedef struct 
{
	u8 Location_Inf[100];//������ݳ���
	u16 Location_Inf_Lens;
}BaseLocationSave_Struct;



typedef  struct 
{
  u32 Location_Queue_Len;
  u32 Tail,Head;
  BaseLocationSave_Struct Location_Save_Buffer[Location_Queue_Max];
}Location_Queue_Struct;


/*λ����Ϣ���Ͷ���*/
extern System_Flag_Struct	System_Flag; //ϵͳ��־λ��ṹ��	
extern Location_Queue_Struct Location_Queue;//λ����Ϣ����

extern Location_Queue_Struct Second_Location_Queue;//�ڶ�������

extern void Location_Queue_Init(Location_Queue_Struct *Pscr);
extern int Enter_Location_Queue(Location_Queue_Struct *Dest,BaseLocationSave_Struct *Inf_point);
extern int Exit_Location_Queue(Location_Queue_Struct *Pscr);
extern int IS_Location_Queue_Empty(Location_Queue_Struct *Pscr);



extern void First_Server_Location_Inf_Send(void);
extern void First_Server_Location_Inf_Send(void);

extern void Analysis_Location_Inf(Location_Inf_Struct* scr_struct,BaseLocationSave_Struct *Inf_Data);//������������
extern void timeout_location_report(void);

#endif
