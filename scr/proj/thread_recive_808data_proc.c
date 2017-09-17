/****************************************************
 * Include Files
 ****************************************************/ 
 
 #include "thread_recive_808data_proc.h"



 /****************************************************
* External variables declaration
*****************************************************/


/*****************************************************
* Local variables declaration(Static)
******************************************************/
static int8_t RE_Temp_Dest[TcpDataBufSize]={0};
static int8_t RE_Temp_buf[TcpDataBufSize]={0};//bufer缓存
static int8_t An_Temp_data_buff[TcpDataBufSize]={0};

/* 
* ===  FUNCTION  ======================================================================
*  Name: remote_message_rev_data_pack
*  Description:  接收TCP一帧完整的数据 
*  Parameters :  *Rev_Buff        :目标BUFFER
*                *Source_Buff     :源数据包
*                Re_Lens          :源数据长度
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-15          
* =====================================================================================
*/
void remote_message_rev_data_pack(uint8_t *Source_Buff,uint16_t Re_Lens,struct list_head *data_pack)//user1
{
        uint8_t  cData=0;
        uint16_t Datat_cnt;
        uint16_t Effect_Data_Lens=0;//有效收长度
        static uint16_t TotalCnt=0;//静态变量总计数
        static uint16_t Rev_Data_Flag=0;
        static uint8_t get_spec_flag=0;
        Effect_Data_Lens = Re_Lens;//获取有效数据长度
        memset(RE_Temp_buf,0,sizeof(RE_Temp_buf));
        memcpy(RE_Temp_buf,Source_Buff,Effect_Data_Lens);//接收BUFFER不可靠!!!!!!
        if(Effect_Data_Lens)//字符串长度
        {
            for(Datat_cnt=0;Datat_cnt<Effect_Data_Lens;Datat_cnt++)//保证一次校验一位
            {
                cData =*(RE_Temp_buf+Datat_cnt);//读值
                switch(Rev_Data_Flag)
                {   
                    case 0:
                    {
                        if(cData==Tcp_Fram_X7E)//头帧1
                        {
                            Rev_Data_Flag=1;
                            continue;
                        }
                        else
                        {
                            Rev_Data_Flag=0;
                            TotalCnt=0;
                            continue;
                        }
                    }
                    break;
                    case 1:
                    if(cData==Tcp_Fram_X7E)
                    {   
                        receive_pro_list_add(data_pack,RE_Temp_Dest,TotalCnt);
                        Rev_Data_Flag=0;
                        TotalCnt=0;
                        continue;   
                    }
                    else
                    {
                        if(cData==Tcp_Fram_X7D&&(!get_spec_flag))//有特征字符
                        {
                            TotalCnt++;
                            get_spec_flag=1;
                            continue;
                        }
                        if(get_spec_flag)//特征字符尾字符判断
                        {
                            if(cData==Tcp_Fram_X7E_Tail)
                             {
                                *(RE_Temp_Dest+TotalCnt-1)=Tcp_Fram_X7E;
                                  get_spec_flag=0;      
                                  continue;
                             }
                             else if(cData==Tcp_Fram_X7D_Tail)
                             {
                                *(RE_Temp_Dest+TotalCnt-1)=Tcp_Fram_X7D;
                                 get_spec_flag=0;       
                                 continue;
                             }
                             else
                             {
                                Rev_Data_Flag=0;
                                get_spec_flag=0;
                                TotalCnt=0;
                                return ;
                             } 
                        }
                      *(RE_Temp_Dest+TotalCnt)=cData;
                        TotalCnt++; 
                    }
                    break;
                    default:
                    break;          
                }
            }
        }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Tcp_RevFrame
 *  Description:  解析完整的一帧Tcp下发数据 
 *  Parameters :  pSource          :原始数据数组 
 *				  MSG_ID     :消息ID
 *				  *pDest	 :目标储存BUFFER
 *				  pLens		   :信息流水号         
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
_RET_INFOR analyze_data_pack_frame (uint8_t *pSource,uint16_t *Data_ID,u16 *PackageNum,uint16_t *Package_Cnt,uint8_t *pDest,uint16_t *pLens,uint16_t *remote_msg_sn)
{
     uint16_t cnt=0;
     uint16_t tempLens=0;
     uint8_t ScrCheckSum=0,CheckSum=0;
     uint16_t tempPos=0;
	 Msg_Attribute Rev_Msg;
	 /**数据ID******************/
     if(Data_ID!=NULL) Big_End_BytetoWord(pSource + tempPos,Data_ID);
     tempPos+=2;
     
	 /**数据属性***************/
     Big_End_BytetoWord(pSource+tempPos,&(Rev_Msg.Msg_Abbri));
     tempPos+=2;
     
	 /**数据长度***************/
     tempLens = Rev_Msg.Bits.MessLens; //数据长度
     if(pLens!=NULL) *pLens=tempLens;//数据长度
     
	 /**手机号***************/
     tempPos+=6;
     
	 /**数据流水号***************/
	 if(remote_msg_sn!=NULL) Big_End_BytetoWord(pSource+tempPos,remote_msg_sn);
     tempPos += 2;
	 
     if(Rev_Msg.Bits.SubpackFlag == 0x01) //是否分包标识
     {
        /**分包数***************/
		 Big_End_BytetoWord(pSource+tempPos,PackageNum);//包个数
         tempPos += 2;
         /**包序号***************/
		 Big_End_BytetoWord(pSource+tempPos,Package_Cnt);//包序号
         tempPos += 2;
	 }
     if(pDest!=NULL)//给目标数组赋值
     {
         for(cnt=0;cnt<tempLens;cnt++)
         {
             *(pDest++)=*(pSource+tempPos);
             tempPos += 1;
         }
     }
	 /**校验和***************/
     ScrCheckSum=*(pSource+tempPos);//校验和
     
	 /**校验计算***************/
     CheckSum=CrcXOR(pSource,tempPos);//异或校验
     tempPos += 1;
     if(CheckSum!=ScrCheckSum)
     {
         return RET_CRC_ERR;     //打印校验错误信息
     }
     else
     {
         return RET_NORMAL;//正确返回
     }
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_TCP_Authen
 *  Description:  处理接收到的通用应答
 *  Parameters : Temp_data_buff ;接收到的有效数据指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
 
 void analyze_tcp_rev_comm(int16_t socket_l,int8_t *Temp_data_buff,uint16_t sn) //user3
 {
    uint16_t serven=0;
	uint8_t msg_result=0;
	uint16_t remote_id=0;
    Big_End_BytetoWord(Temp_data_buff+2,&remote_id);
    msg_result = *(Temp_data_buff+4);
	if(socket_l == Main_Link.mode_Socket)
	{
     	#if APP_TRACE_DBG
    	eat_trace("analyze_tcp_rev_comm --> Main_link:id:%0x,reslut:%u",remote_id,msg_result);
    	#endif
    	switch(remote_id)//通用应答的类型
    	{
    		case Tcp_authentication_ID://鉴权应答
        		{
        			switch(msg_result)
        			{
        				case 0x00:
        				{	
                            #if APP_TRACE_DBG
                            eat_trace("analyze_tcp_rev_comm --> Main_link: 鉴权 ok.");
                            #endif
                            if(system_param.param_save.statue.wether_system_update == EAT_TRUE)
                            {
                                updat_program_reslut_report(0x02,0x00);//升级成功
                                system_param.param_save.statue.wether_system_update=EAT_FALSE;
                                
                                system_para_save_at_once();
                                #if APP_TRACE_DBG 
                                eat_trace("is_update_app --> State:%u.",system_param.param_save.statue.wether_system_update);
                                #endif
                            }
                            System_Flag.System_Flag_Bit.MainAutenSucess = EAT_TRUE;//鉴权成功
                            
                            if(System_Flag.System_Flag_Bit.record_data==EAT_TRUE)
                            {
                                System_Flag.System_Flag_Bit.record_data=EAT_FALSE;
                            }
                            
                            /***休眠锁车指令***/
                            if(system_param.lock_car_save.lock_ip==0x01)
                            {
                                lock_car_strategy_wirless_off_send_proc(Main_Link.mode_Socket);
                            }
                            
                            first_platform_certification_time_out(EAT_FALSE);
                            gprs_bearers_time_out(EAT_FALSE);//激活成功
                            first_server_conect_time_out(EAT_FALSE);
                            
                            task_msg_send(EAT_USER_1,EAT_USER_1,Timer5Start);
        					
        				}
        				break;
        				case 0x01:
        				{
                            #if APP_TRACE_DBG
                            eat_trace("analyze_tcp_rev_comm --> Main_link:鉴权 fail.");
                            #endif
                            first_platform_certification_time_out(EAT_FALSE);

                            memset(system_param.terminl_registe.first_authentication_id,0,strlen(system_param.terminl_registe.first_authentication_id));
                            system_para_save_at_once();
                            
                            task_msg_send(EAT_USER_1,EAT_USER_1,FirstPlatformReg);
        				}
        				break;
        				case 0x05:
        				{
                            if(System_Flag.System_Flag_Bit.MainAutenSucess==EAT_FALSE)
                            {
                                #if APP_TRACE_DBG
                                eat_trace("analyze_tcp_rev_comm --> Main_link:鉴权--> sucess.");
                                #endif
                                if(system_param.param_save.statue.wether_system_update==EAT_TRUE)
                                {
                                    updat_program_reslut_report(0x02,0x00);//升级成功
                                    system_param.param_save.statue.wether_system_update=EAT_FALSE;
                                    system_para_save_at_once();
                                    #if APP_TRACE_DBG 
                                    eat_trace("is_update_app --> State:%u.",system_param.param_save.statue.wether_system_update);
                                    #endif
                                }
                                System_Flag.System_Flag_Bit.MainAutenSucess = EAT_TRUE;//鉴权成功
                                first_platform_certification_time_out(EAT_FALSE);
                                
                                gprs_bearers_time_out(EAT_FALSE);//激活成功
                                first_server_conect_time_out(EAT_FALSE);
                                
                                task_msg_send(EAT_USER_1,EAT_USER_1,Timer5Start);
                            }
                            #if APP_TRACE_DBG 
                            eat_trace("analyze_tcp_rev_comm --> Main_link:remote sleep.");
                            #endif
                        }
        				break;
        				case 0x06:
        				{
                            #if APP_TRACE_DBG 
                            eat_trace("analyze_tcp_rev_comm --> Main_link:remote wake up.");
                            #endif
                        }
        				break;
        				default:
        				break;
        			}
        		}
        		break;
    		case Tcp_Heart_Beat_ID://心跳通用应答
        		{
        			switch(msg_result)
        			{
        				case 0x00:
                        #if APP_TRACE_DBG
                        eat_trace("analyze_tcp_rev_comm --> Main_link: 心跳 ok.");
                        #endif
                        System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
        				break;
        				
        				case 0x01:
                        #if APP_TRACE_DBG
                        eat_trace("analyze_tcp_rev_comm --> Main_link: 心跳 fail.");
                        #endif
                        System_Flag.System_Flag_Bit.Hart_Beat_Send = EAT_FALSE;
        				break;
        				
        				default:
        				break;
        			}
        		}
        		break;
    		
    		case 0x0200:
    		switch(msg_result)
        		{
        			case 0x00:
                    #if APP_TRACE_DBG
                    eat_trace("analyze_tcp_rev_comm --> main_link: 位置信息成功  .");
                    #endif
        			if(System_Flag.System_Flag_Bit.Location_Send_State == Busy_State)
        			{
        				System_Flag.System_Flag_Bit.Location_Send_State = Idle_State;
        			}
        			break;
        			
        			case 0x01:
        			break;
        			
        			default:
        			break;
        		}
        		break;
    		
    		case 0x0704:
        		{
        			if(System_Flag.System_Flag_Bit.Queue_Fade_Area == EAT_TRUE)
        			{
        				Location_Queue_Init(&Location_Queue);//初始化缓存
        				System_Flag.System_Flag_Bit.Queue_Fade_Area = EAT_FALSE;
        			}
        			
        			if(System_Flag.System_Flag_Bit.File_Fade_Area == EAT_TRUE)
        			{
         				System_Flag.System_Flag_Bit.File_Fade_Area = EAT_FALSE;
        			    Clear_Location_data_save();
        			}
        			
        		}
        		break;
    		case tcp_can_data_send_id:
        	    {
            		Big_End_BytetoWord(Temp_data_buff,&serven);
            		if(System_Flag.System_Flag_Bit.can_data_send_state == Busy_State)
            		{
                        #if APP_TRACE_DBG 
                        eat_trace("tcp_can_data_send_id -->sn:%u,can_data_send_serial_num:%u.!!",serven,can_data_send_serial_num);
                        #endif
                        System_Flag.System_Flag_Bit.can_data_send_state = Idle_State;
                        can_send_pro_list_del(&analysised_can_list);
                        return;
            		}
        		}
        		break;
            case tcp_media_update:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_rev_comm --> tcp_media_update send msg!!");
                 #endif
                 media_update_timeout_ctrl(EAT_FALSE);
                 task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
                 break;
            
            case lock_car_heart_beat_id:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_rev_comm --> first lock_car_heart_beat_id!");
                 #endif
                 lock_car_strategy_send_proc(lock_car_heart_beat_id,Temp_data_buff+4,1);
                 break;
                 
    		default:
    		    break;
    	}
	}
	else if(socket_l==Second_Link.mode_Socket)
	{
        #if APP_TRACE_DBG
        eat_trace("analyze_tcp_rev_comm --> second_link:id:%0x,reslut:%u",remote_id,msg_result);
        #endif
        switch(remote_id)//通用应答的类型
         {
            case Tcp_authentication_ID://鉴权应答
            {
                switch(msg_result)
                {
                    case 0x00:
                    {   
                        #if APP_TRACE_DBG
                        eat_trace("analyze_tcp_rev_comm --> second_link: 鉴权 ok.");
                        #endif
 
                        System_Flag.System_Flag_Bit.SecondAutenSucess = EAT_TRUE;//鉴权成功
                        
                        /***休眠锁车指令***/
                        if( system_param.lock_car_save.lock_ip==0x02)
                        {
                            lock_car_strategy_wirless_off_send_proc(Second_Link.mode_Socket);
                        }
                        second_platform_certification_time_out(EAT_FALSE);
                        second_server_conect_time_out(EAT_FALSE);
                        task_msg_send(EAT_USER_2,EAT_USER_2,Timer6Start);
                                
                    }
                    break;
                    default:
                    {
                        #if APP_TRACE_DBG
                        eat_trace("analyze_tcp_rev_comm --> Second_link:鉴权 fail.");
                        #endif
                        second_platform_certification_time_out(EAT_FALSE);
                            
                        memset(system_param.terminl_registe.second_authentication_id,0,strlen(system_param.terminl_registe.second_authentication_id));
                        task_msg_send(EAT_USER_2,EAT_USER_2,SecondPlatformReg);
                    }
                    break;
                }
            }
            break;
            case Tcp_Heart_Beat_ID://心跳通用应答
            {
                 switch(msg_result)
                 {
                     case 0x00:
                     #if APP_TRACE_DBG
                     eat_trace("analyze_tcp_rev_comm --> Second_link: 心跳 ok.");
                     #endif
                     System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
                     break;
                            
                     case 0x01:
                     #if APP_TRACE_DBG
                     eat_trace("analyze_tcp_rev_comm --> Second_link: 心跳 fail.");
                     #endif
                     System_Flag.System_Flag_Bit.Second_Hart_Beat_Send = EAT_FALSE;
                     break;
                            
                     default:
                     break;
                 }
              }
              break;
                    
              case 0x0200:
              switch(msg_result)
              {
                   case 0x00:
                   #if APP_TRACE_DBG
                   eat_trace("analyze_tcp_rev_comm --> second_link: 位置信息成功  .");
                   #endif
                   if(System_Flag.System_Flag_Bit.Second_Location_Send_State == Busy_State)
                   {
                       System_Flag.System_Flag_Bit.Second_Location_Send_State = Idle_State;
                   }
                   break;
                        
                   case 0x01:
                   break;
                        
                   default:
                   break;
               }
               break;
                    
               case 0x0704:
               {
                   if(System_Flag.System_Flag_Bit.Second_Queue_Fade_Area == EAT_TRUE)
                   {
                       Location_Queue_Init(&Second_Location_Queue);//初始化缓存
                       System_Flag.System_Flag_Bit.Second_Queue_Fade_Area = EAT_FALSE;
                   }
                   if(System_Flag.System_Flag_Bit.Second_File_Fade_Area == EAT_TRUE)
                   {
                       System_Flag.System_Flag_Bit.Second_File_Fade_Area = EAT_FALSE;
                       second_Clear_Location_data_save();
                   }
               }
               break;
               case tcp_can_data_send_id:
                   {
                        Big_End_BytetoWord(Temp_data_buff,&serven);
                        if(System_Flag.System_Flag_Bit.second_can_data_send_state == Busy_State)
                        {
                              #if APP_TRACE_DBG 
                              eat_trace("tcp_can_data_send_id -->sn:%u,can_data_send_serial_num:%u.!!",serven,can_data_send_serial_num);
                              #endif
                              System_Flag.System_Flag_Bit.second_can_data_send_state = Idle_State;
                              can_send_pro_list_del(&analysised_can_list);
                              return;
                         }
                    }
                    break;
               case lock_car_heart_beat_id:
                    #if APP_TRACE_DBG 
                    eat_trace("analyze_tcp_rev_comm --> second lock_car_heart_beat_id!");
                    #endif
                    lock_car_strategy_send_proc(lock_car_heart_beat_id,Temp_data_buff+4,1);
                    break;
                default:
                    break;
       }
	}
 }
 
/* 
* ===  FUNCTION  ======================================================================
*  Name: man_aram_qveren_proc
*  Description: 人工报警确认
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7           
* =====================================================================================
*/
static void man_aram_qveren_proc(void)
{
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Sos = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Danger = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Region_In_Out = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Route_In_Out = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Route_Driving_OutorShort = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Ignited = EAT_FALSE;
    Real_Time_Location_Inf.Alarm_Inf.Alarm_Bit.Illegality_Vector = EAT_FALSE;
    System_Flag.System_Flag_Bit.Rout_Tim_Over = EAT_FALSE;
    System_Flag.System_Flag_Bit.Rout_Tim_Short = EAT_FALSE;
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_VDR_Common_Replay
 *  Description:  终端通用应答
 *  Parameters :  Sern_num 流水号
 *				  Data_ID 对应ID
 *                Result  应答结果
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void Tcp_VDR_Common_Replay(char_t socket,uint16_t Sern_num,uint16_t Data_ID,uint8_t Result)
{
	uint8_t Tempbuf[5]={0};
	uint16_t msgs_sern=0;
	Big_End_WordtoByte(Sern_num,Tempbuf);//消息流水
	Big_End_WordtoByte(Data_ID,Tempbuf+2);
	*(Tempbuf+4)=Result;
    Tcp_Message_PacKage_And_Send(socket,&msgs_sern,Tcp_VDR_Common_Replay_ID,Tempbuf,5,0,0,0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_TCP_Type_Setup
 *  Description:  分析接收的TCP数据并发做出应对
 *  Parameters : *Source_Buff  :原始数据指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */ 
void analyze_tcp_type_setup(int16_t socket,int8_t *pSource,uint16_t data_cnt)
{
	uint16_t tempType=0,tempLens=0;
	uint16_t Sn=0;
	uint16_t PackageNum=0;
	uint16_t Package_Cnt=0;
	_RET_INFOR ret;
	int8_t An_Temp_data_buff[1536]={0};
    #if APP_TRACE_DBG 
    eat_trace("analyze_tcp_type_setup--> analyze orgoin data hex  !!!");
    HexToAsicc(pSource,data_cnt);
    #endif
	ret = analyze_data_pack_frame(pSource,&tempType,&PackageNum,&Package_Cnt,An_Temp_data_buff,&tempLens,&Sn);//解析数据
	if(ret==RET_NORMAL)
	{
        #if APP_TRACE_DBG
        eat_trace("Analyze_TCP_Type_Setup --> tempType:%0x,tempLens:%d,sn:%d !",tempType,tempLens,Sn);
        #endif
		switch((uint16_t)(tempType))
		{
            case Tcp_Rev_Comm_ID://通用应答
            	analyze_tcp_rev_comm(socket,An_Temp_data_buff,Sn);
            	break;
            	
            case Tcp_Register_Rev_ID://注册
            	analyze_tcp_registe(socket,An_Temp_data_buff,tempLens,Sn);
            	break;

            case TCP_VDR_Seting_ID://参数设置
                tcp_seting_param_proc(socket,An_Temp_data_buff);//USER3
            	Tcp_VDR_Common_Replay(socket,Sn,TCP_VDR_Seting_ID,0x00);
            	break;
            /*查询全部设置参数*/
            case TCP_VDR_ChaXun_ALL:
            	tcp_all_data_request(socket,Sn);
            	break;
            /*查询指定参数*/
            case TCP_VDR_ChaXun_ID://查询指定ID设置参数
                tcp_request_param_serven(socket,An_Temp_data_buff,Sn);
            	break;
            /*远程升级*/
            case remoto_update_processor://远程升级
                remote_sim800_updata(PackageNum,Package_Cnt,An_Temp_data_buff,tempLens,Sn);
            	break;

            case dbc_data_Issued:
            	Tcp_VDR_Common_Replay(socket,Sn,dbc_data_Issued,0x00);
                dbc_recive_and_save(DBC_FILE,PackageNum,Package_Cnt,An_Temp_data_buff,tempLens);
            	break;

            case dbc_data_vision_ask://dbc版本查询
            	dbc_version_update();
            	break;
                
            case remote_sleep_setting://休眠设置
            	break;

            case Tcp_Location_Ask_ID://位置查询
                Tcp_Location_Infor_Ask(socket,Sn);
            	break;

            case tcp_location_tracking_id://位置跟踪参数设置
            	location_tracking_proc(Sn,An_Temp_data_buff);
                Tcp_VDR_Common_Replay(socket,Sn,tcp_location_tracking_id,0x00);
            	break;
            	
            /**人工报警确认***/
            case man_aram_qveren:
                 man_aram_qveren_proc();
                 Tcp_VDR_Common_Replay(socket,Sn,man_aram_qveren,0x00);
                 break;

            /**8.23 文本信息下发***/
            case txt_information_issued:
                remote_txt_data_proc(An_Temp_data_buff,tempLens);
                Tcp_VDR_Common_Replay(socket,Sn,txt_information_issued,0x00);
            	break;
            	
            /*IC卡信息查询*/
            case tcp_ic_ask_id:
                 tcp_driver_data_ask_proc(socket);
                 break;
                 
            /*电话回拨*/
            case tcp_dail_vdr_call_id:
                 tcp_vdr_dail_call(socket,Sn,An_Temp_data_buff);
                 break;
                 
            /*8.43 行驶记录数据采集命令*/
            case drive_record_iquire_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到行驶记录参数查询指令!.");
                 #endif
                 if(socket==Main_Link.get_ip_statue)
                 {
                    inquire_record_ip=0x01;
                 }
                 else if(socket==Second_Link.get_ip_statue)
                 {
                     inquire_record_ip=0x02;
                 }
                 enter_inquire_dring_record_queue(*An_Temp_data_buff,Sn,An_Temp_data_buff+1,tempLens - 1);
                 break;
                 
            /****************行驶记录参数设置****************/
            case drive_record_set_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到行驶记录参数设置指令!.");
                 #endif
                 uart1_send_data_frame(sim800_dring_record_inquire,An_Temp_data_buff+1,tempLens - 1);
                 Tcp_VDR_Common_Replay(socket,Sn,sim800_dring_record_inquire,0x00);
                 break;

                 
            /*设置圆形区域*/
            case  Tcp_QuYu_Yuan_Set_ID :
                  #if APP_TRACE_DBG 
                  eat_trace("analyze_tcp_type_setup --> 设置圆形区域 !!");
                  #endif
                  Tcp_QuYu_Yuan_Server(socket,An_Temp_data_buff,tempLens,Sn);
                  break;
             
            /* 删除圆形区域*/
            case Tcp_QuYu_Yuan_Delet_ID:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_type_setup --> 删除圆形区域 !!");
                 #endif
                 Tcp_QuYu_Delet_Yuan_Servn(socket,An_Temp_data_buff,Sn);
                 break;

             /*设置矩形区域*/
            case  Tcp_QuYu_JuXing_Set_ID :
                  #if APP_TRACE_DBG 
                  eat_trace("analyze_tcp_type_setup --> 设置矩形区域 !!");
                  #endif
                  Tcp_QuYu_Ju_Server(socket,An_Temp_data_buff,tempLens,Sn);
                  break;
             
            /* 删除矩形区域*/
            case Tcp_QuYu_JuXing_Delet_ID:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_type_setup --> 删除矩形区域 !!");
                 #endif
                 Tcp_QuYu_Delet_Ju_Servn(socket,An_Temp_data_buff,Sn);
                 break;
                 
            /*设置多边形区域*/
            case  Tcp_QuYu_Duobian_Set_ID :
                  #if APP_TRACE_DBG 
                  eat_trace("analyze_tcp_type_setup --> 设置多边形区域 !!");
                  #endif
                  Tcp_QuYu_Duo_Server(socket,An_Temp_data_buff,tempLens,Sn);
                  break;
             
            /*删除多边形区域*/
            case Tcp_QuYu_Duobian_Delet_ID:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_type_setup --> 删除多边形区域 !!");
                 #endif
                 Tcp_QuYu_Delet_Duo_Servn(socket,An_Temp_data_buff,Sn);
                 break;

            /*设置路线*/
            case  Tcp_LuXian_Set_ID :
                  #if APP_TRACE_DBG 
                  eat_trace("analyze_tcp_type_setup --> 设置路线 !!");
                  #endif
                  Tcp_LuXian_Server(socket,An_Temp_data_buff,tempLens,Sn);
                  break;
             
            /*删除路线*/
            case Tcp_LuXian_Delet_ID:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_type_setup --> 删除路线 !!");
                 #endif
                 Tcp_Delet_LuXian_Servn(socket,An_Temp_data_buff,Sn);
                 break;
                 
            case tcp_take_pictures_id_replay:
                 #if APP_TRACE_DBG 
                 eat_trace("analyze_tcp_type_setup --> tcp_media_update send msg!!");
                 #endif
                 media_update_timeout_ctrl(EAT_FALSE);
                 task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
                 break;
            /**摄像头立即拍摄命令***/
            case tcp_take_pictures_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到拍照指令!.");
                 #endif
                 tcp_take_pictures_server(socket,An_Temp_data_buff,Sn);
                 break;
            /****************录音****************/
            case tcp_voice_record:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到录音指令!.");
                 #endif
                 tcp_voice_record_proc(socket,An_Temp_data_buff,Sn);
                 break;
            /****************多媒体检索****************/
            case tcp_media_search_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到多媒体检索!.");
                 #endif
                 tcp_media_search(socket,An_Temp_data_buff,Sn);
                 break;
            /****************单条多媒体检索****************/
            case tcp_signel_media_search_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 收到单条多媒体检索!.");
                 #endif
                 tcp_single_media_search(An_Temp_data_buff);
                 break;
            /*****储存多媒体上传*******/
            case save_tcp_media_update:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 储存多媒体上传!.");
                 #endif
                 save_media_tcp_serch_update(An_Temp_data_buff);
                 break;
            /***终端属性***/
            case tcp_vdr_attributes_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 终端属性查询!.");
                 #endif
                 tcp_vdr_attributes_request(socket);
                 break;
                 
            /*车辆控制指令*/
            case tcp_car_contrl_id :
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 车辆控制 !.");
                 #endif
                 tcp_car_ctrl_proc(socket,Sn,An_Temp_data_buff);
                 break;
                 
            /*终端控制指令*/
            case tcp_vdr_ctrl_id:
                 #if APP_TRACE_DBG
                 eat_trace("Analyze_TCP_Type_Setup --> 车辆控制 !.");
                 #endif
                 remote_vdr_ctrl_proc(An_Temp_data_buff);
                 break;
                 
             /*锁车控制指令*/
            case lock_car_contrl_id:
                 Tcp_VDR_Common_Replay(socket,Sn,lock_car_contrl_id,0x00);
                 if(socket==Main_Link.mode_Socket)
                 {
                    system_param.lock_car_save.lock_ip=0x01;
                 }
                 else
                 {
                     system_param.lock_car_save.lock_ip=0x02;
                 }
                 system_para_save_at_once();
                 lock_car_strategy_send_proc(lock_car_contrl_id,An_Temp_data_buff,tempLens);
                 break;
                 
                 /*锁车查询指令*/
            case lock_car_request_id:
                 Tcp_VDR_Common_Replay(socket,Sn,lock_car_request_id,0x00);
                 lock_car_strategy_send_proc(lock_car_request_id,An_Temp_data_buff,tempLens);
                 break;
                 /*锁车标定指令*/    
            case lock_car_demarcate_id:
                 Tcp_VDR_Common_Replay(socket,Sn,lock_car_demarcate_id,0x00);
                 lock_car_strategy_send_proc(lock_car_demarcate_id,An_Temp_data_buff,tempLens);
                 break;
            default:
                break;
		}
	}
	else
	{
        #if APP_TRACE_DBG
        eat_trace("Analyze_TCP_Type_Setup --> Fail.");
        #endif
	}
}

