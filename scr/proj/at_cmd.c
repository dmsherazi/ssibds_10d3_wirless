/****************************************************
 * Include Files
****************************************************/ 
 
#include "at_cmd.h"

 /****************************************************
* External variables declaration
*****************************************************/
SEND_AT_CMD_Queue at_cmd;
char_t Wether_SIM_DEF=EAT_FALSE;//SIM卡是否在位

/*****************************************************
* Local variables declaration(Static)
******************************************************/

const AT_CMD_SEND Send_AT_cmd[]=
{
    {AT_ECOH_INIT,     "ATE0\r\n",            "OK"    },/*关闭回显,并接收回显设置OK*/
    {AT_CSDT,       "AT+CSDT=1\r\n",          "OK"    },/*开SIM卡检测OK*/
    {AT_ECOH_CIPMUX,   "AT+CIPMUX=1\r\n",     "OK"    },/*开多IP连接模式OK*/
    {AT_CHECK_SIG,     "AT+CSQ\r\n",          "+CSQ: "},/*检测信号强度*/
    {AT_CLOSE_VOLTAGE_CHECK,"AT+CBATCHK=0\r\n",  "OK" },/*关闭电压检测*/
    {AT_DELET_ALL_SMS,"AT+CMGD=1,4\r\n",  "OK" },/*删除短信*/
    {AT_SIM_INSET,"AT+CSMINS?\r\n",        "+CSMINS: "},/*SIM卡状态*/\
    {AT_SET_TTS_PARAM,"AT+CTTSPARAM=80,0,50,52,0\r\n","OK"},/*语音播报设置*/
    {AT_SET_TTS_VOL,"AT+CLVL=80\r\n","OK"},/*语音播报设置*/
    {at_call_not_ring,"AT+CALM=1\r\n","OK"},/*不响铃*/
    {at_call_num,"AT+CLIP=1\r\n","OK"},/*来电显示*/
    {AT_PARAM_SAVE,"AT&W_SAVE","OK"},/*参数保存*/
    {AT_TTS_IC_IN,"AT+CTTS=2,\"您已登陆\"\r\n","CTTS: 0"},/*IC卡插入*/
    {AT_TTS_IC_OUT,"AT+CTTS=2,\"司机卡已拔出\"\r\n","CTTS: 0"},/*IC卡拔出*/
    {AT_TTS_SP_OUT,"AT+CTTS=2,\"您已超速，请减速行驶\"\r\n","CTTS: 0"},/*超速提示*/
    {AT_TTS_PL_DRIVE,"AT+CTTS=2,\"您已疲劳驾驶，请停车休息\"\r\n","CTTS: 0"},/*疲劳驾驶提示*/  
    {AT_TTS_SPING,"AT+CTTS=2,\"您即将超速，请减速行驶\"\r\n","CTTS: 0"},/*即将超速提示*/
    {AT_TTS_PLING,"AT+CTTS=2,\"您将疲劳驾驶，请停车休息\"\r\n","CTTS: 0"},/*即将疲劳提示*/
    {AT_TTS_SP_ERROR,"AT+CTTS=2,\"车速异常\"\r\n","CTTS: 0"},/*速度异常*/
    {AT_CHANG_IC_CARD,"AT+CTTS=2,\"请停车换卡!\"\r\n","CTTS: 0"},/*停产换卡*/
    {at_not_printf,"AT+CTTS=2,\"行驶中，请勿打印!\"\r\n","CTTS: 0"},/*停产换卡*/
    {AT_ANTENA_ERROR,"AT+CTTS=2,\"定位天线断开报警，请检查故障\"\r\n","CTTS: 0"},/*天线断开报警*/
    {at_not_in_iccard,"AT+CTTS=2,\"您未登录，请停车插卡\"\r\n","CTTS: 0"},/*未登录请插卡*/
    {at_power_down,"AT+CTTS=2,\"主电源掉电\"\r\n","CTTS: 0"}/*主电源掉电*/
};

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: SEND_AT_CMD_Queue
  *  Description:  
  *  Parameters : *q队列结构体指针 
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
 static void init_at_cmd_send_queue(SEND_AT_CMD_Queue *q)
 {
    memset(q,0,sizeof(q));
    q->queuesize =AT_CMD_QUEUE_MAX;
    q->tail = 0;
    q->head = 0;
    q->execution_statue = AT_CMD_idle;
 }
void enter_at_cmd_send_queue(SEND_AT_CMD_Queue *q,AT_CMD_NAME name)
 {
    unsigned char Tail = (q->tail+1)%(q->queuesize); //取余保证，当quil=queuesize-1时，再转回
    if (Tail == q->head) //此时队列没有空间
    {
		#if at_process_debug_printf 
		eat_trace("enter_at_cmd_send_queue --> 队列满.");
		#endif
    }
    else
    {
        q->AT_cmd_buf[q->tail]=name;
        q->tail = Tail;
		#if at_process_debug_printf 
		eat_trace("enter_at_cmd_send_queue --> 进队列head:%d,tail:%d.",q->head,q->tail);
		#endif
    }
 }
void exit_at_cmd_send_queue(SEND_AT_CMD_Queue *q)
 {
     if(q->tail == q->head)     
     {
         #if at_process_debug_printf 
         eat_trace("exit_at_cmd_send_queue --> 队列为空.");
         #endif
     }
     else
     {
         q->head = (q->head+1) % (q->queuesize);
     }
     #if at_process_debug_printf 
     eat_trace("exit_at_cmd_send_queue --> 出队列head:%d,tail:%d.",q->head,q->tail);
     #endif
 }
 static char_t is_at_cmd_send_queue_empty(SEND_AT_CMD_Queue *q)
 {
      if(q->head == q->tail)
      {
          #if at_process_debug_printf 
          eat_trace("is_at_cmd_send_queue_empty --> 队列为空.");
          #endif
          return EAT_TRUE;
      }
      else
      {
          return EAT_FALSE;
      }
 }
 static char_t is_at_cmd_send_queue_full(SEND_AT_CMD_Queue *q)
 {
     if((q->tail+1)% q->queuesize == q->head)
     {
         return EAT_TRUE;
     }
     else
     {
         return EAT_FALSE;
     }
 }
 
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name: SEND_AT_CMD_Queue
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */
void at_cmd_queue_process(void)
{
    if(!(is_at_cmd_send_queue_empty(&at_cmd))==EAT_TRUE)
    {
        #if at_process_debug_printf 
        eat_trace("at_cmd_queue_process --> waiting.");
        #endif
        if(at_cmd.execution_statue == AT_CMD_idle)
        {
            #if at_process_debug_printf 
            eat_trace("at_cmd_queue_process --> ing.");
            #endif
            eat_modem_write(Send_AT_cmd[at_cmd.AT_cmd_buf[at_cmd.head]].pCmdString,\
                        strlen(Send_AT_cmd[at_cmd.AT_cmd_buf[at_cmd.head]].pCmdString));
            at_cmd.execution_statue = AT_CMD_busy;
            AtCmdTimeOut_cb(EAT_TRUE,5);
        }
    }
}
/*======================ENDSEND_AT_CMD_Queue===================================*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: wether_sim_is_on
 *  Description: wether simis on
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
static void wether_sim_is_on(uint8_t *p_str)
 {
    char_t *ret;
	ret=(char *)strstrLast((char*)p_str,"+CPIN: READY");//SIM卡是否插入
	if(ret!=NULL) 
	{
        #if at_process_debug_printf 
        eat_trace("wether_sim_is_on --> State:sim on.");
        #endif
        enter_at_cmd_send_queue(&at_cmd,AT_CSDT);
        Wether_SIM_DEF=EAT_TRUE;
	}
	ret=(char *)strstrLast((char*)p_str,"+CPIN: NOT READY");//SIM卡未插入
	if(ret!=NULL) 
	{
	    
        #if at_process_debug_printf 
        eat_trace("wether_sim_is_on --> State:sim off.");
        #endif
        Wether_SIM_DEF=EAT_FALSE;
	}	
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: wether_system_start
 *  Description: wether system start
 *        Input:            
 *       Output:
 *       Return:               
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
static void wether_system_start(uint8_t *p_str)
 {
    char_t *ret=NULL;
	static char_t core_start = EAT_FALSE;//内核是否启动
	static char_t start_at=EAT_FALSE;
    if(start_at==EAT_FALSE)
    {
        init_at_cmd_send_queue(&at_cmd);
        enter_at_cmd_send_queue(&at_cmd,AT_ECOH_INIT);
        enter_at_cmd_send_queue(&at_cmd,AT_CLOSE_VOLTAGE_CHECK);        
        enter_at_cmd_send_queue(&at_cmd,AT_CSDT);
        enter_at_cmd_send_queue(&at_cmd,AT_ECOH_CIPMUX);
        enter_at_cmd_send_queue(&at_cmd,AT_CHECK_SIG);
        enter_at_cmd_send_queue(&at_cmd,AT_SIM_INSET);
        enter_at_cmd_send_queue(&at_cmd,AT_SET_TTS_PARAM);
        enter_at_cmd_send_queue(&at_cmd,AT_SET_TTS_VOL);
        enter_at_cmd_send_queue(&at_cmd,at_call_not_ring);
        enter_at_cmd_send_queue(&at_cmd,at_call_num);
        enter_at_cmd_send_queue(&at_cmd,AT_PARAM_SAVE);
        start_at=EAT_TRUE;
    }
	ret=(char *)strstrLast((char*)p_str,"SMS Ready");
	if(ret!=NULL) 
	{
		core_start = EAT_TRUE;
	}
	if((Wether_SIM_DEF==EAT_TRUE)&&(core_start==EAT_TRUE)) 
	{
		#if at_process_debug_printf 
		eat_trace("wether_system_start --> system start ok.");
		#endif
		core_start=EAT_FALSE;
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
	}
 }
 
 /* 
  * ===  FUNCTION  ======================================================================
  *         Name: get_csq_cb
  *  Description: get_csq_cb
  *        Input:            
  *       Output:
  *       Return:               
  *       author: Jumping create at 2013-12-16
  * =====================================================================================
  */
 static void strstr_at_cmd_cb(uint8_t *p_str)
  {
    char_t *pHead = NULL;
	char_t *pTail = NULL;
	uint8_t decbit;
	uint8_t tempH,tempL;
	if(at_cmd.execution_statue == AT_CMD_busy)
	{
        if(at_cmd.AT_cmd_buf[at_cmd.head]== AT_CHECK_SIG)
        {
            pHead=(char *)strstrLast(p_str,Send_AT_cmd[AT_CHECK_SIG].pRetString);
            if(pHead==NULL)
            {
                 Real_Time_Location_Inf.Sing_strength=99;
                 #if at_process_debug_printf 
                 eat_trace("strstr_at_cmd_cb AT_CHECK_SIG -->error.");
                 #endif
            }
            else
        	{
        		pTail=(char *)strstrLast(pHead,",");
        	    decbit=(unsigned char)(pTail-pHead);
        		if((decbit-1)==1)//注意这个长度问题！
        		{
        		    tempL=*(unsigned char *)pHead;
        	    	Real_Time_Location_Inf.Sing_strength=tempL-0x30;//ASCL转十进制
        		}
        		else
        		{
        			tempH=*(unsigned *)pHead;
        			tempL=*(unsigned *)(pTail-2);	        	    		
        	    	Real_Time_Location_Inf.Sing_strength=(tempH-0x30)*10+(tempL-0x30); 
        	    	if(Real_Time_Location_Inf.Sing_strength>32)Real_Time_Location_Inf.Sing_strength=32;
        		}
                exit_at_cmd_send_queue(&at_cmd);
                at_cmd.execution_statue = AT_CMD_idle;
                AtCmdTimeOut_cb(EAT_FALSE,0);//解除超时
        		#if at_process_debug_printf 
                eat_trace("strstr_at_cmd_cb AT_CHECK_SIG -->Sing_strength:%d.",Real_Time_Location_Inf.Sing_strength);
                #endif
        	}
        }
        else if(at_cmd.AT_cmd_buf[at_cmd.head]== AT_SIM_INSET)//SIM卡插入检测
        {
            pHead=(char *)strstrLast(p_str,Send_AT_cmd[AT_SIM_INSET].pRetString);
            if(pHead==NULL) 
            {
                #if APP_TRACE_DBG 
                eat_trace("strstr_at_cmd_cb AT_SIM_INSET -->error.");
                #endif
            }
            else
            {
                 pTail=(char *)strstrLast(pHead,",");
                 tempL=*(unsigned char *)pTail;      
                 Wether_SIM_DEF=tempL-0x30;
                 #if at_process_debug_printf 
                 eat_trace("strstr_at_cmd_cb AT_SIM_INSET -->state:%u.",Wether_SIM_DEF);
                 #endif
                 exit_at_cmd_send_queue(&at_cmd);
                 at_cmd.execution_statue = AT_CMD_idle;
                 AtCmdTimeOut_cb(EAT_FALSE,0);//解除超时
            }
        }
        else
        {
            #if 0//APP_TRACE_DBG 
            eat_trace("strstr_at_cmd_cb other -->strin:%s,%s",p_str,Send_AT_cmd[at_cmd.AT_cmd_buf[at_cmd.head]].pRetString);
            #endif
            pHead=(char *)strstrLast(p_str,Send_AT_cmd[at_cmd.AT_cmd_buf[at_cmd.head]].pRetString);
            if(pHead!=NULL)
            {
        		#if at_process_debug_printf 
                eat_trace("strstr_at_cmd_cb  -->sucess.");
                #endif
                exit_at_cmd_send_queue(&at_cmd);
                at_cmd.execution_statue = AT_CMD_idle;
                AtCmdTimeOut_cb(EAT_FALSE,0);//解除超时
            }
        }
	}
	else
	{
	    #if at_process_debug_printf 
        eat_trace("strstr_at_cmd_cb at_cmd.execution_statue -->idle.");
        #endif  
	}
  }
 /* 
  * ===  FUNCTION  ======================================================================
  *         Name: mdm_rx_proc_cb
  *  Description: mdm_rx_proc_cb
  *        Input:            
  *       author: Jumping create at 2013-12-16
  * =====================================================================================
  */
void mdm_rx_proc_cb(uint8_t *p_str)//at 处理返回函数
{
    /*系统开始与否判断*/
    wether_system_start(p_str);
    
    /*SIM卡是否在*/
    wether_sim_is_on(p_str);
    
    /*AT指令*/
    strstr_at_cmd_cb(p_str); 
    
    /*录音指令指令*/
    strstr_voice_record_cmd_cb_proc(p_str);
    
    /*电话处理*/
    call_ring_data_num_proc(p_str);
    
    /*电话结束处理*/
    phone_over_proc(p_str);
} 
/* 
* ===  FUNCTION  ======================================================================
*         Name: at_alarm_tone_proc
*  Description: at_alarm_tone_proc
*        Input: 语音播报处理         
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void at_alarm_tone_proc(uint8_t *alarm_type)
{
    switch(*alarm_type)
	{
	    case 0x01:/*开机提示*/
             enter_at_cmd_send_queue(&at_cmd,at_not_in_iccard);
    		 break;
    		
		case 0x02:/*司机卡插入*/
			if(system_param.location_reporting_param.scheme!=0x00)
			{
                befor_mileage_base=system_param.mileage;
			}
			driver_ic_card_data_report(alarm_type+7,0x01);
            //enter_at_cmd_send_queue(&at_cmd,AT_TTS_IC_IN);
            System_Flag.System_Flag_Bit.ic_inout = EAT_TRUE;
			break;
			
		case 0x03:/*司机卡拔出*/
			driver_ic_card_data_report(alarm_type+7,0x02);
			
            //enter_at_cmd_send_queue(&at_cmd,AT_TTS_IC_OUT);
            
            System_Flag.System_Flag_Bit.ic_inout = EAT_FALSE;
			acc_off_mileage_base = system_param.mileage;
			break;
			
		case 0x04:/*超速*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_OUT);
			break;

		case 0x05:/*疲劳驾驶*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_PL_DRIVE);
			break;

		case 0x06:
			break;

		case 0x07:/*即将疲劳驾驶*/
             enter_at_cmd_send_queue(&at_cmd,AT_TTS_PLING);
			 break;

		case 0x08:/*即将超速*/
             enter_at_cmd_send_queue(&at_cmd,AT_TTS_SPING);
			 break;

		case 0x09:/*速度异常*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
			break;

		case 0x0A:/*停车换卡*/
            enter_at_cmd_send_queue(&at_cmd,AT_CHANG_IC_CARD);
			break;
			
        case 0x0B:/*禁止打印*/
            enter_at_cmd_send_queue(&at_cmd,at_not_printf);
            break;
        case 0x0c:/*主电源掉电*/
    	    #if 1//at_process_debug_printf 
            eat_trace("at_alarm_tone_proc -->>主电源掉电");
            #endif  
            enter_at_cmd_send_queue(&at_cmd,at_power_down);
            enter_at_cmd_send_queue(&at_cmd,at_power_down);
            enter_at_cmd_send_queue(&at_cmd,at_power_down);
            break;
		default:
			break;
					
    }
}
/* 
* ===  FUNCTION  ======================================================================
*         Name: antena_indef_alarm_tone
*  Description: antena_indef_alarm_tone
*        Input:   天线断开 语音播报处理         
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void antena_indef_alarm_tone(void)
{
    enter_at_cmd_send_queue(&at_cmd,AT_ANTENA_ERROR);
}

