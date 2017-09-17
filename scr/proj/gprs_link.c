/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "gprs_Link.h"

 /****************************************************
* External variables declaration
*****************************************************/

PresentLinkParameter_Struct Main_Link={EAT_FALSE,MAIN_SERVER,EAT_FALSE,{0,0,0,0},-1};//服务器
PresentLinkParameter_Struct Second_Link={EAT_FALSE,SECOND_SERVER,EAT_FALSE,{0,0,0,0},-1};

/*****************************************************
* Local variables declaration(Static)
******************************************************/
static uint8_t ReceiveGprsBuff[TcpDataBufSize]={0};
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: gprs_bearer_cb_pro
 *  Description: 激活网络返回处理
 *        Input:
 *               state:: the current state about bearer, please refer to  cbm_bearer_state_enum
 *       Output:
 *       Return:
 *               typedef::
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
static void gprs_bearer_cb_pro(gprs_bear_cb state)
{
    switch(state)
    {
        case deactivated: //注销
        
            #if gprs_link_debug_printf 
            eat_trace("gprs_bearer_cb_pro -->State:注销。");
            #endif
            /*清除激活状态*/
            System_Flag.System_Flag_Bit.Mode_Active =EAT_FALSE;

            /*清除连接状态*/
            Main_Link.link_staue=EAT_FALSE;
            Second_Link.link_staue=EAT_FALSE;
            
            /*清除鉴权状态*/
            System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
            System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
            break;

        case activating ://正在激活
            #if gprs_link_debug_printf 
            eat_trace("gprs_bearer_cb_pro -->State:正在激活。");
            #endif
            break;

        case activated:
            #if gprs_link_debug_printf 
            eat_trace("gprs_bearer_cb_pro -->State:激活成功。");
            #endif
            
            /*置位激活状态解除*/
            System_Flag.System_Flag_Bit.Mode_Active_ing=EAT_FALSE;

            /*置位激活状态*/
            System_Flag.System_Flag_Bit.Mode_Active =EAT_TRUE;
            /*关闭激活超时*/
            gprs_bearers_time_out(EAT_FALSE);//激活成功
            /*开始连接*/
            task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
            if(system_param.param_save.statue.secondip_open_switch==EAT_TRUE)
            {
                task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
            }
            break;

        case deactivating:
            #if gprs_link_debug_printf 
            eat_trace("bear_notify_cb--> State:正在注销。");
            #endif
            break;
            
        case bear_other:
            #if gprs_link_debug_printf 
            eat_trace("bear_notify_cb -->State:未知。");
            #endif
            break;
        default:
        break;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void
 *  Description: call back function about eat_gprs_bearer_open
 *        Input:
 *               state:: the current state about bearer, please refer to  cbm_bearer_state_enum
 *               ip_addr[4]:: if state is CBM_ACTIVATED, this parameter is valid, it indicate the
 *                            local IP address.
 *       Output:
 *       Return:
 *               typedef::
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
eat_bear_notify bear_notify_cb(cbm_bearer_state_enum state,uint8_t ip_addr[4])
{
    gprs_bear_cb id;
    if (state & CBM_DEACTIVATED) id = deactivated;
    else if (state & CBM_ACTIVATING) id = activating;
    else if (state & CBM_ACTIVATED) 
    {
        id = activated;
        #if gprs_link_debug_printf 
        eat_trace("bear_notify_cb -->remote ip:%u.%u.%u.%u",\
                ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
        #endif
    }
    
    else if (state & CBM_DEACTIVATING) id = deactivating;
    else id = bear_other;
    gprs_bearer_cb_pro(id);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: Eat_open_gprs_bearer and Eat_close_gprs_bearer
 *  Description: open/close gprs bearer
 *        Input:
 *          
 *       Output: 1
 *       author: J create at 2014-4-24
 * =====================================================================================
 */
void gprs_bearer_open(u8 *apn,u8 *user, u8 *password)
{
    s8 ret = 0;
	/* open gprs bearer */
	ret = eat_gprs_bearer_open(apn,user,password,bear_notify_cb);
    if(CBM_OK != ret)
    {
		#if gprs_link_debug_printf
		eat_trace("gprs_bearer_open--> error:%d",ret);
		#endif
    }
    else
    {
		#if gprs_link_debug_printf
		eat_trace("gprs_bearer_open--> ok.");
		#endif
		gprs_bearer_cb_pro(activated);
    }
    ret = eat_gprs_bearer_hold() ;       /* hold bearer,after call this function, the bearer will be holding. 
                                             in default, when last actived socket be closed, the bearer will be released */                      
    if(CBM_OK != ret)
    {
		#if gprs_link_debug_printf
    	eat_trace("eat_gprs_bearer_hold()--> error:%d",ret);
    	#endif
    }	
    else
    {
		#if gprs_link_debug_printf
    	eat_trace("eat_gprs_bearer_hold()--> ok");
    	#endif
    }	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void
 *  Description: call back function about eat_soc_gethostbyname
 *        Input:
 *               request_id:: it set by  eat_soc_gethostbyname
 *               result:: the result about gethostname.
 *               ip_addr[4]:: if the result is TRUE, this parameter indicate the IP address
 *                            of hostname.
 *       Output:
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
eat_hostname_notify hostname_notify_cb(u32 request_id,eat_bool result,u8 ip_addr[4])
{  
	if(result==EAT_TRUE)
	{
        if( (MAIN_SERVER == request_id) || (BACKUP_MAIN_SEVER == request_id))//主IP
        {
            #if gprs_link_debug_printf
            eat_trace("hostname_notify_cb get MAIN_SERVER ip: %u.%u.%u.%u",\
                    ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
            #endif
            memcpy(&Main_Link.link_ip[0],ip_addr,4);
            Main_Link.get_ip_statue = EAT_TRUE;
            task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);

        }
        else if((SECOND_SERVER == request_id) || (BACKUP_SECOND_SERVER == request_id))
        {
            #if gprs_link_debug_printf
            eat_trace("hostname_notify_cb get SECOND_SERVER ip: %u.%u.%u.%u",\
                    ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
            #endif
            memcpy(&Second_Link.link_ip[0],ip_addr,4);
            Second_Link.get_ip_statue = EAT_TRUE;
            task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
        }
	}
	else
	{
		#if gprs_link_debug_printf
		eat_trace("hostname_notify_cb get ip error id: %d",result);
		#endif
	}
}

/*****************************************************************************
 * Function :GetIPFromDomainName
 * Description:  Get IP From Domain Name
 * Parameters :
 *  	resetCnf  : 
 * Returns:
 * 	GETIPRESULT_Enum	
* NOTE
*     Nothing
 *****************************************************************************/
GETIPRESULT_Enum GetIPFromDomainName(uint8_t *p_domainame,uint8_t *p_ip,LinkIndex_Enum index )
{
    char_t ret=0;
	int data[4]={0};
	uint8_t Address_IP[4]={0};
    uint8_t find_cnt=0;
    uint8_t addres_len=0;
	eat_bool Ret=EAT_FALSE;
    addres_len = strlen(p_domainame);
	for(find_cnt=0;find_cnt<strlen(p_domainame);find_cnt++)
    {
		if((*(p_domainame+find_cnt) > 0x39)&&(*(p_domainame+find_cnt)!='.')){
			Ret=EAT_TRUE;
			break;
		}
	}
	if(EAT_TRUE==Ret)
    {
		eat_soc_gethost_notify_register(hostname_notify_cb);
        ret = eat_soc_gethostbyname((const char *)p_domainame,p_ip,&addres_len,index);
        if (SOC_SUCCESS == ret)
    	{
            #if gprs_link_debug_printf
            eat_trace("GetIPFromDomain_SOC_SUCCESS-->IP:%u.%u.%u.%u",\
            p_ip[0],p_ip[1],p_ip[2],p_ip[3]);
            #endif
            return GETIPSUCESS; 
        } 
        else if(SOC_WOULDBLOCK == ret)
        {
          	#if gprs_link_debug_printf
    		eat_trace("GetIPFromDomain_SOC_SUCCESS-->获取IP等待。");	
    		#endif
            
            return GETIPWATING;
       	}
       	else 
    	{
    	  	#if gprs_link_debug_printf 
    	   	eat_trace("GetIPFromDomain_SOC_SUCCESS-->获取IP失败。");
    	   	#endif
            
            return GETIPFAIL;
    	}
	}
	else
    { 
		if((sscanf((const char *)p_domainame,"%d.%d.%d.%d",(int *)data,(int *)(data+1),(int *)(data+2),(int *)(data+3)))>0)
		{
			*p_ip = (u8)*data;
			*(p_ip+1) = (u8)*(data+1);
			*(p_ip+2) = (u8)*(data+2);
			*(p_ip+3) = (u8)*(data+3);
			
            #if gprs_link_debug_printf 
			eat_trace("GetIPFromDomain-->IP:%u.%u.%u.%u",
                *p_ip,*(p_ip+1),*(p_ip+2),*(p_ip+3));
    	   	#endif
            return GETIPSUCESS; 
		}
        else
        {
    	   	eat_trace("GetIPFromDomain_SOC_SUCCESS-->获取IP失败。");
            return GETIPFAIL;
        }
    }
}
 /* ===  FUNCTION  ======================================================================
 *         Name: void
 *  Description: call back function  about soc_create(无线通讯中最重要的函数)
 *        Input: 
 *               s:: the socket ID
 *               event:: the notify event about current socket, please refer to soc_event_enum
 *               result:: if event is SOC_CONNECT, this paramter indicate the result of connect
 *               ack_size:: if event is  SOC_ACKED, this parameter is valid, it indicate the acked 
 *                          size fo send data.   
 *       Output:
 *       Return:
 *       author: Jumping create at 2013-12-16
 * =====================================================================================**/

eat_soc_notify soc_notify_cb(s8 s,soc_event_enum event,eat_bool result, u16 ack_size)
{
    uint8_t id = 0;
    uint32_t receive_leghth;
    if(event&SOC_READ) id = 0;
    else if (event&SOC_WRITE)  id = 1;
    else if (event&SOC_ACCEPT) id = 2;
    else if (event&SOC_CONNECT)id = 3;
    else if (event&SOC_CLOSE)  id = 4;
    else if (event&SOC_ACKED)  id = 5;
	if(id==0)//接收到数据
	{
        #if gprs_link_debug_printf 
		eat_trace("socket recive data--> mode id:%d.",s);
    	#endif
    	receive_leghth=eat_soc_recv(s,ReceiveGprsBuff,sizeof(ReceiveGprsBuff));
    	if(s == Main_Link.mode_Socket)
    	{
            remote_message_rev_data_pack(ReceiveGprsBuff,(uint16_t)receive_leghth,&first_ip_data_pack);
            if(Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)//休眠解析数据
            {
                task_msg_send(EAT_USER_0,EAT_USER_1,FirstSeverSleepMsgProc);
            }

    	}
    	else if(s == Second_Link.mode_Socket)
    	{
            remote_message_rev_data_pack(ReceiveGprsBuff,(uint16_t)receive_leghth,&second_ip_data_pack);
            if(Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep == EAT_TRUE)//休眠解析数据
            {
                task_msg_send(EAT_USER_0,EAT_USER_2,SeconSeverdSleepMsgProc);
            }
    	}
	}
	else if (id ==3)//打开端口成功
	{
	    #if gprs_link_debug_printf 
		eat_trace("conect return -->id:%d,result:%d",id,result);
		#endif 
		if(result==EAT_TRUE)
		{
		    #if gprs_link_debug_printf 
		    eat_trace("socket connect sucess--> mode id:%d.",s);
    	    #endif
    	    if(s == Main_Link.mode_Socket)
    	    {
                Main_Link.link_staue =EAT_TRUE;//连接成功
                task_msg_send(EAT_USER_0,EAT_USER_1,Timer3Start);

                first_server_conect_time_out(EAT_FALSE);//关闭连接超时
                task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
    	    }
    	    else if(s == Second_Link.mode_Socket)
    	    {
                Second_Link.link_staue =EAT_TRUE;
                
                task_msg_send(EAT_USER_0,EAT_USER_2,Timer4Start);
                second_server_conect_time_out(EAT_FALSE);//关闭连接超时
                
                task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
    	    }
    	    
		}
	}
	else if (id ==4)//端口关闭；要返回数据
	{	
		#if gprs_link_debug_printf 
		eat_trace("socket closed by remote -->socket id:%d",s);
		#endif 
		if(s == Main_Link.mode_Socket)
        {
            Main_Link.link_staue=EAT_FALSE;
            System_Flag.System_Flag_Bit.MainAutenSucess = EAT_FALSE;//鉴权成功
            Main_Link.get_ip_statue=EAT_FALSE;
            first_platform_certification_time_out(EAT_FALSE);
            first_server_conect_time_out(EAT_FALSE);
            gprs_bearers_time_out(EAT_FALSE);
            if(Wether_SIM_DEF==EAT_TRUE)
            {
                task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);
            }
            else
            {
                #if gprs_link_debug_printf 
                eat_trace("socket closed by remote -->sim card outside!!");
                #endif 
            }
        }
    	else if(s == Second_Link.mode_Socket)
    	{
            Second_Link.link_staue=EAT_FALSE;
            Second_Link.get_ip_statue=EAT_FALSE;
            System_Flag.System_Flag_Bit.SecondAutenSucess = EAT_FALSE;
            second_platform_certification_time_out(EAT_FALSE);
            second_platform_certification_time_out(EAT_FALSE);
            gprs_bearers_time_out(EAT_FALSE);
            if(Wether_SIM_DEF==EAT_TRUE)
            {
                task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
            }
            else
            {
                #if gprs_link_debug_printf 
                eat_trace("socket closed by remote -->sim card outside!!");
                #endif 
            }
    	}
	}
    else if (id ==5)//数据发送成功
	{
	    if(s == Main_Link.mode_Socket)
        {
            if(System_Flag.System_Flag_Bit.fade_detail_gnss==EAT_TRUE)
            {
                System_Flag.System_Flag_Bit.fade_detail_gnss=EAT_FALSE;
            }
            #if gprs_link_debug_printf 
            eat_trace("socket data send sucess -->Main_Link!!");
            #endif 
        }
    	else if(s == Second_Link.mode_Socket)
    	{
            #if gprs_link_debug_printf 
            eat_trace("socket data send sucess -->second link !");
            #endif 
    	}
	}
    else 
    {
        #if gprs_link_debug_printf 
		eat_trace("conect return -->id:%d",id);
		#endif 
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: socket_connect
 *  Description: creat 
 *        Input:
 *          
 *       Output: 1
 *       author: J.X.B create at 2014-4-24
 * =====================================================================================
 */
eat_bool socket_connect(socket_type_enum conect_type,uint8_t *ip_conect,const uint32_t remote_socked_id,int16_t*local_socket_id)
{
	u32 VAL;
	u8 val = 0;
	s8 ret = 0;
	uint32_t temp_socket;
	sockaddr_struct address={0};
	uint8_t FLAG_SOC_ASYNC=EAT_FALSE;
	uint8_t FLAG_SOC_NBIO=EAT_FALSE;
	uint8_t FLAG_SOC_NODELAY=EAT_FALSE;
	uint8_t FLAG_SOC_RCVBUF=EAT_FALSE;
	uint8_t FLAG_SOC_SENDBUF=EAT_FALSE;
	static uint8_t rest_cnt=0;
    temp_socket = remote_socked_id;
	eat_soc_notify_register(soc_notify_cb);
	*local_socket_id = eat_soc_create(conect_type,0);
	if(*local_socket_id < 0)
	{
		#if gprs_link_debug_printf  
        eat_trace("eat_soc_create Fail-->id:%d",*local_socket_id);
		#endif	
	}
    else
    {
        #if gprs_link_debug_printf
	    eat_trace("eat_soc_create sucess-->id:%d",*local_socket_id);
        #endif
    }

    
	val = (SOC_READ | SOC_WRITE | SOC_CLOSE | SOC_CONNECT|SOC_ACCEPT);
	ret = eat_soc_setsockopt(*local_socket_id,SOC_ASYNC,&val,sizeof(val));
	if (ret != SOC_SUCCESS)
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_ASYNC -->fail id:%d.",ret);
		#endif
        FLAG_SOC_ASYNC=EAT_TRUE;
	}
    else
    { 
        #if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_ASYNC -->sucess.");
		#endif  
    }
    
	val = EAT_TRUE;
	ret = eat_soc_setsockopt(*local_socket_id,SOC_NBIO, &val, sizeof(val));
	if (ret != SOC_SUCCESS)
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_NBIO -->fail id:%d",ret);
		#endif
        FLAG_SOC_NBIO=EAT_TRUE;
	}
	else
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_NBIO -->sucess.");
		#endif
	}
    
	val = TRUE;
	ret = eat_soc_setsockopt(*local_socket_id, SOC_NODELAY, &val, sizeof(val));
	if (ret != SOC_SUCCESS)
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_NODELAY -->fail id:%d",ret);
		#endif
		FLAG_SOC_NODELAY=EAT_TRUE;
	}
	else
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_setsockopt SOC_NODELAY -->sucess.");
		#endif
	}
    ret = eat_soc_getsockopt(*local_socket_id, SOC_NODELAY, &VAL, sizeof(VAL));
	if (ret != SOC_SUCCESS){
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_Getsockopt SOC_NODELAY -->fail id:%d",ret);
		#endif
	}
	else 
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_Getsockopt SOC_NODELAY -->OK");
		#endif
	}
    ret=eat_soc_getsockopt(*local_socket_id, SOC_RCVBUF, &VAL, sizeof(VAL));
    if (ret != SOC_SUCCESS)
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_getsockopt SOC_RCVBUF -->fail id:%d",ret);
		#endif
        FLAG_SOC_RCVBUF=EAT_TRUE;
	}
	else
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_getsockopt SOC_RCVBUF -->sucess.");
		eat_trace(" SOC_RCVBUF -->size:%u.",VAL);
		#endif
	}
	ret=eat_soc_getsockopt(*local_socket_id, SOC_SENDBUF, &VAL, sizeof(VAL));
    if (ret != SOC_SUCCESS)
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_getsockopt SOC_SENDBUF -->fail id:%d",ret);
		#endif
		FLAG_SOC_SENDBUF=EAT_TRUE;
	}
	else
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_getsockopt SOC_SENDBUF -->sucess.");
		eat_trace(" SOC_SENDBUF -->size:%u.",VAL);
		#endif
	}
	address.sock_type = conect_type;
	address.addr_len = 4;
    #if gprs_link_debug_printf
    eat_trace("conect ip -->  socket:%u",remote_socked_id);
    #endif
	address.port = (u16)temp_socket;        // TCP server port 
	memcpy(address.addr,ip_conect,4);            //TCP server ip address
    #if gprs_link_debug_printf
    eat_trace("conect ip --> IP:%u.%u.%u.%u. socket:%u",address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.port);
    #endif
	ret = eat_soc_connect(*local_socket_id,&address); 
	if(ret >= 0)
	{
		#if gprs_link_debug_printf
		eat_trace("eat_soc_connect --> sucess.");
		#endif
		#if gprs_link_debug_printf 
		eat_trace("socket connect sucess--> mode id:%d.",*local_socket_id);
    	#endif
    	if(*local_socket_id == Main_Link.mode_Socket)
    	{
            Main_Link.link_staue =EAT_TRUE;//连接成功
            task_msg_send(EAT_USER_0,EAT_USER_1,Timer3Start);

            first_server_conect_time_out(EAT_FALSE);//关闭连接超时
            task_msg_send(EAT_USER_0,EAT_USER_1,FirstPlatformReg);
    	}
    	else if(*local_socket_id == Second_Link.mode_Socket)
    	{
            Second_Link.link_staue =EAT_TRUE;
                
            task_msg_send(EAT_USER_0,EAT_USER_2,Timer4Start);
            second_server_conect_time_out(EAT_FALSE);//关闭连接超时
                
            task_msg_send(EAT_USER_0,EAT_USER_2,SecondPlatformReg);
    	}
        return EAT_TRUE;
	}
	else if (SOC_WOULDBLOCK ==ret) 
	{
		#if gprs_link_debug_printf 
		eat_trace("eat_soc_connect --> connecting.");
		#endif
        return EAT_FALSE;
	}
	else 
	{
	  	#if gprs_link_debug_printf 
		eat_trace("eat_soc_connect -->fail id:%d.",ret);
		#endif
        if((FLAG_SOC_ASYNC==EAT_TRUE)&&(FLAG_SOC_NBIO==EAT_TRUE)&&(FLAG_SOC_NODELAY==EAT_TRUE)&&(FLAG_SOC_RCVBUF==EAT_TRUE)&&(FLAG_SOC_SENDBUF==EAT_TRUE))
        {
            rest_cnt++;
            if(rest_cnt==2)
            {
                rest_cnt=0;
                #if gprs_link_debug_printf 
                eat_trace("socket_connect system fail-->eat_reset_module.");
                #endif
                eat_reset_module();
            }
        }
        return EAT_FALSE;
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: close_gprs_bearer
 *  Description: logout gprs
 *        Input:
 *          
 *       Output: 1
 *       author: J create at 2014-4-24
 * =====================================================================================
 */
eat_bool close_gprs_bearer(void)
{
    s8 ret = 0;
	/* close gprs bearer */
	ret = eat_gprs_bearer_release();
	if(CBM_OK != ret)
	{
    	#if gprs_link_debug_printf 
		eat_trace("close_gprs_bearer --> sucess .");
		#endif
        return EAT_TRUE;
	}
	else
	{
        #if gprs_link_debug_printf 
		eat_trace("close_gprs_bearer --> error id:%d",ret);
		#endif
	    return EAT_FALSE;
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: Eat_socket_close
 *  Description: 关闭端口
 *        Input:
 *          
 *       Output: 1
 *       author: J create at 2014-4-24
 * =====================================================================================
 */
eat_bool socket_close(char_t local_socket)
{
	s8 ret = 0;
    ret = eat_soc_close(local_socket);
    if(ret != SOC_SUCCESS)
    {
    	#if gprs_link_debug_printf
   		eat_trace("socket_close --> error id:%d.",ret);
   		#endif
   		return EAT_FALSE;
    }
    else
    {
        #if gprs_link_debug_printf
   		eat_trace("socket_close --> sucess.");
   		#endif
        return EAT_TRUE;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: Eat_socket_close
 *  Description: 关闭端口
 *        Input:
 *          
 *       Output: 1
 *       author: J create at 2014-4-24
 * =====================================================================================
 */
void close_secondip_link(void)
{
    
    #if gprs_link_debug_printf
    eat_trace("close_secondip_link --> 关闭第二IP连接 !.");
    #endif
    if(System_Flag.System_Flag_Bit.SecondAutenSucess==EAT_TRUE)
    {
        socket_close(Second_Link.mode_Socket);
        eat_timer_stop(EAT_TIMER_6);
        System_Flag.System_Flag_Bit.SecondAutenSucess=EAT_FALSE;
    }
    System_Flag.System_Flag_Bit.Second_Hart_Beat_Send=EAT_FALSE;
    System_Flag.System_Flag_Bit.Second_Location_Send_State=EAT_FALSE;
    System_Flag.System_Flag_Bit.Second_Queue_Fade_Area=EAT_FALSE;
    System_Flag.System_Flag_Bit.Second_File_Fade_Area=EAT_FALSE;
}
void close_frist_iplink(void)
{
    #if gprs_link_debug_printf
    eat_trace("close_secondip_link --> 关闭第一IP连接 !.");
    #endif
    if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_TRUE)
    {
        socket_close(Main_Link.mode_Socket);
        eat_timer_stop(EAT_TIMER_5);
        System_Flag.System_Flag_Bit.MainAutenSucess=EAT_FALSE;
    }
    System_Flag.System_Flag_Bit.Hart_Beat_Send=EAT_FALSE;
    System_Flag.System_Flag_Bit.Location_Send_State=EAT_FALSE;
    System_Flag.System_Flag_Bit.Queue_Fade_Area=EAT_FALSE;
    System_Flag.System_Flag_Bit.File_Fade_Area=EAT_FALSE;
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  void Tcp_Reg_JU_All(void)
 *  Description: 注册信息是否完整判断
 *  Parameters : 
 *  Return     :  0 : 信息不全 1 ;信息完整 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
  
 char wehther_launch_tcp_conect(LinkIndex_Enum index)
 {
	uint8_t p_num=0;
	uint8_t term_id=0;
	uint8_t ip_stat=0;
	uint8_t car_inf_state= 0;
	
 	p_num = strlen(system_param.SaveTerminlPhone.terminl_phone_nums);
 	term_id = strlen(system_param.terminl_registe.terminl_id);//ID
 	if(term_id!=7) term_id=0;
 	if(system_param.terminl_registe.plate_coror!= 0)
 	{
 		car_inf_state =strlen(system_param.terminl_registe.plate_number);
 	}
 	else
 	{
 		car_inf_state =strlen(system_param.terminl_registe.vehicle_vin);
 	}
 	if((strlen(system_param.SaveServerParameterArray[index].server_domin))&&(system_param.SaveServerParameterArray[index].server_socket))//IP
 	{
 	    ip_stat=EAT_TRUE;
 	}
 	else
 	{
 	    ip_stat=EAT_FALSE;
 	}
 	if(car_inf_state&&p_num&&term_id&&ip_stat)
 	{
        #if gprs_link_debug_printf
   		eat_trace("wehther_launch_tcp_conect --> sucess.");
   		#endif
		return 1;
 	}
 	else
 	{
        #if gprs_link_debug_printf
   		eat_trace("wehther_launch_tcp_conect --> fail.");
   		#endif
 		return 0;
 	}
 }

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: Eat_socket_close
 *  Description: 链接
 *        Input:
 *       author: J create at 2014-4-24
 * =====================================================================================
 */
void link_first_server_proc(void)
{
   if(Main_Link.get_ip_statue == EAT_FALSE)
   {
      if(Main_Link.link_index == MAIN_SERVER)
      {
           if(wehther_launch_tcp_conect(MAIN_SERVER))
           {
                if(GetIPFromDomainName(
                    system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,\
                                        Main_Link.link_ip,MAIN_SERVER)==GETIPSUCESS)//获取IP
                {
                    Main_Link.get_ip_statue = EAT_TRUE;
                    socket_close(Main_Link.mode_Socket);
                    socket_connect(SOC_SOCK_STREAM,&Main_Link.link_ip[0],\
                    system_param.SaveServerParameterArray[Main_Link.link_index].server_socket,\
                                                             &Main_Link.mode_Socket);
                }
           }
           else
           {
                first_server_conect_time_out(EAT_FALSE);//连接超时
                Main_Link.get_ip_statue=EAT_FALSE;
                Main_Link.link_index = BACKUP_MAIN_SEVER;
                task_msg_send(EAT_USER_0,EAT_USER_1,LinkFisrtServer);//发送消息到USER_2
                
                #if gprs_link_debug_printf
           		eat_trace("link_First_server_proc --> min_link_连接信息缺失 切换备服 !.");
           		#endif
 
           }
      }
      else if(Main_Link.link_index == BACKUP_MAIN_SEVER)
      {
           if(wehther_launch_tcp_conect(BACKUP_MAIN_SEVER))
           {
                if(GetIPFromDomainName(
                    system_param.SaveServerParameterArray[BACKUP_MAIN_SEVER].server_domin,\
                                    Main_Link.link_ip,BACKUP_MAIN_SEVER)==GETIPSUCESS)
                {
                    Main_Link.get_ip_statue = EAT_TRUE;
                    socket_close(Main_Link.mode_Socket);
                    socket_connect(SOC_SOCK_STREAM,&Main_Link.link_ip[0],\
                    system_param.SaveServerParameterArray[Main_Link.link_index].server_socket,\
                                                             &Main_Link.mode_Socket);
                }
           }
           else
           {
               #if gprs_link_debug_printf
               eat_trace("link_First_server_proc --> main_link 连接信息缺失 !");
               #endif
               Main_Link.get_ip_statue=EAT_FALSE;
               Main_Link.link_index = MAIN_SERVER;
               first_server_conect_time_out(EAT_FALSE);//连接超时
               if((System_Flag.System_Flag_Bit.Mode_Active ==EAT_FALSE)&&(System_Flag.System_Flag_Bit.Mode_Active_ing==EAT_FALSE))
               {
                   task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
               }
           }
      }
   }
   else
   {
       #if gprs_link_debug_printf
       eat_trace("link_first_server_proc --> ip data is ok !.");
       #endif
       socket_close(Main_Link.mode_Socket);
       socket_connect(SOC_SOCK_STREAM,&Main_Link.link_ip[0],\
       system_param.SaveServerParameterArray[Main_Link.link_index].server_socket,\
                                                &Main_Link.mode_Socket);
   }
}
void link_second_server_proc(void)
{
   if(Second_Link.get_ip_statue == EAT_FALSE)
   {
      if(Second_Link.link_index == SECOND_SERVER)
      {
           if(wehther_launch_tcp_conect(SECOND_SERVER))
           {
                if(GetIPFromDomainName(
                    system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,\
                                        Second_Link.link_ip,SECOND_SERVER)==GETIPSUCESS)
                {
                    Second_Link.get_ip_statue = EAT_TRUE;
                    socket_close(Second_Link.mode_Socket);
                    socket_connect(SOC_SOCK_STREAM,&Second_Link.link_ip[0],\
                    system_param.SaveServerParameterArray[Second_Link.link_index].server_socket,\
                                                             &Second_Link.mode_Socket);
                }
           }
           else
           {
              
              second_server_conect_time_out(EAT_FALSE);//连接超时
              Second_Link.get_ip_statue=EAT_FALSE;
              Second_Link.link_index = BACKUP_SECOND_SERVER;
              task_msg_send(EAT_USER_0,EAT_USER_2,LinkSecondServer);
              
              #if gprs_link_debug_printf
              eat_trace("link_second_server_proc --> second_link_min 连接信息缺失 切换备服 !.");
              #endif
           }
      }
      else if(Second_Link.link_index == BACKUP_SECOND_SERVER)
      {
           if(wehther_launch_tcp_conect(BACKUP_SECOND_SERVER))
           {
                if(GetIPFromDomainName(
                    system_param.SaveServerParameterArray[BACKUP_SECOND_SERVER].server_domin,\
                                    Second_Link.link_ip,BACKUP_SECOND_SERVER)==GETIPSUCESS)
                {
                     Second_Link.get_ip_statue = EAT_TRUE;
                     socket_close(Second_Link.mode_Socket);
                     socket_connect(SOC_SOCK_STREAM,&Second_Link.link_ip[0],\
                     system_param.SaveServerParameterArray[Second_Link.link_index].server_socket,\
                                                        &Second_Link.mode_Socket);
                }
           }
           else
           {
                  Second_Link.get_ip_statue = EAT_FALSE;
                  Second_Link.link_index = SECOND_SERVER;
                  second_server_conect_time_out(EAT_FALSE);//连接超时
                  if((System_Flag.System_Flag_Bit.Mode_Active ==EAT_FALSE)&&(System_Flag.System_Flag_Bit.Mode_Active_ing==EAT_FALSE))
                  {
                      task_msg_send(EAT_USER_0,EAT_USER_0,StartBearerOpen);//发送消息到USER_2
                  }
                  #if gprs_link_debug_printf
                  eat_trace("link_second_server_proc --> second_link back_up link 连接信息缺失.");
                  #endif
           }
      }
   }
   else
   {
        
       #if gprs_link_debug_printf
       eat_trace("link_second_server_proc --> second ip data is ok !.");
       #endif
       socket_connect(SOC_SOCK_STREAM,&Second_Link.link_ip[0],\
       (int16_t)system_param.SaveServerParameterArray[Second_Link.link_index].server_socket,\
                                                &Second_Link.mode_Socket);
   }
}

