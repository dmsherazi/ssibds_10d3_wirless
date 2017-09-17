

#ifndef _UART1_RECEIVE_DATA_PROC_H
#define _UART1_RECEIVE_DATA_PROC_H

/****宏定义****/
#include "eat_interface.h"

#define uart1_data_pro_debug_printf 0

/******宏定义参数解析********/
#ifdef  ASK_REV_INFO  
#define FRAME_NORMAL           	0x00
#define FRAME_CRC_ERR          	0x01
#define FRAME_HIATUS            0x02
#endif  


#define NO_CRC_AREA_LENS      6 //两帧头，两帧尾，两校验
#define CRC_AREA_FIXED_LENS   4 // 1Byte校验类型  1Byte 命令码 2Byte数据长度
#define FRAME_HEAD_LENS       2 //帧头长度  

#define Frame_Head_One 0x7e //帧头1
#define Frame_Head_two 0x7d //帧头2

/************************SIM800 To BOARD Data ID**************************/
/**请求全部的GNSS数据**/
#define ask_all_gness_data 0x6A
/**接收GNSS数据**/
#define board_all_gnss_rev 0x7A


#define board_update_id             0x6B //升级
#define board_update_ret_id         0x7B //升级

#define system_sleep_switch 0xB3
#define system_sleep_ref 0xA3

#define sleep_action 0x55

#define sleep_up_action 0xAA

#define board_send_to_sim800 0x73 //  1S信息

/*语音播报类型id*/
#define voice_type_id   0x71 

/*行驶记录数回传*/
#define driver_data_record_ref   0x72 

/***DBCcan信息***/
#define dbc_can_id_seting 0xA0
#define dbc_can_id_rev 0xB0


/***CANID设置标定信息***/
#define can_setting_id 0x64
#define receive_can_id 0x74

/***标定信息***/
#define demarcate_id 0x95

/***获取标定信息***/
#define get_demarcate 0x96
#define get_demarcate_ask 0x86

/***升级指令***/
#define updata_sim800_process 0x93
#define updata_sim800_ref 0x83

/***查询终端ID IP 手机号***/
#define request_id_ip_phonenum 0x76

/***txt信息个数查询***/
#define txt_cnt_inquire 0x90

/***txt信息内容查询***/
#define txt_infor_inquire 0x91

/***SIM800软件版本查询***/
#define sim800_software_vion_inquire 0x94

/***菜单设置信息***/
#define memu_setting_data_id 0x99

/***菜单设置信息***/
#define remote_lock_car_rev 0x9A

/***主控MCU设置信息***/
#define seting_mcu_data 0x65


/***串口1解析***/
#define uar1_receive_max 1536 //串口1接收最大值
#define uar1_analysis_list_max 20

/***串口1发送***/
#define uar1_send_max 1536 //串口1接收最大值
#define uar1_send_list_max 20

/***串口1解析***/
typedef struct _analysis_uart1_data_list
{
   uint16_t data_len;
   uint8_t data_buf[uar1_receive_max];//数组头指针
   struct list_head re_list;
}analysis_data_pack_list;//tcp接收缓存
/***串口1发送***/
typedef struct _uart1_send_list
{
   uint16_t data_len;
   uint8_t data_buf[uar1_send_max];//数组头指针
   struct list_head re_list;
}uart1_send_data_pack_list;//tcp接收缓存
/***串口1解析数据链表***/
extern struct list_head uart1_anaylsis_list;
/***串口1发送数据链表***/
extern struct list_head uart1_send_list;

/***状态指示union***/
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
 *  Description:   分析接到的数据并储存于完整帧缓存 Rev_Buff
 *  Parameters :  *Rev_Buff     :完整的数据帧
 *				  *Source_Buff  :原始数据指针
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-6 			 
 * =====================================================================================
 */    
extern void  uart1_receive_integrated_frame_proc(uint8_t *Source_Buff,uint16_t Re_Lens);//USER0
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: Create__Send_Communication_Frame
  *  Description:   通信帧封装 
  *  Parameters :  *ID           :发送数据ID
  *                *Temp_buffer  :发送数据的缓存
  *  
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
  * =====================================================================================
  */ 
extern void uart1_send_data_frame(uint8_t ID,uint8_t *Temp_buffer,uint16_t Lens);

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_pro_list_init
 *  Description: 初始化串口1数据解析
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_pro_list_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_list_add
 *  Description: 增加解析链表结点
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_anaylsis_pro_list_del
 *  Description: 删除节点
 *  Parameters : 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_anaylsis_pro_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_init
 *  Description: 初始化串口1发送解链表初始化
 *  Parameters :  void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_init(void);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_add
 *  Description: 增加串口一发送链表结点
 *  Parameters : *list_parm :发送链表指针 buff:发送数据指针  data_cnt:发送数据长度
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_send_list_del
 *  Description: 删除uart1 发送链表结点
 *  Parameters : *list_parm :发送链表指针 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
extern void uart1_send_list_del(struct list_head *list_parm);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_receive_queue_proc
 *  Description: 处理UART1接收到的数据 在MS信息中处理
 *  Parameters :  void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void uart1_receive_queue_proc(void);
/* 
* ===  FUNCTION  ======================================================================
*  Name: uart1_send_queue_proc
*  Description: UART1发送处理函数
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
void uart1_send_queue_proc(void);
/* 
* ===  FUNCTION  ======================================================================
*  Name: board_mode_seting 
*  Description:   终端控制 
*  Parameters :  CMD:0x00；终端锁定:
*                     0x01:
                      0x02://北斗
*                     0x03://GPS模式
*                     0x04://混合模式
*                     0x05: 复位
*                     0x06: 恢复出厂设置
*	
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
* =====================================================================================
*/ 
extern void board_mode_seting(uint8_t cmd);

#endif
