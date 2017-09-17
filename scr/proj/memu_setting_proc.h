
#ifndef _MEMU_SETTING_PROC_H
    #define _MEMU_SETTING_PROC_H
    
    #define memu_debug_printf 1

    #include <stdio.h>
    #include <string.h>
    #include"eat_interface.h"

    /* 
* ===  FUNCTION  ======================================================================
*  void memu_setting_data_pro(uint8_t *data)
*  Description: 设置菜单信息
*  FileName：memu_setting_data_pro
*  return: null
*  author: J.x.b  create at 2016-5-21
* =====================================================================================
*/
extern void memu_setting_data_pro(uint8_t *data);
/* 
* ===  FUNCTION  ======================================================================
*  void vehicle_infor_seting(uint8_t data_id)
*  Description: 设置车辆信息
*  FileName：vehicle_infor_seting
*  return: null
*  author: J.x.b  create at 2016-5-21
* =====================================================================================
*/
extern void vehicle_infor_seting(uint8_t data_id);

#endif
