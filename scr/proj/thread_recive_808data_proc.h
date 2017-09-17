/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   xxxxxx.c
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
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef THREAD_RECIVE_808DATA_H
   #define THREAD_RECIVE_808DATA_H



/********************************************************************
 * Include Files
 ********************************************************************/
       #include"eat_interface.h"


/********************************************************************
* Macros
 ********************************************************************/
 
/**********************ƽ̨�·�ID*****************/
#define Tcp_Rev_Comm_ID     (uint16_t)0x8001//�ն�Ӧ��ͨ��ID
#define TCP_VDR_Seting_ID   (uint16_t)0x8103//��������
#define remoto_update_processor (uint16_t)0x8108//����
#define remote_sleep_setting (uint16_t)0xd105  //����ָ��

#define TCP_VDR_ChaXun_ALL  (uint16_t)0x8104//��ѯȫ������
#define TCP_VDR_ChaXun_ID   (uint16_t)0x8106//��ѯָ������
#define tcp_request_param_replay (uint16_t)0x0104//��ѯ�ն˲���Ӧ��

#define  Tcp_Down_Though_ID    (uint16_t)0x8900
#define Tcp_Location_Ask_ID    (uint16_t)0x8201 //λ�ò�ѯ
#define tcp_location_tracking_id (uint16_t)0x8202
/****************�˹�����ȷ��****************/
#define man_aram_qveren (uint16_t)0x8203

/******************�ն˷���ID*********************/
#define Tcp_VDR_Common_Replay_ID (uint16_t)0x0001 //�ն�ͨ��Ӧ��
#define Tcp_Pass_Though_ID    (uint16_t)0x0900//͸��

/****************8.23 �ı���Ϣ�·�****************/
#define txt_information_issued    (uint16_t)0x8300 //ƽ̨�ı��·�

/****************��ʻ��¼���ݲɼ�****************/
#define drive_record_iquire_id (uint16_t)0x8700
/****************��ʻ��¼��������****************/
#define drive_record_set_id (uint16_t)0x8701


/***��������***/
#define tcp_take_pictures_id (uint16_t)0x8801
/***����Ӧ��***/
#define tcp_take_pictures_id_replay (uint16_t)0x8800

/***¼����ʼ����***/
#define tcp_voice_record (uint16_t)0x8804
/*�洢��ý�����ݼ���*/
#define tcp_media_search_id (uint16_t)0x8802
/***�����ý�峤��ID***/
#define save_tcp_media_update (uint16_t)0x8803

/***������ý�����***/
#define tcp_signel_media_search_id (uint16_t)0x8805

/***�ն�����***/
#define tcp_vdr_attributes_id   0x8107
/*��ѯ�ն�����Ӧ��*/
#define tcp_vdr_attributes_replay_id   0x0107

/***��������***/
#define tcp_car_contrl_id   0x8500
/*��������Ӧ��*/
#define tcp_car_contrl_id_replay_id   0x0500

/*�ն˿���*/
#define tcp_vdr_ctrl_id 0x8105


/******************���п���ID*********************/
#define lock_car_contrl_id 0xF001
#define lock_car_request_id 0xF003
#define lock_car_demarcate_id 0xF006//�궨ָ��
#define lock_car_heart_beat_id 0xF002//����ָ��


/********************************************************************
* Types
********************************************************************/


/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/


/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void remote_message_rev_data_pack(uint8_t *Source_Buff,uint16_t Re_Lens,struct list_head *data_pack);//user1
extern void analyze_tcp_type_setup(int16_t socket,int8_t *pSource,uint16_t dat_cnt);
extern void Tcp_VDR_Common_Replay(char_t socket,uint16_t Sern_num,uint16_t Data_ID,uint8_t Result);

/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    





