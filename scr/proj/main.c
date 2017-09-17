/********************************************************************
 *                Copyright SSI/co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   main.c
 * version       :   0.10
 * Description   :   
 * Authors       :   J.X.B
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 **************************************************************************/

/******************************************************************************
 *  Include Files
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "eat_interface.h"
#include "eat_clib_define.h" //only in main.c

#define main_debug_printf 1
static EatEntryPara_st app_para;
/********************************************************************
 * Macros
 ********************************************************************/
static u16 wr_uart_offset = 0; //ÓÃÀ´±ê¼ÇuartÎ´Ð´ÍêµÄÊý¾Ý
static u16 wr_uart_len = 0; //ÏÂ´ÎÐèÒªÍù´®¿ÚÐ´µÄÊý¾Ý³¤¶È
/********************************************************************
 * Types
 ********************************************************************/
typedef void (*app_user_func)(void*);

/********************************************************************
 * External Functions declaration
 ********************************************************************/
extern void APP_InitRegions(void);
/********************************************************************
 * Local Function declaration
 ********************************************************************/
void app_main(void *data);
void app_user1(void *data);
void app_user2(void *data);
void app_user3(void *data);

void app_func_ext1(void *data);
extern void mdm_rx_proc(const EatEvent_st* event);
extern void sim_bsp_read_uart_data(const EatEvent_st* event);

/********************************************************************
 * Local Function
 ********************************************************************/
#pragma arm section rodata = "APP_CFG"
 APP_ENTRY_FLAG 
#pragma arm section rodata
#pragma arm section rodata="APPENTRY"
	const EatEntry_st AppEntry = 
	{
		app_main,
		app_func_ext1,
		(app_user_func)app_user1,//app_user1,
		(app_user_func)app_user2,//app_user2,
		(app_user_func)app_user3,//app_user3,
		(app_user_func)EAT_NULL,//app_user4,
		(app_user_func)EAT_NULL,//app_user5,
		(app_user_func)EAT_NULL,//app_user6,
		(app_user_func)EAT_NULL,//app_user7,
		(app_user_func)EAT_NULL,//app_user8,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL
	};
#pragma arm section rodata
/***************************************************************************
 * Local Functions
 ***************************************************************************/
static void app_func_ext1(void *data)
{	
    sim_bsp_init_dbg();//usb uart3;   
	
	eat_pin_set_mode(EAT_PIN5_DCD, EAT_PIN_MODE_GPIO);//ÉèÖÃÎªGPIOÄ£Ê½
	eat_gpio_setup(EAT_PIN5_DCD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//ÉèÖÃÄ¬ÈÏLOWEAT_GPIO_LEVEL_LOW
	
	eat_pin_set_mode(EAT_PIN50_COL1, EAT_PIN_MODE_GPIO);//ÉèÖÃÎªGPIOÄ£Ê½
	eat_gpio_setup(EAT_PIN50_COL1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//ÉèÖÃÄ¬ÈÏLOW

	eat_pin_set_mode(EAT_PIN49_COL2, EAT_PIN_MODE_GPIO);//ÉèÖÃÎªGPIOÄ£Ê½
	eat_gpio_setup(EAT_PIN49_COL2, EAT_GPIO_DIR_INPUT, EAT_GPIO_LEVEL_LOW);//ÉèÖÃÄ¬ÈÏLOW
}
/***************************************************************************
 * GOLBLE app_main
 Functions£ºÖ÷Ñ­»·Á÷³Ì;
 ***************************************************************************/
static void app_main(void *data)
{
	EatEvent_st event;
	EatEntryPara_st *para;
    APP_InitRegions();//Init app RAM 
    APP_init_clib(); 
    para = (EatEntryPara_st*)data;
    memcpy(&app_para,para,sizeof(EatEntryPara_st));
    if(app_para.is_update_app && app_para.update_app_result)
	{
	    eat_update_app_ok(); //clear update APP flag
	}
    sim_bsp_init_uart1();//ÏµÍ³³õÊ¼»¯
    sim_bsp_init_uart2();
	sys_param_init_proc();
    eat_timer_start(EAT_TIMER_1, 200);//¿ªÆô¶¨Ê±Æ÷
	eat_timer_start(EAT_TIMER_2, 1000);//¿ªÆô¶¨Ê±Æ÷
/*****************************Ö÷Á÷³Ì*****************************************/
    while(EAT_TRUE)
    {
      eat_get_event(&event);
	  switch (event.event)
	  {
        case EAT_EVENT_TIMER :
    		if(event.data.timer.timer_id==EAT_TIMER_1)
    		{
    		   system_clock_timer1_ms();
    		}
    		else if(event.data.timer.timer_id==EAT_TIMER_2)
    		{
    		   system_clock_timer2_s();
    		}
    		else if(event.data.timer.timer_id==EAT_TIMER_7)
    		{
               #if APP_TRACE_DBG 
               eat_trace("time7_sleep_data_proc --> time 7.");
               #endif
               time7_sleep_data_proc();
    		}
            else if(event.data.timer.timer_id==EAT_TIMER_10)
            {
                take_pictures_time10_pro();
            }
    		break;
    		
	  	case EAT_EVENT_MDM_READY_RD:
    		mdm_rx_proc(&event);
    		break;
    		
			case EAT_EVENT_UART_READY_RD:
    		sim_bsp_read_uart_data(&event);
    		break;	
    		
		case EAT_EVENT_USER_MSG://ÏûÏ¢´¦Àå¡ª¡ªí
		    thread_msg_proc_main(*(event.data.user_msg.data));
		    break;
		case EAT_EVENT_INT:
            #if main_debug_printf 
            eat_trace("app_main --> receive wake up.");
            #endif
            system_user0_wake_up();
		break;
        case EAT_EVENT_ADC ://adÖµ
            if(event.data.adc.pin == EAT_PIN25_ADC)
            {
               adc_value_pro(event.data.adc.v);
               adc_power_porc();
            }
            break;
	    default:
		    break;
   }
  }	
}
static void app_user1(void *data)// Fisrt SERVER ´¦Àí
{
    EatEvent_st event;
    while(EAT_TRUE)
    {
        eat_get_event_for_user(EAT_USER_1, &event);
		switch (event.event)
		{
			case EAT_EVENT_TIMER :
    			if(event.data.timer.timer_id==EAT_TIMER_3)
        		{
        		   time3_mainlink_anaisy();
        		}
        		else if(event.data.timer.timer_id==EAT_TIMER_5)
        		{
                   eat_adc_get(EAT_PIN25_ADC,0,NULL);//ADCÖµ
        		   time5_mainlink_heart();
        		}
			    break;
			    
           case EAT_EVENT_MDM_READY_RD:
                mdm_rx_proc(&event);
                break;
			    
			case EAT_EVENT_UART_READY_RD:
    		    sim_bsp_read_uart_data(&event);
			    break;
			
			case EAT_EVENT_USER_MSG:
                thread_msg_proc_user1(*(event.data.user_msg.data));
			    break;
			    
            case EAT_EVENT_ADC ://adÖµ
                if(event.data.adc.pin == EAT_PIN25_ADC)
                {
                   adc_value_pro(event.data.adc.v);
                   adc_power_porc();
                }
                break;
			default:
			break;
		}
   }
}

static void app_user2(void *data)// µÚ¶þ·þÎñÆ÷ SERVER ´¦Àí
{
    EatEvent_st event;
    while(EAT_TRUE)
    {
        eat_get_event_for_user(EAT_USER_2, &event);
		switch (event.event)
		{
			case EAT_EVENT_TIMER :
			    if(event.data.timer.timer_id==EAT_TIMER_4)
        		{
        		   time4_secondlink_anaisy();
        		}
        		else if(event.data.timer.timer_id==EAT_TIMER_6)
        		{
        		   time6_secondlink_heart();
        		}
			    break;
			
			case EAT_EVENT_MDM_READY_RD://ÄÚºËAT·µ»Ø
                mdm_rx_proc(&event);
			    break;
			    
			case EAT_EVENT_UART_READY_RD:
    		    sim_bsp_read_uart_data(&event);
			    break;
			
			case EAT_EVENT_UART_SEND_COMPLETE:
			    break;
            case EAT_EVENT_USER_MSG:
                thread_msg_proc_user2(*(event.data.user_msg.data));
                break;
                
           case EAT_EVENT_ADC ://adÖµ
               if(event.data.adc.pin == EAT_PIN25_ADC)
               {
                  adc_value_pro(event.data.adc.v);
                  adc_power_porc();
               }
               break;
			default:
			    break;
		}
   }
}

static void app_user3(void *data)
{
    EatEvent_st event;
    eat_timer_start(EAT_TIMER_8, 200);//¿ªÆô¶¨Ê±Æ÷
    while(EAT_TRUE)
    {
        eat_get_event_for_user(EAT_USER_3, &event);
		switch (event.event)
		{
			case EAT_EVENT_TIMER :
			    if(event.data.timer.timer_id==EAT_TIMER_8)
        		{
                    time8_can_data_proc();
        		}
			    break;
			
			case EAT_EVENT_MDM_READY_RD://ÄÚºËAT·µ»Ø
                mdm_rx_proc(&event);
			    break;
			
			case EAT_EVENT_UART_READY_RD:
    		    sim_bsp_read_uart_data(&event);
			    break;
			
			case EAT_EVENT_UART_SEND_COMPLETE:
			    break;
            case EAT_EVENT_USER_MSG:
                thread_msg_proc_user3(*(event.data.user_msg.data));
                break;
            
            case EAT_EVENT_ADC ://adÖµ
                if(event.data.adc.pin == EAT_PIN25_ADC)
                {
                   adc_value_pro(event.data.adc.v);
                   adc_power_porc();
                }
                break;
			default:
			    break;
		}
   }
}

