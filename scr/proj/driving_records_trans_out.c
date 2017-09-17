
#include "driving_records_trans_out.h"

/*��ѯ��ʽ��¼���ݶ���*/
inquire_dring_record_queue inquire_dring_record;
/*��ѯ����ip*/
extern uint8_t inquire_record_ip=0x00;//0x01 MANI 0x02:SECOND

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: gb19056_inquier_data_envelope
  *  Description:  �������ݷ�װ
  *  Parameters : gb_cmd:����������
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
	check_sum=CrcXOR(send_dat,6);//���У��
	send_dat[6]=check_sum;
	
    #if dring_record_debug_printf 
    eat_trace("gb19056_inquier_data_envelope --> ԭʼ����.");
	HexToAsicc(send_dat,7);
    #endif
	uart1_send_data_frame(sim800_dring_record_inquire,send_dat,7);
}


 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: dring_record_error_envlope
 *  Description:  dest:Ŀ��ָ�� error_data:��������
 *  Parameters :  
 *                Result  Ӧ����
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
    eat_trace("dring_record_error_envlope -->��ʻ��¼���ʹ���.");
	HexToAsicc(send_data,5);
    #endif
	memcpy(dest,send_data,5);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: driver_record_data_check
 *  Description:  ����ָ���Ƿ�����
 *  Parameters :  check_cmd:���� buffer:����buufer ���ݳ���
 *                ���ش�������
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
char driver_record_data_check(uint8_t check_cmd,uint8_t *buffer,uint16_t lens)
{
	uint8_t checkSum=0;
	checkSum=CrcXOR(buffer,lens-1);//���У��
	if(checkSum==*(buffer+lens-1))
	{
        #if dring_record_debug_printf 
        eat_trace("driver_record_data_check -->������ȷ��.");
        #endif
		return check_ok;	
	}
	else
	{ 
	    /*********����������Ϣ���ѯ��Ϣ***********/
		if(check_cmd > 0x80)//���ô���
		{
            #if dring_record_debug_printf 
            eat_trace("driver_record_data_check -->������ϢУ��ʧ�ܡ�");
            #endif
			return check_set_error;
		}
		else
		{
            #if dring_record_debug_printf 
            eat_trace("driver_record_data_check -->��ѯ��ϢУ��ʧ�ܡ�");
            #endif
			return check_inquire;
		}
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : hy_package_driver_record_update
 *  Description: ����ƽ̨���ݷ�װ
 *  Parameters : RCMD,���u8 *Pouceԭʼ����,u8*DdesĿ�걨��,u16 *Dlens
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */

uint8_t hy_package_driver_record_update(uint8_t socket,uint8_t inquire_id,uint32_t start,uint32_t end_time,uint16_t block)
{
	uint16_t Block_Cnt = 0;//���ݿ����
	uint32_t File_Size = 0;//�ļ���С
	uint8_t Temp_Time[6] = {0};//ʱ��
	uint16_t Temp_Cnt = 0;//��ʱ����
	uint32_t TimerSec = 0;//Sʱ��
	uint16_t Data_OFFEST_Cnt = 0;
	uint16_t Driver_DataLen = 0;
	uint8_t Temp_Drive[680] = {0};
	uint8_t Check_XOR = 0;
	uint16_t msg_servn = 0;
    /*****���������ļ�****/
	file_create(package_driver_record_file);
	switch(inquire_id)
	{
	    /*****�ɼ�ָ������ʻ�ٶȼ�¼(08H)****/
		case 0x08: //�ٶ�
    		get_file_size(driver_record_08H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (u16)(File_Size/126);
    		file_read(driver_record_08H,0,Temp_Time,6);//��ʱ����Ϣ
            /*****���ҷ���Ҫ���ļ�д���ļ�****/
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_08H,Temp_Cnt*126,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_08H,Temp_Cnt*126,Temp_Drive,126);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*126,Temp_Drive,126);//��¼�ļ�
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
	    /*****�ɼ�ָ����λ����Ϣ��¼(09H)****/
		case 0x09://λ����Ϣ
      		get_file_size(driver_record_09H,&File_Size);//��ȡ�ļ���С
      		Block_Cnt = (u16)(File_Size/666);//5G���ݿ�
      		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
      		{
      			file_read(driver_record_09H,Temp_Cnt*666,Temp_Time,6);//��������Ϣ
      			TimerSec=bcd_timer_to_sec(Temp_Time);
      			if((TimerSec >= start)&&(TimerSec <= end_time))
      			{
      				file_read(driver_record_09H,Temp_Cnt*666,Temp_Drive,666);//��������Ϣ
      				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*666,Temp_Drive,666);//��¼�ļ�
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
		/*�ɼ�ָ�����¹��ɵ��¼(10H)*/
		case 0x10:
       		get_file_size(driver_record_10H,&File_Size);//��ȡ�ļ���С
       		Block_Cnt = (u16)(File_Size/234);//5G���ݿ�
       		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
       		{
       			file_read(driver_record_10H,Temp_Cnt*234,Temp_Time,6);//��������Ϣ
       			TimerSec=bcd_timer_to_sec(Temp_Time);
       			if((TimerSec >= start)&&(TimerSec <= end_time))
       			{
       				file_read(driver_record_10H,Temp_Cnt*234,Temp_Drive,234);//��������Ϣ
       				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*234,Temp_Drive,234);//��¼�ļ�
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
		/**�ɼ�ָ���ĳ�ʱ��ʻ��¼(11H)**/
		case 0x11:
    		get_file_size(driver_record_11H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (uint16_t)(File_Size/50);//5G���ݿ�
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_11H,18+Temp_Cnt*50,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_11H,Temp_Cnt*50,Temp_Drive,50);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*50,Temp_Drive,50);//��¼�ļ�
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
        /*��ʻ����ݼ�¼(12H)*/
		case 0x12:
    		get_file_size(driver_record_12H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (u16)(File_Size/25);//5G���ݿ�
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_12H,Temp_Cnt*25,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_12H,Temp_Cnt*25,Temp_Drive,25);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*25,Temp_Drive,25);//��¼�ļ�
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
        /*�ɼ�ָ���ļ�¼���ⲿ�����¼(13H)*/
		case 0x13:
    		get_file_size(driver_record_13H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (u16)(File_Size/7);//5G���ݿ�
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_13H,Temp_Cnt*7,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_13H,Temp_Cnt*7,Temp_Drive,7);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*7,Temp_Drive,7);//��¼�ļ�
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
        /*�ɼ�ָ���ļ�¼�ǲ����޸ļ�¼(14H)*/
		case 0x14:
    		get_file_size(driver_record_14H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (u16)(File_Size/7);//5G���ݿ�
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_14H,Temp_Cnt*7,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_14H,Temp_Cnt*7,Temp_Drive,7);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*7,Temp_Drive,7);//��¼�ļ�
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
		
        /*�ٶ�״̬��־(15H)*/
		case 0x15:
    		get_file_size(driver_record_15H,&File_Size);//��ȡ�ļ���С
    		Block_Cnt = (u16)(File_Size/133);//5G���ݿ�
    		for(Temp_Cnt=0;Temp_Cnt<Block_Cnt;Temp_Cnt++)
    		{
    			file_read(driver_record_15H,1+Temp_Cnt*133,Temp_Time,6);//��������Ϣ
    			TimerSec=bcd_timer_to_sec(Temp_Time);
    			if((TimerSec >= start)&&(TimerSec <= end_time))
    			{
    				file_read(driver_record_15H,Temp_Cnt*133,Temp_Drive,133);//��������Ϣ
    				file_write(package_driver_record_file,9+Data_OFFEST_Cnt*133,Temp_Drive,133);//��¼�ļ�
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
		Check_XOR^=CrcXOR(Temp_Drive+3,6);//����
		file_write(package_driver_record_file,0,Temp_Drive,9);//��¼�ļ�
		file_write(package_driver_record_file,9+Driver_DataLen,&Check_XOR,1);//��¼�ļ�д��У����

        //�ļ�׼��OK

	}
	else
	{
        eat_fs_Delete(package_driver_record_file);
 		Big_End_WordtoByte(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,Temp_Drive);//��Ϣ��ˮ
		Temp_Drive[2]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Temp_Drive[3]=0x55;
		Temp_Drive[4]=0x7A;
		Temp_Drive[5]=inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;
		Big_End_WordtoByte(Driver_DataLen,Temp_Drive+6);
		Temp_Drive[8]=0x00;
		Temp_Drive[9]=CrcXOR(Temp_Drive+3,6);//����
		
        Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_servn,0x0700,Temp_Drive,10,0,0,0);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: hy_dring_record_data_inquire_proc
 *  Description:  ����ƽ̨��ѯ��ʻ��¼����
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
 	/****����ѯ�����Ƿ�У��Ϸ�****/
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
            eat_trace("dring_record_data_inquire_proc -->TCP send-������ϢУ��ʧ�ܡ�");
            #endif
            Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_servn,0x8700,envlope_data,8,0,0,0);
 		}
 		else
 		{
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->TCP send-��ѯ��ϢУ��ʧ�ܡ�");
            #endif
			Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x01);//ͨ��Ӧ��
 		}
 	}
 	else
 	{
        if(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd > 0x80)
        {
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->������Ϣ����");
            #endif
 			if(((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0x82)&&\
 			    (inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0x84))|| \
 			     ((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0xC2)&&\
 			        (inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0xC4)))
	 		{
				Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x00);//ͨ��Ӧ��
                uart1_send_data_frame(sim800_dring_record_inquire,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
	 		}
	 		else
	 		{
	 			Tcp_VDR_Common_Replay(L_socket,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].inquire_servn_num,0x8701,0x01);//ͨ��Ӧ��
	 		}

        }
        else
        {
            #if dring_record_debug_printf 
            eat_trace("dring_record_data_inquire_proc -->TCP send-��ѯ��ϢУ��ʧ�ܡ�");
            #endif
            if((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd>=0x00)&&(inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd<=0x15))
            {
               /****************�ɼ�ָ������ʻ�ٶȼ�¼ ��λMIN***************/
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
                    eat_trace("SSI_FS_debug���ļ����Ͳɼ����");
                    uart1_send_data_frame(sim800_dring_record_inquire,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff,inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_len);
                    return;
                }
                start_time = bcd_timer_to_sec((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+6));//��ʼ��
                end_time = bcd_timer_to_sec((inquire_dring_record.dring_recod_arry[inquire_dring_record.head].data_buff+12));//��ֹ��

                #if dring_record_debug_printf 
                eat_trace("dring_record_data_inquire_proc -->��װҪ���͵����ݡ�");//dbc�����߳�
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
  *  Description:  ������ʻ��¼����
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
            eat_trace("inquire_dring_record_queue_proc --> ������Ϣ��user3 ������Ϣ��װ����");//dbc�����߳�
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_3,driver_record_data_proc);
            inquire_dring_record.inquire_queue_state = EAT_TRUE;
        }
        else
        {
            #if dring_record_debug_printf 
            eat_trace("inquire_dring_record_queue_proc --> ����Ϊ�ա���");//dbc�����߳�
            #endif
        }
    }
}


/*********************************��ʻ��¼�ɼ�������к���****************************************/
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_init
  *  Description:  ��ʼ��inquire_dring_record ���� ��ʻ��¼���ݲ�ѯ����
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
  *  Description:  ������ʻ��¼���ݲ�ѯ����
  *  Parameters :dr_cmd:��ʽ��¼���servn_num:��ˮ��
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
void enter_inquire_dring_record_queue(uint8_t dr_cmd,uint16_t servn_num,uint8_t *data,uint8_t data_len)
 {
    uint8_t Tail = 0;
    Tail = (inquire_dring_record.tail+1)%(inquire_dring_record.queuesize); 
    if (Tail == inquire_dring_record.head) //��ʱ����û�пռ�
    {
		#if dring_record_debug_printf 
		eat_trace("enter_inquire_dring_record_queue --> ������.");
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
		eat_trace("enter_inquire_dring_record_queue --> ������head:%d,tail:%d.",inquire_dring_record.head,inquire_dring_record.tail);
		#endif
    }
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_Driver_Data_Trans
 *  Description:  ��ʻ��¼���ݵ���
 *  Parameters :  
 *				  *Pbuffer ��Դ����Pubffer
 *                *Lens    ���ݳ���
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
	
	*(temp_buffer+offest) = inquire_dring_record.dring_recod_arry[inquire_dring_record.head].dring_record_cmd;//������
	offest+=sizeof(uint8_t);
	
	memcpy(temp_buffer+offest,buffer,Lens);//ת����
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
*  Description:  ������ʻ��¼���ݲ�ѯ����
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
  *  Description:  ������ʻ��¼���ݲ�ѯ����
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
         eat_trace("exit_inquire_dring_record_queue --> ����Ϊ��.");
         #endif
         
     }
     else
     {
         inquire_dring_record.inquire_queue_state=EAT_FALSE;
         inquire_dring_record.head = (inquire_dring_record.head+1) % (inquire_dring_record.queuesize);
     }
     #if dring_record_debug_printf 
     eat_trace("exit_inquire_dring_record_queue --> ������head:%d,tail:%d.",inquire_dring_record.head,inquire_dring_record.tail);
     #endif
 }
 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: is_inquire_dring_record_queue_empty
   *  Description:  ��ʻ��¼��ѯ�����Ƿ�Ϊ��
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
          eat_trace("is_at_cmd_send_queue_empty --> ����Ϊ��.");
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
   *  Description:  ��ʻ��¼��ѯ�����Ƿ�Ϊ��
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


