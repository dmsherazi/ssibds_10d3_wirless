
#include "driving_records_trans_out.h"

/*查询形式记录数据队列*/
inquire_dring_record_queue inquire_dring_record;
/*查询数据ip*/
extern uint8_t inquire_record_ip=0x00;//0x01 MANI 0x02:SECOND

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: gb19056_inquier_data_envelope
  *  Description:  国标数据封装
  *  Parameters : gb_cmd:国标命令字
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void gb19056_inquier_data_envelope(uint8_t gb_cmd)
{
	uint8_t check_sum = 0;
	uint8_t send_dat[12]={0};
	send_dat[0]=0xAA;
	send_dat[1]=0x75;
	send_dat[2]=gb_cmd;
	send_dat[3]=0;
	send_dat[4]=0;
	send_dat[5]=0;
	check_sum=CrcXOR(send_dat,6);//异或校验
	send_dat[6]=check_sum;
	
    #if dring_record_debug_printf 
    eat_trace("gb19056_inquier_data_envelope --> 原始数据.");
	HexToAsicc(send_dat,7);
    #endif
	uart1_send_data_frame(sim800_dring_record_inquire,send_dat,7);
}


 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: dring_record_error_envlope
 *  Description:  dest:目标指针 error_data:错误数据
 *  Parameters :  
 *                Result  应答结果
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
void dring_record_error_envlope(uint8_t *dest,uint8_t error_data)
{
	uint8_t send_data[10]={0};
	send_data[0]=0x55;
	send_data[1]=0x7A;
	send_data[2]=error_data;
	send_data[3]=0x00;
	send_data[4]=(u8)CrcXOR(send_data,4);	
	
    #if dring_record_debug_printf 
    eat_trace("dring_record_error_envlope -->行驶记录发送错误。.");
	HexToAsicc(send_data,5);
    #endif
	memcpy(dest,send_data,5);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: driver_record_data_check
 *  Description:  检验指令是否有误
 *  Parameters :  check_cmd:命令 buffer:数据buufer 数据长度
 *                返回错误类型
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
char driver_record_data_check(uint8_t check_cmd,uint8_t *buffer,uint16_t lens)
{
	uint8_t checkSum=0;
	checkSum=CrcXOR(buffer,lens-1);//异或校验
	if(checkSum==*(buffer+lens-1))
	{
        #if dring_record_debug_printf 
        eat_trace("driver_record_data_check -->命令正确。.");
        #endif
		return check_ok;	
	}
	else
	{ 
	    /*********区分设置信息与查询信息***********/
		if(check_cmd > 0x80)//设置错误
		{
            #if dring_record_debug_printf 
            eat_trace("driver_record_data_check -->设置信息校验失败。");
            #endif
			return check_set_error;
		}
		else
		{
            #if dring_record_debug_printf 
            eat_trace("driver_record_data_check -->查询信息校验失败。");
            #endif
			return check_inquire;
		}
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : hy_package_driver_record_update
 *  Description: 货运平台数据封装
 *  Parameters : RCMD,命令，u8 *Pouce原始报文,u8*Ddes目标报文,u16 *Dlens
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */

uint8_t hy_package_driver_record_update(uint8_t socket,uint8_t inquire_id,uint32_t start,uint32_t end_time,uint16_t block)
{
	uint16_t Block_Cnt = 0;//数据块个数
	uint32_t File_Size = 0;//文件大小
	uint8_t Temp_Time[6] = {0};//时间
	uint16_t Temp_Cnt = 0;//临时计数
	uint32_t TimerSec = 0;//S时间
	uint16_t Data_OFFEST_Cnt = 0;
	uint16_t Driver_DataLen = 0;
	uint8_t Temp_Drive[680] = {0};
	uint8_t Check_XOR = 0;
	uint16_t msg_servn = 0;
    /*****创建缓存文件****/
	file_create(package_driver_record_file);
	switch(inquire_id)
	{
	    /*****采集指定的行驶速度记录(08H)****/
		case 0x08: //速度
    		get_file_size(driver_record_08H,&File_Size);//获取文件大小
    		Block_Cnt = (u16)(File_Size/126);
    		file_read(driver_record_08H,0,Temp_Time,6);//读时间信息
            /*****查找符合要求文件写入文件****/
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_08H,Temp_Cnt*126,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_08H,Temp_Cnt*126,Temp_Drive,126);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*126,Temp_Drive,126);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,126);//10+Data_OFFEST_Cnt*126
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*126;
		    break;
	    /*****采集指定的位置信息记录(09H)****/
		case 0x09://位置信息
      		get_file_size(driver_record_09H,&File_Size);//获取文件大小
      		Block_Cnt = (u16)(File_Size/666);//5G数据块
      		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
      		{
      			file_read(driver_record_09H,Temp_Cnt*666,Temp_Time,6);//读附加信息
      			TimerSec=bcd_timer_to_sec(Temp_Time);
      			if((TimerSec >= start)&&(TimerSec <= end_time))
      			{
      				file_read(driver_record_09H,Temp_Cnt*666,Temp_Drive,666);//读附加信息
      				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*666,Temp_Drive,666);//记录文件
      				Check_XOR^=CrcXOR(Temp_Drive,666);
      				Data_OFFEST_Cnt++;
      			}
      			if(Data_OFFEST_Cnt==block)
      			{
      				break;
      			}
      		}
      		Driver_DataLen=Data_OFFEST_Cnt*666;	
      		break;
		/*采集指定的事故疑点记录(10H)*/
		case 0x10:
       		get_file_size(driver_record_10H,&File_Size);//获取文件大小
       		Block_Cnt = (u16)(File_Size/234);//5G数据块
       		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
       		{
       			file_read(driver_record_10H,Temp_Cnt*234,Temp_Time,6);//读附加信息
       			TimerSec=bcd_timer_to_sec(Temp_Time);
       			if((TimerSec >= start)&&(TimerSec <= end_time))
       			{
       				file_read(driver_record_10H,Temp_Cnt*234,Temp_Drive,234);//读附加信息
       				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*234,Temp_Drive,234);//记录文件
       				Check_XOR^=CrcXOR(Temp_Drive,234);
       				Data_OFFEST_Cnt++;
       			}
       			if(Data_OFFEST_Cnt==block)
       			{
       				break;
       			}
       		}
       		Driver_DataLen=Data_OFFEST_Cnt*234;		
       		break;
		/**采集指定的超时驾驶记录(11H)**/
		case 0x11:
    		get_file_size(driver_record_11H,&File_Size);//获取文件大小
    		Block_Cnt = (uint16_t)(File_Size/50);//5G数据块
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_11H,18+Temp_Cnt*50,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_11H,Temp_Cnt*50,Temp_Drive,50);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*50,Temp_Drive,50);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,50);
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*50;		
    		break;
        /*驾驶人身份记录(12H)*/
		case 0x12:
    		get_file_size(driver_record_12H,&File_Size);//获取文件大小
    		Block_Cnt = (u16)(File_Size/25);//5G数据块
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_12H,Temp_Cnt*25,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_12H,Temp_Cnt*25,Temp_Drive,25);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*25,Temp_Drive,25);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,25);
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*25;		
    		break;
        /*采集指定的记录仪外部供电记录(13H)*/
		case 0x13:
    		get_file_size(driver_record_13H,&File_Size);//获取文件大小
    		Block_Cnt = (u16)(File_Size/7);//5G数据块
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_13H,Temp_Cnt*7,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_13H,Temp_Cnt*7,Temp_Drive,7);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*7,Temp_Drive,7);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,7);
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*7;		
    		break;
        /*采集指定的记录仪参数修改记录(14H)*/
		case 0x14:
    		get_file_size(driver_record_14H,&File_Size);//获取文件大小
    		Block_Cnt = (u16)(File_Size/7);//5G数据块
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_14H,Temp_Cnt*7,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_14H,Temp_Cnt*7,Temp_Drive,7);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*7,Temp_Drive,7);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,7);
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*7;			
    		break;
		
        /*速度状态日志(15H)*/
		case 0x15:
    		get_file_size(driver_record_15H,&File_Size);//获取文件大小
    		Block_Cnt = (u16)(File_Size/133);//5G数据块
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_15H,1+Temp_Cnt*133,Temp_Time,6);//读附加信息
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_15H,Temp_Cnt*133,Temp_Drive,133);//读附加信息
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*133,Temp_Drive,133);//记录文件
    				Check_XOR^=CrcXOR(Temp_Drive,133);
    				Data_OFFEST_Cnt++;
    			}
    			if(Data_OFFEST_Cnt==block)
    			{
    				break;
    			}
    		}
    		Driver_DataLen=Data_OFFEST_Cnt*133;			
    		break;
		
		default:
		break;
	}
	if(Driver_DataLen!=0x00)
	{
 		Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,Temp_Drive);
		Temp_Drive[2]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Temp_Drive[3]=0x55;
		Temp_Drive[4]=0x7A;
		Temp_Drive[5]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Big_End_WordtoByte(Driver_DataLen,Temp_Drive+6);
		Temp_Drive[8]=0x00;
		Check_XOR^=CrcXOR(Temp_Drive+3,6);//数据
		file_write(package_driver_record_file,0,Temp_Drive,9);//记录文件
		file_write(package_driver_record_file,9+Driver_DataLen,&Check_XOR,1);//记录文件写入校验码

        //文件准备OK

	}
	else
	{
        eat_fs_Delete(package_driver_record_file);
 		Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,Temp_Drive);//消息流水
		Temp_Drive[2]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Temp_Drive[3]=0x55;
		Temp_Drive[4]=0x7A;
		Temp_Drive[5]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Big_End_WordtoByte(Driver_DataLen,Temp_Drive+6);
		Temp_Drive[8]=0x00;
		Temp_Drive[9]=CrcXOR(Temp_Drive+3,6);//数据
		
        Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_servn,0x0700,Temp_Drive,10,0,0,0);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: hy_dring_record_data_inquire_proc
 *  Description:  货运平台查询行驶记录数据
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
void hy_dring_record_data_inquire_proc(void)
 {
 	uint16_t msg_servn=0;
 	uint8_t envlope_data[20]={0};
 	uint8_t dr_inquire_check;
 	uint32_t start_time = 0;
 	uint32_t end_time = 0;
 	uint16_t record_block;
 	uint8_t L_socket=0;
 	if(inquire_record_ip==0x01)
 	{
 	    L_socket=Main_Link.mode_Socket;
 	}
 	else if(inquire_record_ip==0x02)
 	{
 	    L_socket=Second_Link.mode_Socket;
 	}
 	else
 	{
 	    return;
 	}
 	/****检测查询数据是否校验合法****/
 	dr_inquire_check = driver_record_data_check(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd,\
 	                     inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,\
 	                        inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
 	if(dr_inquire_check != check_ok)
 	{
 		Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,envlope_data);
 		*(envlope_data+2) = inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
 		if(dr_inquire_check == check_set_error)
 		{
 			dring_record_error_envlope(envlope_data+3,0xFB);
 		}
 		else if(dr_inquire_check == check_inquire)
 		{
 			dring_record_error_envlope(envlope_data+3,0xFA);
 		}
 		if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd > 0x80)
 		{
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->TCP send-设置信息校验失败。");
            #endif
            Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_servn,0x8700,envlope_data,8,0,0,0);
 		}
 		else
 		{
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->TCP send-查询信息校验失败。");
            #endif
			Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x01);//通用应答
 		}
 	}
 	else
 	{
        if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd > 0x80)
        {
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->设置信息处理");
            #endif
 			if(((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0x82)&&\
 			    (inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0x84))|| \
 			     ((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0xC2)&&\
 			        (inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0xC4)))
	 		{
				Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x00);//通用应答
                uart1_send_data_frame(sim800_dring_record_inquire,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
	 		}
	 		else
	 		{
	 			Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x01);//通用应答
	 		}

        }
        else
        {
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->TCP send-查询信息校验失败。");
            #endif
            if((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0x00)&&(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0x15))
            {
               /****************采集指定的行驶速度记录 单位MIN***************/
               if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd == 0x08)
                {
                   inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff[11] = 0;
                   inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff[17] = 0;

                   Big_End_BytetoWord(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+19,&record_block);
                }
                else if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd == 0x09)
                {
                    memset(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+11,0,2);
                    memset(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+17,0,2);
                    Big_End_BytetoWord(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+19,&record_block);
                }
                else if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>0x09)
                {
                    Big_End_BytetoWord(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+19,&record_block);
                }
                else if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<0x08)
                {
                    eat_trace("SSI_FS_debug【文件大发送采集命令】");
                    uart1_send_data_frame(sim800_dring_record_inquire,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
                    return;
                }
                start_time = bcd_timer_to_sec((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+6));//起始点
                end_time = bcd_timer_to_sec((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+12));//终止点

                #if dring_record_debug_printf 
                eat_trace("dring_record_data_inquire_proc -->封装要发送的数据。");//dbc解析线程
                #endif
                
                uart1_send_data_frame(sim800_dring_record_inquire,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
                //hy_package_driver_record_update(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd,start_time,end_time,record_block);
                
            }
            else
            {
                Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,envlope_data);
                *(envlope_data+2) = inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
                dring_record_error_envlope(envlope_data+3,0xFA);
                Tcp_Message_PacKage_And_Send(L_socket,&msg_servn,0x8700,envlope_data,8,0,0,0);
            }
        }
 		
 	}
}
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_proc
  *  Description:  处理行驶记录数据
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void inquire_dring_record_queue_proc(void)
{
    if(inquire_dring_record.inquire_queue_state == EAT_FALSE)
    {
        if(is_inquire_dring_record_queue_empty()==EAT_FALSE)
        {
            #if dring_record_debug_printf 
            eat_trace("inquire_dring_record_queue_proc --> 发送消息到user3 进行消息封装处理");//dbc解析线程
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_3,driver_record_data_proc);
            inquire_dring_record.inquire_queue_state = EAT_TRUE;
        }
        else
        {
            #if dring_record_debug_printf 
            eat_trace("inquire_dring_record_queue_proc --> 队列为空。。");//dbc解析线程
            #endif
        }
    }
}


/*********************************行驶记录采集命令队列函数****************************************/
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_init
  *  Description:  初始化inquire_dring_record 队列 行驶记录数据查询队列
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void inquire_dring_record_queue_init(void)
{
    memset(&inquire_dring_record,0,sizeof(inquire_dring_record));
    inquire_dring_record.queuesize = dring_record_max;
    inquire_dring_record.tail= 0;
    inquire_dring_record.head= 0;
    inquire_dring_record.inquire_queue_state=EAT_FALSE;
}
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  进入行驶记录数据查询队列
  *  Parameters :dr_cmd:形式记录命令，servn_num:流水号
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void enter_inquire_dring_record_queue(uint8_t dr_cmd,uint16_t servn_num,uint8_t *data,uint8_t data_len)
 {
    uint8_t Tail = 0;
    Tail = (inquire_dring_record.tail+1)%(inquire_dring_record.queuesize); 
    if (Tail == inquire_dring_record.head) //此时队列没有空间
    {
		#if dring_record_debug_printf 
		eat_trace("enter_inquire_dring_record_queue --> 队列满.");
		#endif
    }
    else
    {
        inquire_dring_record.dring_recod_arry[inquire_dring_record.tail].dring_record_cmd = dr_cmd;
        inquire_dring_record.dring_recod_arry[inquire_dring_record.tail].inquire_servn_num = servn_num;
        inquire_dring_record.dring_recod_arry[inquire_dring_record.tail].data_len = data_len;
        memcpy(inquire_dring_record.dring_recod_arry[inquire_dring_record.tail].data_buff,data,data_len);
        inquire_dring_record.tail = Tail;
		#if dring_record_debug_printf 
		eat_trace("enter_inquire_dring_record_queue --> 进队列head:%d,tail:%d.",inquire_dring_record.head,inquire_dring_record.tail);
		#endif
    }
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_Driver_Data_Trans
 *  Description:  行驶记录数据导出
 *  Parameters :  
 *				  *Pbuffer 来源数据Pubffer
 *                *Lens    数据长度
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void tcp_driver_data_update(uint8_t *buffer,uint16_t Lens)
{
	uint8_t socket=0;
	uint8_t temp_buffer[1024]=0;
	uint16_t offest=0,msgs_sern=0;
	
	Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,temp_buffer);
	offest+=sizeof(uint16_t);
	
	*(temp_buffer+offest) = inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;//命令字
	offest+=sizeof(uint8_t);
	
	memcpy(temp_buffer+offest,buffer,Lens);//转数据
	offest+=Lens;
 	if(inquire_record_ip==0x01)
 	{
 	    socket=Main_Link.mode_Socket;
 	}
 	else if(inquire_record_ip==0x02)
 	{
 	    socket=Second_Link.mode_Socket;
 	}
 	else
 	{
 	    return;
 	}
    Tcp_Message_PacKage_And_Send(socket,&msgs_sern,inquire_driver_data_ref,temp_buffer,offest,0,0,0);

}
 
/* 
* ===  FUNCTION  ======================================================================
*  Name: enter_inquire_dring_record_queue
*  Description:  进入行驶记录数据查询队列
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
void inquire_dring_record_queue_porc(void)
{
    
}
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  进入行驶记录数据查询队列
  *  Parameters : void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void exit_inquire_dring_record_queue(void)
 {
     if(inquire_dring_record.tail == inquire_dring_record.head)     
     {
         #if dring_record_debug_printf 
         eat_trace("exit_inquire_dring_record_queue --> 队列为空.");
         #endif
         
     }
     else
     {
         inquire_dring_record.inquire_queue_state=EAT_FALSE;
         inquire_dring_record.head = (inquire_dring_record.head+1) % (inquire_dring_record.queuesize);
     }
     #if dring_record_debug_printf 
     eat_trace("exit_inquire_dring_record_queue --> 出队列head:%d,tail:%d.",inquire_dring_record.head,inquire_dring_record.tail);
     #endif
 }
 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: is_inquire_dring_record_queue_empty
   *  Description:  行驶记录查询队列是否为空
   *  Parameters : void
   *  Return     :  void 
   *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
   * =====================================================================================
   */ 
char_t is_inquire_dring_record_queue_empty(void)
 {
      if(inquire_dring_record.head == inquire_dring_record.tail)
      {
          #if dring_record_debug_printf 
          eat_trace("is_at_cmd_send_queue_empty --> 队列为空.");
          #endif
          return EAT_TRUE;
      }
      else
      {
          return EAT_FALSE;
      }
 }
 
 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: is_inquire_dring_record_queue_full
   *  Description:  行驶记录查询队列是否为满
   *  Parameters : void
   *  Return     :  void 
   *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
   * =====================================================================================
   */ 
char_t is_inquire_dring_record_queue_full(void)
 {
     if((inquire_dring_record.tail+1)% inquire_dring_record.queuesize == inquire_dring_record.head)
     {
         return EAT_TRUE;
     }
     else
     {
         return EAT_FALSE;
     }
 }


