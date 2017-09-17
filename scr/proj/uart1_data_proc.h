

#ifndef _UART1_RECEIVE_DATA_PROC_H
#define _UART1_RECEIVE_DATA_PROC_H

/****�궨��****/
#include "eat_interface.h"

#define uart1_data_pro_debug_printf 0

/******�궨���������********/
#ifdef  ASK_REV_INFO  
#define FRAME_NORMAL           	0x00
#define FRAME_CRC_ERR          	0x01
#define FRAME_HIATUS            0x02
#endif  


#define NO_CRC_AREA_LENS      6 //��֡ͷ����֡β����У��
#define CRC_AREA_FIXED_LENS   4 // 1ByteУ������  1Byte ������ 2Byte���ݳ���
#define FRAME_HEAD_LENS       2 //֡ͷ����  

#define Frame_Head_One 0x7e //֡ͷ1
#define Frame_Head_two 0x7d //֡ͷ2

/************************SIM800 To BOARD Data ID**************************/
/**����ȫ����GNSS����**/
#define ask_all_gness_data 0x6A
/**����GNSS����**/
#define board_all_gnss_rev 0x7A


#define board_update_id             0x6B //����
#define board_update_ret_id         0x7B //����

#define system_sleep_switch 0xB3
#define system_sleep_ref 0xA3

#define sleep_action 0x55

#define sleep_up_action 0xAA

#define board_send_to_sim800 0x73 //  1S��Ϣ

/*������������id*/
#define voice_type_id   0x71 

/*��ʻ��¼���ش�*/
#define driver_data_record_ref   0x72 

/***DBCcan��Ϣ***/
#define dbc_can_id_seting 0xA0
#define dbc_can_id_rev 0xB0


/***CANID���ñ궨��Ϣ***/
#define can_setting_id 0x64
#define receive_can_id 0x74

/***�궨��Ϣ***/
#define demarcate_id 0x95

/***��ȡ�궨��Ϣ***/
#define get_demarcate 0x96
#define get_demarcate_ask 0x86

/***����ָ��***/
#define updata_sim800_process 0x93
#define updata_sim800_ref 0x83

/***��ѯ�ն�ID IP �ֻ���***/
#define request_id_ip_phonenum 0x76

/***txt��Ϣ������ѯ***/
#define txt_cnt_inquire 0x90

/***txt��Ϣ���ݲ�ѯ***/
#define txt_infor_inquire 0x91

/***SIM800����汾��ѯ***/
#define sim800_software_vion_inquire 0x94

/***�˵�������Ϣ***/
#define memu_setting_data_id 0x99

/***�˵�������Ϣ***/
#define remote_lock_car_rev 0x9A

/***����MCU������Ϣ***/
#define seting_mcu_data 0x65


/***����1����***/
#define uar1_receive_max 1536 //����1�������ֵ
#define uar1_analysis_list_max 20

/***����1����***/
#define uar1_send_max 1536 //����1�������ֵ
#define uar1_send_list_max 20

/***����1����***/
typedef struct _analysis_uart1_data_list
{
   uint16_t data_len;
   uint8_t data_buf[uar1_receive_max];//����ͷָ��
   struct list_head re_list;
}analysis_data_pack_list;//tcp���ջ���
/***����1����***/
typedef struct _uart1_send_list
{
   uint16_t data_len;
   uint8_t data_buf[uar1_send_max];//����ͷָ��
   struct list_head re_list;
}uart1_send_data_pack_list;//tcp���ջ���
/***����1������������***/
extern struct list_head uart1_anaylsis_list;
/***����1������������***/
extern struct list_head uart1_send_list;

/***״ָ̬ʾunion***/
 typedef union 
 {
     uint8_t state_byte;
     struct
     {
         uint8_t strength_bit :1;
         uint8_t can_data_send_bit :1;
         uint8_t location_data_send_bit :1;
         uint8_t state_rev :5;
     }state_byte_struct;
 }SimState_union;
 extern SimState_union  Sim_State;

 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_receive_integrated_frame_proc
 *  Description:   �����ӵ������ݲ�����������֡���� Rev_Buff
 *  Parameters :  *Rev_Buff     :����������֡
 *				  *Source_Buff  :ԭʼ����ָ��
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-6 			 
 * =====================================================================================
 */    
extern void  uart1_receive_integrated_frame_proc(uint8_t *Source_Buff,uint16_t Re_Lens);//USER0
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: Create__Send_Communication_Frame
  *  Description:   ͨ��֡��װ 
  *  Parameters :  *ID           :��������ID
  *                *Temp_buffer  :�������ݵĻ���
  *  
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
  * =====================================================================================
  */ 
extern void uart1_send_data_frame(uint8_t ID,uint8_t *Temp_buffer,uint16_t Lens);

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_pro_list_init
 *  Description: ��ʼ������1���ݽ���
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_pro_list_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_list_add
 *  Description: ���ӽ���������
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_pro_list_del
 *  Description: ɾ���ڵ�
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_pro_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_init
 *  Description: ��ʼ������1���ͽ������ʼ��
 *  Parameters :  void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_add
 *  Description: ���Ӵ���һ����������
 *  Parameters : *list_parm :��������ָ�� buff:��������ָ��  data_cnt:�������ݳ���
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_del
 *  Description: ɾ��uart1 ����������
 *  Parameters : *list_parm :��������ָ�� 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_receive_queue_proc
 *  Description: ����UART1���յ������� ��MS��Ϣ�д���
 *  Parameters :  void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void uart1_receive_queue_proc(void);
/* 
* ===  FUNCTION  ======================================================================
*  Name: uart1_send_queue_proc
*  Description: UART1���ʹ�����
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
void uart1_send_queue_proc(void);
/* 
* ===  FUNCTION  ======================================================================
*  Name: board_mode_seting 
*  Description:   �ն˿��� 
*  Parameters :  CMD:0x00���ն�����:
*                     0x01:
                      0x02://����
*                     0x03://GPSģʽ
*                     0x04://���ģʽ
*                     0x05: ��λ
*                     0x06: �ָ���������
*	
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
* =====================================================================================
*/ 
extern void board_mode_seting(uint8_t cmd);

#endif
