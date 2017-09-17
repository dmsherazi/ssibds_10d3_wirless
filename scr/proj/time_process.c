/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "time_process.h"

 /****************************************************
* External variables declaration
*****************************************************/


/*****************************************************
* Local variables declaration(Static)
******************************************************/
static TimeoutEvent AtCmdTimeoutParam={EAT_FALSE,{5,0}};//AT 指令超时参数
static TimeoutEvent GprsBearersTimeoutParam={EAT_FALSE,{0,0}};//激活超时参数
static TimeoutEvent FirstConectTimeoutParam={EAT_FALSE,{0,0}};//第一服务器连接超时参数
static TimeoutEvent SecondConectTimeoutParam={EAT_FALSE,{0,0}};//第二服务器连接超时参数
static TimeoutEvent FirstPlatformParam={EAT_FALSE,{0,0}};//注册鉴权超市
static TimeoutEvent SecondPlatformParam={EAT_FALSE,{0,0}};//注册鉴权超市
static TimeoutEvent UpdateBoard={EAT_FALSE,{0,0}};
static TimeoutEvent media_data_update={EAT_FALSE,{10,3}};
static TimeoutEvent lock_cmd_send={EAT_FALSE,{5,3}};

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: lock_car_cmd_timeout_cb
 *  Description: 平台锁车指令发发送            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void lock_car_cmd_timeout_cb(char_t cmd)
{    
    #if time_process_debug_printf 
    eat_trace("lock_car_cmd_timeout_cb -->start or over :%d",cmd);
    #endif
    lock_cmd_send.on_off = cmd;
}
static void lock_car_cmd_timeout_handle(void) 
{
    static uint8_t data_cnt=0;
    static uint8_t time_data_plus=0;
    if(EAT_TRUE == lock_cmd_send.on_off) //AT 指令超市
    {
       time_data_plus++; 
       if(time_data_plus==lock_cmd_send.time_out_struct.waiting_time)
       {
         data_cnt++;
         time_data_plus=0;
         lock_cmd_send.on_off =EAT_FALSE;
         if(data_cnt==lock_cmd_send.time_out_struct.timeout_times)
         {
             #if time_process_debug_printf 
             eat_trace("lock_car_cmd_timeout_handle --> 多次超时");
             #endif
             data_cnt=0;
         }
         else
         {
             #if time_process_debug_printf 
             eat_trace("lock_car_cmd_timeout_handle -->ing cnt:%u",lock_cmd_send.time_out_struct.timeout_times);
             #endif
             lock_car_strategy_send();
             lock_car_cmd_timeout_cb(EAT_TRUE);
         }
       }
    }
    else
    {
        if(data_cnt!=0)data_cnt=0;
        if(time_data_plus!=0)time_data_plus=0;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: update_board_timeout_cb
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void media_update_timeout_ctrl(char_t cmd)
{
    media_data_update.on_off = cmd;
}
static void media_update_timeout_proc(void) //超时处理函数
{
    static uint8_t data_cnt=0;
    static uint8_t time_data_plus=0;
    if(EAT_TRUE == media_data_update.on_off) //AT 指令超市
    {
       time_data_plus++;
       media_update_timeout_ctrl(EAT_FALSE);
       if(time_data_plus==media_data_update.time_out_struct.waiting_time)
       {
         data_cnt++;
         time_data_plus=0;
         media_data_update.on_off =EAT_FALSE;
         if(data_cnt==media_data_update.time_out_struct.timeout_times)
         {
             #if APP_TRACE_DBG 
             eat_trace("media_update_timeout_proc -->end");
             #endif
             data_cnt=0;
         }
         else
         {
             #if APP_TRACE_DBG 
             eat_trace("media_update_timeout_proc -->ing cnt:%u",media_data_update.time_out_struct.timeout_times);
             #endif
             task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
         }
       }
    }
    else
    {
        if(data_cnt!=0)data_cnt=0;
        if(time_data_plus!=0)time_data_plus=0;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: update_board_timeout_cb
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void update_board_timeout_cb(char_t cmd,uint32_t wait_time)
{
    UpdateBoard.on_off = cmd;
    UpdateBoard.time_out_struct.waiting_time =wait_time;
    UpdateBoard.time_out_struct.timeout_times=3;
}
static void update_board_timeout_handle(void) //超时处理函数
{
    static uint8_t data_cnt=0;
    if(EAT_TRUE == UpdateBoard.on_off) //AT 指令超市
    {
       UpdateBoard.time_out_struct.waiting_time--; 
       if(0==UpdateBoard.time_out_struct.waiting_time)
       {
         UpdateBoard.on_off =EAT_FALSE;
         data_cnt++;
         if(data_cnt==UpdateBoard.time_out_struct.timeout_times)
         {
             #if APP_TRACE_DBG 
             eat_trace("update_board_timeout_handle -->end");
             #endif
             data_cnt=0;
             uart1_send_data_frame(board_update_id,updata_file,update_size);
             update_board_timeout_cb(EAT_FALSE,0);
         }
         else
         {
             #if APP_TRACE_DBG 
             eat_trace("update_board_timeout_handle -->ing cnt:%u",UpdateBoard.time_out_struct.timeout_times);
             #endif
             uart1_send_data_frame(board_update_id,updata_file,update_size);
             update_board_timeout_cb(EAT_TRUE,5);
         }
       }
    }
}
void AtCmdTimeOut_cb(char_t cmd,uint32_t wait_time)
{
    AtCmdTimeoutParam.on_off = cmd;
    AtCmdTimeoutParam.time_out_struct.waiting_time =wait_time;
}
static void AtCmdTimeOutHandle(void) //超时处理函数
{
    if(EAT_TRUE == AtCmdTimeoutParam.on_off) //AT 指令超市
    {
       AtCmdTimeoutParam.time_out_struct.waiting_time--; 
       if(0==AtCmdTimeoutParam.time_out_struct.waiting_time)
       {
         AtCmdTimeoutParam.on_off =EAT_FALSE; 
         exit_at_cmd_send_queue(&at_cmd);
         at_cmd.execution_statue = AT_CMD_idle;
       }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearers_time_out
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void gprs_bearers_time_out(char_t cmd)
{
    GprsBearersTimeoutParam.on_off = cmd;
    GprsBearersTimeoutParam.time_out_struct.timeout_times = system_param.SaveTcpRetransTimes.timeout_times;
    GprsBearersTimeoutParam.time_out_struct.waiting_time = system_param.SaveTcpRetransTimes.waiting_time;
}
static void GprsBearersTimeoutHandle(void)
{
   static uint32_t timout_count=0;
   static uint32_t timout_times=0;
   if(GprsBearersTimeoutParam.on_off==EAT_TRUE)
   {
        timout_count++;
        if(timout_count==GprsBearersTimeoutParam.time_out_struct.waiting_time*(timout_times + 1))
        {
            timout_count = 0;
            timout_times++;
            if(timout_times == GprsBearersTimeoutParam.time_out_struct.timeout_times)
            {
                timout_times = 0;
                GprsBearersTimeoutParam.on_off =EAT_FALSE;
                close_gprs_bearer();
            }
            System_Flag.System_Flag_Bit.Mode_Active_ing=EAT_FALSE;                    
            task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
        }
   }
   else
   {
        if((timout_count!=0)||(timout_times!=0))
        {
            timout_count=0;
            timout_times=0;
        }
   }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearers_time_out
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void first_server_conect_time_out(char_t cmd)
{
    FirstConectTimeoutParam.on_off = cmd;
    FirstConectTimeoutParam.time_out_struct.timeout_times = system_param.SaveTcpRetransTimes.timeout_times;
    FirstConectTimeoutParam.time_out_struct.waiting_time= system_param.SaveTcpRetransTimes.waiting_time;
}
static void FirstServerConectTimeoutHandle(void)
{
    static uint32_t timout_count=0;
    static uint32_t timout_times=0;
    if(EAT_TRUE==FirstConectTimeoutParam.on_off)
    {
        timout_count++;
        if(timout_count==FirstConectTimeoutParam.time_out_struct.waiting_time*(timout_times + 1))
        {
            timout_count = 0;
            timout_times++;
            #if time_process_debug_printf
            eat_trace("FirstServerConectTimeoutHandle --> main_link 重连次数:%d !",timout_times);
            #endif
            if(timout_times == FirstConectTimeoutParam.time_out_struct.timeout_times)
            {
                timout_times = 0;
                Main_Link.get_ip_statue=EAT_FALSE;
                if(Main_Link.link_index == MAIN_SERVER)
                {
                    Main_Link.link_index = BACKUP_MAIN_SEVER;
                    task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);//发送消息到USER_2
                    
                    #if time_process_debug_printf
                    eat_trace("FirstServerConectTimeoutHandle --> 切换连接第一服务器备份服务器 !");
                    #endif
                }
                else if(Main_Link.link_index == BACKUP_MAIN_SEVER)
                {
                    if(Second_Link.link_staue ==EAT_FALSE)
                    {
                        first_server_conect_time_out(EAT_FALSE);//停止第二服务器连接超时
                        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
                    }
                    else
                    {
                        Main_Link.link_index = MAIN_SERVER;
                        //task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);//发送消息到USER_2
                    }
                    
                }
                FirstConectTimeoutParam.on_off =EAT_FALSE;
            }
            else
            {
                task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
            }
        }
    }
    else
    {
        if((timout_count!=0)||(timout_times!=0))
        {
            timout_count=0;
            timout_times=0;
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearers_time_out
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void second_server_conect_time_out(char_t cmd)
{
    SecondConectTimeoutParam.on_off = cmd;
    SecondConectTimeoutParam.time_out_struct.timeout_times= system_param.SaveTcpRetransTimes.timeout_times;
    SecondConectTimeoutParam.time_out_struct.waiting_time= system_param.SaveTcpRetransTimes.waiting_time;
}
static void SecondServerConectTimeoutHandle(void)
{
    static uint32_t timout_count=0;
    static uint32_t timout_times=0;
       if(EAT_TRUE==SecondConectTimeoutParam.on_off)
       {
            timout_count++;
            if(timout_count==SecondConectTimeoutParam.time_out_struct.waiting_time*(timout_times + 1))
            {
                timout_count = 0;
                timout_times++;
                #if time_process_debug_printf
                eat_trace("SecondServerConectTimeoutHandle --> second_link 重连次数:%d !",timout_times);
                #endif
                if(timout_times == SecondConectTimeoutParam.time_out_struct.timeout_times)
                {
                    timout_times = 0;
                    Second_Link.get_ip_statue=EAT_FALSE;
                    if(Second_Link.link_index == SECOND_SERVER)
                    {
                        Second_Link.link_index = BACKUP_SECOND_SERVER;
                        task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                        #if time_process_debug_printf
                        eat_trace("SecondServerConectTimeoutHandle --> second_link 切换连接第二服务器备份服务器 !");
                        #endif
                    }
                    else if(Second_Link.link_index == BACKUP_SECOND_SERVER)//重新激活。
                    {
                        if(Main_Link.link_staue ==EAT_FALSE)
                        {
                            second_server_conect_time_out(EAT_FALSE);//停止第二服务器连接超时
                            task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
                        }
                        else
                        {
                            Second_Link.link_index = SECOND_SERVER;
                            
                            //task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                        }
                        
                    }
                    SecondConectTimeoutParam.on_off =EAT_FALSE;
                }
                else
                {
                    task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                }
            }
       }
       else
       {
            if((timout_count!=0)||(timout_times!=0))
            {
                timout_count=0;
                timout_times=0;
            }
       }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearers_time_out
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void first_platform_certification_time_out(char_t cmd)
{
    FirstPlatformParam.on_off = cmd;
    FirstPlatformParam.time_out_struct.timeout_times= system_param.SaveTcpRetransTimes.timeout_times;
    FirstPlatformParam.time_out_struct.waiting_time= system_param.SaveTcpRetransTimes.waiting_time;
}
static void FirstPlatformCertificationTimeoutHandle(void)
{
    static uint32_t timout_count=0;
    static uint32_t timout_times=0;
    if(EAT_TRUE==FirstPlatformParam.on_off)
    {
        timout_count++;
        if(timout_count==FirstPlatformParam.time_out_struct.waiting_time*(timout_times + 1))
        {
            timout_count = 0;
            timout_times++;
            
            #if time_process_debug_printf
            eat_trace("SecondPlatformCertificationTimeoutHandle --> main_link 鉴权次数:%d !",timout_times);
            #endif
            if(timout_times == FirstPlatformParam.time_out_struct.timeout_times)
            {
                timout_times = 0;
                FirstPlatformParam.on_off =EAT_FALSE;
                if(strlen(system_param.terminl_registe.first_authentication_id)!=0)
                {
                    memset(system_param.terminl_registe.first_authentication_id,0,strlen(system_param.terminl_registe.first_authentication_id));
                    task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
                }
                else
                {
                    //task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
                }
                #if time_process_debug_printf
                eat_trace("SecondPlatformCertificationTimeoutHandle --> main_link 鉴权次数:%d !",timout_times);
                #endif
            }
            else
            {
                task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
            }
        }
    }
    else
    {
        if((timout_count!=0)||(timout_times!=0))
        {
            timout_count=0;
            timout_times=0;
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearers_time_out
 *  Description: AtCmdTimeOut_cb            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void second_platform_certification_time_out(char_t cmd)
{
    SecondPlatformParam.on_off = cmd;
    SecondPlatformParam.time_out_struct = system_param.SaveTcpRetransTimes;
}
static void SecondPlatformCertificationTimeoutHandle(void)
{
    static uint32_t timout_count=0;
    static uint32_t timout_times=0;
       if(EAT_TRUE==SecondPlatformParam.on_off)
       {
            timout_count++;
            if(timout_count==SecondPlatformParam.time_out_struct.waiting_time*(timout_times + 1))
            {
                timout_count = 0;
                timout_times++;
                #if time_process_debug_printf
                eat_trace("SecondPlatformCertificationTimeoutHandle --> second_link 鉴权次数:%d !",timout_times);
                #endif
                if(timout_times == SecondPlatformParam.time_out_struct.timeout_times)
                {
                    timout_times = 0;
                    SecondPlatformParam.on_off =EAT_FALSE;
                    if(strlen(system_param.terminl_registe.second_authentication_id)!=0)
                    {
                        memset(system_param.terminl_registe.second_authentication_id,0,strlen(system_param.terminl_registe.second_authentication_id));
                        task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
                    }
                    else
                    {
                        //task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
                    }
                    #if time_process_debug_printf
                    eat_trace("SecondPlatformCertificationTimeoutHandle --> second_link 三次鉴权失败 !");
                    #endif
                }
                else
                {
                    task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
                }
            }
       }
       else
       {
            if((timout_count!=0)||(timout_times!=0))
            {
                timout_count=0;
                timout_times=0;
            }
       }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: event_timeout
 *  Description: system clock time base 1s
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void event_timeout(void) //超时处理总函数
{
   AtCmdTimeOutHandle();//AT 超时
   update_board_timeout_handle();//升级超时

   GprsBearersTimeoutHandle();//gprs激活超时函数
   
   /*多媒体上传超时*/
   media_update_timeout_proc();
   
   /*锁车超时处理*/
   lock_car_cmd_timeout_handle();
   
   FirstServerConectTimeoutHandle();
   SecondServerConectTimeoutHandle();

   FirstPlatformCertificationTimeoutHandle();
   SecondPlatformCertificationTimeoutHandle();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: system_clock
 *  Description: system clock time base 500s
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */ 
void system_clock_timer1_ms(void)//500ms uart 处理队列
{
    /*AT 指令队列处理*/
    at_cmd_queue_process();
    
    /*串口一接收数据处理*/
    uart1_receive_queue_proc();
    
    /*串口一发送数据处理*/
    uart1_send_queue_proc();
    
    /*上传CAN数据*/
    idle_update_can_data_proc(Main_Link.mode_Socket);

    /*平台查询行驶记录数据*/
    inquire_dring_record_queue_proc();

    /*开启定时器*/
	eat_timer_start(EAT_TIMER_1, 100);//开启定时器
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: system_clock
 *  Description: system clock time base 500s
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */ 
void send_data_to_main_board(void)
{
    static uint8_t send_data_cnt=0;    
    send_data_cnt++;
    if(send_data_cnt==2)
    {
        send_data_cnt=0;
        enter_at_cmd_send_queue(&at_cmd,AT_CHECK_SIG);
        enter_at_cmd_send_queue(&at_cmd,AT_SIM_INSET);
    }
    if(UpdateBoard.on_off==EAT_FALSE)
    {
        uart1_sim800_send_1s_proc();
    }
    
}
void system_clock_timer2_s(void)
{
    /*超时处理事件*/
    event_timeout();
    
    /*短信发送超时处理*/
    sms_send_time_out_handle();
    
    /*位置定时上报*/
    timeout_location_report();
    
    /*与主控间的通信*/
    send_data_to_main_board();
    
    /*升级超时处理*/
    updata_time_out();
    
     /*定时拍照*/
    take_pictures_regularly();
    
    /*定Gnss数据开关*/
    detail_gnss_data_updata_ctrl();
    
    /*透传GNSS数据 FADE*/
    penetrate_record_gnss_data(Main_Link.mode_Socket);

    /*获取电压值*/
    eat_adc_get(EAT_PIN25_ADC,0,NULL);//ADC值
    
    /*休眠判断*/
    //system_sleep_juged();
	eat_timer_start(EAT_TIMER_2, 1000);//开启定时器
}

void time3_mainlink_anaisy(void)
{
        #if 0 //APP_TRACE_DBG 
        eat_trace("time3_mainlink_anaisy--> mode First_Analyze_TCP_Type_Setup");
		HexToAsicc(&(FirstIpAnaData.TCP_Data_buf[FirstIpAnaData.head][0]),FirstIpAnaData.Tcp_Len[FirstIpAnaData.head]);
        #endif
		receive_pro_list_del(Main_Link.mode_Socket,&first_ip_data_pack);
	    eat_timer_start(EAT_TIMER_3, 200);  
}
void time4_secondlink_anaisy(void)
{
    
    #if 0
	if(IS_TCP_Data_QueueEmpty(&SecondIpAnaData)==EAT_FALSE)
 	{
		//Second_Analyze_TCP_Type_Setup(Second_Link.mode_Socket,&(SecondIpAnaData.TCP_Data_buf[SecondIpAnaData.head][0]));
		Exit_TCP_Data_Queue(&SecondIpAnaData);	
	}
	#endif
    receive_pro_list_del(Second_Link.mode_Socket,&second_ip_data_pack);
	eat_timer_start(EAT_TIMER_4, 200);
}
void time5_mainlink_heart(void)
{
    static uint32_t location_up=0;
    First_Server_Heat_Beat_Send();
    if(Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)
    {
        location_up++;
        link_in_system_sleep_proc();
        get_sim800_store_timer();
        if(sys_sleep_state_black_or_light())
        {
            #if APP_TRACE_DBG 
            eat_trace("time5_mainlink_heart--> 白天");
            #endif
            if(location_up>=((uint32_t)(system_param.location_reporting_param.sleeping_frze/system_param.HeartBeatUpdataFre))) 
            {
                location_up=0;
                First_Server_Location_Inf_Send();
            }
        }
        else
        {
            if(location_up>=((uint32_t)(system_param.location_reporting_param.night_sleep_frze/system_param.HeartBeatUpdataFre))) 
            {
                location_up=0;
                First_Server_Location_Inf_Send();
            }

        }
	}
    eat_timer_start(EAT_TIMER_5, system_param.HeartBeatUpdataFre*1000);
}
void time6_secondlink_heart(void)
{
	static uint32_t location_up=0;
    Second_Server_Heat_Beat_Send();
    if(Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)
    {
        location_up++;
        second_link_in_system_sleep_proc();
        get_sim800_store_timer();
        if(sys_sleep_state_black_or_light())
        {
            if(location_up>=((uint32_t)(system_param.location_reporting_param.sleeping_frze/system_param.HeartBeatUpdataFre))) 
            {
                location_up=0;
                Second_Server_Location_Inf_Send();
            }
        }
        else
        {
            if(location_up>=((uint32_t)(system_param.location_reporting_param.night_sleep_frze/system_param.HeartBeatUpdataFre))) 
            {
                location_up=0;
                Second_Server_Location_Inf_Send();
            }

        }
	}
    eat_timer_start(EAT_TIMER_6, system_param.HeartBeatUpdataFre*1000);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: link_in_system_sleep_proc
 *  Description: 休眠状态下链接处理
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */ 
void link_in_system_sleep_proc(void)
{
    if(GprsBearersTimeoutParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("link_in_system_sleep_proc --> GprsBearersTimeoutParam.on_off==EAT_TRUE!!!!.");
        #endif
        GprsBearersTimeoutParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
    if(FirstConectTimeoutParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("link_in_system_sleep_proc --> FirstConectTimeoutParam.on_off==EAT_TRUE!!!!.");
        #endif
        FirstConectTimeoutParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
    if(FirstPlatformParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("link_in_system_sleep_proc --> FirstPlatformParam.on_off==EAT_TRUE!!!!.");
        #endif
        FirstPlatformParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
}
void second_link_in_system_sleep_proc(void)
{
    if(GprsBearersTimeoutParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("second_link_in_system_sleep_proc --> GprsBearersTimeoutParam.on_off==EAT_TRUE!!!!.");
        #endif
        GprsBearersTimeoutParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
    if(SecondConectTimeoutParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("second_link_in_system_sleep_proc --> FirstConectTimeoutParam.on_off==EAT_TRUE!!!!.");
        #endif
        SecondConectTimeoutParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
    if(SecondPlatformParam.on_off==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("second_link_in_system_sleep_proc --> FirstPlatformParam.on_off==EAT_TRUE!!!!.");
        #endif
        SecondPlatformParam.on_off=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
    }
}

void time7_sleep_data_proc(void)
{
    static uint32_t sleep_cnt=0;
    if(list_empty(&analysised_can_list)==EAT_TRUE)
    {
        #if APP_TRACE_DBG 
        eat_trace("time7_sleep_data_proc --> sleep is_can_send_queue_empty.");
        #endif
        if(sleep_cnt!=0)
        {
            sleep_cnt=0;
        }
        system_sleep();
        eat_timer_stop(EAT_TIMER_7);
    }
    else
    {
        #if APP_TRACE_DBG 
        eat_trace("time7_sleep_data_proc --> queue not empty!!.");
        #endif
        sleep_cnt++;
        idle_update_can_data_proc(Second_Link.mode_Socket);
        if(sleep_cnt==system_param.HeartBeatUpdataFre*2)
        {
            sleep_cnt=0;
            #if APP_TRACE_DBG 
            eat_trace("time7_sleep_data_proc --> sleep SystemSleepFre tiome out.");
            #endif
            system_sleep();
            eat_timer_stop(EAT_TIMER_7);
        }
        else
        {
            eat_timer_start(EAT_TIMER_7,1000);
        }
    }
}
void time8_can_data_proc(void)
{
    can_data_analysis_proc();
    eat_timer_start(EAT_TIMER_8, 200);//开启定时器
}

