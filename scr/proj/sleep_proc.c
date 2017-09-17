
/****************************************************
 * Include Files
****************************************************/ 
 
 #include "sleep_proc.h"

 /****************************************************
* External variables declaration
*****************************************************/


/*****************************************************
* Local variables declaration(Static)
******************************************************/

 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: system_sleep
  *  Description:  
  *  Parameters :    
  *  Return     :   
  *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
  * =====================================================================================
  */
void system_sleep(void)
 {
     Real_Time_Location_Inf.Location_Speed =0;//�����ٶ�
     Real_Time_Location_Inf.Travel_Speed = 0;//��ʻ��¼�ٶ�
     Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep =EAT_TRUE;
     eat_lcd_light_sw(EAT_FALSE,0);
     
     eat_uart_close(EAT_UART_1);
     eat_uart_close(EAT_UART_2);
     eat_pin_set_mode(EAT_PIN5_DCD, EAT_PIN_MODE_GPIO);
	 eat_gpio_setup(EAT_PIN5_DCD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����ΪGPIOģʽ
	 eat_pin_set_mode(EAT_PIN7_CTS, EAT_PIN_MODE_GPIO);
	 eat_gpio_setup(EAT_PIN7_CTS, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����ΪGPIOģʽ
	 eat_pin_set_mode(EAT_PIN8_RTS, EAT_PIN_MODE_GPIO);
	 eat_gpio_setup(EAT_PIN8_RTS, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����ΪGPIOģʽ
	 eat_pin_set_mode(EAT_PIN9_TXD, EAT_PIN_MODE_GPIO);
   	 eat_gpio_setup(EAT_PIN9_TXD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����ΪGPIOģʽ
   	 eat_pin_set_mode(EAT_PIN10_RXD, EAT_PIN_MODE_GPIO);
   	 eat_gpio_setup(EAT_PIN10_RXD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����ΪGPIOģʽ	
     eat_timer_stop(EAT_TIMER_1);
     eat_timer_stop(EAT_TIMER_2);
     eat_timer_stop(EAT_TIMER_3);
     eat_timer_stop(EAT_TIMER_4);
     eat_timer_stop(EAT_TIMER_8);
     eat_sleep_enable(EAT_TRUE);       //��������     
 }
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: system_user0_wake_up
  *  Description:  
  *  Parameters :    
  *  Return     :   
  *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
  * =====================================================================================
  */
void system_user0_wake_up(void)
{
    Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep =EAT_FALSE;
    task_msg_send(EAT_USER_0,EAT_USER_1,SystemWakeup);//������Ϣ��USER_2
    eat_timer_start(EAT_TIMER_1, 200);//������ʱ��
	eat_timer_start(EAT_TIMER_2, 1000);//������ʱ��
    eat_sleep_enable(EAT_FALSE);       //��������     
}
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: system_user1_wake_up
  *  Description:  
  *  Parameters :    
  *  Return     :   
  *  author     :  J.x.b create at TQ-TVM at 2016-4-10          
  * =====================================================================================
  */
void system_user1_wake_up(void)
{
	eat_timer_start(EAT_TIMER_4, 100);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: sys_sleep_state_black_or_light
*  Description:  //����ʱ������ͬ
*  Parameters :    
*  Return     :   
*  author     :  J.x.b create at TQ-TVM at 2016-4-10          
* =====================================================================================
*/
uint8_t sys_sleep_state_black_or_light(void)
{
    uint32_t now_time,min_time,max_time;
    uint8_t hour_t,mintin_t;
    
	BCD_To_Dec(&Real_Time_Location_Inf.Curren_Time[3],&hour_t,1);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",hour_t);
    #endif
	BCD_To_Dec(&Real_Time_Location_Inf.Curren_Time[4],&mintin_t,1);
	
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",mintin_t);
    #endif
    now_time = (uint32_t)(hour_t*3600 + mintin_t*60);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",now_time);
    #endif
    
	BCD_To_Dec(&system_param.location_reporting_param.time_night[0],&hour_t,1);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",hour_t);
    #endif
	BCD_To_Dec(&system_param.location_reporting_param.time_night[1],&mintin_t,1);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",mintin_t);
    #endif
    min_time = (uint32_t)(hour_t*3600 + hour_t*60);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",min_time);
    #endif
	BCD_To_Dec(&system_param.location_reporting_param.time_night[2],&hour_t,1);
	
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",hour_t);
    #endif
	BCD_To_Dec(&system_param.location_reporting_param.time_night[3],&mintin_t,1);
	
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",mintin_t);
    #endif
    max_time = (uint32_t)(hour_t*3600 + hour_t*60);
    #if system_sleep_debug_printf
    eat_trace("sys_sleep_state_black_or_light--> now_time hour_t:%d .",max_time);
    #endif
    if((now_time>=min_time)&&(now_time<=max_time))
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

