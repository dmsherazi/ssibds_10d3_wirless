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
 *0.10  2016/9/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef TIME_PROCESS_H
   #define TIME_PROCESS_H


/********************************************************************
* Macros
********************************************************************/
#define time_process_debug_printf 0
/********************************************************************
 * Include Files
 ********************************************************************/
#include "eat_interface.h"


/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/

/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void AtCmdTimeOut_cb(char_t cmd,uint32_t wait_time);
extern void gprs_bearers_time_out(char_t cmd);
extern void first_server_conect_time_out(char_t cmd);
extern void second_server_conect_time_out(char_t cmd);
extern void first_platform_certification_time_out(char_t cmd);
extern void second_platform_certification_time_out(char_t cmd);

extern void system_clock_timer1_ms(void);
extern void system_clock_timer2_s(void);
extern void time3_mainlink_anaisy(void);
extern void time4_secondlink_anaisy(void);
extern void event_timeout(void); //超时处理总函数
extern void time5_mainlink_heart(void);
extern void time6_secondlink_heart(void);

extern void link_in_system_sleep_proc(void);
extern void second_link_in_system_sleep_proc(void);

extern void update_board_timeout_cb(char_t cmd,uint32_t wait_time);

extern void time7_sleep_data_proc(void);
extern void time8_can_data_proc(void);
extern void media_update_timeout_ctrl(char_t cmd);



    
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    



