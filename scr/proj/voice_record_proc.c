
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: voice_record_proc.c
 *  Description: 录音处理函数。
 *        
 *       author: J.x.b  create at 2014-5-12
 * =====================================================================================
*/
#include "voice_record_proc.h"

 /*录音命令 首字节为长度信息*/
uint8_t at_cmd_record_fram[29]={0};
/* 录音停止指令*/
const uint8_t voice_record_stop[] = "AT+CREC=2\r\n";
/* 录音查询指令*/
const char voice_record_check[] = "+CREC: ";

/*多媒体属性结构体*/
multimedia_add_struct multimedia_voice;
uint16_t voice_Name[22]=L"D:\\REC\\6.WAV"; //当前声音名称
uint16_t voice_Name_add[22]=L"D:\\REC\\6.bin"; //录音信息附加信息

/*录音信息总数量*/
uint32_t voic_num_total=0;

/*录音信息总附加信息*/
media_add_struct voice_record_sys_add;

/*录音信息总附加信息*/

 /* 
 * ===  FUNCTION  ======================================================================
 *  Name       : EAT_Start_Record_Fram_Creat
 *  Description: 开始记录的录音设置
 *  Parameters : 
 *               record_timer:录音时间 0:表示一直录音
 *				 Quation : 0 8K
 *                         1 11K
 *						   2 23K
 *						   3 32K
 *       author: J.x.b  create at 2014-7-16
 * =====================================================================================
 */
static void record_fram_creat(uint16_t record_timer,uint8_t quation)
{  
    #if voice_record_debug_printf
    eat_trace("record_fram_creat-->record_timer:%d,quation:%d!!",record_timer,quation);
    #endif
	sprintf(at_cmd_record_fram,"AT+CREC=1,6,1,%u,1,%u,0\r\n",record_timer,quation);
}
 /* 
  * ===  FUNCTION  ======================================================================
  *  Name       : voice_record_request
  *  Description: 获取录音状态  
  *  Parameters : void
  *       author: J.x.b  create at 2014-5-28
  * =====================================================================================
  */
 void voice_record_request(void)
 {
     #if voice_record_debug_printf
     eat_trace("voice_record_request-->record voice send!");
     #endif
     
     System_Flag.System_Flag_Bit.record_voice=EAT_TRUE;
     eat_modem_write(at_cmd_record_fram, strlen(at_cmd_record_fram));
     /**系统处于录音状态**/
 }
  /* 
  * ===  FUNCTION  ======================================================================
  *  Name       : voice_record_stop_proc
  *  Description: 录音停止 
  *  Parameters : void
  *       author: J.x.b  create at 2014-5-28
  * =====================================================================================
  */
 static void voice_record_stop_proc(void)
 {
     #if voice_record_debug_printf
     eat_trace("voice_record_stop-->record voice stop!!");
     #endif
     
     eat_modem_write(voice_record_stop, strlen(voice_record_stop));
     /**系统处于录音状态**/
 }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: voice_record_system_add_init
 *  Description: 录音系统初始化
 *  Parameters :  Data    void
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
 * =====================================================================================
 */ 
 void voice_record_system_add_init(void)
 {
     int32_t file_read_result;
     multimedia_add_struct temp_add;
     #if voice_record_debug_printf 
     eat_trace("voice_record_system_add_init -->> creat rec fold !!!.");
     #endif
     fs_Create_file_flod(voice_rec_fold);
     #if voice_record_debug_printf 
     eat_trace("voice_record_system_add_init -->> voice_record_add_sys !!!.");
     #endif
     file_create(voice_record_add_sys);
     file_read_result=file_read(voice_record_add_sys,0,&voic_num_total,sizeof(voic_num_total));
     if(file_read_result==OPERATION_NO_ERROR)
     {
        file_read(voice_record_add_sys,(voic_num_total-1)*sizeof(temp_add)+sizeof(temp_add),&temp_add,sizeof(temp_add));
        multimedia_voice.media_chanl_id = voic_num_total;
        #if voice_record_debug_printf 
        eat_trace("voice_record_system_add_init -->> sucess voic_num_total:%d !!!.",voic_num_total);
        #endif
     }
     else
     {
         voic_num_total=0;
         multimedia_voice.media_chanl_id = voic_num_total;
         #if voice_record_debug_printf 
         eat_trace("voice_record_system_add_init -->>error voic_num_total: 0 !!!.");
         #endif
     }
 }

/* 
* ===  FUNCTION  ======================================================================
*  Name: voice_record_system_add_infor
*  Description: 录音系统附加 信息
*  Parameters :  Data    void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void voice_record_delet(uint32_t media_id)
{
    uint32_t ii=0;
    for(ii=0;ii<voic_num_total;ii++)
    {
        file_read(voice_record_add_sys,ii*sizeof(voice_record_sys_add)+sizeof(voic_num_total),&voice_record_sys_add,sizeof(voice_record_sys_add));
        if(media_id !=voice_record_sys_add.media_id)
        {
            file_write(voice_record_add_sys,ii*sizeof(voice_record_sys_add)+sizeof(voic_num_total),&voice_record_sys_add,sizeof(voice_record_sys_add));
        }
    }
    voic_num_total-=1;
    file_write(voice_record_add_sys,0,&voic_num_total,sizeof(voic_num_total));
    voice_record_rename(media_id);
    voice_record_add_rename(media_id);
    eat_fs_Delete(voice_Name);
    eat_fs_Delete(voice_Name_add);
    #if voice_record_debug_printf 
    eat_trace("voice_record_delet -->>delet data !!!.");
    #endif
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: voice_record_rename
*  Description: 修改录音名称
*  Parameters :  voice_id :录音名称
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void voice_record_rename(uint32_t voice_id)
{
   uint8_t str[10]={0};
   char data_len=0,ii=0;
   /********1 数字转换为字符串*******/
   #if voice_record_debug_printf 
   eat_trace("voice_record_rename -->> int to ascall start!");
   #endif
   data_len=itoa(voice_id,str);
   #if voice_record_debug_printf 
   eat_trace("voice_record_rename -->> itoa :%s,data_len:%d.",str,data_len);
   #endif
   for(ii=0;ii<14;ii++)
   {
       *(voice_Name+7+ii)=0x00; 
   }
   for(ii=0;ii<data_len;ii++)
   {
      *(voice_Name+7+ii)=(uint16_t)str[ii];
   }
   *(voice_Name+7+ii) =(uint16_t)'.';
   *(voice_Name+8+ii) = (uint16_t)'W';
   *(voice_Name+9+ii) = (uint16_t)'A';
   *(voice_Name+10+ii) = (uint16_t)'V';
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: voice_record_add_rename
*  Description: 修改录音 附件信息
*  Parameters :  voice_id :录音名称
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void voice_record_add_rename(uint32_t voice_id)
{
   uint8_t str[10]={0};
   char data_len=0,ii=0;
   /********1 数字转换为字符串*******/
   data_len=itoa(voice_id,str);
   #if voice_record_debug_printf 
   eat_trace("voice_record_rename -->> itoa :%s.",str);
   #endif
   for(ii=0;ii<15;ii++)
   {
       *(voice_Name_add+7+ii)=0x00; 
   }
   for(ii=0;ii<data_len;ii++)
   {
      *(voice_Name_add+7+ii)=(uint16_t)str[ii];
   }
   *(voice_Name_add+7+ii) = (uint16_t)'.';
   *(voice_Name_add+8+ii) = (uint16_t)'b';
   *(voice_Name_add+9+ii) = (uint16_t)'i';
   *(voice_Name_add+10+ii) = (uint16_t)'n';
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: voice_record_over_proc
*  Description: 修改录音 附件信息
*  Parameters :  收到录音结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void voice_record_over_proc(void)
{
   /***1 重命名录音文件***/
   voice_record_rename(multimedia_voice.media_id);
   eat_fs_Rename(L"D:\\REC\\6.WAV",voice_Name);//重命名   
   #if voice_record_debug_printf 
   eat_trace("voice_record_over_proc -->> rename voice file !!!.");
   #endif
   
   /***2 重命名录音文件附加信息***/
   voice_record_add_rename(multimedia_voice.media_id);
   file_create(voice_Name_add);
   file_write(voice_Name_add,0,&multimedia_voice,sizeof(multimedia_add_struct));
   #if voice_record_debug_printf 
   eat_trace("voice_record_over_proc -->> creat signal voice add file !!!.");
   #endif

   /***3 更新系统附加信息***/
   file_write(voice_record_add_sys,voic_num_total*sizeof(voice_record_sys_add)+sizeof(voic_num_total),&voice_record_sys_add,sizeof(voice_record_sys_add));
   voic_num_total++;
   file_write(voice_record_add_sys,0,&voic_num_total,sizeof(voic_num_total));
   
   #if voice_record_debug_printf 
   eat_trace("voice_record_over_proc -->> voic_num_total:%d  !!!.",voic_num_total);
   #endif

   if(multimedia_voice.process_type==0x00)
   {
      #if voice_record_debug_printf 
      eat_trace("voice_record_over_proc -->> 录音进上传链表!!!.");
      #endif
      media_anaylsis_list_add(&media_send_list,multimedia_voice.media_type,multimedia_voice.media_id);
      #if voice_record_debug_printf 
      eat_trace("voice_record_over_proc -->> 发送上传录音信息消息.");
      #endif
      task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
   }
   
}
/* 
* ===  FUNCTION  ======================================================================
*         Name: voice_record_trigged
*  Description: 录音触发
*        Input: media_event :事件类型   触发类型 0：平台下发指令；1：定时动作；2：抢劫报警触发；3：碰撞侧翻报警触发；4：门开拍照；5：门关拍照；6：车门由开变关，时速从＜20 公里到超过 20 公里；7：定距拍照    
*               media_chanl_id:通道ID
*               process_type:处理类型 0：实时上传；1：保存
*       Output:
*       Return:  uint8_t 0x01 录音忙 0X00 录音开始             
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
uint8_t voice_record_trigged(uint8_t media_event,uint16_t record_timer,uint8_t quation,uint8_t process_type)
{
    BaseLocationSave_Struct Location_Inf_Data;//发发送缓存数据
    if(System_Flag.System_Flag_Bit.record_voice == EAT_TRUE)
    {
        /***录音进行中 录音 拒绝再次录音***/
        #if voice_record_debug_printf 
        eat_trace("voice_record_trigged -->> voice record is recording please waitor !!!.");
        #endif
        return 0x01;
    }
    else
    {
        /***1\单条录音信息附件信息****/
        multimedia_voice.media_id=voic_num_total+1; 
        multimedia_voice.media_type = 0x01;
        multimedia_voice.media_format = 0x03;
        multimedia_voice.media_event = media_event;
        multimedia_voice.media_chanl_id = 0x01;
        multimedia_voice.process_type = process_type;
        /**当前位置信息**/
    	Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
        memcpy(multimedia_voice.media_location,Location_Inf_Data.Location_Inf,28);
        
        /***2 system录音信息附件信息****/
        voice_record_sys_add.media_id=multimedia_voice.media_id;
        voice_record_sys_add.chanel_id=multimedia_voice.media_chanl_id;
        voice_record_sys_add.media_event=multimedia_voice.media_event;
        voice_record_sys_add.media_id = multimedia_voice.media_id;
        /**将时间转换为S储存**/
        voice_record_sys_add.media_time=bcd_timer_to_sec(Real_Time_Location_Inf.Curren_Time);

        record_fram_creat(record_timer, quation);
        /***3 system开始录音****/
        voice_record_request();
    }
}
/* 
* ===  FUNCTION  ======================================================================
*         Name: strstr_voice_record_cmd_cb_proc
*  Description: 抓取录音状态
*       Output:
*       Return:  uint8_t 0x01 录音忙 0X00 录音开始             
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void strstr_voice_record_cmd_cb_proc(uint8_t *p_str)
{
     char_t *pHead = NULL;
     uint8_t tempH,tempL;
     uint8_t voice_record=0;
     
     if(record_test_flag == EAT_TRUE) 
     {
        loca_record_test(p_str);
     }
     if(System_Flag.System_Flag_Bit.record_voice==EAT_TRUE)
     {
         pHead=(char *)strstrLast(p_str,voice_record_check);
         if(pHead==NULL)
         {
              #if voice_record_debug_printf 
              eat_trace("strstr_voice_record_cmd_cb_proc --> no data record !.");
              #endif
              return;
         }
         else
         {
             #if voice_record_debug_printf 
             eat_trace("strstr_voice_record_cmd_cb_proc --> 查找到录音想关信息!.");
             #endif
             tempL=*(unsigned char *)pHead;      
             voice_record=tempL-0x30;
             switch(voice_record)
             {
                case 0:
                     #if voice_record_debug_printf 
                     eat_trace("strstr_voice_record_cmd_cb_proc --> case 0: 录音空闲。 !.");
                     #endif
                     break;
                case 1:
                     #if voice_record_debug_printf 
                     eat_trace("strstr_voice_record_cmd_cb_proc --> case 1:正在录音。 !.");
                     #endif
                     break;
                case 2:
                     #if voice_record_debug_printf 
                     eat_trace("strstr_voice_record_cmd_cb_proc --> case 2:录音结束。 !.");
                     #endif
                     voice_record_over_proc();
                     System_Flag.System_Flag_Bit.record_voice=EAT_FALSE;
                     break;
                case 3:
                
                     break;
                default:
                    break;
             }
         }
      }
}

 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: TCP_Record_Server
 *  Description:   录音信息解析，动作函数澹(接收服务器信息)
 *  Parameters :   Data_Buff     :完整的数据帧
 *				   Len  :原始数据指针
 *				   Servn:函数流水号
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-21 			 
 * =====================================================================================
 */
 void tcp_voice_record_proc(char socket,uint8_t *tcp_buffer,uint16_t msg_servn)
 {
    uint16_t voice_record_time;//录音时间
    uint8_t voice_quation;//录音质量4个等级，由服务器下发
    if(*tcp_buffer==0x00)// 停止录音
    {
        #if voice_record_debug_printf 
        eat_trace("tcp_voice_record_proc -- 录音结束 tcp !.");
		#endif
        voice_record_stop_proc();
        Tcp_VDR_Common_Replay(socket,msg_servn,tcp_voice_record,0x00);
    }
    else
    {
        if(System_Flag.System_Flag_Bit.record_voice == EAT_TRUE)
        {
            #if voice_record_debug_printf 
            eat_trace("tcp_voice_record_proc -- 当前状态为录音 !.");
            #endif
            Tcp_VDR_Common_Replay(socket,msg_servn,tcp_voice_record,0x01);
        }
        else
        {
            #ifdef voice_record_debug_printf
            eat_trace("tcp_voice_record_proc -- tcp 录音开始 !.");
            #endif
            Big_End_BytetoWord(tcp_buffer+1,&voice_record_time);//解析出录音时间
            voice_record_trigged(0x00,voice_record_time,*(tcp_buffer+4),*(tcp_buffer+3));
            Tcp_VDR_Common_Replay(socket,msg_servn,tcp_voice_record,0x00);
        }
        
    }
	
 }

