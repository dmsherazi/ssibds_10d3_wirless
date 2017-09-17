/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "Inflection_pass_up.h"

 /****************************************************
* External variables declaration
*****************************************************/
#if 0
void inflect_pass_up_proc(void)
{
    uint8_t inflect_cnt=0;
    uint8_t out_signal=0;
    uint8_t check_base=0;
    static uint8_t data_fre=0;
    static uint8_t inflect_offest=0;
    if(inflect_offest!=INFLECT_MAX)
    {
        data_fre++;
        if(data_fre == INFLECT_FRE)
        {
            data_fre=0;
            memcpy(&Inflect_Location_Array[inflect_offest],&Real_Time_Location_Inf,sizeof(Real_Time_Location_Inf));
            inflect_offest++;
        }
    }
    else
    {
        for(inflect_cnt=0;inflect_cnt<INFLECT_MAX;inflect_cnt++)
        {
            if(abs(Inflect_Location_Array[check_base].Location_Direction-Inflect_Location_Array[inflect_cnt].Location_Direction)>INFLECT_ANGLE)
            {
                  #if inflect_debug_printf 
                  eat_trace("inflect_pass_up_proc --> inflecting:check_baee:%u,inflect_offest:%u.",check_base,inflect_offest);
                  eat_trace("inflect_pass_up_proc --> true data ,check_baee:%u,inflect_offest:%u.",Inflect_Location_Array[check_base].Location_Direction,Inflect_Location_Array[inflect_offest].Location_Direction);
                  #endif
                  system_location_upadate(&Inflect_Location_Array[inflect_cnt]);  
                  check_base = inflect_cnt;
            }
            else
            {
                #if inflect_debug_printf 
                eat_trace("inflect_pass_up_proc --> NOT inflecting.");
                #endif
            }
        }
        inflect_offest=0;
     }
}
#endif
void inflect_pass_up_proc(void)
{
    static Location_Inf_Struct Inflect_Location_Array;
    #if inflect_debug_printf 
    eat_trace("inflect_pass_up_proc --> inflecting:%u",abs(Inflect_Location_Array.Location_Direction-Real_Time_Location_Inf.Location_Direction));
    #endif
    if(abs(Inflect_Location_Array.Location_Direction-Real_Time_Location_Inf.Location_Direction)>INFLECT_ANGLE)
    {
          #if inflect_debug_printf 
          eat_trace("inflect_pass_up_proc --> inflecting:%u",abs(Inflect_Location_Array.Location_Direction-Real_Time_Location_Inf.Location_Direction));
          #endif
          system_location_upadate();  
          memcpy(&Inflect_Location_Array,&Real_Time_Location_Inf,sizeof(Real_Time_Location_Inf));
    }
    else
    {
          #if inflect_debug_printf 
          eat_trace("inflect_pass_up_proc --> NOT inflecting.");
          #endif
    }
}

