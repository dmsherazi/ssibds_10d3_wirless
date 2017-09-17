
#ifndef _POWER_ADC_PROC_H_
    #define _POWER_ADC_PROC_H_
    
    #include <stdio.h>
    #include <string.h>
    #include"eat_interface.h"

    #define power_adc_porc_debug_printf 0
    /***********Ƿѹ��׼��ѹֵ 16v *************/
    #define undervoltage_reference_value 160 
    #define undervoltage_reference_value_max 165 
    /*********** ���������� 2���ε���16VǷѹ *************/
    #define averag_times 6 
 /*
* ===  FUNCTION  ======================================================================
*  Name: adc_power_porc
*  Description:  �ⲿ������Ƿѹ���ƴ���
*  Parameters :  ֱ�ӿ���            
*  Return     :  void 
*  author     : J.x.b create at SSI_1D03 at 2017-2-5             
* =====================================================================================
*/ 
extern void adc_power_porc(void);
 
 /*
 * ===  FUNCTION  ======================================================================
 *  Name: adc_value_pro
 *  Description:  ADֵ����
 *  Parameters :  volate_value: ϵͳ���صĵ�ѹ����ֵ            
 *  Return     :  void 
 *  author     : J.x.b create at SSI_1D03 at 2017-2-5             
 * =====================================================================================
 */ 
extern void adc_value_pro(u32 volate_value);
extern void get_adc_period(void);

    #endif
