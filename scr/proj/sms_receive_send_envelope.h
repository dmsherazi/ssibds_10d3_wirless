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
    u8 SCA[44];       // 短消息服务中心号码(SMSC地址)
    u8 TPA[44];       // 目标号码或回复号码(TP-DA或TP-RA)
    u8 TP_PID;        // 用户信息协议标识(TP-PID)
    u8 TP_DCS;        // 用户信息编码方式(TP-DCS)
    u8 TP_SCTS[16];   // 服务时间戳字符串(TP_SCTS), 接收时用到
    u8 TP_UD[161];    // 原始用户信息(编码前或解码后的TP-UD)
    u8 index;         // 短消息序号，在读取时用到
} SM_PARAM;

typedef struct _sms_send_list
{
   uint8_t addr_len;
   uint8_t send_addr[44];//发送地址
   uint8_t data_len;
   uint8_t data_buf[sms_send_buffer_max];//数组头指针
   struct list_head re_list;
}sms_send_pack_list;//tcp接收缓存

extern struct list_head sms_send_list;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void init_sms_setting(void)
 *  Description: 初始化短信设置
 *        Input: *pSrc :短信内容,  Dest:短信地址         
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void init_sms_setting(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void receive_pdu_sms_pro(u8 *pScr,u16 *nLength);
 *  Description: 接受PDU短信并处理
 *        Input: *pSrc :短信内容,  Dest:短信地址         
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void receive_pdu_sms_pro(u8 *pScr,u16 *nLength);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void sms_time_out_handle(void)
 *  Description: 短信发送超时处理
 *        Input: 每次超市30s 工两次        
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
extern void sms_send_time_out_handle(void);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void send_sms_data_pro(uint8_t *buff,uint8_t *dest)
 *  Description: *buff :数据  *dest:目标号码
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
 *  Description: 发送报警信息
 *        Input: ucode_data,报警信息内容     
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void worning_sms_send(u8* ucode_data);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  从链表中删除发送成功的短信
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void sms_send_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  发送短信
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void sms_send_action_pro(struct list_head *list_parm);

extern void sms_stoarg_delet(uint16_t index);

#endif

