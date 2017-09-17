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

#ifndef AT_CMD_H
   #define AT_CMD_H



/********************************************************************
 * Include Files
 ********************************************************************/
#include "eat_interface.h"
 

/********************************************************************
* Macros
 ********************************************************************/
#define at_process_debug_printf 0

#define AT_CMD_QUEUE_MAX 30
/********************************************************************
* Types
********************************************************************/
typedef enum
{
     AT_ECOH_INIT=0,
     AT_CSDT,
     AT_ECOH_CIPMUX,
     AT_CHECK_SIG,
     AT_CLOSE_VOLTAGE_CHECK,
     AT_DELET_ALL_SMS,//ɾ�����ж���
     AT_SIM_INSET,//sim���Ƿ������
     AT_SET_TTS_PARAM,//TTS��������
     AT_SET_TTS_VOL,//TTS��������
     at_call_not_ring,//����
     at_call_num,//������ʾ
     AT_PARAM_SAVE,//��������
     AT_TTS_IC_IN,//IC������
     AT_TTS_IC_OUT,//IC���γ�
     AT_TTS_SP_OUT,//���ٱ���
     AT_TTS_PL_DRIVE,//ƣ�ͼ�ʻ
     AT_TTS_SPING,//��������
     AT_TTS_PLING,//����ƣ�ͼ�ʻ
     AT_TTS_SP_ERROR,//�ٶ��쳣
     AT_CHANG_IC_CARD,//ͣ������
     at_not_printf,//ͣ����ӡ
     AT_ANTENA_ERROR,//���߹���
     at_not_in_iccard,//δ��IC��
     at_power_down,//����
}AT_CMD_NAME;/*ATָ������*/

typedef struct
{
    AT_CMD_NAME enAtCmd;
    const char_t*  pCmdString;
    const char_t*  pRetString;
}AT_CMD_SEND;/*����ATָ��*/

typedef enum
{
     AT_CMD_idle=0,
     AT_CMD_busy
}AT_CMD_SATUR;/*ATָ������*/

typedef struct 
{
    unsigned char queuesize;   //����Ĵ�С
    unsigned char head, tail;  //���е�ͷ��β�±�
    AT_CMD_NAME AT_cmd_buf[AT_CMD_QUEUE_MAX];//����ͷָ��
    AT_CMD_SATUR execution_statue;
}SEND_AT_CMD_Queue;


/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/
extern char_t Wether_SIM_DEF;//SIM���Ƿ���λ
extern SEND_AT_CMD_Queue at_cmd;//AT ����
/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void at_cmd_queue_process(void); //��timer2�е���
extern void mdm_rx_proc_cb(uint8_t *p_str);//at �����غ���
extern void exit_at_cmd_send_queue(SEND_AT_CMD_Queue *q);
extern void enter_at_cmd_send_queue(SEND_AT_CMD_Queue *q,AT_CMD_NAME name);
/* 
* ===  FUNCTION  ======================================================================
*         Name: at_alarm_tone_proc
*  Description: at_alarm_tone_proc
*        Input: �����������͡�  
*         0x01:������ʾ
* 		0x02:˾��������
* 		0x03:˾�����γ�
* 		0x04:����
* 		0x05:ƣ�ͼ�ʻ
*		0x06:
*		0x07:��������
*		0x08:����ƣ�ͼ�ʻ
*		0x09:�ٶ��쳣
*         0x0A:ͣ������
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
extern void at_alarm_tone_proc(uint8_t *alarm_type);
 /* 
* ===  FUNCTION  ======================================================================
*         Name: antena_indef_alarm_tone
*  Description: ���߶Ͽ� ������������
*        Input: void           
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void antena_indef_alarm_tone(void);   
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

