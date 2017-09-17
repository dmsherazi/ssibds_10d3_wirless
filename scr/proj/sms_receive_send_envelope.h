#ifndef _SMS_RECEIVE_SEND_ENVELOPE_H
	#define _SMS_RECEIVE_SEND_ENVELOPE_H



#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include"eat_interface.h"

#define sms_receive_send_debuge_pintf 0

#define sms_send_max 5
#define sms_send_buffer_max 350


#define GSM_7BIT        0x13
#define GSM_8BIT        0x17
#define GSM_UCS2        0x1b


typedef struct 
{
    u8 SCA[44];       // ����Ϣ�������ĺ���(SMSC��ַ)
    u8 TPA[44];       // Ŀ������ظ�����(TP-DA��TP-RA)
    u8 TP_PID;        // �û���ϢЭ���ʶ(TP-PID)
    u8 TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
    u8 TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
    u8 TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD)
    u8 index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

typedef struct _sms_send_list
{
   uint8_t addr_len;
   uint8_t send_addr[44];//���͵�ַ
   uint8_t data_len;
   uint8_t data_buf[sms_send_buffer_max];//����ͷָ��
   struct list_head re_list;
}sms_send_pack_list;//tcp���ջ���

extern struct list_head sms_send_list;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void init_sms_setting(void)
 *  Description: ��ʼ����������
 *        Input: *pSrc :��������,  Dest:���ŵ�ַ         
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void init_sms_setting(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void receive_pdu_sms_pro(u8 *pScr,u16 *nLength);
 *  Description: ����PDU���Ų�����
 *        Input: *pSrc :��������,  Dest:���ŵ�ַ         
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void receive_pdu_sms_pro(u8 *pScr,u16 *nLength);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void sms_time_out_handle(void)
 *  Description: ���ŷ��ͳ�ʱ����
 *        Input: ÿ�γ���30s ������        
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void sms_send_time_out_handle(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void send_sms_data_pro(uint8_t *buff,uint8_t *dest)
 *  Description: *buff :����  *dest:Ŀ�����
 *        Input:       
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void send_sms_data_pro(uint8_t *buff,uint8_t *dest);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void worning_sms_send(u8* ucode_data)
 *  Description: ���ͱ�����Ϣ
 *        Input: ucode_data,������Ϣ����     
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void worning_sms_send(u8* ucode_data);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  ��������ɾ�����ͳɹ��Ķ���
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void sms_send_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  ���Ͷ���
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void sms_send_action_pro(struct list_head *list_parm);

extern void sms_stoarg_delet(uint16_t index);

#endif

