 
#include "lock_car_strategy.h"

/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_rev_proc(uint8_t *pscr,uint16_t data_len)
*  Description:  接收跛行相关策略指令处理
*  Parameters :  socket_t: 内部端口号码
*                data_len:数据长度
*                p ,p1前两个字节为发送的808数据字节
*                P+2:后为发送数据
*              
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_rev_proc(uint8_t *pscr,uint16_t data_len)
{
    uint8_t send_dat[32]={0};
    uint8_t data_offfest=0;
    uint16_t socket_id=0;
    uint16_t msgs_sern,msg_id;
    /*808 ID转换*/
    Big_End_BytetoWord(pscr,&msg_id);
    if(System_Flag.System_Flag_Bit.MainAutenSucess == EAT_TRUE)
    {
        #if lock_car_debug_pirntf
        eat_trace("lock_car_strategy_rev_proc --> msg_id:%0x!!.",msg_id);
        HexToAsicc(pscr,data_len);
        #endif
        if(msg_id!=0x0001)
        {
            /*发送相关ID*/
            if(system_param.lock_car_save.lock_ip==0x01)
            {
                socket_id=Main_Link.mode_Socket;
            }
            else if(system_param.lock_car_save.lock_ip==0x02)
            {
                socket_id=Second_Link.mode_Socket;
            }
            Tcp_Message_PacKage_And_Send(socket_id,&msgs_sern,msg_id,pscr+2,data_len-2,0,0,0);
        }
    }
    else
    {
        if(msg_id != 0xF002)
        {
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_rev_proc --> 不在线保存数据!");
            #endif
            /*发送相关ID*/
            system_param.lock_car_save.re_save_state = EAT_TRUE;
            system_param.lock_car_save.re_lock_data_len = data_len;
            memcpy(system_param.lock_car_save.re_lock_data,pscr,data_len);
            system_para_save_at_once();
        }
    }
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_send(void)
*  Description:  发送缓存跛行数据
*  Parameters :  lock_car_id: 跛行ID
*                lock_data:数据体内容
*                data_len:数据体长度
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_send(void)
{
    uint8_t send_dat[32]={0};
    uint8_t data_offfest=0;
    uint16_t msgs_sern;
    /*控制指令 808 ID转换*/
    #if lock_car_debug_pirntf
    eat_trace("lock_car_strategy_send --> lock_car_id:%0x!!.",system_param.lock_car_save.lock_id);
    HexToAsicc(system_param.lock_car_save.lock_data,system_param.lock_car_save.lock_data_len);
    #endif
    Big_End_WordtoByte(system_param.lock_car_save.lock_id,send_dat);
    data_offfest += 2;

    switch(system_param.lock_car_save.lock_id)
    {
        /*开启认证*/
        case 0xF001: 
            if(*(system_param.lock_car_save.lock_data)==0x01)
            {
                memcpy(send_dat+data_offfest,system_param.lock_car_save.lock_data,system_param.lock_car_save.lock_data_len);
                data_offfest += system_param.lock_car_save.lock_data_len;
                
                memcpy(send_dat+data_offfest,system_param.terminl_registe.terminl_id,strlen(system_param.terminl_registe.terminl_id));
                data_offfest += strlen(system_param.terminl_registe.terminl_id);
            }
            else
            {
                memcpy(send_dat+data_offfest,system_param.lock_car_save.lock_data,system_param.lock_car_save.lock_data_len);
                data_offfest += system_param.lock_car_save.lock_data_len;
            }
            break;
            
        default:
            memcpy(send_dat+data_offfest,system_param.lock_car_save.lock_data,system_param.lock_car_save.lock_data_len);
            data_offfest += system_param.lock_car_save.lock_data_len;
            break;
    }
    uart1_send_data_frame(remote_lock_car_send,send_dat,data_offfest);
    #if lock_car_debug_pirntf
    eat_trace("lock_car_strategy_send --> send:%0x!!.",remote_lock_car_send);
    HexToAsicc(send_dat,data_offfest);
    #endif
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_send_proc(uint16_t lock_car_id,uint8_t *lock_data,uint16_t data_len)
*  Description:  发送跛行相关策略指令处理
*  Parameters :  lock_car_id: 跛行ID
*                lock_data:数据体内容
*                data_len:数据体长度
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_send_proc(uint16_t lock_car_id,uint8_t *lock_data,uint16_t data_len)
{
    if((Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)||(Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC ==EAT_FALSE))
    {
        if(lock_car_id != 0xF002)
        {
            system_param.lock_car_save.sleep_lock_id = lock_car_id;
            system_param.lock_car_save.sleep_lock_data_len = data_len;
            memcpy(system_param.lock_car_save.sleep_data_save,lock_data,data_len);
            system_param.lock_car_save.save_state = EAT_TRUE;
            system_para_save_at_once();
            
            
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_send_proc --> 休眠储存平台数据，lock_car_id:%0x!!.",lock_car_id);
            HexToAsicc(lock_data,data_len);
            #endif
        }
    }
    else
    {
        system_param.lock_car_save.lock_id = lock_car_id;
        system_param.lock_car_save.lock_data_len = data_len;
        memcpy(system_param.lock_car_save.lock_data,lock_data,data_len);
        lock_car_strategy_send();
        lock_car_cmd_timeout_cb(EAT_TRUE);
    }
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_sleep_send_proc()
*  Description:  跛行休眠数据处理
*  Parameters :  
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_sleep_send_proc(void)
{
    static uint8_t lock_car_sleep =0;
    {
        if(system_param.lock_car_save.save_state == EAT_TRUE)
        {
            lock_car_sleep++;
        }
        if(lock_car_sleep==15)
        {
            lock_car_sleep =0;
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_sleep_send_proc --> send:休眠存储数据发送。");
            #endif
            lock_car_strategy_send_proc(system_param.lock_car_save.sleep_lock_id,system_param.lock_car_save.sleep_data_save,system_param.lock_car_save.sleep_lock_data_len);
            system_param.lock_car_save.save_state = EAT_FALSE;
            system_para_save_at_once();
         }
    }
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_wirless_off_send_proc()
*  Description:  不在线休眠数据处理
*  Parameters :  
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_wirless_off_send_proc(char_t socket_t)
{
    if(system_param.lock_car_save.re_save_state == EAT_TRUE)
    {
        #if lock_car_debug_pirntf
        eat_trace("lock_car_strategy_wirless_off_send_proc --> send:不在线未上传跛行数据。");
        #endif
        lock_car_strategy_rev_proc(system_param.lock_car_save.re_lock_data,system_param.lock_car_save.re_lock_data_len);
        system_param.lock_car_save.re_save_state = EAT_FALSE;
        system_para_save_at_once();
    }
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: lock_car_strategy_sleep_send_proc()
*  Description:  跛行休眠数据处理
*  Parameters :  
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2017-2-7           
* =====================================================================================
*/ 
void lock_car_strategy_save_board_data(uint8_t *rve_data)
{
    uint8_t cmd_data=0;
    uint8_t xor_data=0;
    uint8_t send_data[65]={0};
    cmd_data = *rve_data;
    if(cmd_data==0x01)
    {
        #if lock_car_debug_pirntf
        eat_trace("lock_car_strategy_save_board_data --> 写跛行参数cmd_data:%d!",cmd_data);
        #endif
        xor_data=CrcXOR(rve_data+1,64);
        memcpy(system_param.lock_car_save.lock_car_data_save,rve_data+1,64);
        system_param.lock_car_save.lock_car_data_save[65] = xor_data;
        system_para_save_at_once();
        
        file_read(File_SystemParaSave,0,&system_param,sizeof(system_param));
        xor_data=CrcXOR(system_param.lock_car_save.lock_car_data_save,64);
        if(system_param.lock_car_save.lock_car_data_save[65]!=CrcXOR(system_param.lock_car_save.lock_car_data_save,64))
        {
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_save_board_data --> send:数据保存校验error!");
            #endif
            memcpy(system_param.lock_car_save.lock_car_data_save,rve_data+1,64);
            system_param.lock_car_save.lock_car_data_save[65] = xor_data;
            system_para_save_at_once();
        }
        else
        {
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_save_board_data --> send:数据保存校验ok!");
            #endif
        }
        send_data[0]=0x41;
        uart1_send_data_frame(lock_car_data_save_board_rev,send_data,1);
    }
    else if(cmd_data==0x02)
    {
        #if lock_car_debug_pirntf
        eat_trace("lock_car_strategy_save_board_data --> 读跛行参数cmd_data:%d!",cmd_data);
        #endif
        file_read(File_SystemParaSave,0,&system_param,sizeof(system_param));
        xor_data=CrcXOR(system_param.lock_car_save.lock_car_data_save,64);
        if(system_param.lock_car_save.lock_car_data_save[65]!=CrcXOR(system_param.lock_car_save.lock_car_data_save,64))
        {
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_save_board_data --> 读数据保存校验error!");
            #endif
            file_read(File_SystemParaSave,0,&system_param,sizeof(system_param));
        }
        else
        {
            #if lock_car_debug_pirntf
            eat_trace("lock_car_strategy_save_board_data --> 读数据保存校验ok!");
            #endif
        }
        send_data[0]=0x42;
        memcpy(send_data+1,system_param.lock_car_save.lock_car_data_save,64);
        uart1_send_data_frame(lock_car_data_save_board_rev,send_data,65);
    }
    else
    {
        #if lock_car_debug_pirntf
        eat_trace("lock_car_strategy_save_board_data --> 非法数据!!!!");
        #endif
    }
}
