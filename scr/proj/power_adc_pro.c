
#include "power_adc_pro.h"
/* 
* ===  FUNCTION  ======================================================================
*  Name: wake_adc_power_porc
*  Description:  ������״̬��Ƿѹ����
*  Parameters :  ���� undervoltage_reference_value ��Ƿѹ���Ƿѹ����             
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D03 at 2017-2-5           
* =====================================================================================
*/ 
static void wake_adc_power_porc(void)
{
    static u8 undervoltage_cnt=0;
    static u8 wake_volutage_cnt=0;
    static u8 undervoltage_state=EAT_FALSE;
    if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
    {
        if(undervoltage_state==EAT_FALSE)
        {
            undervoltage_cnt++;
            if(undervoltage_cnt==averag_times)
            {
                #if power_adc_porc_debug_printf
                eat_trace("wake_adc_power_porc -->> Ƿѹ��ʼ���Ƶ�Դ��");
                #endif
                undervoltage_cnt=0;
                /********����ߵ�ƽ��5V��Դ**********/
                eat_pin_set_mode(EAT_PIN35_PWM1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
                eat_gpio_setup(EAT_PIN35_PWM1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����Ĭ��LOW
                undervoltage_state=EAT_TRUE;
            }
        }
        if(wake_volutage_cnt==0)
        {
            wake_volutage_cnt = 0;
        }
    }
    else 
    {
        if(undervoltage_state==EAT_TRUE)
        {
            wake_volutage_cnt++;
            if(wake_volutage_cnt==averag_times)
            {
                undervoltage_state=EAT_FALSE;
                /********����͵�ƽ�ش�5V��Դ**********/
                eat_pin_set_mode(EAT_PIN35_PWM1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
                eat_gpio_setup(EAT_PIN35_PWM1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����Ĭ��LOW
                #if power_adc_porc_debug_printf
                eat_trace("wake_adc_power_porc -->> Ƿѹ�����");
                #endif
                wake_volutage_cnt=0;
            }
        }
        if(undervoltage_cnt==0)
        {
            undervoltage_cnt = 0;
        }
    }
}
/*
* ===  FUNCTION  ======================================================================
*  Name: sleep_adc_power_proc
*  Description:  ����״̬��Ƿѹ����
*  Parameters :  ֱ�ӿ���            
*  Return     :  void 
*  author     : J.x.b create at SSI_1D03 at 2017-2-5             
* =====================================================================================
*/ 

static void sleep_adc_power_proc(void)
{
    /***************************����Ƿѹ����**********************************/ 
    if(Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage == EAT_FALSE)
    {
          if(Real_Time_Location_Inf.Power_volate<undervoltage_reference_value)
          {
              Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage = EAT_TRUE;
              if(Shield_SMS_Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
              {
                  worning_sms_send("Ƿѹ");
              }
              system_location_upadate();
              #if power_adc_porc_debug_printf
              eat_trace("sleep_adc_power_proc -->> ����Ƿѹ��ʼ���Ƶ�Դ��");
              #endif
              /********����ߵ�ƽ��5V��Դ**********/
              eat_pin_set_mode(EAT_PIN35_PWM1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
              eat_gpio_setup(EAT_PIN35_PWM1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);//����Ĭ��LOW
          }
    }
    else
    {
        if(Real_Time_Location_Inf.Power_volate>=undervoltage_reference_value)
        {
            Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Undervoltage = EAT_FALSE;
            if(Shield_SMS_Alarm_Inf.Alarm_Bit.Undervoltage == EAT_TRUE)
            {
                 worning_sms_send("Ƿѹ���");
            }
            system_location_upadate();
            #if power_adc_porc_debug_printf
            eat_trace("sleep_adc_power_proc -->> ����Ƿѹ�����");
            #endif
            /********����͵�ƽ�ش�5V��Դ**********/
            eat_pin_set_mode(EAT_PIN35_PWM1, EAT_PIN_MODE_GPIO);//����ΪGPIOģʽ
            eat_gpio_setup(EAT_PIN35_PWM1, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);//����Ĭ��LOW
        }
   }
}
/*
* ===  FUNCTION  ======================================================================
*  Name: adc_power_porc
*  Description:  �ⲿ������Ƿѹ���ƴ���
*  Parameters :  ֱ�ӿ���            
*  Return     :  void 
*  author     : J.x.b create at SSI_1D03 at 2017-2-5             
* =====================================================================================
*/ 
void get_adc_period(void)
{
    static u8 period_data=0;
    period_data++;
    if(period_data==15)
    {
        period_data=0;
        eat_adc_get(EAT_PIN25_ADC,NULL,NULL);//ADCֵ
    }
}
void adc_power_porc(void)
{
    if(Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep==EAT_TRUE)
    {
        sleep_adc_power_proc();
    }
    else
    {
        wake_adc_power_porc();
    }
}
/*
* ===  FUNCTION  ======================================================================
*  Name: adc_value_pro
*  Description:  ADֵ����
*  Parameters :  volate_value: ϵͳ���صĵ�ѹ����ֵ            
*  Return     :  void 
*  author     : J.x.b create at SSI_1D03 at 2017-2-5             
* =====================================================================================
*/ 
void adc_value_pro(u32 volate_value)
{
    u32 volate=0;
    #if power_adc_porc_debug_printf
    eat_trace("adc_value_pro -->>power_adc orgin = %d.",volate_value);
    #endif
    volate=((volate_value*611)/51)+700;
    #if power_adc_porc_debug_printf
    eat_trace("adc_value_pro -->>volate orgin = %d.",volate);
    #endif
    Real_Time_Location_Inf.Power_volate = (u16)(volate/100);
    if((Real_Time_Location_Inf.Power_volate<210)&&(Real_Time_Location_Inf.Power_volate>=150))
    {
       Real_Time_Location_Inf.Power_volate-=2; 
    }
    else
    {
        Real_Time_Location_Inf.Power_volate-=3; 
    }
    #if power_adc_porc_debug_printf
    eat_trace("adc_value_pro -->>power_adc = %d.",Real_Time_Location_Inf.Power_volate);
    #endif
}

