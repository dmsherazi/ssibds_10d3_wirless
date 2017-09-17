#ifndef UART_DATA_QUEUE_PROC_H
    #define UART_DATA_QUEUE_PROC_H
    
#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define can_reve_data_pro_debug_printf 1

#define ASK_REV_INFO           	0xFF //回复信息

#define receive_can_data_max 1536
#define receive_can_list_max 5

/***接收CAN数据结构***/
typedef struct 
{
   uint16_t data_len;
   uint8_t data_buf[receive_can_data_max];//数组头指针
   struct list_head re_list;
}receive_can_data_struct;

/***接收CAN数据链表***/
extern struct list_head receive_can_data_list;
/* 
* ===  FUNCTION  ======================================================================
*  Name: void receive_can_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
*  Description:  增加CAN接收数据链表
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
extern void receive_can_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: can_data_analysis_proc
 *  Description:  CAN数据解析 处理链表中的数据
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
extern void can_data_analysis_proc(void);
/* 
* ===  FUNCTION  ======================================================================
*  Name: can_orign_data_update
*  Description:  CAN数据上传
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
extern void can_orign_data_update(uint8_t *buffer,uint8_t data_cnt);

#endif

