
#include "take_pictures.h"




/*图片信息附加信息*/
multimedia_add_struct multimedia_picture_pb;
/*图片信息总附加信息*/
media_add_struct pictures_sys_add_pb;

/*图片总数量*/
uint32_t pictures_num_total=0;
/*图片总包数*/
uint8_t get_farm_num = 0;//包数目

/*图片当前包数*/
uint8_t now_farme_num = 0;
/*发送拍照开始帧ms*/
take_pic_time_process rev_take_ret_data_timeout={EAT_FALSE,5,6000};
/*发送拍照开始帧ms*/
take_pic_time_process rev_get_data_timeout={EAT_FALSE,25,1000};

/*发送拍照张数间隔ms*/
take_pictures_attributes take_attributes;
uint32_t data_delay_test=76000;

struct list_head camer_take_list;

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
void take_pictures_pro_list_init(void)
{
    INIT_LIST_HEAD(&camer_take_list);
}
static void take_pictures_list_add(struct list_head *list_parm,multimedia_add_struct *pic_add,media_add_struct *pic_add_sys)
{
   take_pictures_list *mem_prt;
   if(is_list_enful(list_parm,take_pictures_max) ==EAT_TRUE)
   {
       mem_prt = (take_pictures_list *)list_entry(list_parm->next,take_pictures_list,re_list);
       list_del(list_parm->next);
       eat_mem_free(mem_prt);
   }
   mem_prt =(take_pictures_list *)eat_mem_alloc(sizeof(take_pictures_list));
   memcpy(&(mem_prt->pictures_add),pic_add,sizeof(multimedia_add_struct));
   memcpy(&(mem_prt->sys_add_pictures),pic_add_sys,sizeof(media_add_struct));
   #if take_pictures_debug_printf 
   eat_trace("take_pictures_over_proc -->> 链表数据.chinel_id:%d,%d,%d",mem_prt->pictures_add.media_chanl_id,mem_prt->pictures_add.picture_size,mem_prt->pictures_add.picture_qulity);
   eat_trace("take_pictures_over_proc -->> systme:meida_time :%d",mem_prt->sys_add_pictures.media_time);
   eat_trace("take_pictures_over_proc -->>systme:media_id:%d!",mem_prt->sys_add_pictures.media_id);
   #endif
   tail_list_add(&(mem_prt->re_list),list_parm,take_pictures_max);
}
void take_pictures_list_del(struct list_head *list_parm,multimedia_add_struct *pic_add,media_add_struct *pic_add_sys)
{
    take_pictures_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (take_pictures_list *)list_entry(list_parm->next,take_pictures_list,re_list);
        memcpy(pic_add,&(mem_prt->pictures_add),sizeof(multimedia_add_struct)) ;
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_proc -->>orign hex !");
        HexToAsicc(pic_add->media_location,28);
        #endif
        
        memcpy(pic_add_sys,&(mem_prt->sys_add_pictures),sizeof(media_add_struct)) ;
        
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_proc -->>add_system.id:%d !",pic_add_sys->media_id);
        #endif
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_fail_proc--> list is empty.");
        #endif
    }
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
void picture_rename(uint32_t picture_id,uint16_t *picture_Name)
{
   uint8_t str[10]={0};
   char data_len=0,ii=0;
   /********1 数字转换为字符串*******/
   #if voice_record_debug_printf 
   eat_trace("picture_rename -->> int to ascall start!");
   #endif
   data_len=itoa(picture_id,str);
   #if voice_record_debug_printf 
   eat_trace("picture_rename -->> itoa :%s,data_len:%d.",str,data_len);
   #endif
   for(ii=0;ii<14;ii++)
   {
       *(picture_Name+7+ii)=0x00; 
   }
   for(ii=0;ii<data_len;ii++)
   {
      *(picture_Name+7+ii)=(uint16_t)str[ii];
   }
   *(picture_Name+7+ii) =(uint16_t)'.';
   *(picture_Name+8+ii) = (uint16_t)'J';
   *(picture_Name+9+ii) = (uint16_t)'P';
   *(picture_Name+10+ii) = (uint16_t)'G';
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
void picture_add_rename(uint32_t voice_id,uint16_t *picture_Name_add)
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
       *(picture_Name_add+7+ii)=0x00; 
   }
   for(ii=0;ii<data_len;ii++)
   {
      *(picture_Name_add+7+ii)=(uint16_t)str[ii];
   }
   *(picture_Name_add+7+ii) = (uint16_t)'.';
   *(picture_Name_add+8+ii) = (uint16_t)'b';
   *(picture_Name_add+9+ii) = (uint16_t)'i';
   *(picture_Name_add+10+ii) = (uint16_t)'n';
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: uart_set_send_porc
*  Description: 设置85为发送
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void flow_program_delay(uint32_t delay)
{
    delay=delay/10;
    if(delay==0)delay=1;
    while(delay--);
}

void uart_485_set_send_porc(void)
{
    eat_pin_set_mode(EAT_PIN5_DCD, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(EAT_PIN5_DCD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_HIGH);
    flow_program_delay(128);
    #if 0//take_pictures_debug_printf 
    eat_trace("uart_485_set_send_porc -->> 设置为发送延时;!!!.");
    #endif
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: uart_set_send_porc
*  Description: 设置85为发送
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void uart_485_set_rev_porc(void)
{
    flow_program_delay(data_delay_test);
    eat_pin_set_mode(EAT_PIN5_DCD, EAT_PIN_MODE_GPIO);
    eat_gpio_setup(EAT_PIN5_DCD, EAT_GPIO_DIR_OUTPUT, EAT_GPIO_LEVEL_LOW);
    #if 0//take_pictures_debug_printf 
    eat_trace("uart_485_set_rev_porc -->> 设置为接收!.");
    #endif
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: camer_send_switch
 *  Description:   摄像头发送数据转义 加标识头标识尾
 *  Parameters :  形参1-- pScr -> 源数组    
 *				  形参2 -- pDes -> 目标数组   
 *				  形参3 -- sLens -> 源数组长度 
 *		
 *	
 *  Return     :   word -> 返回转义后的长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */
uint16_t camer_send_switch(uint8_t *pScr,uint8_t *pDes,uint16_t sLens)
{
	uint16_t  temp_lens=0;
	uint16_t  cnt;
	uint8_t *pScr_temp;
	uint8_t *pDes_temp;
	pScr_temp=pScr;
	pDes_temp=pDes+1;
	for(cnt=0;cnt<sLens;cnt++)
		{
			switch(*pScr_temp)
			{
				case Tcp_Fram_X7E:
				*(pDes_temp++)=0x1B;
				*(pDes_temp++)=0x65;
				temp_lens+=2;
				pScr_temp++;
				break;
				case 0x1B:
				*(pDes_temp++)=0x1B;
				*(pDes_temp++)=0x00;		
				temp_lens+=2;
				pScr_temp++;
				break;
				default:
				*(pDes_temp++)=*(pScr_temp++);
				temp_lens++;
				break;
			
			}
		}
	*pDes_temp=Tcp_Fram_X7E;//尾加标示符
	*pDes=Tcp_Fram_X7E;//头加标示符
	return  temp_lens+2;
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: take_pictures_start_pro
*  Description: 开始拍照
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void take_pictures_over_fail_proc(void)
{
    uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";

    /***删除图片文件***/
    picture_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture name:%d.jpg !.",multimedia_picture_pb.media_id);
    #endif
   
    /***2 删除附加信息***/
    picture_add_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture add name:%d.bin !",multimedia_picture_pb.media_id);
    #endif
        
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> 清楚开始拍照标志.");
    #endif

    System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
   
}

/*
* ===  FUNCTION  ======================================================================
*  Name: get_picture_over_ok_porc
*  Description: 获取图片OK
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void get_picture_over_ok_porc(void)
{
     /***3 图片更新系统附加信息***/
     file_write(pictures_add_sys,pictures_num_total*sizeof(pictures_sys_add_pb)+sizeof(pictures_num_total),&pictures_sys_add_pb,sizeof(pictures_sys_add_pb));
     pictures_num_total++;
     file_write(pictures_add_sys,0,&pictures_num_total,sizeof(pictures_num_total));
     
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc -->> pictures_num_total:%d  !!!.",pictures_num_total);
     #endif
     
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc--> 清楚开始拍照标志.");
     #endif
     if(multimedia_picture_pb.process_type==0x00)
     {
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_ok_proc -->> 图片进上传链表!!!.");
        #endif
        media_anaylsis_list_add(&media_send_list,multimedia_picture_pb.media_type,multimedia_picture_pb.media_id);
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_ok_proc -->> 发送上传图片信息消息.");
        #endif
        task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
     }
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc--> 清除拍照标志.");
     #endif
     System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
}
/*
* ===  FUNCTION  ======================================================================
*  Name: get_picture_over_fail_porc
*  Description: 获取图片失败删除图片
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void get_picture_over_fail_porc(void)
{
    uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
    /***删除图片文件***/
    picture_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture name:%d.jpg !.",multimedia_picture_pb.media_id);
    #endif
   
    /***2 删除附加信息***/
    picture_add_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture add name:%d.bin !",multimedia_picture_pb.media_id);
    #endif
        
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> 清除拍照标志.");
    #endif
    System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> 发送处理图片信息");
    #endif
    task_msg_send(EAT_USER_1,EAT_USER_1,take_pictures_action);
}
/* ===  FUNCTION  ======================================================================
*  Name:  void send_camer_start_frame(uint8_t Address,uint8_t Size,uint8_t Quality)
*  Description:  发送开开始拍照数据帧
*  Parameters :  Address     :地址 
*             Size    :分辨率
*             Quality :质量
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-29             
* =====================================================================================
*/
void send_camer_start_frame(uint8_t Address,uint8_t Size,uint8_t Quality)
{
   uint8_t Farm_Buff[6]={0};
   uint8_t Send_Buff[20]={0};
   uint16_t D_Lens;//数据长度
   uint16_t XOR_PUS;
   uint32_t t1,t2;
   *Farm_Buff = Address;
   *(Farm_Buff+1) = 0x00;//发送拍照请求
   *(Farm_Buff+2) =Address;
   if((Size==PIC_Size_320X240)|Size==PIC_Size_352X248)
   {
       switch(Quality)
       {
           case 0x01://很好
           *(Farm_Buff+3) =0x22;
           break;
           case 0x02://好
           *(Farm_Buff+3) =0x12;
           break;
           case 0x03://较好
           *(Farm_Buff+3) =0x11;
           break;
           default://一般
           *(Farm_Buff+3) =0x21;
           break;  
       }
   }
   else if(Size==PIC_Size_176X164)//与次相近
   {
       switch(Quality)
       {
           case 0x01://很好
           *(Farm_Buff+3) =0x16;
           break;
           case 0x02://好
           *(Farm_Buff+3) =0x06;
           break;
           case 0x03://较好
           *(Farm_Buff+3) =0x05;
           break;
           default://一般
           *(Farm_Buff+3) =0x15;
           break;  
       }
   }
   else //其他与值相近
   {
       switch(Quality)
       {
           case 0x01://很好
           *(Farm_Buff+3) =0x14;
           break;
           case 0x02://好
           *(Farm_Buff+3) =0x04;
           break;
           case 0x03://较好
           *(Farm_Buff+3) =0x03;
           break;
           default://一般
           *(Farm_Buff+3) =0x13;
           break;  
       }
   }
   XOR_PUS=*Farm_Buff + *(Farm_Buff+1) + *(Farm_Buff+2) + *(Farm_Buff+3);
   Big_End_WordtoByte(XOR_PUS,Farm_Buff+4);
   D_Lens=camer_send_switch(Farm_Buff,Send_Buff,6);
   
   uart_485_set_send_porc();
   eat_uart_write(EAT_UART_2, Send_Buff, D_Lens);//发送数据
   uart_485_set_rev_porc();

   carmer_start_timeout_switch(EAT_TRUE);
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: take_pictures_start_pro
*  Description: 开始拍照
*  Parameters :  收到拍照结束后的处理
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void take_pictures_start_pro(void)
{
   uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
   /***创建图片文件***/
   picture_rename(multimedia_picture_pb.media_id,temp_name);
   file_create(temp_name);
   #if take_pictures_debug_printf 
   eat_trace("take_pictures_start_pro -->> rename creat picture;!!!.");
   #endif
   
   /***2 重命名图片文件附加信息***/
   picture_add_rename(multimedia_picture_pb.media_id,temp_name);
   file_create(temp_name);
   file_write(temp_name,0,&multimedia_picture_pb,sizeof(multimedia_add_struct));
   #if take_pictures_debug_printf 
   eat_trace("take_pictures_over_proc -->> creat signal picture add file !!!.");
   #endif


   #if take_pictures_debug_printf 
   eat_trace("take_pictures_over_proc -->> 发送数据开始拍照数据帧 !!!!!.");
   #endif
   send_camer_start_frame(multimedia_picture_pb.media_chanl_id,multimedia_picture_pb.picture_size,multimedia_picture_pb.picture_qulity);
       
}

/* 
* ===  FUNCTION  ======================================================================
*         Name: take_pictures_trigged
*  Description: 录音触发
*        Input: media_event :事件类型   触发类型 0：平台下发指令；1：定时动作；2：抢劫报警触发；3：碰撞侧翻报警触发；4：门开拍照；5：门关拍照；6：车门由开变关，时速从＜20 公里到超过 20 公里；7：定距拍照    
*               media_chanl_id:通道ID
*               process_type:处理类型 0：实时上传；1：保存
*       Output:
*       Return:  uint8_t 0x01 录音忙 0X00 录音开始             
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void take_pictures_trigged(uint8_t chanel_id,uint8_t picture_size,uint8_t picture_qulity,uint8_t media_event,uint8_t process_type)
{
    /*图片信息附加信息*/
    multimedia_add_struct multimedia_picture;
    /*图片信息总附加信息*/
    media_add_struct pictures_sys_add;
    BaseLocationSave_Struct Location_Inf_Data;//发发送缓存数据
    /***1拍照 信息附件信息****/
    multimedia_picture.media_type = 0x00;
    multimedia_picture.media_format = 0x00;
    multimedia_picture.media_event = media_event;
    multimedia_picture.media_chanl_id = chanel_id;
    multimedia_picture.picture_size = picture_size;
    multimedia_picture.picture_qulity= picture_qulity;
    multimedia_picture.process_type = process_type;
    /**当前位置信息**/
    Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
    memcpy(multimedia_picture.media_location,Location_Inf_Data.Location_Inf,28);
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged --> time hex !.");
    HexToAsicc(multimedia_picture.media_location,28);
    #endif
    
    /***2 system 拍照信息附件信息****/
    
    pictures_sys_add.chanel_id =chanel_id;
    pictures_sys_add.media_event=media_event;
    /**将时间转换为S储存**/
    pictures_sys_add.media_time=bcd_timer_to_sec(Real_Time_Location_Inf.Curren_Time);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged -->> take_pictures_list_add !!!mediatime:%d.",pictures_sys_add.media_time);
    #endif
    take_pictures_list_add(&camer_take_list,&multimedia_picture,&pictures_sys_add);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged -->> 发送拍照处理!!!.");
    #endif
    task_msg_send(EAT_USER_1,EAT_USER_1,take_pictures_action);
    
}
/* 
* ===  FUNCTION  ======================================================================
*         Name: take_picture_action_proc
*  Description: 拍照处理
*       Output:
*       Return:              
*       author: Jumping create at 2016-12-16
* =====================================================================================
*/
void take_picture_action_proc(void)
{
    if(System_Flag.System_Flag_Bit.take_picture==EAT_FALSE)
    {
        #if take_pictures_debug_printf 
        eat_trace("take_picture_action_proc--> 拍照空闲.");
        #endif
        if(list_empty(&camer_take_list)!=EAT_TRUE)
        {
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> 拍照链表中有带拍照的数据!.");
            #endif
            System_Flag.System_Flag_Bit.take_picture=EAT_TRUE;
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> 发送拍照请求帧.");
            #endif
            
           take_pictures_list_del(&camer_take_list,&multimedia_picture_pb,&pictures_sys_add_pb);
           multimedia_picture_pb.media_id =pictures_num_total+1;
           pictures_sys_add_pb.media_id = pictures_num_total+1;
           #if take_pictures_debug_printf 
           eat_trace("take_picture_action_proc--> 图片名称:%u.jpg",multimedia_picture_pb.media_id);
           #endif
           take_pictures_start_pro();
        }
        else
        {
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> 拍照链表空!!!!.");
            #endif
        }
        
    }
    else
    {
        #if take_pictures_debug_printf 
        eat_trace("take_picture_action_proc--> 拍照忙稍后再试.");
        #endif
    }
}
/* ===  FUNCTION  ======================================================================
 *  Name: Camer_Frame_Send_Ask_Cmd
 *  Description: 请求图片内容
 *  Parameters :  Address    :地址 
 *				  Cnt_num	 :包序号
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
static void send_ask_camer_frame(uint8_t Address,uint8_t Cnt_num)
 {
 	uint8_t Farm_Buff[6]={0};
 	uint8_t Send_Buff[20]={0};
 	uint16_t D_Lens = 0;//数据长度
 	uint16_t XOR_PUS = 0;
 	uint32_t t1,t2;
 	*Farm_Buff = Address;
 	*(Farm_Buff+1) = 0x02;//发送拍照请求
 	*(Farm_Buff+2) =Address;
 	*(Farm_Buff+3) =Cnt_num;//包序数	
 	XOR_PUS=*Farm_Buff + *(Farm_Buff+1) + *(Farm_Buff+2) + *(Farm_Buff+3);

 	Big_End_WordtoByte(XOR_PUS,Farm_Buff+4);
 	D_Lens=camer_send_switch(Farm_Buff,Send_Buff,6);

 	
    uart_485_set_send_porc();
	eat_uart_write(EAT_UART_2, Send_Buff, D_Lens);//发送数据	
	uart_485_set_rev_porc();

	camer_get_farme_timeout_switch(EAT_TRUE);
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Camer_RevFrame
 *  Description:  解析完整的一帧Tcp下发数据 
 *  Parameters :  pSource    :原始数据数组 
 *				  pLens		   :信息流水号         
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
static void analyze_camer_rev_frame (uint8_t *pSource,uint16_t pLens)
{
	uint8_t CMD_ID;//命令ID
	uint8_t Camer_Adderess;//摄像头地址
	uint16_t Data_Len=0;
	uint16_t Xor_Ret,PLUS_Num;
    static uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
	Big_End_BytetoWord(pSource+pLens-2,&PLUS_Num);
	Xor_Ret=PLUS_XOR(pSource,pLens-2);//校验
	if(PLUS_Num==Xor_Ret)
	{
		Camer_Adderess=*pSource;//地址
		CMD_ID=*(pSource+1);//命令
		switch (CMD_ID)
		{
			case 0x00://收到拍照回复信息
			{
                picture_rename(multimedia_picture_pb.media_id,temp_name);
                
                Big_End_BytetoWord(pSource+3,&Data_Len);
				get_farm_num = Data_Len/512;
				Data_Len = Data_Len%512;
				if(Data_Len!=0) get_farm_num = get_farm_num+1;//是否整帧
				
                #if take_pictures_debug_printf
                eat_trace("analyze_camer_rev_frame--> take_pictures_over_ok_proc !!");         
                #endif
                carmer_start_timeout_switch(EAT_FALSE);
                
                #if take_pictures_debug_printf
                eat_trace("analyze_camer_rev_frame--> uart_485_set_send_porc!!");         
                #endif
                now_farme_num=0;
                send_ask_camer_frame(multimedia_picture_pb.media_chanl_id,now_farme_num);

                
                #if take_pictures_debug_printf
                eat_trace("analyze_camer_rev_frame--> get_farm_num_SUB:%d !",get_farm_num);         
                #endif
			}
			break;
			case 0x02://收到图片回传信息
			{
                camer_get_farme_timeout_switch(EAT_FALSE);
                
				now_farme_num =*(pSource+2);
				file_write(temp_name,now_farme_num*512,pSource+3,512);//写图片
				now_farme_num += 1;
                #if take_pictures_debug_printf
                eat_trace("analyze_camer_rev_frame-->now_farme_num:%d,sub_farm_num:%d !",now_farme_num,get_farm_num);         
                #endif
				if(now_farme_num<get_farm_num)
				{
                    send_ask_camer_frame(multimedia_picture_pb.media_chanl_id,now_farme_num);
                    
				}
				else if(get_farm_num == now_farme_num)
				{
                    #if take_pictures_debug_printf
                    eat_trace("analyze_camer_rev_frame-->get pictures over !");         
                    #endif
                    get_picture_over_ok_porc();
				}
			}	
			break;
			default:
			break;	
		}		
	}	
	else
	{
        #if take_pictures_debug_printf
        eat_trace("analyze_camer_rev_frame--> receive picture orign check error !!");         
        #endif
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_Message_Rev_One_Packge
 *  Description:  接收TCP一帧完整的数据 
 *  Parameters :  *Rev_Buff        :目标BUFFER
 *				  *Source_Buff     :源数据包
 *				  Re_Lens          :源数据长度
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-28			 
 * =====================================================================================
 */
void camer_rev_one_packge(uint8_t *Source_Buff,uint16_t Re_Lens)
{
	uint8_t cData=0;
	uint16_t Datat_cnt=0;
    uint16_t Effect_Data_Lens=0;//有效收长度
    static uint8_t Temp_Dest[640]={0};
    static u16 TotalCnt=0;//静态变量总计数
    static uint8_t Rev_Data_Flag=0;
	static uint8_t get_spec_flag=0;
	uint16_t FE_Cnt=0;
	uint16_t FE_PLUS=0;//校验和
	Effect_Data_Lens = Re_Lens;//获取有效数据长度
	if(Effect_Data_Lens)//字符串长度
	{	
		for(Datat_cnt=0;Datat_cnt<Effect_Data_Lens;Datat_cnt++)//保证一次校验一位
		{
			cData =*(Source_Buff+Datat_cnt);//读值
			switch(Rev_Data_Flag)
		    {	
				case 0:
				{
					if(cData==Tcp_Fram_X7E)//头帧1
					{
						Rev_Data_Flag=1;
						#if take_pictures_debug_printf
                        eat_trace("camer_rev_one_packge--> receive 开始 0X7E!! !");         
						#endif
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
                    #if take_pictures_debug_printf
                    eat_trace("camer_rev_one_packge--> receive 结束 0X7E!! !");         
                    #endif
					if(TotalCnt<2)
					{
						Rev_Data_Flag=0;
						TotalCnt=0;
						return;
					}
					else
					{
                        Big_End_BytetoWord(Temp_Dest+TotalCnt-2,&FE_PLUS);
						if((*(Temp_Dest+TotalCnt-2)==0x7E)&&(*(Temp_Dest+TotalCnt-1)==0x7E))
						{
							FE_PLUS = FE_PLUS-2*(FE_Cnt-2);//校验和改变
						}
						else if((*(Temp_Dest+TotalCnt-2)==0x7E)||(*(Temp_Dest+TotalCnt-1)==0x7E))
						{
							FE_PLUS = FE_PLUS-2*(FE_Cnt-1);//校验和改变
						}
						else
						{
							FE_PLUS = FE_PLUS - FE_Cnt*2;//校验和改变
						}
                        Big_End_WordtoByte(FE_PLUS,Temp_Dest+TotalCnt-2);
						analyze_camer_rev_frame(Temp_Dest,TotalCnt);//数据分析
						Rev_Data_Flag=0;
						TotalCnt=0;
						return;
					}	
				}
				else
				{
					if(cData==0x1B&&(!get_spec_flag))//有特征字符
					{
					    TotalCnt++;
						get_spec_flag=1;
						continue;
					}
					if(get_spec_flag)//特征字符尾字符判断
					{
					    if(cData==0x65)
					     {
					     	*(Temp_Dest+TotalCnt-1)=0x7E;
							  get_spec_flag=0; 
							  FE_Cnt++;//FE个数
						      continue;
					     }
						 else if(cData==0x00)
						 {
					        *(Temp_Dest+TotalCnt-1)=0x1B;
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
				  *(Temp_Dest+TotalCnt)=cData;
			    	TotalCnt++; 
				}
				break;
				default:
				break;			
			}
		}
		if(Rev_Data_Flag)
		{
			Rev_Data_Flag=0;
			TotalCnt=0;
			return;	
		}
	}
}
/* ===  FUNCTION  ====================================================================
 *  Name: Camer_Frame_Send_Sleep_Cmd
 *  Description: 摄像头休眠
 *  Parameters :  Address    :地址
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
void tcp_take_pictures_server(char_t socket,uint8_t *data_buff,uint16_t servn)//通道上一命名未执行完不接受下一条
{
    if(take_attributes.take_time_state==EAT_TRUE)
    {
        tcp_take_picture_result_replay(socket,servn,0x01,0);
    }
    else
    {
    	take_attributes.chanel_id= *data_buff;//通道号
    	Big_End_BytetoWord(data_buff+1,&take_attributes.pictures_num);
    	Big_End_BytetoWord(data_buff+3,&take_attributes.take_frze);
    	if(take_attributes.pictures_num==0)  
    	{
    		take_attributes.take_time_state = EAT_FALSE;
    	}
    	else
    	{
    	    take_attributes.picture_size = *(data_buff+6);
    	    take_attributes.picture_qulity = *(data_buff+7);
    	    take_attributes.media_event = 0x00;
    	    take_attributes.process_type= *(data_buff+5);
    	    
            #if voice_record_debug_printf 
            eat_trace("tcp_take_pictures_server -->> chanel_id:%d,size:%d,qulity:%d,envent:%d,process:%d !.",take_attributes.chanel_id,take_attributes.picture_size,take_attributes.picture_qulity,take_attributes.media_event,take_attributes.process_type);
            #endif
            take_pictures_trigged(take_attributes.chanel_id,take_attributes.picture_size,take_attributes.picture_qulity,take_attributes.media_event,take_attributes.process_type);
            take_attributes.pictures_num -= 1;//张数
    		if(take_attributes.chanel_id <= 0x03)
    		{
    			if(take_attributes.pictures_num>1)
    			{
                    take_attributes.take_time_state = EAT_TRUE;
    				if(take_attributes.take_frze<10)take_attributes.take_frze=15;
    			}
    			else
    			{
    			    take_attributes.take_frze=0;
    			    take_attributes.pictures_num = 0;
    			}
                tcp_take_picture_result_replay(socket,servn,0x00,take_attributes.pictures_num);
    		}
    		else
    		{
                tcp_take_picture_result_replay(socket,servn,0x02,0);
    		}
    	}
	}
}

 /* ===  FUNCTION  ====================================================================
 *  Name: take_picture_system_add_init
 *  Description: 拍照初始化
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
 void take_picture_system_add_init(void)
 {
     int32_t file_read_result;
     multimedia_add_struct temp_add;
     #if voice_record_debug_printf 
     eat_trace("take_picture_system_add_init -->> creat PIC fold !!!.");
     #endif
     fs_Create_file_flod(pictures_pic_fold);
     #if voice_record_debug_printf 
     eat_trace("take_picture_system_add_init -->> pictures_add_sys !!!.");
     #endif
     file_create(pictures_add_sys);
     file_read_result=file_read(pictures_add_sys,0,&pictures_num_total,sizeof(pictures_num_total));
     if(file_read_result==OPERATION_NO_ERROR)
     {
        #if voice_record_debug_printf 
        eat_trace("take_picture_system_add_init -->> sucess pictures_num_total:%d !!!.",pictures_num_total);
        #endif
     }
     else
     {
         pictures_num_total = 0;
         #if voice_record_debug_printf 
         eat_trace("take_picture_system_add_init -->>error pictures_num_total: 0 !!!.");
         #endif
     }
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: tcp_take_picture_result_replay
 *  Description:  创建图片
 *  Parameters :  TCP拍照应答			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
 void tcp_take_picture_result_replay(char_t socket,uint16_t msg_svn,uint8_t result,uint16_t pic_cnt)
 {
 	uint16_t ii=0,offfest=0;
 	uint8_t send_data[512]={0};
 	Big_End_WordtoByte(msg_svn,send_data);
 	offfest += 2;
 	*(send_data + offfest) = result;
 	offfest += 1;
 	if(result==0x00)
 	{
 		Big_End_WordtoByte(pic_cnt+1,send_data + offfest);
        offfest += 2;
 		for(ii=0;ii<pic_cnt+1;ii++)
 		{
 			Big_End_DwordtoByte(pictures_num_total+ii,send_data + offfest);
            offfest += 4;
 		}
 	}
 	else
 	{
        #if voice_record_debug_printf 
        eat_trace("tcp_take_picture_result_replay -->> 通道不支持 !!!.");
        #endif
 	}
    Tcp_Message_PacKage_And_Send(socket,&msg_svn,tcp_take_picture_replay,send_data,offfest,0,0,0);
 }

 /* ===  FUNCTION  ====================================================================
 *  Name: Tcp_DuoMei_SuoJian
 *  Description: 多媒体检索
 *  Parameters :  Address :地址
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
 void tcp_media_search(char_t socket,uint8_t *data_buf,uint16_t msgsn)
 {
 	uint32_t Stat_Time=0;//开始时间
 	uint32_t End_Time=0;//结束时间
 	uint8_t send_data[512]={0};
    uint16_t media_Name[22]=L"D:\\PIC\\X.BIN";
    uint32_t ii,media_cnt;
    uint16_t serach_cnt=0,msg_sn=0;
 	multimedia_add_struct multimedia;
    media_add_struct sys_add;
 	uint16_t data_offest = 0;
    uint8_t media_typef,media_chanel_id,media_event;
    
 	media_typef = *data_buf;
 	media_chanel_id = *(data_buf+1);
 	media_event = *(data_buf+2);
 	Stat_Time=bcd_timer_to_sec(data_buf+3);
    #if 1//debuge_printf_switch 
    eat_trace("sys_gps_rmc_parse11 --> rtc_time:20%0x-%0x-%0x %0x:%0x:%0x",*(data_buf+3),*(data_buf+4),*(data_buf+5),*(data_buf+6),*(data_buf+7),*(data_buf+8));
    #endif
 	End_Time=bcd_timer_to_sec(data_buf+9);
    #if 1//debuge_printf_switch 
    eat_trace("sys_gps_rmc_parse11 --> rtc_time:20%0x-%0x-%0x %0x:%0x:%0x",*(data_buf+9),*(data_buf+10),*(data_buf+11),*(data_buf+12),*(data_buf+13),*(data_buf+14));
    #endif
 	Big_End_WordtoByte(msgsn,send_data);
 	data_offest += 2;

 	data_offest += 2;
    if(media_typef==0x00)
    {
        #if take_pictures_debug_printf 
        eat_trace("tcp_media_search -->> medid type:图片!!!.");
        #endif
	    media_cnt = pictures_num_total;
	}
	else if(media_typef==0x01) 
	{
        #if take_pictures_debug_printf 
        eat_trace("tcp_media_search -->> medid type:录音!!!.");
        #endif
	    media_cnt = voic_num_total;
	}
	for(ii=0;ii<media_cnt;ii++)
	{
        if(media_typef==0)
        {
            file_read(pictures_add_sys,ii*sizeof(sys_add)+sizeof(pictures_num_total),&sys_add,sizeof(sys_add));
            #if take_pictures_debug_printf 
            eat_trace("tcp_media_search -->> medid type:图片!!! .");
            #endif
        }
        else if(media_typef==1) 
        {
            file_read(voice_record_add_sys,ii*sizeof(sys_add)+sizeof(voic_num_total),&sys_add,sizeof(sys_add));
            #if take_pictures_debug_printf 
            eat_trace("tcp_media_search -->> medid type:录音!!! .");
            #endif
        }
        #if take_pictures_debug_printf 
        eat_trace("tcp_media_search -->> medid id:%d .",sys_add.media_id);
        #endif
        if((media_chanel_id==0)||(sys_add.chanel_id==media_chanel_id))
        {
            if(media_event==sys_add.media_event)
            {
                #if take_pictures_debug_printf
                eat_trace("tcp_media_search--> start:%d,end:%d,curren:%d !!!.",Stat_Time,End_Time,sys_add.media_time);
                #endif
            	if(((sys_add.media_time)>=Stat_Time)&&((sys_add.media_time)<=End_Time))
            	{
            		if(media_typef==0)
                    {
                        picture_add_rename(sys_add.media_id,media_Name);
                    }
                    else if(media_typef==1) 
                    {
                        voice_record_add_rename(sys_add.media_id);
                        memcpy(media_Name,voice_Name_add,sizeof(voice_Name_add));
                    }
                    
                    Big_End_DwordtoByte(sys_add.media_id,send_data+data_offest);
                    data_offest += sizeof(sys_add.media_id);
                    
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search-->sizeof(sys_add.media_id)=%d !",sizeof(sys_add.media_id));
                    #endif
            		file_read(media_Name,0,&multimedia,sizeof(multimedia));//读附加信息
            		*(send_data+data_offest) = multimedia.media_type;
                    data_offest += sizeof(multimedia.media_type);
                    
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search-->sizeof(multimedia.media_type)=%d !",sizeof(multimedia.media_type));
                    #endif
            		*(send_data+data_offest) = multimedia.media_chanl_id;
                    data_offest += sizeof(multimedia.media_chanl_id);
                    
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search-->sizeof(multimedia.media_chanl_id)=%d !",sizeof(multimedia.media_chanl_id));
                    #endif
            		*(send_data+data_offest) = multimedia.media_event;
                    data_offest += sizeof(multimedia.media_event);
                    
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search-->sizeof(multimedia.media_event)=%d !",sizeof(multimedia.media_event));
                    #endif
            		memcpy(send_data+data_offest,multimedia.media_location,sizeof(multimedia.media_location));
                    data_offest += sizeof(multimedia.media_location);
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search-->sizeof(multimedia.media_location)=%d !",sizeof(multimedia.media_location));
                    #endif
            	    serach_cnt++;	

                    if(media_typef==0)
                    {
                        #if take_pictures_debug_printf
                        eat_trace("tcp_media_search-->符合ID:%u.jpg !",sys_add.media_id);
                        #endif
                    }
                    else if(media_typef==1) 
                    {
                        #if take_pictures_debug_printf
                        eat_trace("tcp_media_search-->符合ID:%u.wav!",sys_add.media_id);
                        #endif
                    }
            	}
            	else
            	{
                    #if take_pictures_debug_printf
                    eat_trace("tcp_media_search--> media time not match !");
                    #endif
            	}
        	}
        	else
        	{
                #if take_pictures_debug_printf
                eat_trace("tcp_media_search--> media event not match !");
                #endif
        	}
		}
		else
		{
            #if take_pictures_debug_printf
            eat_trace("tcp_media_search--> media chinel not match !");
            #endif
		}
	}
	Big_End_WordtoByte(serach_cnt,send_data+2);
    #if take_pictures_debug_printf>0
    eat_trace("tcp_media_search--> orgin data check serach_cnt:%d, data_offest:%d!",serach_cnt,data_offest);
    #endif
    HexToAsicc(send_data,data_offest);
    Tcp_Message_PacKage_And_Send(socket,&msg_sn,tcp_media_search_replay_id,send_data,data_offest,0,0,0);
 }
 /* ===  FUNCTION  ====================================================================
 *  Name: tcp_serch_update(uint8_t *data_buf,uint16_t Sn)
 *  Description: 多媒体检索上传
 *  Parameters :  Address :地址
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
void save_media_tcp_serch_update(uint8_t *data_buf)
{
    uint32_t Stat_Time=0;//开始时间
 	uint32_t End_Time=0;//结束时间
    uint32_t ii,media_cnt;
 	multimedia_add_struct multimedia;
    media_add_struct sys_add;
    uint8_t media_typef;
    
 	media_typef = *data_buf;
 	Stat_Time=bcd_timer_to_sec(data_buf+3);
 	End_Time=bcd_timer_to_sec(data_buf+9);
 	
    #if take_pictures_debug_printf 
    eat_trace("save_media_tcp_serch_update -->> voice_record_add_sys !!!.");
    #endif
    if(media_typef==0)
    {
	    media_cnt = pictures_num_total;
	}
	else if(media_typef==1) 
	{
	    media_cnt = voic_num_total;
	}
	for(ii=0;ii<media_cnt;ii++)
	{
        if(media_typef==0)
        {
            file_read(pictures_add_sys,ii*sizeof(media_add_struct)+sizeof(pictures_num_total),&sys_add,sizeof(media_add_struct));
        }
        else if(media_typef==1) 
        {
            file_read(voice_record_add_sys,ii*sizeof(media_add_struct)+sizeof(voic_num_total),&sys_add,sizeof(media_add_struct));
        }
        #if take_pictures_debug_printf 
        eat_trace("save_media_tcp_serch_update -->> start:%d,end:%d,curren:%d !!!.",Stat_Time,End_Time,sys_add.media_time);
        #endif
		if(((sys_add.media_time)>=Stat_Time)&&((sys_add.media_time)<=End_Time))
		{
            #if take_pictures_debug_printf 
            eat_trace("take_pictures_over_ok_proc -->> 图片进上传链表!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,media_typef,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("take_pictures_over_ok_proc -->> 发送上传图片信息消息.");
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
        }
	}	
            
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: tcp_single_media_Search
 *  Description: 检索单条指令           
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void tcp_single_media_search(uint8_t *scr_data)
{
	uint32_t ii;
	uint32_t media_id=0;
	uint32_t media_cnt;
	uint16_t media_name[22];
 	multimedia_add_struct multimedia;
    media_add_struct sys_add;
	Big_End_BytetoDword(scr_data,&media_id);
	media_cnt=pictures_num_total;
	for(ii=0;ii<media_cnt;ii++)
	{
        file_read(pictures_add_sys,ii*sizeof(sys_add)+sizeof(pictures_num_total),&sys_add,sizeof(sys_add));
		if(media_id==sys_add.media_id)
		{
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> 图片进上传链表!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,0x00,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> 发送上传图片信息消息.");
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
			return;
		}
	}
	media_cnt=voic_num_total;
    for(ii=0;ii<media_cnt;ii++)
    {
        file_read(voice_record_add_sys,ii*sizeof(sys_add)+sizeof(voic_num_total),&sys_add,sizeof(sys_add));
        if(media_id==sys_add.media_id)
        {
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> 录音进上传链表!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,0x01,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> 发送上传录音信息消息.");
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
            return;
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: carmer_start_timeout_switch
 *  Description: 拍照超时打开           
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void carmer_start_timeout_switch(uint8_t switch_cmd)
{
    if(switch_cmd == EAT_TRUE)
    {
        #if take_pictures_debug_printf 
        eat_trace("carmer_start_timeout_switch --> timeout satart !");
        #endif
        rev_take_ret_data_timeout.time_state=EAT_TRUE;
        task_msg_send(EAT_USER_1,EAT_USER_0,take_pictures_timeout);
    }
    else
    {
        #if take_pictures_debug_printf 
        eat_trace("camer_start_timeout_handle -->time out stop ");
        #endif
        rev_take_ret_data_timeout.times=3;
        rev_take_ret_data_timeout.time_state=EAT_FALSE;
        eat_timer_stop(EAT_TIMER_10);
    }
}
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: camer_start_timeout_cb
 *  Description: 拍照超时处理            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
static void camer_start_timeout_handle(void) //超时处理函数
{
    if(rev_take_ret_data_timeout.time_state==EAT_TRUE)
    {
         rev_take_ret_data_timeout.times--;
         if(rev_take_ret_data_timeout.times==0)
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_start_timeout_handle --> 连续三次发送获取帧失败删除照片信息。");
             #endif
            
             carmer_start_timeout_switch(EAT_FALSE);
             take_pictures_over_fail_proc();
         }
         else
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_start_timeout_handle -->ing cnt:%u",rev_take_ret_data_timeout.times);
             #endif
             send_camer_start_frame(multimedia_picture_pb.media_chanl_id,multimedia_picture_pb.picture_size,multimedia_picture_pb.picture_qulity);
         }
    }
    else
    {
        if(rev_take_ret_data_timeout.times==0) rev_take_ret_data_timeout.times=3;
    }
     
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: camer_get_farme_timeout_switch
 *  Description: 获取图片数据打开         
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void camer_get_farme_timeout_switch(uint8_t switch_cmd)
{
    if(switch_cmd == EAT_TRUE)
    {
        #if 0//take_pictures_debug_printf 
        eat_trace("camer_get_farme_timeout_switch --> timeout satart !");
        #endif
        rev_get_data_timeout.time_state=EAT_TRUE;
        task_msg_send(EAT_USER_1,EAT_USER_0,take_pictures_timeout);
    }
    else
    {
        #if 0//take_pictures_debug_printf 
        eat_trace("camer_get_farme_timeout_switch -->time out stop !");
        #endif
        rev_get_data_timeout.times=20;
        rev_get_data_timeout.time_state=EAT_FALSE;
        eat_timer_stop(EAT_TIMER_10);
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: camer_get_farme_timeout_cb
 *  Description: 获取图片数据帧超时处理         
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
 
static void camer_get_farme_timeout_handle(void) //超时处理函数
{
    if(rev_get_data_timeout.time_state==EAT_TRUE)
    {
         rev_get_data_timeout.times--;
         if(rev_get_data_timeout.times==0)
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_get_farme_timeout_handle --> 连续三次发送获取帧失败，拍照结束");
             #endif
             camer_get_farme_timeout_switch(EAT_FALSE);
             get_picture_over_fail_porc();
         }
         else
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_get_farme_timeout_handle -->ing cnt:%u",rev_get_data_timeout.times);
             #endif
             send_ask_camer_frame(multimedia_picture_pb.media_chanl_id,now_farme_num);
         }
    }
    else
    {
        if(rev_get_data_timeout.times==0) rev_get_data_timeout.times=20;
    }
     
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void take_pictures_regularly(void) 
 *  Description:  创建图片
 *  Parameters :  Time_Out P   超时标志结构体			  	
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-31			 
 * =====================================================================================
 */
void take_pictures_regularly(void)
{
    static uint16_t times_cnt=0,times=0;
    if(take_attributes.take_time_state==EAT_TRUE)
    {
        times_cnt++;
        if(times_cnt==take_attributes.take_frze)
        {
            times_cnt=0;
            take_pictures_trigged(take_attributes.chanel_id,take_attributes.picture_size,take_attributes.picture_qulity,take_attributes.media_event,take_attributes.process_type);
            times++;
            #if take_pictures_debug_printf 
            eat_trace("take_pictures_regularly -->times:%u",times+1);
            #endif
            if(times==take_attributes.pictures_num)
            {
                times=0;
                take_attributes.take_time_state=EAT_FALSE;
                #if take_pictures_debug_printf 
                eat_trace("take_pictures_regularly -->over !");
                #endif
            }
        }
    }
    else
    {
        if(times_cnt!=0) times_cnt=0;
        if(times!=0) times=0;
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: take_pictures_timeout_msg_proc
 *  Description: 定时器开启处理       
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void take_pictures_timeout_msg_proc(void)
{
   if(rev_take_ret_data_timeout.time_state==EAT_TRUE)
    {
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_timeout_msg_proc -->rev_take_ret_data_timeout.send_data_delay:%u ms",rev_take_ret_data_timeout.send_data_delay);
        #endif
        
	    eat_timer_start(EAT_TIMER_10,rev_take_ret_data_timeout.send_data_delay);
    }
    else if(rev_get_data_timeout.time_state==EAT_TRUE)
    {
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_timeout_msg_proc -->rev_get_data_timeout.send_data_delay:%u ms",rev_get_data_timeout.send_data_delay);
        #endif
        
	    eat_timer_start(EAT_TIMER_10,rev_get_data_timeout.send_data_delay);
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: take_pictures_time10_pro
 *  Description: 定时器10处理      
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void take_pictures_time10_pro(void)
{
     camer_start_timeout_handle();
     camer_get_farme_timeout_handle();
}

