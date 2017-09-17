
/****************************************************
 * Include Files
 ****************************************************/ 
 
 #include "can_receive_data_proc.h"


/****************************************************
* External variables declaration
*****************************************************/
 struct list_head receive_can_data_list;;//接收can 数据

/*****************************************************
* Local variables declaration(Static)
******************************************************/
/* 
* ===  FUNCTION  ======================================================================
*  Name: can_orign_data_update
*  Description:  CAN数据上传
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
void can_orign_data_update(uint8_t *buffer,uint8_t data_cnt)
{
    uint16_t sernvn=0;
    uint8_t socket;
    static uint8_t data_buffer[1543]={0};
    static uint16_t update_time=0,orign_can_cnt=0;
    if(system_param.can_collect_param.can1_upload_interval != 0)
    {
        update_time++;
        if(orign_can_cnt<128)
        {
            memcpy(data_buffer+7+orign_can_cnt*12,buffer,data_cnt*12);
            orign_can_cnt += data_cnt;
            if(update_time==system_param.can_collect_param.can1_upload_interval)
            {
                update_time=0;
                Big_End_WordtoByte(orign_can_cnt,data_buffer);
                memcpy(data_buffer+2,Real_Time_Location_Inf.Curren_Time+3,3);
                if(orign_can_cnt!=0)
                {
                    if(system_param.can_collect_param.update_socket==0x01)
                    {
                        socket=Main_Link.mode_Socket;
                    }
                    else if(system_param.can_collect_param.update_socket==0x02)
                    {
                        socket=Second_Link.mode_Socket;
                    }
                    if((orign_can_cnt<=70)&&(orign_can_cnt!=0))
                    {
                        Tcp_Message_PacKage_And_Send(socket,&sernvn,0x0705,data_buffer,12*orign_can_cnt+7,EAT_FALSE,0,0);
                    }
                    else
                    {
                        Big_End_WordtoByte(orign_can_cnt,data_buffer);
                        memcpy(data_buffer+2,Real_Time_Location_Inf.Curren_Time+3,3);
                        Tcp_Message_PacKage_And_Send(socket,&sernvn,0x0705,data_buffer,(12*70+7),EAT_TRUE,2,1);
                        Tcp_Message_PacKage_And_Send(socket,&sernvn,0x0705,data_buffer+(12*70+7),(orign_can_cnt-70)*12,EAT_TRUE,2,2);
                    }
                    orign_can_cnt=0;
                }
            }
        }
        else
        {
            #if can_reve_data_pro_debug_printf
            eat_trace("can_orign_data_update --> 数据个数大于:128!");
            #endif
            orign_can_cnt=0;
            Big_End_WordtoByte(orign_can_cnt,data_buffer);
            memcpy(data_buffer+2,Real_Time_Location_Inf.Curren_Time+3,3);
            if(system_param.can_collect_param.update_socket==0x01)
            {
                socket=Main_Link.mode_Socket;
            }
            else if(system_param.can_collect_param.update_socket==0x02)
            {
                socket=Second_Link.mode_Socket;
            }
            Tcp_Message_PacKage_And_Send(socket,&sernvn,0x0705,data_buffer,(12*70 + 7),EAT_TRUE,2,1);//分包数目
            Tcp_Message_PacKage_And_Send(socket,&sernvn,0x0705,data_buffer+(12*70 + 7),(orign_can_cnt-70)*12,EAT_TRUE,2,2);//分包数目
        }
    }
    else
    {
        #if can_reve_data_pro_debug_printf
        eat_trace("can_orign_data_update --> 数据上传间隔:0，不上传 !");
        #endif
    }
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: analyze_recive_can_data
*  Description:  处理接收到的CAN数据
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
static void analyze_recive_can_data(uint8_t *pscr)//解析上报
{
    uint16_t can_data_cnt=0;
    memcpy(can_data_send.curren_time,pscr,6);
    Big_End_BytetoWord(pscr+6,&can_data_cnt); 
    #if 0//APP_TRACE_DBG 
    eat_trace("analyze_recive_can_data --> can_data_send:%u.",can_data_cnt);
    HexToAsicc(pscr,20);
    #endif
    if((can_data_cnt!=0)&&(DbcDataSave.save_message_num!=0))
    {
        analyze_can_data(can_data_cnt,pscr + 8);
        #if 0//APP_TRACE_DBG 
        eat_trace("analyze_recive_can_data --> can_data_send:%u.",can_data_send.param_num);
        #endif
        can_send_pro_list_add(&analysised_can_list);//进入发送处理链表
    }
    else
    {
        if(Sim_State.state_byte_struct.can_data_send_bit==EAT_TRUE)
        {
            Sim_State.state_byte_struct.can_data_send_bit=EAT_FALSE;
        }
    }
}

/***接收CAN数据链表处理***/
void receive_can_pro_list_init(void)
{
    INIT_LIST_HEAD(&receive_can_data_list);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: void receive_can_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
*  Description:  增加CAN接收数据链表
*  Parameters :  void     
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
void receive_can_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
{
   receive_can_data_struct *mem_prt;
   mem_prt =(receive_can_data_struct *)eat_mem_alloc(sizeof(receive_can_data_struct));
   memset(mem_prt,0,sizeof(receive_can_data_struct));
   
   mem_prt->data_len = data_cnt;
   memcpy(mem_prt->data_buf,buff,data_cnt);
   if(is_list_enful(list_parm,receive_can_list_max) ==EAT_TRUE)
   {
       list_del(list_parm);
   }
   tail_list_add(&(mem_prt->re_list),list_parm,receive_can_list_max);
}
void receive_can_pro_list_del(struct list_head *list_parm)
{
    receive_can_data_struct *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (receive_can_data_struct *)list_entry(list_parm->next,receive_can_data_struct,re_list);
		analyze_recive_can_data(mem_prt->data_buf);
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if 0//list_debug 
        eat_trace("receive_pro_list_del--> list is empty.");
        #endif
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: can_data_analysis_proc
 *  Description:  CAN数据解析 处理链表中的数据
 *  Parameters :  void     
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void can_data_analysis_proc(void)
{
   receive_can_pro_list_del(&receive_can_data_list);
}
