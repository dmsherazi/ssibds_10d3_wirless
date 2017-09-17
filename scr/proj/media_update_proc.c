
#include "media_update_proc.h"

struct list_head media_send_list;

/* 
* ===  FUNCTION  ======================================================================
*  Name: media_anaylsis_pro_list_init
*  Description: 多媒体上传处理链表
*  Parameters : void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-8
* =====================================================================================
*/ 
/***多媒体上传链表初始化***/
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
*  Description: 多媒体上传处理链表，删除节点
*  Parameters : media_type: 0x00 :图片 0x01:录音
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
 *  Description:   录音信息上传
 *  Parameters :   
 *	
 *				   D_Len	 :数据长度
 *				   *Data     :数据指针
 *				   PassWay_ID:通道号
 *				   Encoding_Format 录音格式编码；2：MP3；3：WAV；4：WMV；
 *				   Event_Coding  :事件项编码；0：平台下发指令；
 *											  1：定时动作；
 *											  2：抢劫报警触发；
 *											  3：碰撞侧翻报警触发；其他保留
 *				  Package_Flag  :是否分包 
 *				  PackageNum    :分的总包数
 *				  Package_Cnt	:分包序号
 *				   *Data         :数据指针
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
	//附加信息
	uint16_t pic_temp[22]=L"D:\\PIC\\X.JPG";
	static uint16_t temp_media_name[22];//目前正字发送的数据的 名字
	static uint16_t media_add_name[22];//附加信息BUFFER

	uint8_t media_list_ret;//链表处理返回
	uint8_t media_update_buff[980]={0};
	static uint8_t media_upadate_start = EAT_FALSE;//是否忙的标志位
	static uint8_t media_type;//0X00:图片 0X
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
            eat_trace("media_update_pro--> 上传录音文件 文件名!.");
            #endif
            voice_record_rename(media_id);
            memcpy(temp_media_name,voice_Name,sizeof(temp_media_name));
            
            #if media_updata_debug_printf 
            eat_trace("media_update_pro--> 上传录音文件 附加信息!.");
            #endif
            voice_record_add_rename(media_id);
            memcpy(media_add_name,voice_Name_add,sizeof(media_add_name));
        }
        else
        {
            #if media_updata_debug_printf 
            eat_trace("media_update_pro--> 上传图片文件 !.");
            #endif
            
            picture_rename(media_id,pic_temp);
            memcpy(temp_media_name,pic_temp,sizeof(pic_temp));
            
            picture_add_rename(media_id,pic_temp);
            memcpy(media_add_name,pic_temp,sizeof(pic_temp));
        }
		get_file_size(temp_media_name,&file_size);//获取文件大小
		package_num = (uint16_t)(file_size/media_package_max);
		package_last = (uint16_t)(file_size%media_package_max);
		#if media_updata_debug_printf
		eat_trace("media_update_pro-->File_Size:%u,package_num:%d,package_last:%d",file_size,package_num,package_last);
		#endif
		
		file_read((const uint16_t *)media_add_name,0,&media_add,sizeof(media_add));//读附加信息
		#if media_updata_debug_printf
		eat_trace("media_update_pro-->read media event:%u,formt:%u,chanel_id:%d",media_add.media_event,media_add.media_format,media_add.media_chanl_id);
		#endif
		
		Big_End_DwordtoByte(media_id,media_update_buff+media_offest);
		media_offest += 4;
		
		*(media_update_buff+media_offest)= media_add.media_type;//类型
		media_offest += 1;
		
		*(media_update_buff+media_offest)= media_add.media_format;//格式
		media_offest += 1;
		
		*(media_update_buff+media_offest)= media_add.media_event;//事件
		media_offest += 1;
		
		
		*(media_update_buff+media_offest)= media_add.media_chanl_id;//通道
		media_offest += 1;
		
		memcpy(media_update_buff+media_offest,media_add.media_location,sizeof(media_add.media_location));//位置信息
		media_offest += sizeof(media_add.media_location);
		
		if(((package_num==0)&&(package_last!=0))||((package_num==1)&&(package_last==0)))
		{
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> 数据不分包..");
            #endif
			file_read(temp_media_name,0,media_update_buff+media_offest,(uint16_t)file_size);
            media_offest += file_size;
            Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,0,0,0);
            media_update_timeout_ctrl(EAT_TRUE);
            
            media_offest=0;
			package_num = 0;
			package_last= 0;
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> 数据发送完成!..返回，发送下一个数据，发送数据消息。。");
            #endif
            
            return;
			
		}
		else if((package_num>1)&&(package_last!=0))
		{
			package_num+=1;//包数增加1
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> 分包数据,包数:%d..",package_num);
            #endif
		}
		
        file_read(temp_media_name,0,media_update_buff+media_offest,media_package_max);
        media_offest += media_package_max;
        
        Tcp_Message_PacKage_And_Send(socket_t,&msgs_sern,tcp_media_update,media_update_buff,media_offest,EAT_TRUE,package_num,package_cnt);
        media_update_timeout_ctrl(EAT_TRUE);

        media_offest=0;
        package_cnt++;//包序数增加
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
            eat_trace("media_update_pro--> 最后一包数据发送完成!!!!..");
            #endif
        }
        else
        {
            
            #if media_updata_debug_printf
            eat_trace("media_update_pro--> 总包数:%u,当前包数:%u..",package_num,package_cnt);
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

