
#include "TTS_sound_pro.h"

/*�ı���Ϣ��¼*/
txt_record_struct txt_record;

uint16_t navigation_file[24]=L"D:\\txt_record\\D000.txt";
uint16_t can_error_file[24]=L"D:\\txt_record\\E000.txt";

/*�ı���Ϣ��¼*/
uint8_t navigation_serial_number = 0;
uint8_t can_error_serial_number = 0;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: read_txt_data_record
 *  Description: ��״̬
 *        input: ��ʼ��txt��¼
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
void read_txt_data_record(void)
{
    
    /*�����ı��ļ���*/
    fs_Create_file_flod(txt_record_file);
    /*�����ı����ļ�����*/
    file_create(txt_data_record);
    /*��ȡ�ļ�����*/
    file_read(txt_data_record,0,&txt_record,sizeof(txt_record));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: txt_data_record_init
 *  Description: txt
 *        input: ��ʼ��txt��¼
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
void txt_data_record_init(void)
{
    
    if(system_param.param_save.statue.wether_first_power_on==EAT_FALSE)
    {
        /*�����ı��ļ���*/
        fs_Create_file_flod(txt_record_file);
        /*�����ı����ļ�����*/
        file_create(txt_data_record);
        memset(&txt_record,0,sizeof(txt_record));
        file_write(txt_data_record,0,&txt_record,sizeof(txt_record));
    }
    else
    {
        read_txt_data_record();
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: tts_tone_envlope_play
 *  Description: ��װ����������
 *        input: scr_tone:��Ҫ�����ı�����ָ��
 *       author: Jumping create at 2015-12-16
 * =====================================================================================
 */
void tts_tone_envlope_play(uint8_t *scr_tone)
{
    uint16_t tone_len=0;
    uint8_t tone_scr[1024]={0};
    tone_len = (uint16_t)sprintf(tone_scr,"AT+CTTS=2,\"%s\"\r\n",scr_tone);
    eat_modem_write(tone_scr,tone_len);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  void clean_tts_odanoa(uint8_t *scr,uint16_t *len) 
 *  Description:���TTS��Ϣ�е�od oa 
 *  Input:   speed 
 *  Output: 1
 *  author: J create at 2015-4-24
 * =====================================================================================
 */
 static void clean_tts_odanoa(uint8_t *scr,uint16_t *len)
 {
 	uint16_t cnt1 = 0;
 	uint16_t cnt = 0;
 	for(cnt=0;cnt<*len;cnt++)
 	{
 		if((*(scr+cnt)!=0x0A)&&(*(scr+cnt)!=0x0D))
 		{
 			*(scr+cnt1)=*(scr+cnt);
 			 cnt1++;
 		}
 	}
 	*len=cnt1;
 }

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Tcp_Register
 *  Description:  ������յ���ͨ��Ӧ��
 *  Parameters : Temp_data_buff ;���յ�����Ч����ָ��
 *             :  tempLens ����
 *			   :  Sn ��ˮ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void remote_txt_data_proc(uint8_t *remote_point,uint16_t data_len)
{
	uint16_t scr_data_len=0;
	remote_txt_data txt_data;
	uint8_t rev_tts_data[1024]={0};
	txt_data.txt_rev = *remote_point;
    /*�Ƿ���������*/
    if(txt_data.txt_bit.tone_play==0x01)
    {
        #if tts_tone_debug_printf >0
		eat_trace("remote_txt_data_proc --> �յ�����������Ϣ!");
        #endif 
        eat_modem_write("AT+CTTS=0\r\n",strlen("AT+CTTS=0\r\n"));//����ƽ̨�·�������������
        if(txt_data.txt_bit.urgent==0x00)
        {
		    memcpy(rev_tts_data,remote_point+1,data_len-1);
            scr_data_len=data_len-1;
            clean_tts_odanoa(rev_tts_data,&scr_data_len);
        }
        else
        {
            memcpy(rev_tts_data,"����",strlen("����"));
		    memcpy(rev_tts_data+strlen("����"),remote_point+1,data_len-1);
            scr_data_len = data_len-1+strlen("����");
            clean_tts_odanoa(rev_tts_data,&scr_data_len);
        }
        tts_tone_envlope_play(rev_tts_data);
    }
    
    /*���ĵ�����Ϣ*/
    if(txt_data.txt_bit.navigation_can_error==0x00)
    {
        #if tts_tone_debug_printf
		eat_trace("remote_txt_data_proc --> ���ĵ�����Ϣ!!");
        #endif 
        
        /*��¼������Ϣ*/
        if(txt_record.navigation_num == 50)
        {
            if(navigation_serial_number==50) navigation_serial_number=0;
            navigation_serial_number +=1;
            memcpy(&txt_record.navig_time[navigation_serial_number-1][0],Real_Time_Location_Inf.Curren_Time,6);
        }
        else
        {
            txt_record.navigation_num += 1;
            memcpy(&txt_record.navig_time[txt_record.navigation_num-1][0],Real_Time_Location_Inf.Curren_Time,6);
        }
        file_write(txt_data_record,0,&txt_record,sizeof(txt_record));

        /*������Ϣ��Ϣ*/
        if(txt_record.navigation_num == 50)
        {
            int8_change_uint16_ascall(navigation_serial_number,navigation_file+15);
            eat_fs_Delete(navigation_file);
        }
        else
        {
            #if tts_tone_debug_printf
            eat_trace("remote_txt_data_proc --> �޸�����:num:%d:%d,%d,%d",txt_record.navigation_num,*(navigation_file+15),*(navigation_file+16),*(navigation_file+17));
            #endif 
            int8_change_uint16_ascall(txt_record.navigation_num,navigation_file+15);
        }
        #if tts_tone_debug_printf
		eat_trace("remote_txt_data_proc --> �޸�����:num:%d:%d,%d,%d",txt_record.navigation_num,*(navigation_file+15),*(navigation_file+16),*(navigation_file+17));
        #endif 
        eat_fs_Delete(navigation_file);
        file_create((const uint16_t*)navigation_file);
        /*�����Ϣ*/
        if(txt_data.txt_bit.urgent==0x00)
        {
             file_write(navigation_file,0,remote_point+1,data_len-1);
        }
        else
        {
            file_write(navigation_file,0,"����",strlen("����"));
            file_write(navigation_file,strlen("����"),remote_point+1,data_len-1);
        }
    }
    else/*can������Ϣ*/
    {
        #if tts_tone_debug_printf >0
        eat_trace("remote_txt_data_proc --> CAN������Ϣ!!");
        #endif 
        if(txt_record.navigation_num == 50)
        {
            if(can_error_serial_number==50) can_error_serial_number=0;
            can_error_serial_number += 1;
            memcpy(&txt_record.can_time[can_error_serial_number-1][0],Real_Time_Location_Inf.Curren_Time,6);
        }
        else
        {
            txt_record.can_error_num+= 1;
            memcpy(&txt_record.can_time[txt_record.can_error_num-1][0],Real_Time_Location_Inf.Curren_Time,6);
        }
        file_write(txt_data_record,0,&txt_record,sizeof(txt_record));
         
        /*������Ϣ��Ϣ*/
        if(txt_record.navigation_num == 50)
        {
            int8_change_uint16_ascall(can_error_serial_number,can_error_file +15);
            eat_fs_Delete(can_error_file);
        }
        else
        {
            int8_change_uint16_ascall(txt_record.can_error_num,can_error_file +15);
        }
        eat_fs_Delete(can_error_file);
        file_create(can_error_file);
        /*�����Ϣ*/
        if(txt_data.txt_bit.urgent==0x00)
        {
             file_write(can_error_file,0,remote_point+1,data_len-1);
        }
        else
        {
            file_write(can_error_file,0,"����",strlen("����"));
            file_write(can_error_file,strlen("����"),remote_point+1,data_len-1);
        }
    }
    /*��ʾ�ı�*/
	if(txt_data.txt_bit.displaly==0x01)
	{
        #if tts_tone_debug_printf >0
        eat_trace("remote_txt_data_proc --> �յ��ն���ʾ�ı�!!");
        #endif 
	}
    /*�������ʾ�ı�*/
	if(txt_data.txt_bit.advert_display==0x01)
	{
        #if tts_tone_debug_printf >0
        eat_trace("remote_txt_data_proc --> �����Ļ��ʾ�ı�!!");
        #endif 
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  ��ѯTXT���ݸ���
 *  Parameters : Temp_data_buff ;���յ�����Ч����ָ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void txt_data_cnt_inquire(uint8_t cmd_data)
{
    uint8_t memcp_cnt = 0;
 	uint8_t txt_inquire_ret[320]={0};
 	txt_inquire_ret[0] = cmd_data;
    switch(cmd_data)
    {
        /*���ĵ�����Ϣ����*/
        case 0x01:
             txt_inquire_ret[1] = txt_record.navigation_num;
             for(memcp_cnt=0;memcp_cnt<txt_record.navigation_num;memcp_cnt++)
             {
                 memcpy(txt_inquire_ret+2+memcp_cnt*6,&(txt_record.navig_time[memcp_cnt][0]),6);
             }
             uart1_send_data_frame(send_txt_cnt_inquire,txt_inquire_ret,txt_record.navigation_num*6+2);
             #if tts_tone_debug_printf
             eat_trace("txt_data_cnt_inquire --> ������Ϣ����:%d !!",txt_record.navigation_num);
             #endif 
             break;
        /*can ������Ϣ*/
        case 0x02:
             txt_inquire_ret[1] = txt_record.can_error_num;
             for(memcp_cnt=0;memcp_cnt<txt_record.can_error_num;memcp_cnt++)
             {
                 memcpy(txt_inquire_ret+2+memcp_cnt*6,&(txt_record.can_time[memcp_cnt][0]),6);
             }
             uart1_send_data_frame(send_txt_cnt_inquire,txt_inquire_ret,txt_record.can_error_num*6+2);
             #if tts_tone_debug_printf
             eat_trace("txt_data_cnt_inquire --> ������Ϣ����:%d !!",txt_record.can_error_num);
             #endif 
             break;
        default:
            break;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: txt_data_cnt_inquire
 *  Description:  ��ѯTXT��������
 *  Parameters :  cmd data :0x01��������0x02:CAN���ϡ�TXT_nuM���ݱ��
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-16 			 
 * =====================================================================================
 */
void txt_data_infor_inquire(uint8_t cmd_data,uint8_t txt_num)
{
    uint32_t data_len = 0;
 	uint8_t txt_inquire_ret[1024]={0};
 	txt_inquire_ret[0] = cmd_data;
    switch(cmd_data)
    {
        /*���ĵ�����Ϣ����*/
        case 0x01:
             if(txt_num>txt_record.navigation_num)/*��Ϣ����Ϊ����*/
             {
                Big_End_WordtoByte(0,txt_inquire_ret+1);
                data_len = 0;
             }
             else
             {
                 int8_change_uint16_ascall(txt_num,navigation_file+15);
                 get_file_size(navigation_file,&data_len);
                 Big_End_WordtoByte(data_len,txt_inquire_ret+1);
                 file_read(navigation_file,0,txt_inquire_ret+3,data_len);
             }
             break;
        /*can ������Ϣ*/
        case 0x02:
             if(txt_num>txt_record.can_error_num)/*��Ϣ����Ϊ����*/
             {
                Big_End_WordtoByte(0,txt_inquire_ret+1);
                data_len = 0;
             }
             else
             {
                 int8_change_uint16_ascall(txt_num,can_error_file+15);
                 get_file_size(can_error_file,&data_len);
                 Big_End_WordtoByte(data_len,txt_inquire_ret+1);
                 file_read(can_error_file,0,txt_inquire_ret+3,data_len);
             }
             break;
        default:
            break;
    }
    uart1_send_data_frame(send_txt_infor_inquire,txt_inquire_ret,data_len + 3);
    #if tts_tone_debug_printf
    eat_trace("txt_data_infor_inquire --> ������Ϣ����:%s !!",txt_inquire_ret+3);
    #endif 
}

