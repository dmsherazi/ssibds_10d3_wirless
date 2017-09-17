
 
#ifndef SMS_SETTING_PROC_H
    #define SMS_SETTING_PROC_H

    #define sms_setting_debug_printf 1

    #include <stdio.h>
    #include <locale.h>
    #include <string.h>
    #include <stdlib.h>
    #include"eat_interface.h"

typedef enum 
{
    set_ok=0,
    set_head_error,
    set_key_error,
    set_no_define,
    set_format_error
}sms_setting_ret;


extern uint8_t sms_setting_key[7];
extern uint8_t sms_cmd_rest_cmd;
extern uint8_t sms_action_proc(uint8_t *sms_buffe,uint16_t sms_len,uint8_t *scr_num);

    #endif 
