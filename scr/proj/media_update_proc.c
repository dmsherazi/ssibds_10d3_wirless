
#include "media_update_proc.h"

struct list_head media_send_list;

/* 
* ===  FUNCTION  ======================================================================
*  Name: media_anaylsis_pro_list_init
*  Description: ��ý���ϴ���������
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
/***��ý���ϴ������ʼ��***/
void media_update_pro_list_init(void)
{
    INIT_LIST_HEAD(&media_send_list);
}
void media_anaylsis_list_add(struct list_head *list_parm,uint8_t media_type,uint16_t media_id)
{
   media_send_data_pack_list *mem_prt;
   if(is_list_enful(list_parm,media_send_list_max) ==EAT_TRUE)
   {
       mem_prt = (media_send_data_pack_list *)list_entry(list_parm->next,media_send_data_pack_list,re_list);
       list_del(list_parm->next);
       eat_mem_free(mem_prt);
   }
   mem_prt =(media_send_data_pack_list *)eat_mem_alloc(sizeof(media_send_data_pack_list));
   mem_prt->media_type = media_type;
   mem_prt->media_id= media_id;
   tail_list_add(&(mem_prt->re_list),list_parm,media_send_list_max);
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: media_pro_list_del(struct list_head *list_parm,uint8_t *media_type,uint16_t *media_id)
*  Description: ��ý���ϴ���������ɾ���ڵ�
*  Parameters : media_type: 0x00 :ͼƬ 0x01:¼��
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
uint8_t media_pro_list_del(struct list_head *list_parm,uint8_t *media_type,uint16_t *media_id)
{
    media_send_data_pack_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (media_send_data_pack_list *)list_entry(list_parm->next,media_send_data_pack_list,re_list);
        *media_type = mem_prt->media_type;
        *media_id = mem_prt->media_id;
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
        return 0x01;
    }
    else
    {
        #if media_updata_debug_printf 
        eat_trace("uart1_anaylsis_pro_list_del--> list is empty.");
        #endif
        
        return 0x00;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: TCP_Recrod_Data_Send
 *  Description:   ¼����Ϣ�ϴ�
 *  Parameters :   
 *	
 *				   D_Len	 :���ݳ���
 *				   *Data     :����ָ��
 *				   PassWay_ID:ͨ����
 *				   Encoding_Format ¼����ʽ���룻2��MP3��3��WAV��4��WMV��
 *				   Event_Coding  :�¼�����룻0��ƽ̨�·�ָ�
 *											  1����ʱ������
 *											  2�����ٱ���������
 *											  3����ײ�෭������������������
 *				  Package_Flag  :�Ƿ�ְ� 
 *				  PackageNum    :�ֵ��ܰ���
 *				  Package_Cnt	:�ְ����
 *				   *Data         :����ָ��
 *  Return     :  void v
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-22 			 
 * =====================================================================================
 */
void media_update_pro(uint16_t socket_t)
{   
	static uint32_t file_size;
	static uint16_t package_num;
	static uint16_t package_cnt = 1;
	static uint16_t package_last;
	//������Ϣ
	uint16_t pic_temp[22]=L"D:\\PIC\\X.JPG";
	static uint16_t temp_media_name[22];//Ŀǰ���ַ��͵����ݵ� ����
	static uint16_t media_add_name[22];//������ϢBUFFER

	uint8_t media_list_ret;//��������
	uint8_t media_update_buff[980]={0};
	static uint8_t media_upadate_start = EAT_FALSE;//�Ƿ�æ�ı�־λ
	static uint8_t media_type;//0X00:ͼƬ 0X
	uint16_t media_id;
	multimedia_add_struct media_add;
	uint16_t media_offest=0;
	uint16_t msgs_sern;
	if(media_upadate_start==EAT_FALSE)
	{
        #if media_updata_debug_printf 
        eat_trace("media_update_pro--> read media id !.");
        #endif
	    if(media_pro_list_del(&media_send_list,&media_type,&media_id)!=EAT_TRUE) return;
	    if(media_type==0x01)
	    {
            #if media_updata_debug_printf 
            eat_trace("media_update_pro--> �ϴ�¼���ļ� �ļ���!.");
            #endif
            voice_record_rename(media_id);
            memcpy(temp_media_name,voice_Name,sizeof(temp_media_name));
            
            #if media_updata_debug_printf 
            eat_trace("media_update_pro--> �ϴ�¼���ļ� ������Ϣ!.");
            #endif
            voice_record_add_rename(media_id);
            memcpy(media_add_name,voice_Name_add,sizeof(media_add_name));
        }
        else
        {
            #if media_updata_debug_printf 
            eat_trace("media_update_pro--> �ϴ�ͼƬ�ļ� !.");
            #endif
            
            picture_rename(media_id,pic_temp);
            memcpy(temp_media_name,pic_temp,sizeof(pic_temp));
            
            picture_add_rename(media_id,pic_temp);
            memcpy(media_add_name,pic_temp,sizeof(pic_temp));
        }
		get_file_size(temp_media_name,&file_size);//��ȡ�ļ���С
		package_num = (uint16_t)(file_size/media_package_max);
		package_last = (uint16_t)(file_size%media_package_max);
		#if media_updata_debug_printf
		eat_trace("media_update_pro-->File_Size:%u,package_num:%d,package_last:%d",file_size,package_num,package_last);
		#endif
		
		file_read((const uint16_t *)media_add_name,0,&media_add,sizeof(media_add));//��������Ϣ
		#if media_updata_debug_printf
		eat_trace("media_update_pro-->read media event:%u,formt:%u,chanel_id:%d",media_add.media_event,media_add.media_format,media_add.media_chanl_id);
		#endif
		
		Big_End_DwordtoByte(media_id,media_update_buff+media_offest);
		media_offest += 4;
		
		*(media_update_buff+media_offest)= media_add.media_type;//����
		media_offest += 1;
		
		*(media_update_buff+media_offest)= media_add.media_format;//��ʽ
		media_offest += 1;
		
		*(media_update_buff+media_offest)= media_add.media_event;//�¼�
		media_offest += 1;
		
		
		*(media_update_buff+media_offest)= media_add.media_chanl_id;//ͨ��
		media_offest += 1;
		
		memcpy(media_update_buff+media_offest,media_add.media_location,sizeof(media_add.media_location));//λ����Ϣ
		media_offest += sizeof(media_add.media_location);
		
		if(((package_num==0)&&(package_last!=0))||((package_num==1)&&(package_last==0)))
		{
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> ���ݲ��ְ�..");
            #endif
			file_read(temp_media_name,0,media_update_buff+media_offest,(uint16_t)file_size);
            media_offest += file_size;
            Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,0,0,0);
            media_update_timeout_ctrl(EAT_TRUE);
            
            media_offest=0;
			package_num = 0;
			package_last= 0;
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> ���ݷ������!..���أ�������һ�����ݣ�����������Ϣ����");
            #endif
            
            return;
			
		}
		else if((package_num>1)&&(package_last!=0))
		{
			package_num+=1;//��������1
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> �ְ�����,����:%d..",package_num);
            #endif
		}
		
        file_read(temp_media_name,0,media_update_buff+media_offest,media_package_max);
        media_offest += media_package_max;
        
        Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,EAT_TRUE,package_num,package_cnt);
        media_update_timeout_ctrl(EAT_TRUE);

        media_offest=0;
        package_cnt++;//����������
        media_upadate_start = EAT_TRUE;
		return;
	}
	else
	{
        if(package_cnt ==package_num)
        {
            if(package_last==0)
            {
                file_read(temp_media_name,(package_cnt-1)*media_package_max, media_update_buff+media_offest,media_package_max);
                media_offest+=media_package_max;
                
                Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,EAT_TRUE,package_num,package_cnt);
                media_update_timeout_ctrl(EAT_TRUE);
            }
            else
            {
                file_read(temp_media_name,(package_cnt-1)*media_package_max, media_update_buff+media_offest,package_last);
                media_offest += package_last;
                Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,EAT_TRUE,package_num,package_cnt);
                media_update_timeout_ctrl(EAT_TRUE);
            }
            package_cnt=1;
            media_upadate_start = EAT_FALSE;
            
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> ���һ�����ݷ������!!!!..");
            #endif
        }
        else
        {
            
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> �ܰ���:%u,��ǰ����:%u..",package_num,package_cnt);
            #endif
            file_read(temp_media_name,(package_cnt-1)*media_package_max, media_update_buff+media_offest,media_package_max);
            media_offest+=media_package_max;
            Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,EAT_TRUE,package_num,package_cnt);
            media_update_timeout_ctrl(EAT_TRUE);
            
            package_cnt++;
        }
        media_offest=0;
     }
}

