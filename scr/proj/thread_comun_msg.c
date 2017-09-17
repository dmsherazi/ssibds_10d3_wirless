/****************************************************
 * Include Files
****************************************************/ 
 
#include "thread_comun_msg.h"

 /****************************************************
* External variables declaration
*****************************************************/

/*****************************************************
* Local variables declaration(Static)
******************************************************/


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: thread_msg_proc_main
 *  Description: main 消息处理
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void task_msg_send(EatUser_enum user_src, EatUser_enum user_dst,ThreadMsgName data)
{
	uint8_t Temp_Data=0;
	Temp_Data = (u8)data;
	eat_send_msg_to_user(user_src,user_dst,EAT_FALSE,1,&Temp_Data,EAT_NULL);//发送消息到USER_2
}
void thread_msg_proc_main(uint8_t msg_id)
{
    switch(msg_id)
    {
        case StartBearerOpen://系统开始消息
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_main --> msg:StartBearerOpen.");
            #endif
            if(System_Flag.System_Flag_Bit.Mode_Active==EAT_TRUE)
            {
                System_Flag.System_Flag_Bit.Mode_Active=EAT_FALSE;
            }
		    if(Wether_SIM_DEF==EAT_TRUE)
		    {
                if(Real_Time_Location_Inf.Sing_strength>=3)
                {
                    if(System_Flag.System_Flag_Bit.Mode_Active_ing==EAT_FALSE)
                    {
                        close_gprs_bearer();
                        gprs_bearer_open(system_param.SaveMainBeareParameter.beare_apn,\
                                            system_param.SaveMainBeareParameter.beare_user,\
                                            system_param.SaveMainBeareParameter.beare_password);//激活
                      System_Flag.System_Flag_Bit.Mode_Active_ing=EAT_TRUE;                    
                    }
                }
                else
                {
                    #if thread_msg_por_debug_printf 
                    eat_trace("thread_msg_proc_main --> signal_strength<2");
                    #endif
                }
                gprs_bearers_time_out(EAT_TRUE);
            }
            else
            {
                #if thread_msg_por_debug_printf 
                eat_trace("thread_msg_proc_main -->sim card is off, bearer is inviad.");
                #endif
            }
            break;
        case SystemSleepMsg://系统休眠
            System_Flag.System_Flag_Bit.second_can_data_send_state=EAT_FALSE;
            System_Flag.System_Flag_Bit.can_data_send_state=EAT_FALSE;
            eat_timer_start(EAT_TIMER_7,5000);
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_main --> msg:SystemSleepMsg.");
            #endif
            break;
            
        case take_pictures_timeout:
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_user1 --> msg:take_pictures_timeout:拍照超时处理消息.");
            #endif
            take_pictures_timeout_msg_proc();
            break;
        case closet_socket://关闭端口处理

             break;
        default:
            break;
            
    }
}
void thread_msg_proc_user1(uint8_t msg_id)
{
    switch(msg_id)
    {
        case LinkFisrtServer://系统开始消息
            first_server_conect_time_out(EAT_TRUE);//连接超时
            link_first_server_proc();
            break;
        case FirstPlatformReg:  
             first_platform_certification_time_out(EAT_TRUE);
             platform_certification(Main_Link.mode_Socket);
             break;
        case Timer3Start:
             eat_timer_start(EAT_TIMER_3, 100); 
             break;
        case Timer5Start:
             time5_mainlink_heart();
             break;
        case FirstSeverSleepMsgProc:
             do
             {
                 #if APP_TRACE_DBG 
                 eat_trace("system sleep first ip--> Analyze gprs data step");
                 #endif
                 receive_pro_list_del(Main_Link.mode_Socket,&first_ip_data_pack);
             }while(list_empty(&first_ip_data_pack)!=EAT_TRUE);
             break;
            case SystemWakeup:
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_user1 --> msg:SystemWakeup.");
            #endif
            system_user1_wake_up();
            break;
        case media_update_msg:
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_user1 --> msg:media_update_msg.:多媒体上传消息");
            #endif
            media_update_pro(Main_Link.mode_Socket);
            break;
            break;
        case take_pictures_action:
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_user1 --> msg:take_pictures_action:拍照.");
            #endif
            take_picture_action_proc();
            break;
        case uart_485_data_send:
            #if thread_msg_por_debug_printf 
            eat_trace("thread_msg_proc_user1 --> msg:take_pictures_action:485检测发送.");
            #endif
            create_send_rs485_frame();//485测试;;
            create_send_rs485_frame();//485测试
            create_send_rs485_frame();//485测试
            break;
        default:
            break;                
    }
}
void thread_msg_proc_user2(uint8_t msg_id)
{
    switch(msg_id)
    {
        case LinkSecondServer://连接第二服务器
             second_server_conect_time_out(EAT_TRUE);//连接超时
             link_second_server_proc();
             break;
        case SecondPlatformReg:  
             second_platform_certification_time_out(EAT_TRUE);
             platform_certification(Second_Link.mode_Socket);
             break;
       case Timer4Start:
             eat_timer_start(EAT_TIMER_4, 100); 
             break;
       case Timer6Start:
             time6_secondlink_heart();
             break;
        case SeconSeverdSleepMsgProc:
            do
            {
                #if APP_TRACE_DBG 
                eat_trace("system sleep first ip--> Analyze gprs data step");
                #endif
                receive_pro_list_del(Second_Link.mode_Socket,&second_ip_data_pack);
            }while(list_empty(&second_ip_data_pack)!=EAT_TRUE);
            break;
        default:
            break;                
    }
}

void thread_msg_proc_user3(uint8_t msg_id)
{
   switch(msg_id)
    {
        case dbc_file_read_msg:
              #if thread_msg_por_debug_printf 
              eat_trace("thread_msg_proc_user3 --> dbc_file_read_msg");
              #endif
              dbc_file_read_line();
              break;
         case dbc_file_analyze_msg:
              dbc_file_analyze();
              task_msg_send(EAT_USER_3,EAT_USER_3,dbc_file_read_msg);//发送消息到USER_2
              #if thread_msg_por_debug_printf 
              eat_trace("thread_msg_proc_user3 --> dbc_file_analyze_msg");
              #endif
              break;
        case dbc_file_analyze_end_msg:
              dbc_file_analyze();
              #if thread_msg_por_debug_printf 
              eat_trace("thread_msg_proc_user3 --> seting can id");
              #endif
              board_can_inf_setting();
              break;
        case analyze_can_data_msg:
             //analyze_recive_can_data();
             break;
        case driver_record_data_proc:

             #if thread_msg_por_debug_printf 
             eat_trace("thread_msg_proc_user3 --> driver_record_data_proc !");
             #endif
             hy_dring_record_data_inquire_proc();

             break;
        default:
              break;   
     }
}


