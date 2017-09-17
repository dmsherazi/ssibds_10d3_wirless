/****************************************************
 * Include Files
****************************************************/ 
 
#include "at_cmd.h"

 /****************************************************
* External variables declaration
*****************************************************/
SEND_AT_CMD_Queue at_cmd;
char_t Wether_SIM_DEF=EAT_FALSE;//SIM���Ƿ���λ

/*****************************************************
* Local variables declaration(Static)
******************************************************/

const AT_CMD_SEND Send_AT_cmd[]=
{
    {AT_ECOH_INIT,     "ATE0\r\n",            "OK"    },/*�رջ���,�����ջ�������OK*/
    {AT_CSDT,       "AT+CSDT=1\r\n",          "OK"    },/*��SIM�����OK*/
    {AT_ECOH_CIPMUX,   "AT+CIPMUX=1\r\n",     "OK"    },/*����IP����ģʽOK*/
    {AT_CHECK_SIG,     "AT+CSQ\r\n",          "+CSQ: "},/*����ź�ǿ��*/
    {AT_CLOSE_VOLTAGE_CHECK,"AT+CBATCHK=0\r\n",  "OK" },/*�رյ�ѹ���*/
    {AT_DELET_ALL_SMS,"AT+CMGD=1,4\r\n",  "OK" },/*ɾ������*/
    {AT_SIM_INSET,"AT+CSMINS?\r\n",        "+CSMINS: "},/*SIM��״̬*/\
    {AT_SET_TTS_PARAM,"AT+CTTSPARAM=80,0,50,52,0\r\n","OK"},/*������������*/
    {AT_SET_TTS_VOL,"AT+CLVL=80\r\n","OK"},/*������������*/
    {at_call_not_ring,"AT+CALM=1\r\n","OK"},/*������*/
    {at_call_num,"AT+CLIP=1\r\n","OK"},/*������ʾ*/
    {AT_PARAM_SAVE,"AT&W_SAVE","OK"},/*��������*/
    {AT_TTS_IC_IN,"AT+CTTS=2,\"���ѵ�½\"\r\n","CTTS: 0"},/*IC������*/
    {AT_TTS_IC_OUT,"AT+CTTS=2,\"˾�����Ѱγ�\"\r\n","CTTS: 0"},/*IC���γ�*/
    {AT_TTS_SP_OUT,"AT+CTTS=2,\"���ѳ��٣��������ʻ\"\r\n","CTTS: 0"},/*������ʾ*/
    {AT_TTS_PL_DRIVE,"AT+CTTS=2,\"����ƣ�ͼ�ʻ����ͣ����Ϣ\"\r\n","CTTS: 0"},/*ƣ�ͼ�ʻ��ʾ*/  
    {AT_TTS_SPING,"AT+CTTS=2,\"���������٣��������ʻ\"\r\n","CTTS: 0"},/*����������ʾ*/
    {AT_TTS_PLING,"AT+CTTS=2,\"����ƣ�ͼ�ʻ����ͣ����Ϣ\"\r\n","CTTS: 0"},/*����ƣ����ʾ*/
    {AT_TTS_SP_ERROR,"AT+CTTS=2,\"�����쳣\"\r\n","CTTS: 0"},/*�ٶ��쳣*/
    {AT_CHANG_IC_CARD,"AT+CTTS=2,\"��ͣ������!\"\r\n","CTTS: 0"},/*ͣ������*/
    {at_not_printf,"AT+CTTS=2,\"��ʻ�У������ӡ!\"\r\n","CTTS: 0"},/*ͣ������*/
    {AT_ANTENA_ERROR,"AT+CTTS=2,\"��λ���߶Ͽ��������������\"\r\n","CTTS: 0"},/*���߶Ͽ�����*/
    {at_not_in_iccard,"AT+CTTS=2,\"��δ��¼����ͣ���忨\"\r\n","CTTS: 0"},/*δ��¼��忨*/
    {at_power_down,"AT+CTTS=2,\"����Դ����\"\r\n","CTTS: 0"}/*����Դ����*/
};

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: SEND_AT_CMD_Queue
  *  Description:  
  *  Parameters : *q���нṹ��ָ�� 
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
    unsigned char Tail = (q->tail+1)%(q->queuesize); //ȡ�ౣ֤����quil=queuesize-1ʱ����ת��
    if (Tail == q->head) //��ʱ����û�пռ�
    {
		#if at_process_debug_printf 
		eat_trace("enter_at_cmd_send_queue --> ������.");
		#endif
    }
    else
    {
        q->AT_cmd_buf[q->tail]=name;
        q->tail = Tail;
		#if at_process_debug_printf 
		eat_trace("enter_at_cmd_send_queue --> ������head:%d,tail:%d.",q->head,q->tail);
		#endif
    }
 }
void exit_at_cmd_send_queue(SEND_AT_CMD_Queue *q)
 {
     if(q->tail == q->head)     
     {
         #if at_process_debug_printf 
         eat_trace("exit_at_cmd_send_queue --> ����Ϊ��.");
         #endif
     }
     else
     {
         q->head = (q->head+1) % (q->queuesize);
     }
     #if at_process_debug_printf 
     eat_trace("exit_at_cmd_send_queue --> ������head:%d,tail:%d.",q->head,q->tail);
     #endif
 }
 static char_t is_at_cmd_send_queue_empty(SEND_AT_CMD_Queue *q)
 {
      if(q->head == q->tail)
      {
          #if at_process_debug_printf 
          eat_trace("is_at_cmd_send_queue_empty --> ����Ϊ��.");
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
	ret=(char *)strstrLast((char*)p_str,"+CPIN: READY");//SIM���Ƿ����
	if(ret!=NULL) 
	{
        #if at_process_debug_printf 
        eat_trace("wether_sim_is_on --> State:sim on.");
        #endif
        enter_at_cmd_send_queue(&at_cmd,AT_CSDT);
        Wether_SIM_DEF=EAT_TRUE;
	}
	ret=(char *)strstrLast((char*)p_str,"+CPIN: NOT READY");//SIM��δ����
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
	static char_t core_start = EAT_FALSE;//�ں��Ƿ�����
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
        task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//������Ϣ��USER_2
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
        		if((decbit-1)==1)//ע������������⣡
        		{
        		    tempL=*(unsigned char *)pHead;
        	    	Real_Time_Location_Inf.Sing_strength=tempL-0x30;//ASCLתʮ����
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
                AtCmdTimeOut_cb(EAT_FALSE,0);//�����ʱ
        		#if at_process_debug_printf 
                eat_trace("strstr_at_cmd_cb AT_CHECK_SIG -->Sing_strength:%d.",Real_Time_Location_Inf.Sing_strength);
                #endif
        	}
        }
        else if(at_cmd.AT_cmd_buf[at_cmd.head]== AT_SIM_INSET)//SIM��������
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
                 AtCmdTimeOut_cb(EAT_FALSE,0);//�����ʱ
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
                AtCmdTimeOut_cb(EAT_FALSE,0);//�����ʱ
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
void mdm_rx_proc_cb(uint8_t *p_str)//at �����غ���
{
    /*ϵͳ��ʼ����ж�*/
    wether_system_start(p_str);
    
    /*SIM���Ƿ���*/
    wether_sim_is_on(p_str);
    
    /*ATָ��*/
    strstr_at_cmd_cb(p_str); 
    
    /*¼��ָ��ָ��*/
    strstr_voice_record_cmd_cb_proc(p_str);
    
    /*�绰����*/
    call_ring_data_num_proc(p_str);
    
    /*�绰��������*/
    phone_over_proc(p_str);
} 
/* 
* ===  FUNCTION  ======================================================================
*         Name: at_alarm_tone_proc
*  Description: at_alarm_tone_proc
*        Input: ������������         
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void at_alarm_tone_proc(uint8_t *alarm_type)
{
    switch(*alarm_type)
	{
	    case 0x01:/*������ʾ*/
             enter_at_cmd_send_queue(&at_cmd,at_not_in_iccard);
    		 break;
    		
		case 0x02:/*˾��������*/
			if(system_param.location_reporting_param.scheme!=0x00)
			{
                befor_mileage_base=system_param.mileage;
			}
			driver_ic_card_data_report(alarm_type+7,0x01);
            //enter_at_cmd_send_queue(&at_cmd,AT_TTS_IC_IN);
            System_Flag.System_Flag_Bit.ic_inout = EAT_TRUE;
			break;
			
		case 0x03:/*˾�����γ�*/
			driver_ic_card_data_report(alarm_type+7,0x02);
			
            //enter_at_cmd_send_queue(&at_cmd,AT_TTS_IC_OUT);
            
            System_Flag.System_Flag_Bit.ic_inout = EAT_FALSE;
			acc_off_mileage_base = system_param.mileage;
			break;
			
		case 0x04:/*����*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_OUT);
			break;

		case 0x05:/*ƣ�ͼ�ʻ*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_PL_DRIVE);
			break;

		case 0x06:
			break;

		case 0x07:/*����ƣ�ͼ�ʻ*/
             enter_at_cmd_send_queue(&at_cmd,AT_TTS_PLING);
			 break;

		case 0x08:/*��������*/
             enter_at_cmd_send_queue(&at_cmd,AT_TTS_SPING);
			 break;

		case 0x09:/*�ٶ��쳣*/
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
            enter_at_cmd_send_queue(&at_cmd,AT_TTS_SP_ERROR);
			break;

		case 0x0A:/*ͣ������*/
            enter_at_cmd_send_queue(&at_cmd,AT_CHANG_IC_CARD);
			break;
			
        case 0x0B:/*��ֹ��ӡ*/
            enter_at_cmd_send_queue(&at_cmd,at_not_printf);
            break;
        case 0x0c:/*����Դ����*/
    	    #if 1//at_process_debug_printf 
            eat_trace("at_alarm_tone_proc -->>����Դ����");
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
*        Input:   ���߶Ͽ� ������������         
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void antena_indef_alarm_tone(void)
{
    enter_at_cmd_send_queue(&at_cmd,AT_ANTENA_ERROR);
}

