#ifndef _DRING_RECORDS_TRANS_OUT_H
    #define _DRING_RECORDS_TRANS_OUT_H


#include"eat_interface.h"

#define dring_record_debug_printf 0

/*SIM800接收到导出数据请求*/
#define sim800_dring_record_inquire 0x62 

#define inquire_driver_data_ref 0x0700

/*国标行驶记录仪数据开关*/
#define gb19056_dring_data_switch 0

/*808行驶记录仪数据开关*/
#define jt_808dring_data_switch 0

/*长传行驶记录数据文件*/
#define package_driver_record_file L"D:\\driver_record\\package_driver_record.bin"

/*****行驶速度记录(08H)文件****/
#define driver_record_08H L"D:\\driver_record\\driver_record_08H.bin"
/*****位置信息记录(09H)文件****/
#define driver_record_09H L"D:\\driver_record\\driver_record_09H.bin"
/*****事故疑点记录(10H)****/
#define driver_record_10H L"D:\\driver_record\\driver_record_10H.bin"
/*****事故疑点记录(11H)****/
#define driver_record_11H L"D:\\driver_record\\driver_record_11H.bin"
/*驾驶人身份记录(12H)*/
#define driver_record_12H L"D:\\driver_record\\driver_record_12H.bin"
/*外部供电记录(13H)*/
#define driver_record_13H L"D:\\driver_record\\driver_record_13H.bin"
/*参数修改记录(14H)*/
#define driver_record_14H L"D:\\driver_record\\driver_record_14H.bin"
/*速度状态日志(15H)*/
#define driver_record_15H L"D:\\driver_record\\driver_record_15H.bin"



/*行驶记录仪数据校验枚举*/
typedef enum
{
    check_ok=0,
    check_set_error=0x01,
    check_inquire=0x02
}driver_data_chek_enum;

/*采集行驶记录数据队列信息*/
#define dring_record_max 11 
/*808行驶记录仪数据缓存*/
typedef struct
{
    uint8_t dring_record_cmd;
    uint16_t inquire_servn_num;
    uint8_t data_len;
    uint8_t data_buff[64];
}dring_record_sturct;

/*808行驶记录仪查询队列*/
typedef struct
{
    uint8_t queuesize;   //数组的大小
    uint8_t head, tail;  //队列的头和尾下标
    dring_record_sturct dring_recod_arry[dring_record_max];
    uint8_t inquire_queue_state;
}inquire_dring_record_queue;

extern inquire_dring_record_queue inquire_dring_record;
extern uint8_t inquire_record_ip;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_Driver_Data_Trans
 *  Description:  行驶记录数据导出
 *  Parameters :  
 *				  *Pbuffer 来源数据Pubffer
 *                *Lens    数据长度
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void tcp_driver_data_update(uint8_t *buffer,uint16_t Lens);

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: hy_dring_record_data_inquire_proc
 *  Description:  货运平台查询行驶记录数据
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
extern void hy_dring_record_data_inquire_proc(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_proc
  *  Description:  处理行驶记录数据
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void inquire_dring_record_queue_proc(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_init
  *  Description:  初始化inquire_dring_record 队列 行驶记录数据查询队列
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void inquire_dring_record_queue_init(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  进入行驶记录数据查询队列
  *  Parameters :dr_cmd:形式记录命令，servn_num:流水号 data:数据包数据 data_len:数据长度
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern  void enter_inquire_dring_record_queue(uint8_t dr_cmd,uint16_t servn_num,uint8_t *data,uint8_t data_len);
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  出行驶记录数据查询队列
  *  Parameters : void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void exit_inquire_dring_record_queue(void);
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: is_inquire_dring_record_queue_empty
  *  Description:  行驶记录查询队列是否为空
  *  Parameters : void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern  char_t is_inquire_dring_record_queue_empty(void);
 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: is_inquire_dring_record_queue_full
   *  Description:  行驶记录查询队列是否为满
   *  Parameters : void
   *  Return     :  void 
   *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
   * =====================================================================================
   */ 
extern char_t is_inquire_dring_record_queue_full(void);

#endif
