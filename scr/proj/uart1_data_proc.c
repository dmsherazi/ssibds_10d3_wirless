
#include "uart1_data_proc.h"

/****************************************************
* External variables declaration
*****************************************************/
/***解析，发送链表定义***/
struct list_head uart1_anaylsis_list;
struct list_head uart1_send_list;
/***SIM卡状态验证***/
SimState_union  Sim_State;//状态信息

/***UART1第一级缓存***/
static uint8_t uart1_receive_buffer[uar1_receive_max]={0};
/***UART1完整包数据缓存***/
static uint8_t realy_uart1_data_buffer[uar1_receive_max]={0};
/***UART1解析数据缓存***/
static uint8_t uart1_analysis_buffer[uar1_receive_max]={0};
/***UART1发送数据缓存***/
static uint8_t uart1_send_buffer[uar1_send_max]={0};

/*******************************接收串口1数据***********************************************/
void  uart1_receive_integrated_frame_proc(uint8_t *Source_Buff,uint16_t Re_Lens)//USER0
{
	uint8_t  cData=0;
	uint16_t Datat_cnt;
    uint16_t Effect_Data_Lens=0;
    static uint16_t TotalCnt=0;//静态变量总计数
    static uint8_t  rev_step_flag=0;//完整帧到了哪一步
    static uint16_t data_area_lens=0;//数据长度
	Effect_Data_Lens = Re_Lens;//获取有效数据长度
	memset(uart1_receive_buffer,0,sizeof(uart1_receive_buffer));
	memcpy(uart1_receive_buffer,Source_Buff,Effect_Data_Lens);//接收BUFFER不可靠!!!!!!
	if(Effect_Data_Lens)//字符串长度
	{
		for(Datat_cnt=0;Datat_cnt<Effect_Data_Lens;Datat_cnt++)//保证一次校验一位
		{
			cData =*(uart1_receive_buffer+Datat_cnt);//读值
			switch(rev_step_flag)
		    {	
				case 0:
				if((cData==Frame_Head_One))//头帧1
				{
					rev_step_flag=1;
					*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++;
				}
				else
				{
					rev_step_flag=0;
					TotalCnt=0;
				}
				break;
				case 1:
				if((cData==Frame_Head_two))//头帧2
				{
					rev_step_flag=2;
					*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++;
				}
				else
				{
					rev_step_flag=0;
					TotalCnt=0;
				}
				break;
				case 2:
				if(TotalCnt==4)//len area;
				{
				    data_area_lens+=cData*256;
					*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++;
					break;
				}
				if(TotalCnt==5)//len area;	
				{
					data_area_lens+=cData;
					*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++; 
					break;
				}
		        if(TotalCnt==(data_area_lens+7))//尾校验码
		       	{
		       		*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++; //确定长度
					
                    #if uart1_data_pro_debug_printf 
                    eat_trace("uart1_receive_integrated_frame_proc -->State:收到完整数据帧。");
					//HexToAsicc(realy_uart1_data_buffer,TotalCnt);
                    #endif
                    uart1_anaylsis_list_add(&uart1_anaylsis_list,realy_uart1_data_buffer,TotalCnt);
					TotalCnt=0;
					rev_step_flag=0;
					data_area_lens=0;
					continue;
		       	}
	        	*(realy_uart1_data_buffer+TotalCnt)=cData;
			    TotalCnt++; 
				break;
			    default:
			    break;
			}
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: AnalyzeRevFrame
 *  Description:   分析接到的数据帧
 *  Parameters :  *pSource:原始数据指针
 *				  *pType  :命令码的返回值
 *				  *pDest  :缓存数据buffer指针
 *                *pLens  :数据长度
 *  Return     :  _RET_INFOR 
 *  author     : B.Q create at 2012-12-6    
 *				 J.x.b modify and writ notes for SSI_1D02 at 2014-6-6
 * =====================================================================================
 */
static _RET_INFOR AnalyzeRevFrame (uint8_t *pSource,uint8_t *pType,uint8_t *pDest,uint16_t *pLens)
{
     static uint16_t cnt;
     static uint16_t tempLens=0;
     static uint16_t tempPos=0;
     static uint8_t ScrCheckSum=0,CheckSum=0;
     tempLens=(*(pSource+CRC_AREA_FIXED_LENS))*256+*(pSource+CRC_AREA_FIXED_LENS+1); //数据长度
     tempPos=tempLens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS+1;//目前位置
   	 ScrCheckSum=*(pSource+tempPos);//校验和
   	 CheckSum=CrcXOR(pSource,tempLens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS);//异或校验
   	 if(CheckSum!=ScrCheckSum)//校验码对比
   	 {
   		return RET_CRC_ERR;     //打印校验错误信息
   	 }
	 else
	 {
		 if(pDest!=NULL)//给目标数组赋值
	   	 {
		   	 for(cnt=0;cnt<tempLens;cnt++)
		   	 {
		   	 	*(pDest++)=*(pSource+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+cnt);
		   	 }
	   	 }
	   	 if(pLens!=NULL) *pLens=tempLens;//数据长度
	   	 if(pType!=NULL) *pType=*(pSource+FRAME_HEAD_LENS+1);   	 
	   	 return RET_NORMAL;//正确返回
	 }
}

/* 
 * ===  FUNCTION  ==================== ==================================================
 *  Name: Analyze_Type_Setup
 *  Description:   分析接到的数据并储存于完整帧缓存 Rev_Buff
 *  Parameters :  *Rev_Buff     :完整的数据帧
 *				  *Source_Buff  :原始数据指针
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-6 			 
 * =====================================================================================
 */ 
/*******************************接收串口1数据***********************************************/
static void analysis_uart1_data_proc(uint8_t *pSource)//USER4
{
	uint8_t tempType=0;
	uint16_t tempLens=0;
	_RET_INFOR ret;
	uint16_t Data_Lens=0;
	uint8_t msg_point=0;
	memset(uart1_analysis_buffer,0,sizeof(uart1_analysis_buffer));
	ret=AnalyzeRevFrame(pSource,&tempType,uart1_analysis_buffer,&tempLens);//解析数据
	if(ret!=RET_NORMAL)
	{
		#if 1//uart1_data_pro_debug_printf 
        eat_trace("analysis_uart1_data_proc -->Fail return:%0x!",ret);
        eat_trace("analysis_uart1_data_proc -->Fail ID:%0x,len:%u",tempType,tempLens);
        #endif
	}
	else
	{ 
		#if uart1_data_pro_debug_printf 
        eat_trace("Analyze_Type_Setup 解析数据 --> sucess ID:%0x,len:%u",tempType,tempLens);
        HexToAsicc(uart1_analysis_buffer,tempLens);
        #endif
		switch(tempType)
		{
		    /*语音播报*/
		    case voice_type_id:
		         at_alarm_tone_proc(uart1_analysis_buffer); 
		         break;
		         
		    case driver_data_record_ref:
                 #if uart1_data_pro_debug_printf 
                 eat_trace("analysis_uart1_data_proc --> driver_data_record_ref:0x72.");
                 #endif
                 tcp_driver_data_update(uart1_analysis_buffer,tempLens);
                 exit_inquire_dring_record_queue();
		         break;
            /*1S信息*/
			case board_send_to_sim800:// 1S信息
    			 sim800_board_commucation_proc(uart1_analysis_buffer);
    			 lock_car_strategy_sleep_send_proc();
    			 break;
    	    
			/***跛行控制接受***/
			case remote_lock_car_rev:
                lock_car_cmd_timeout_cb(EAT_FALSE);
    			lock_car_strategy_rev_proc(uart1_analysis_buffer,tempLens);
    			break;
    			
            /***跛行参数***/
            case lock_car_data_save_board_rev:
                lock_car_strategy_save_board_data(uart1_analysis_buffer);
                break;
    			 
           /***查询终端ID IP 手机号***/
            case request_id_ip_phonenum:
                 board_inquire_id_ip_phone_num();
                 break;
           /***TXT个数编号查询***/
    		case txt_cnt_inquire:
    		     txt_data_cnt_inquire(*uart1_analysis_buffer); 
     		     break;
            /***菜单设置***/
            case memu_setting_data_id:
                 HexToAsicc(uart1_analysis_buffer,tempLens);
                 memu_setting_data_pro(uart1_analysis_buffer);
                 break;
            /***查询txt文本内容***/
     		case txt_infor_inquire:
     		     txt_data_infor_inquire(*uart1_analysis_buffer,*(uart1_analysis_buffer+1));
     		     break;
     		/***SIM800软件版本查询***/
            case sim800_software_vion_inquire:
                 board_inquire_sim800_software_version();
                 break;
           /***SIM800软件版本查询***/
			case updata_sim800_process:
                 #if 1//uart1_data_pro_debug_printf 
                 eat_trace("analysis_uart1_data_proc --> receive sim800 update data.");
                 #endif
    			 sim800_upadat_sever(uart1_analysis_buffer);
    			 break;
    			 /*****信息标定*****/
            case demarcate_id:
                 board_demarcate_data_pro(uart1_analysis_buffer,tempLens);
                 #if uart1_data_pro_debug_printf 
                 eat_trace("analysis_uart1_data_proc --> receive demarcate_id:0x95.");
                 #endif
                 break;
            /*收到CAN ID*/
			case receive_can_id:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive can data:0x74.");
                #endif
                can_orign_data_update(uart1_analysis_buffer+2,*(uart1_analysis_buffer+1));
                break;
            /*收到DBC CAN ID*/
                case dbc_can_id_rev:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive can data:0xB0.");
                #endif
                receive_can_list_add(&receive_can_data_list,uart1_analysis_buffer,tempLens);
                break;
            /*升级文件ID*/
            case board_update_ret_id:
                #if uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> board_update_ret_id.");
                #endif
                update_board_timeout_cb(EAT_FALSE,0);
                if((*(uart1_analysis_buffer+11)==0x00)||(*(uart1_analysis_buffer+11)==0x01))
                {
                    Tcp_VDR_Common_Replay(Main_Link.mode_Socket,board_update_servn,remoto_update_processor,*(uart1_analysis_buffer+11));
                    #if uart1_data_pro_debug_printf 
                    eat_trace("analysis_uart1_data_proc --> board_update_ret_ok.");
                    #endif
                }
                else
                {
                    updat_program_reslut_report(0x01,*(uart1_analysis_buffer+11));
                    #if uart1_data_pro_debug_printf 
                    eat_trace("analysis_uart1_data_proc --> board_update_ret_error.");
                    #endif
                }
                break;
                
            /*系统休眠开关*/
            case system_sleep_switch:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive system sleep data.");
                #endif
                uart1_send_data_frame(system_sleep_ref,0,0);
                Real_Time_Location_Inf.Location_Speed =0;//卫星速度
                Real_Time_Location_Inf.Travel_Speed = 0;//行驶记录速度
                Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC =EAT_FALSE;
                Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep =EAT_TRUE;
                task_msg_send(EAT_USER_0,EAT_USER_0,SystemSleepMsg);
                break;
            #if 0 
            /*远程休眠*/
            case remote_sleep_ref://远程设置sleep.
                #if uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive system setting sleep return.");
                #endif
                break;
            #endif
            /*测试状态返回*/
            case test_data_rev:
                 system_prouct_test(*(uart1_analysis_buffer));
                 break;
            /*系统标定*/
            case prout_dedmter:
                 system_prouct_demate(uart1_analysis_buffer);
                 break;
			default:
			    break;	
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Create__Send_Communication_Frame
 *  Description:   通信帧封装 
 *  Parameters :  *ID           :发送数据ID
 *				  *Temp_buffer  :发送数据的缓存
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */ 
void uart1_send_data_frame(uint8_t ID,uint8_t *Temp_buffer,uint16_t Lens)
{
	uint16_t length;
	uint8_t CheckSum=0;
	*uart1_send_buffer = 0x7e;//帧头1
	*(uart1_send_buffer+1) = 0x7d;//帧头2
	*(uart1_send_buffer+2) = 0x01;//校验
	*(uart1_send_buffer+3) = ID;//命令ID
	*(uart1_send_buffer+4) = (unsigned char)(Lens/256);//长度低位
	*(uart1_send_buffer+5) = (unsigned char)(Lens&0x00FF);//长度低位
	for(length=0;length<Lens;length++)
	{
		uart1_send_buffer[length+6]=*(Temp_buffer++);
	}
	CheckSum=CrcXOR(uart1_send_buffer,Lens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS);//异或校验
	*(uart1_send_buffer+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens)=0x00;
	*(uart1_send_buffer+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens+1)=CheckSum;
	uart1_send_list_add(&uart1_send_list,uart1_send_buffer,(FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens+2));
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: board_mode_seting 
*  Description:   终端控制 
*  Parameters :  CMD:0x00；终端锁定:
*                     0x01:
                      0x02://北斗
*                     0x03://GPS模式
*                     0x04://混合模式
*                     0x05: 复位
*                     0x06: 恢复出厂设置
*	
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
* =====================================================================================
*/ 
void board_mode_seting(uint8_t cmd)
{
	uart1_send_data_frame(seting_mcu_data,&cmd,1);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: uart1_receive_queue_proc
 *  Description: 处理UART1接收到的数据 
 *  Parameters : void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void uart1_receive_queue_proc(void)
{
   uart1_anaylsis_pro_list_del(&uart1_anaylsis_list);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: uart1_send_queue_proc
*  Description: UART1发送处理函数
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
void uart1_send_queue_proc(void)
{
    uart1_send_list_del(&uart1_send_list);
}
/***串口1解析链表初始化***/
void uart1_anaylsis_pro_list_init(void)
{
    INIT_LIST_HEAD(&uart1_anaylsis_list);
}
void uart1_anaylsis_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
{
   analysis_data_pack_list *mem_prt;
   if(is_list_enful(list_parm,uar1_analysis_list_max) ==EAT_TRUE)
   {
       mem_prt = (analysis_data_pack_list *)list_entry(list_parm->next,analysis_data_pack_list,re_list);
       list_del(list_parm->next);
       eat_mem_free(mem_prt);
   }
   mem_prt =(analysis_data_pack_list *)eat_mem_alloc(sizeof(analysis_data_pack_list));
   mem_prt->data_len = data_cnt;
   memcpy(mem_prt->data_buf,buff,data_cnt);
   tail_list_add(&(mem_prt->re_list),list_parm,uar1_analysis_list_max);
}
void uart1_anaylsis_pro_list_del(struct list_head *list_parm)
{
    analysis_data_pack_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (analysis_data_pack_list *)list_entry(list_parm->next,analysis_data_pack_list,re_list);
		analysis_uart1_data_proc(mem_prt->data_buf);
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if uart1_data_pro_debug_printf 
        eat_trace("uart1_anaylsis_pro_list_del--> list is empty.");
        #endif
    }
}
/***串口1发送队列***/
void uart1_send_list_init(void)
{
    INIT_LIST_HEAD(&uart1_send_list);
}
void uart1_send_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
{
   uart1_send_data_pack_list *mem_prt;
   if(is_list_enful(list_parm,uar1_send_list_max) ==EAT_TRUE)
   {
       mem_prt = (uart1_send_data_pack_list *)list_entry(list_parm->next,uart1_send_data_pack_list,re_list);
       list_del(list_parm->next);
       eat_mem_free(mem_prt);
   }
   mem_prt =(uart1_send_data_pack_list *)eat_mem_alloc(sizeof(uart1_send_data_pack_list));
   memset(mem_prt,0,sizeof(uart1_send_data_pack_list));
    
   mem_prt->data_len = data_cnt;
   memcpy(mem_prt->data_buf,buff,data_cnt);
   tail_list_add(&(mem_prt->re_list),list_parm,uar1_send_list_max);
}
void uart1_send_list_del(struct list_head *list_parm)
{
    uart1_send_data_pack_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (uart1_send_data_pack_list *)list_entry(list_parm->next,uart1_send_data_pack_list,re_list);
        eat_uart_write(EAT_UART_1,mem_prt->data_buf,mem_prt->data_len);//发送数据
        #if uart1_data_pro_debug_printf 
        eat_trace("uart1_send_list_del--> send hex.");
        HexToAsicc(mem_prt->data_buf,mem_prt->data_len);
        #endif
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if uart1_data_pro_debug_printf 
        eat_trace("uart1_send_list_del--> list is empty.");
        #endif
    }
}



