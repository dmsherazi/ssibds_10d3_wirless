

#include "update_system.h"

uint32_t APP_DATA_RUN_BASE=0;     //app运行地址
uint32_t APP_DATA_STORAGE_BASE=0;     //app 升级程序保存地址
uint8_t  updata_file[307200]={0};
uint16_t board_update_servn =0;
uint16_t update_size=0;
static uint8_t updata_system_flag=EAT_FALSE;
static uint32_t updata_app_size=0;//升级文件大小； 

void updat_result_ret(uint8_t data)//升级回复
{
   uart1_send_data_frame(updata_sim800_ref,&data,1);
}
void sim800_upadat_sever(uint8_t *data_buffer)
{
	uint16_t temp_Len=0;
	static uint32_t updat_offest=0;//储存APP;
    static uint8_t first_int=EAT_FALSE;
	if(first_int==EAT_FALSE)
	{	
	    updat_offest=0;
		Big_End_BytetoDword(data_buffer,&updata_app_size);
		first_int=EAT_TRUE;
	}
	Big_End_BytetoWord(data_buffer+4,&temp_Len);
	memcpy(updata_file+updat_offest,data_buffer+6,temp_Len);
	updat_offest+=temp_Len;
	if(updat_offest == updata_app_size)
	{
        #if APP_TRACE_DBG 
        eat_trace("updat_data_receive data ok --> !!!.");
        #endif
        updata_system_flag=EAT_TRUE;
		//updat_result_ret(0x00);
    }
	else
	{
		updat_result_ret(0x00);
        #if APP_TRACE_DBG 
        eat_trace("updat_result_ret --> ok.");
        #endif
	}
}
static void system_updata_proc(void)
{
    uint8_t *addr=NULL;
    uint32_t app_space_value=0;//app空间值
    updata_system_flag=EAT_FALSE;
    APP_DATA_RUN_BASE=eat_get_app_base_addr(); //升级其实地址
    
    app_space_value=eat_get_app_space();//获取APP大小
    APP_DATA_STORAGE_BASE=APP_DATA_RUN_BASE+(app_space_value>>1);//储存位置
            
    addr=(unsigned char *)APP_DATA_STORAGE_BASE;
    
    eat_flash_erase(addr,updata_app_size);
                    
    eat_flash_write(addr,updata_file,updata_app_size);
            
    eat_update_app((void*)(APP_DATA_RUN_BASE), (void*)(APP_DATA_STORAGE_BASE), updata_app_size , EAT_PIN_NUM, EAT_PIN_NUM,EAT_FALSE);
}

void updata_time_out(void)
{
    static uint8_t time_cnt=0;
    if(updata_system_flag==EAT_TRUE)
    {  
        time_cnt++;
        if(time_cnt==2)
        {
            time_cnt=0;
            system_updata_proc();
        }
    }
}

void remote_sim800_updata(int16_t PackageNum,uint16_t Package_Cnt,uint8_t *Up_Data_BUF,uint16_t Data_Len,u16 Sn)
{
    
    uint8_t *addr=NULL;
    uint32_t app_space_value=0;//app空间值
    static uint32_t file_offest=0;
    static uint32_t FILE_SIZE=0;
    static uint32_t local_file_size=0;
    static uint8_t attribute=0;
    board_update_servn = Sn;
    if(Package_Cnt==1)
    {
        attribute = *Up_Data_BUF;
        Big_End_BytetoDword(Up_Data_BUF+7+*(Up_Data_BUF+6),&FILE_SIZE);
        if(attribute==0x01)
        {
            memcpy(updata_file,Up_Data_BUF+7+*(Up_Data_BUF+6),4);
            Big_End_WordtoByte(Data_Len-11-*(Up_Data_BUF+6),updata_file+4);
            Big_End_WordtoByte(PackageNum,updata_file+6);
            Big_End_WordtoByte(Package_Cnt,updata_file+8);
            memcpy(updata_file+10,Up_Data_BUF+11+*(Up_Data_BUF+6),Data_Len-11-*(Up_Data_BUF+6));
            #if updata_orgin_printf
            eat_trace("updat_data_receive data  --> orgin1!!!.");
            HexToAsicc(updata_file+10,Data_Len-11-*(Up_Data_BUF+6));
            #endif
            uart1_send_data_frame(board_update_id,updata_file,Data_Len-1-*(Up_Data_BUF+6));
            update_size=Data_Len-1-*(Up_Data_BUF+6);
            update_board_timeout_cb(EAT_TRUE,5);
        }
        else if(attribute==0x02)
        {
            file_offest = 0 ;
            memcpy(updata_file + file_offest,Up_Data_BUF+11+*(Up_Data_BUF+6),Data_Len);//
            file_offest += Data_Len-(11+*(Up_Data_BUF+6));
            Tcp_VDR_Common_Replay(Main_Link.mode_Socket,Sn,remoto_update_processor,0x00);
            uart1_send_queue_proc();
        }
        else
        {
            
        }
    }
    else
    {
    	if(attribute==0x01)
    	{
    	    Big_End_DwordtoByte(FILE_SIZE,updata_file);
    	    Big_End_WordtoByte(Data_Len,updata_file+4);
    	    Big_End_WordtoByte(PackageNum,updata_file+6);
    	    Big_End_WordtoByte(Package_Cnt,updata_file+8);
    	    memcpy(updata_file+10,Up_Data_BUF,Data_Len);
            #if updata_orgin_printf
            eat_trace("updat_data_receive data  --> orgin1!!!.");
            HexToAsicc(updata_file+10,Data_Len);
            #endif
            uart1_send_data_frame(board_update_id,updata_file,Data_Len+10);
            update_size=Data_Len+10;
            update_board_timeout_cb(EAT_TRUE,5);
    	}
    	else if(attribute==0x02)
    	{
            if(Package_Cnt==PackageNum)
            {
                memcpy(updata_file + file_offest,Up_Data_BUF,Data_Len);//
                file_offest+= Data_Len;
                Tcp_VDR_Common_Replay(Main_Link.mode_Socket,Sn,remoto_update_processor,0x00);
                if(file_offest==FILE_SIZE)//校验数据是否完整
                {
                    system_param.param_save.statue.wether_system_update = EAT_TRUE;
                    system_para_save_at_once();

                    APP_DATA_RUN_BASE=eat_get_app_base_addr(); //升级其实地址

            		app_space_value=eat_get_app_space();//获取APP大小
            		APP_DATA_STORAGE_BASE=APP_DATA_RUN_BASE+(app_space_value>>1);//储存位置
            		
                    addr=(unsigned char *)APP_DATA_STORAGE_BASE;

            		eat_flash_erase(addr,FILE_SIZE);
                           	
            		eat_flash_write(addr,updata_file,FILE_SIZE);
                    
            		eat_update_app((void*)(APP_DATA_RUN_BASE), (void*)(APP_DATA_STORAGE_BASE), FILE_SIZE , EAT_PIN_NUM, EAT_PIN_NUM, EAT_FALSE);
        		}
        		else
        		{
                    updat_program_reslut_report(0x02,0x01);
                    uart1_send_queue_proc();
        		}
            }
            else
            {
                  memcpy(updata_file + file_offest,Up_Data_BUF,Data_Len);//
                  file_offest += Data_Len;
                  Tcp_VDR_Common_Replay(Main_Link.mode_Socket,Sn,remoto_update_processor,0x00);
            }
    	}
    }
}
void updat_program_reslut_report(uint8_t porit,uint8_t reslut)
{
  uint8_t update_buf[2]={0};
  uint16_t msgs_sern=0;
  update_buf[0]= porit;
  update_buf[1]= reslut;
  Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msgs_sern,0x0108,update_buf,2,0,0,0);
}
#if 0
void ask_package_load(uint16_t SERN,uint16_t Package_Cnt,uint16_t Sn_Start)
{
	static u8 Temp_Data[1024]={0};
	u8 Cnt=0;
	Data_Change(SERN,Temp_Data,Temp_Data+1);
	if((Package_Cnt!=0)&&Sn_Start!=0)
	{
		Temp_Data[2]=(u8)(Package_Cnt-Sn_Start+1);
		for(Cnt=0;Cnt<(Package_Cnt-Sn_Start+1);Cnt++)
		{
			Data_Change(Sn_Start+Cnt,Temp_Data+3+2*Cnt,Temp_Data+4+2*Cnt);
		}
		Tcp_Message_PacKage_And_Send(0x8003,Temp_Data,2*(Package_Cnt-Sn_Start+1)+3,SSI_Packaged_FALSE,0,0);
	}
	else
	{
		Temp_Data[2]=0;
		Updata_OK_Flag=EAT_TRUE;
		Eat_FS_File_extern_write(UPDATA_OK,0,&Updata_OK_Flag,1);
		Tcp_Message_PacKage_And_Send(0x8003,Temp_Data,3,SSI_Packaged_FALSE,0,0);
		HexToAsicc(Temp_Data,3);
	}	
}
#endif
