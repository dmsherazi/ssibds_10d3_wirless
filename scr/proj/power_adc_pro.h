
#ifndef _POWER_ADC_PROC_H_
    #define _POWER_ADC_PROC_H_
    
    #include <stdio.h>
    #include <string.h>
    #include"eat_interface.h"

    #define power_adc_porc_debug_printf 0
    /***********欠压基准电压值 16v *************/
    #define undervoltage_reference_value 160 
    #define undervoltage_reference_value_max 165 
    /*********** 连续检测次数 2两次低于16V欠压 *************/
    #define averag_times 6 
 /*
* ===  FUNCTION  ======================================================================
*  Name: adc_power_porc
*  Description:  外部函数，欠压控制处理
*  Parameters :  直接控制            
*  Return     :  void 
*  author     : J.x.b create at SSI_1D03 at 2017-2-5             
* =====================================================================================
*/ 
extern void adc_power_porc(void);
 
 /*
 * ===  FUNCTION  ======================================================================
 *  Name: adc_value_pro
 *  Description:  AD值处理
 *  Parameters :  volate_value: 系统返回的电压参数值            
 *  Return     :  void 
 *  author     : J.x.b create at SSI_1D03 at 2017-2-5             
 * =====================================================================================
 */ 
extern void adc_value_pro(u32 volate_value);
extern void get_adc_period(void);

    #endif
