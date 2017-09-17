/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   board sim800 one second information
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
 * SIM800���ն�ͨ��ģ��1sͨ����Ϣ��
 *--------------------------------------------------------------------
 ********************************************************************/
#ifndef _BOARD_SIM800_ONE_SECOND_COMMCATE_H_
#define _BOARD_SIM800_ONE_SECOND_COMMCATE_H_
   #define debuge_printf_switch 0
   
   #define SIM_Card_Signal_Strength_ID 0x61 //SIM���ź�ǿ�ȣ�SIM��״̬

   #include "eat_interface.h"
   #include <stdlib.h>

   typedef struct 
   {
       uint8_t  status; //�Ƿ�λ 0x01:��λ 0x00:δ��λ      
       uint8_t  latitude_drect; //ά�ȷ��� 0x01:��γ 0x00:��γ      
       uint8_t  longitude_drect; //���ȷ��� 0x01:���� 0x00:����      
       double  latitude;            
       double  longitude;           
       int     latitude_Degree;    
       int     longitude_Degree;
       uint32_t jt_latitude;
       uint32_t jt_longitude;
       uint16_t gps_speed;
       uint16_t gps_drection;
   }Location_orgin_data;
   typedef union state_recrod_board
   {
	 uint8_t state_data;
	 struct 
	 {
	   uint8_t sate_ip1_lock: 1; 			 
	   uint8_t sate_ip2_lock: 1; 			 
	   uint8_t sate_rev:6;
	 }state_bit;
   }state_record_struct;


 extern state_record_struct state_record;

/* 
* ===  FUNCTION  ======================================================================
*  Name: void sim800_board_commucation(unsigned char *Psource)
*  Description:  ����λ�ã�ʱ�䣬������Ϣ
*  Parameters :  ��                
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-31             
* =====================================================================================
*/       
extern void sim800_board_commucation_proc(uint8_t *Psource); 
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_sim800_send_1s_proc
 *  Description:  SIM800������MB9F506R��������
 *  Parameters :  ID           :void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
 * =====================================================================================
 */ 
 extern void uart1_sim800_send_1s_proc(void);
#endif
