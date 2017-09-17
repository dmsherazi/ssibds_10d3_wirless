
#include "uart1_data_proc.h"

/****************************************************
* External variables declaration
*****************************************************/
/***����������������***/
struct list_head uart1_anaylsis_list;
struct list_head uart1_send_list;
/***SIM��״̬��֤***/
SimState_union  Sim_State;//״̬��Ϣ

/***UART1��һ������***/
static uint8_t uart1_receive_buffer[uar1_receive_max]={0};
/***UART1���������ݻ���***/
static uint8_t realy_uart1_data_buffer[uar1_receive_max]={0};
/***UART1�������ݻ���***/
static uint8_t uart1_analysis_buffer[uar1_receive_max]={0};
/***UART1�������ݻ���***/
static uint8_t uart1_send_buffer[uar1_send_max]={0};

/*******************************���մ���1����***********************************************/
void  uart1_receive_integrated_frame_proc(uint8_t *Source_Buff,uint16_t Re_Lens)//USER0
{
	uint8_t  cData=0;
	uint16_t Datat_cnt;
    uint16_t Effect_Data_Lens=0;
    static uint16_t TotalCnt=0;//��̬�����ܼ���
    static uint8_t  rev_step_flag=0;//����֡������һ��
    static uint16_t data_area_lens=0;//���ݳ���
	Effect_Data_Lens = Re_Lens;//��ȡ��Ч���ݳ���
	memset(uart1_receive_buffer,0,sizeof(uart1_receive_buffer));
	memcpy(uart1_receive_buffer,Source_Buff,Effect_Data_Lens);//����BUFFER���ɿ�!!!!!!
	if(Effect_Data_Lens)//�ַ�������
	{
		for(Datat_cnt=0;Datat_cnt<Effect_Data_Lens;Datat_cnt++)//��֤һ��У��һλ
		{
			cData =*(uart1_receive_buffer+Datat_cnt);//��ֵ
			switch(rev_step_flag)
		    {	
				case 0:
				if((cData==Frame_Head_One))//ͷ֡1
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
				if((cData==Frame_Head_two))//ͷ֡2
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
		        if(TotalCnt==(data_area_lens+7))//βУ����
		       	{
		       		*(realy_uart1_data_buffer+TotalCnt)=cData;
					TotalCnt++; //ȷ������
					
                    #if uart1_data_pro_debug_printf 
                    eat_trace("uart1_receive_integrated_frame_proc -->State:�յ���������֡��");
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
 *  Description:   �����ӵ�������֡
 *  Parameters :  *pSource:ԭʼ����ָ��
 *				  *pType  :������ķ���ֵ
 *				  *pDest  :��������bufferָ��
 *                *pLens  :���ݳ���
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
     tempLens=(*(pSource+CRC_AREA_FIXED_LENS))*256+*(pSource+CRC_AREA_FIXED_LENS+1); //���ݳ���
     tempPos=tempLens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS+1;//Ŀǰλ��
   	 ScrCheckSum=*(pSource+tempPos);//У���
   	 CheckSum=CrcXOR(pSource,tempLens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS);//���У��
   	 if(CheckSum!=ScrCheckSum)//У����Ա�
   	 {
   		return RET_CRC_ERR;     //��ӡУ�������Ϣ
   	 }
	 else
	 {
		 if(pDest!=NULL)//��Ŀ�����鸳ֵ
	   	 {
		   	 for(cnt=0;cnt<tempLens;cnt++)
		   	 {
		   	 	*(pDest++)=*(pSource+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+cnt);
		   	 }
	   	 }
	   	 if(pLens!=NULL) *pLens=tempLens;//���ݳ���
	   	 if(pType!=NULL) *pType=*(pSource+FRAME_HEAD_LENS+1);   	 
	   	 return RET_NORMAL;//��ȷ����
	 }
}

/* 
 * ===  FUNCTION  ==================== ==================================================
 *  Name: Analyze_Type_Setup
 *  Description:   �����ӵ������ݲ�����������֡���� Rev_Buff
 *  Parameters :  *Rev_Buff     :����������֡
 *				  *Source_Buff  :ԭʼ����ָ��
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-6 			 
 * =====================================================================================
 */ 
/*******************************���մ���1����***********************************************/
static void analysis_uart1_data_proc(uint8_t *pSource)//USER4
{
	uint8_t tempType=0;
	uint16_t tempLens=0;
	_RET_INFOR ret;
	uint16_t Data_Lens=0;
	uint8_t msg_point=0;
	memset(uart1_analysis_buffer,0,sizeof(uart1_analysis_buffer));
	ret=AnalyzeRevFrame(pSource,&tempType,uart1_analysis_buffer,&tempLens);//��������
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
        eat_trace("Analyze_Type_Setup �������� --> sucess ID:%0x,len:%u",tempType,tempLens);
        HexToAsicc(uart1_analysis_buffer,tempLens);
        #endif
		switch(tempType)
		{
		    /*��������*/
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
            /*1S��Ϣ*/
			case board_send_to_sim800:// 1S��Ϣ
    			 sim800_board_commucation_proc(uart1_analysis_buffer);
    			 lock_car_strategy_sleep_send_proc();
    			 break;
    	    
			/***���п��ƽ���***/
			case remote_lock_car_rev:
                lock_car_cmd_timeout_cb(EAT_FALSE);
    			lock_car_strategy_rev_proc(uart1_analysis_buffer,tempLens);
    			break;
    			
            /***���в���***/
            case lock_car_data_save_board_rev:
                lock_car_strategy_save_board_data(uart1_analysis_buffer);
                break;
    			 
           /***��ѯ�ն�ID IP �ֻ���***/
            case request_id_ip_phonenum:
                 board_inquire_id_ip_phone_num();
                 break;
           /***TXT������Ų�ѯ***/
    		case txt_cnt_inquire:
    		     txt_data_cnt_inquire(*uart1_analysis_buffer); 
     		     break;
            /***�˵�����***/
            case memu_setting_data_id:
                 HexToAsicc(uart1_analysis_buffer,tempLens);
                 memu_setting_data_pro(uart1_analysis_buffer);
                 break;
            /***��ѯtxt�ı�����***/
     		case txt_infor_inquire:
     		     txt_data_infor_inquire(*uart1_analysis_buffer,*(uart1_analysis_buffer+1));
     		     break;
     		/***SIM800����汾��ѯ***/
            case sim800_software_vion_inquire:
                 board_inquire_sim800_software_version();
                 break;
           /***SIM800����汾��ѯ***/
			case updata_sim800_process:
                 #if 1//uart1_data_pro_debug_printf 
                 eat_trace("analysis_uart1_data_proc --> receive sim800 update data.");
                 #endif
    			 sim800_upadat_sever(uart1_analysis_buffer);
    			 break;
    			 /*****��Ϣ�궨*****/
            case demarcate_id:
                 board_demarcate_data_pro(uart1_analysis_buffer,tempLens);
                 #if uart1_data_pro_debug_printf 
                 eat_trace("analysis_uart1_data_proc --> receive demarcate_id:0x95.");
                 #endif
                 break;
            /*�յ�CAN ID*/
			case receive_can_id:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive can data:0x74.");
                #endif
                can_orign_data_update(uart1_analysis_buffer+2,*(uart1_analysis_buffer+1));
                break;
            /*�յ�DBC CAN ID*/
                case dbc_can_id_rev:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive can data:0xB0.");
                #endif
                receive_can_list_add(&receive_can_data_list,uart1_analysis_buffer,tempLens);
                break;
            /*�����ļ�ID*/
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
                
            /*ϵͳ���߿���*/
            case system_sleep_switch:
                #if 1//uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive system sleep data.");
                #endif
                uart1_send_data_frame(system_sleep_ref,0,0);
                Real_Time_Location_Inf.Location_Speed =0;//�����ٶ�
                Real_Time_Location_Inf.Travel_Speed = 0;//��ʻ��¼�ٶ�
                Real_Time_Location_Inf.VDR_State_Inf.State_Bit.ACC =EAT_FALSE;
                Real_Time_Location_Inf.IO_State_Add.IO_State_Bit.System_Sleep =EAT_TRUE;
                task_msg_send(EAT_USER_0,EAT_USER_0,SystemSleepMsg);
                break;
            #if 0 
            /*Զ������*/
            case remote_sleep_ref://Զ������sleep.
                #if uart1_data_pro_debug_printf 
                eat_trace("analysis_uart1_data_proc --> receive system setting sleep return.");
                #endif
                break;
            #endif
            /*����״̬����*/
            case test_data_rev:
                 system_prouct_test(*(uart1_analysis_buffer));
                 break;
            /*ϵͳ�궨*/
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
 *  Description:   ͨ��֡��װ 
 *  Parameters :  *ID           :��������ID
 *				  *Temp_buffer  :�������ݵĻ���
 *	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */ 
void uart1_send_data_frame(uint8_t ID,uint8_t *Temp_buffer,uint16_t Lens)
{
	uint16_t length;
	uint8_t CheckSum=0;
	*uart1_send_buffer = 0x7e;//֡ͷ1
	*(uart1_send_buffer+1) = 0x7d;//֡ͷ2
	*(uart1_send_buffer+2) = 0x01;//У��
	*(uart1_send_buffer+3) = ID;//����ID
	*(uart1_send_buffer+4) = (unsigned char)(Lens/256);//���ȵ�λ
	*(uart1_send_buffer+5) = (unsigned char)(Lens&0x00FF);//���ȵ�λ
	for(length=0;length<Lens;length++)
	{
		uart1_send_buffer[length+6]=*(Temp_buffer++);
	}
	CheckSum=CrcXOR(uart1_send_buffer,Lens+CRC_AREA_FIXED_LENS+FRAME_HEAD_LENS);//���У��
	*(uart1_send_buffer+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens)=0x00;
	*(uart1_send_buffer+FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens+1)=CheckSum;
	uart1_send_list_add(&uart1_send_list,uart1_send_buffer,(FRAME_HEAD_LENS+CRC_AREA_FIXED_LENS+Lens+2));
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: board_mode_seting 
*  Description:   �ն˿��� 
*  Parameters :  CMD:0x00���ն�����:
*                     0x01:
                      0x02://����
*                     0x03://GPSģʽ
*                     0x04://���ģʽ
*                     0x05: ��λ
*                     0x06: �ָ���������
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
 *  Description: ����UART1���յ������� 
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
*  Description: UART1���ʹ�����
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
void uart1_send_queue_proc(void)
{
    uart1_send_list_del(&uart1_send_list);
}
/***����1���������ʼ��***/
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
/***����1���Ͷ���***/
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
        eat_uart_write(EAT_UART_1,mem_prt->data_buf,mem_prt->data_len);//��������
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



