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
     AT_DELET_ALL_SMS,//删除所有短信
     AT_SIM_INSET,//sim卡是否插入检测
     AT_SET_TTS_PARAM,//TTS参数设置
     AT_SET_TTS_VOL,//TTS音量设置
     at_call_not_ring,//不接
     at_call_num,//来电显示
     AT_PARAM_SAVE,//参数保存
     AT_TTS_IC_IN,//IC卡插入
     AT_TTS_IC_OUT,//IC卡拔出
     AT_TTS_SP_OUT,//超速报警
     AT_TTS_PL_DRIVE,//疲劳驾驶
     AT_TTS_SPING,//即将超速
     AT_TTS_PLING,//即将疲劳驾驶
     AT_TTS_SP_ERROR,//速度异常
     AT_CHANG_IC_CARD,//停产换卡
     at_not_printf,//停车打印
     AT_ANTENA_ERROR,//天线故障
     at_not_in_iccard,//未插IC卡
     at_power_down,//掉电
}AT_CMD_NAME;/*AT指令名称*/

typedef struct
{
    AT_CMD_NAME enAtCmd;
    const char_t*  pCmdString;
    const char_t*  pRetString;
}AT_CMD_SEND;/*发送AT指令*/

typedef enum
{
     AT_CMD_idle=0,
     AT_CMD_busy
}AT_CMD_SATUR;/*AT指令名称*/

typedef struct 
{
    unsigned char queuesize;   //数组的大小
    unsigned char head, tail;  //队列的头和尾下标
    AT_CMD_NAME AT_cmd_buf[AT_CMD_QUEUE_MAX];//数组头指针
    AT_CMD_SATUR execution_statue;
}SEND_AT_CMD_Queue;


/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/
extern char_t Wether_SIM_DEF;//SIM卡是否在位
extern SEND_AT_CMD_Queue at_cmd;//AT 队列
/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void at_cmd_queue_process(void); //在timer2中调用
extern void mdm_rx_proc_cb(uint8_t *p_str);//at 处理返回函数
extern void exit_at_cmd_send_queue(SEND_AT_CMD_Queue *q);
extern void enter_at_cmd_send_queue(SEND_AT_CMD_Queue *q,AT_CMD_NAME name);
/* 
* ===  FUNCTION  ======================================================================
*         Name: at_alarm_tone_proc
*  Description: at_alarm_tone_proc
*        Input: 语音播报类型。  
*         0x01:开机提示
* 		0x02:司机卡插入
* 		0x03:司机卡拔出
* 		0x04:超速
* 		0x05:疲劳驾驶
*		0x06:
*		0x07:即将超速
*		0x08:即将疲劳驾驶
*		0x09:速度异常
*         0x0A:停车换卡
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
extern void at_alarm_tone_proc(uint8_t *alarm_type);
 /* 
* ===  FUNCTION  ======================================================================
*         Name: antena_indef_alarm_tone
*  Description: 天线断开 语音播报处理
*        Input: void           
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void antena_indef_alarm_tone(void);   
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

