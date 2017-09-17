
#include "take_pictures.h"




/*ͼƬ��Ϣ������Ϣ*/
multimedia_add_struct multimedia_picture_pb;
/*ͼƬ��Ϣ�ܸ�����Ϣ*/
media_add_struct pictures_sys_add_pb;

/*ͼƬ������*/
uint32_t pictures_num_total=0;
/*ͼƬ�ܰ���*/
uint8_t get_farm_num = 0;//����Ŀ

/*ͼƬ��ǰ����*/
uint8_t now_farme_num = 0;
/*�������տ�ʼ֡ms*/
take_pic_time_process rev_take_ret_data_timeout={EAT_FALSE,5,6000};
/*�������տ�ʼ֡ms*/
take_pic_time_process rev_get_data_timeout={EAT_FALSE,25,1000};

/*���������������ms*/
take_pictures_attributes take_attributes;
uint32_t data_delay_test=76000;

struct list_head camer_take_list;

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
   eat_trace("take_pictures_over_proc -->> ��������.chinel_id:%d,%d,%d",mem_prt->pictures_add.media_chanl_id,mem_prt->pictures_add.picture_size,mem_prt->pictures_add.picture_qulity);
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
*  Description: �޸�¼������
*  Parameters :  voice_id :¼������
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void picture_rename(uint32_t picture_id,uint16_t *picture_Name)
{
   uint8_t str[10]={0};
   char data_len=0,ii=0;
   /********1 ����ת��Ϊ�ַ���*******/
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
*  Description: �޸�¼�� ������Ϣ
*  Parameters :  voice_id :¼������
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void picture_add_rename(uint32_t voice_id,uint16_t *picture_Name_add)
{
   uint8_t str[10]={0};
   char data_len=0,ii=0;
   /********1 ����ת��Ϊ�ַ���*******/
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
*  Description: ����85Ϊ����
*  Parameters :  �յ����ս�����Ĵ���
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
    eat_trace("uart_485_set_send_porc -->> ����Ϊ������ʱ;!!!.");
    #endif
}
/* 
* ===  FUNCTION  ======================================================================
*  Name: uart_set_send_porc
*  Description: ����85Ϊ����
*  Parameters :  �յ����ս�����Ĵ���
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
    eat_trace("uart_485_set_rev_porc -->> ����Ϊ����!.");
    #endif
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: camer_send_switch
 *  Description:   ����ͷ��������ת�� �ӱ�ʶͷ��ʶβ
 *  Parameters :  �β�1-- pScr -> Դ����    
 *				  �β�2 -- pDes -> Ŀ������   
 *				  �β�3 -- sLens -> Դ���鳤�� 
 *		
 *	
 *  Return     :   word -> ����ת���ĳ���
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
	*pDes_temp=Tcp_Fram_X7E;//β�ӱ�ʾ��
	*pDes=Tcp_Fram_X7E;//ͷ�ӱ�ʾ��
	return  temp_lens+2;
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: take_pictures_start_pro
*  Description: ��ʼ����
*  Parameters :  �յ����ս�����Ĵ���
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void take_pictures_over_fail_proc(void)
{
    uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";

    /***ɾ��ͼƬ�ļ�***/
    picture_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture name:%d.jpg !.",multimedia_picture_pb.media_id);
    #endif
   
    /***2 ɾ��������Ϣ***/
    picture_add_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture add name:%d.bin !",multimedia_picture_pb.media_id);
    #endif
        
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> �����ʼ���ձ�־.");
    #endif

    System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
   
}

/*
* ===  FUNCTION  ======================================================================
*  Name: get_picture_over_ok_porc
*  Description: ��ȡͼƬOK
*  Parameters :  �յ����ս�����Ĵ���
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void get_picture_over_ok_porc(void)
{
     /***3 ͼƬ����ϵͳ������Ϣ***/
     file_write(pictures_add_sys,pictures_num_total*sizeof(pictures_sys_add_pb)+sizeof(pictures_num_total),&pictures_sys_add_pb,sizeof(pictures_sys_add_pb));
     pictures_num_total++;
     file_write(pictures_add_sys,0,&pictures_num_total,sizeof(pictures_num_total));
     
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc -->> pictures_num_total:%d  !!!.",pictures_num_total);
     #endif
     
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc--> �����ʼ���ձ�־.");
     #endif
     if(multimedia_picture_pb.process_type==0x00)
     {
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_ok_proc -->> ͼƬ���ϴ�����!!!.");
        #endif
        media_anaylsis_list_add(&media_send_list,multimedia_picture_pb.media_type,multimedia_picture_pb.media_id);
        #if take_pictures_debug_printf 
        eat_trace("take_pictures_over_ok_proc -->> �����ϴ�ͼƬ��Ϣ��Ϣ.");
        #endif
        task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
     }
     #if take_pictures_debug_printf 
     eat_trace("take_pictures_over_ok_proc--> ������ձ�־.");
     #endif
     System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
}
/*
* ===  FUNCTION  ======================================================================
*  Name: get_picture_over_fail_porc
*  Description: ��ȡͼƬʧ��ɾ��ͼƬ
*  Parameters :  �յ����ս�����Ĵ���
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
static void get_picture_over_fail_porc(void)
{
    uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
    /***ɾ��ͼƬ�ļ�***/
    picture_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture name:%d.jpg !.",multimedia_picture_pb.media_id);
    #endif
   
    /***2 ɾ��������Ϣ***/
    picture_add_rename(multimedia_picture_pb.media_id,temp_name);
    eat_fs_Delete(temp_name);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc -->> delete picture add name:%d.bin !",multimedia_picture_pb.media_id);
    #endif
        
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> ������ձ�־.");
    #endif
    System_Flag.System_Flag_Bit.take_picture=EAT_FALSE;
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_over_fail_proc--> ���ʹ���ͼƬ��Ϣ");
    #endif
    task_msg_send(EAT_USER_1,EAT_USER_1,take_pictures_action);
}
/* ===  FUNCTION  ======================================================================
*  Name:  void send_camer_start_frame(uint8_t Address,uint8_t Size,uint8_t Quality)
*  Description:  ���Ϳ���ʼ��������֡
*  Parameters :  Address     :��ַ 
*             Size    :�ֱ���
*             Quality :����
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-10-29             
* =====================================================================================
*/
void send_camer_start_frame(uint8_t Address,uint8_t Size,uint8_t Quality)
{
   uint8_t Farm_Buff[6]={0};
   uint8_t Send_Buff[20]={0};
   uint16_t D_Lens;//���ݳ���
   uint16_t XOR_PUS;
   uint32_t t1,t2;
   *Farm_Buff = Address;
   *(Farm_Buff+1) = 0x00;//������������
   *(Farm_Buff+2) =Address;
   if((Size==PIC_Size_320X240)|Size==PIC_Size_352X248)
   {
       switch(Quality)
       {
           case 0x01://�ܺ�
           *(Farm_Buff+3) =0x22;
           break;
           case 0x02://��
           *(Farm_Buff+3) =0x12;
           break;
           case 0x03://�Ϻ�
           *(Farm_Buff+3) =0x11;
           break;
           default://һ��
           *(Farm_Buff+3) =0x21;
           break;  
       }
   }
   else if(Size==PIC_Size_176X164)//������
   {
       switch(Quality)
       {
           case 0x01://�ܺ�
           *(Farm_Buff+3) =0x16;
           break;
           case 0x02://��
           *(Farm_Buff+3) =0x06;
           break;
           case 0x03://�Ϻ�
           *(Farm_Buff+3) =0x05;
           break;
           default://һ��
           *(Farm_Buff+3) =0x15;
           break;  
       }
   }
   else //������ֵ���
   {
       switch(Quality)
       {
           case 0x01://�ܺ�
           *(Farm_Buff+3) =0x14;
           break;
           case 0x02://��
           *(Farm_Buff+3) =0x04;
           break;
           case 0x03://�Ϻ�
           *(Farm_Buff+3) =0x03;
           break;
           default://һ��
           *(Farm_Buff+3) =0x13;
           break;  
       }
   }
   XOR_PUS=*Farm_Buff + *(Farm_Buff+1) + *(Farm_Buff+2) + *(Farm_Buff+3);
   Big_End_WordtoByte(XOR_PUS,Farm_Buff+4);
   D_Lens=camer_send_switch(Farm_Buff,Send_Buff,6);
   
   uart_485_set_send_porc();
   eat_uart_write(EAT_UART_2, Send_Buff, D_Lens);//��������
   uart_485_set_rev_porc();

   carmer_start_timeout_switch(EAT_TRUE);
}

/* 
* ===  FUNCTION  ======================================================================
*  Name: take_pictures_start_pro
*  Description: ��ʼ����
*  Parameters :  �յ����ս�����Ĵ���
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
void take_pictures_start_pro(void)
{
   uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
   /***����ͼƬ�ļ�***/
   picture_rename(multimedia_picture_pb.media_id,temp_name);
   file_create(temp_name);
   #if take_pictures_debug_printf 
   eat_trace("take_pictures_start_pro -->> rename creat picture;!!!.");
   #endif
   
   /***2 ������ͼƬ�ļ�������Ϣ***/
   picture_add_rename(multimedia_picture_pb.media_id,temp_name);
   file_create(temp_name);
   file_write(temp_name,0,&multimedia_picture_pb,sizeof(multimedia_add_struct));
   #if take_pictures_debug_printf 
   eat_trace("take_pictures_over_proc -->> creat signal picture add file !!!.");
   #endif


   #if take_pictures_debug_printf 
   eat_trace("take_pictures_over_proc -->> �������ݿ�ʼ��������֡ !!!!!.");
   #endif
   send_camer_start_frame(multimedia_picture_pb.media_chanl_id,multimedia_picture_pb.picture_size,multimedia_picture_pb.picture_qulity);
       
}

/* 
* ===  FUNCTION  ======================================================================
*         Name: take_pictures_trigged
*  Description: ¼������
*        Input: media_event :�¼�����   �������� 0��ƽ̨�·�ָ�1����ʱ������2�����ٱ���������3����ײ�෭����������4���ſ����գ�5���Ź����գ�6�������ɿ���أ�ʱ�ٴӣ�20 ���ﵽ���� 20 ���7����������    
*               media_chanl_id:ͨ��ID
*               process_type:�������� 0��ʵʱ�ϴ���1������
*       Output:
*       Return:  uint8_t 0x01 ¼��æ 0X00 ¼����ʼ             
*       author: Jumping create at 2013-12-16
* =====================================================================================
*/
void take_pictures_trigged(uint8_t chanel_id,uint8_t picture_size,uint8_t picture_qulity,uint8_t media_event,uint8_t process_type)
{
    /*ͼƬ��Ϣ������Ϣ*/
    multimedia_add_struct multimedia_picture;
    /*ͼƬ��Ϣ�ܸ�����Ϣ*/
    media_add_struct pictures_sys_add;
    BaseLocationSave_Struct Location_Inf_Data;//�����ͻ�������
    /***1���� ��Ϣ������Ϣ****/
    multimedia_picture.media_type = 0x00;
    multimedia_picture.media_format = 0x00;
    multimedia_picture.media_event = media_event;
    multimedia_picture.media_chanl_id = chanel_id;
    multimedia_picture.picture_size = picture_size;
    multimedia_picture.picture_qulity= picture_qulity;
    multimedia_picture.process_type = process_type;
    /**��ǰλ����Ϣ**/
    Analysis_Location_Inf(&Real_Time_Location_Inf,&Location_Inf_Data);
    memcpy(multimedia_picture.media_location,Location_Inf_Data.Location_Inf,28);
    
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged --> time hex !.");
    HexToAsicc(multimedia_picture.media_location,28);
    #endif
    
    /***2 system ������Ϣ������Ϣ****/
    
    pictures_sys_add.chanel_id =chanel_id;
    pictures_sys_add.media_event=media_event;
    /**��ʱ��ת��ΪS����**/
    pictures_sys_add.media_time=bcd_timer_to_sec(Real_Time_Location_Inf.Curren_Time);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged -->> take_pictures_list_add !!!mediatime:%d.",pictures_sys_add.media_time);
    #endif
    take_pictures_list_add(&camer_take_list,&multimedia_picture,&pictures_sys_add);
    #if take_pictures_debug_printf 
    eat_trace("take_pictures_trigged -->> �������մ���!!!.");
    #endif
    task_msg_send(EAT_USER_1,EAT_USER_1,take_pictures_action);
    
}
/* 
* ===  FUNCTION  ======================================================================
*         Name: take_picture_action_proc
*  Description: ���մ���
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
        eat_trace("take_picture_action_proc--> ���տ���.");
        #endif
        if(list_empty(&camer_take_list)!=EAT_TRUE)
        {
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> �����������д����յ�����!.");
            #endif
            System_Flag.System_Flag_Bit.take_picture=EAT_TRUE;
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> ������������֡.");
            #endif
            
           take_pictures_list_del(&camer_take_list,&multimedia_picture_pb,&pictures_sys_add_pb);
           multimedia_picture_pb.media_id =pictures_num_total+1;
           pictures_sys_add_pb.media_id = pictures_num_total+1;
           #if take_pictures_debug_printf 
           eat_trace("take_picture_action_proc--> ͼƬ����:%u.jpg",multimedia_picture_pb.media_id);
           #endif
           take_pictures_start_pro();
        }
        else
        {
            #if take_pictures_debug_printf 
            eat_trace("take_picture_action_proc--> ���������!!!!.");
            #endif
        }
        
    }
    else
    {
        #if take_pictures_debug_printf 
        eat_trace("take_picture_action_proc--> ����æ�Ժ�����.");
        #endif
    }
}
/* ===  FUNCTION  ======================================================================
 *  Name: Camer_Frame_Send_Ask_Cmd
 *  Description: ����ͼƬ����
 *  Parameters :  Address    :��ַ 
 *				  Cnt_num	 :�����
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
static void send_ask_camer_frame(uint8_t Address,uint8_t Cnt_num)
 {
 	uint8_t Farm_Buff[6]={0};
 	uint8_t Send_Buff[20]={0};
 	uint16_t D_Lens = 0;//���ݳ���
 	uint16_t XOR_PUS = 0;
 	uint32_t t1,t2;
 	*Farm_Buff = Address;
 	*(Farm_Buff+1) = 0x02;//������������
 	*(Farm_Buff+2) =Address;
 	*(Farm_Buff+3) =Cnt_num;//������	
 	XOR_PUS=*Farm_Buff + *(Farm_Buff+1) + *(Farm_Buff+2) + *(Farm_Buff+3);

 	Big_End_WordtoByte(XOR_PUS,Farm_Buff+4);
 	D_Lens=camer_send_switch(Farm_Buff,Send_Buff,6);

 	
    uart_485_set_send_porc();
	eat_uart_write(EAT_UART_2, Send_Buff, D_Lens);//��������	
	uart_485_set_rev_porc();

	camer_get_farme_timeout_switch(EAT_TRUE);
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Analyze_Camer_RevFrame
 *  Description:  ����������һ֡Tcp�·����� 
 *  Parameters :  pSource    :ԭʼ�������� 
 *				  pLens		   :��Ϣ��ˮ��         
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
static void analyze_camer_rev_frame (uint8_t *pSource,uint16_t pLens)
{
	uint8_t CMD_ID;//����ID
	uint8_t Camer_Adderess;//����ͷ��ַ
	uint16_t Data_Len=0;
	uint16_t Xor_Ret,PLUS_Num;
    static uint16_t temp_name[22]=L"D:\\PIC\\X.JPG";
	Big_End_BytetoWord(pSource+pLens-2,&PLUS_Num);
	Xor_Ret=PLUS_XOR(pSource,pLens-2);//У��
	if(PLUS_Num==Xor_Ret)
	{
		Camer_Adderess=*pSource;//��ַ
		CMD_ID=*(pSource+1);//����
		switch (CMD_ID)
		{
			case 0x00://�յ����ջظ���Ϣ
			{
                picture_rename(multimedia_picture_pb.media_id,temp_name);
                
                Big_End_BytetoWord(pSource+3,&Data_Len);
				get_farm_num = Data_Len/512;
				Data_Len = Data_Len%512;
				if(Data_Len!=0) get_farm_num = get_farm_num+1;//�Ƿ���֡
				
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
			case 0x02://�յ�ͼƬ�ش���Ϣ
			{
                camer_get_farme_timeout_switch(EAT_FALSE);
                
				now_farme_num =*(pSource+2);
				file_write(temp_name,now_farme_num*512,pSource+3,512);//дͼƬ
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
 *  Description:  ����TCPһ֡���������� 
 *  Parameters :  *Rev_Buff        :Ŀ��BUFFER
 *				  *Source_Buff     :Դ���ݰ�
 *				  Re_Lens          :Դ���ݳ���
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-28			 
 * =====================================================================================
 */
void camer_rev_one_packge(uint8_t *Source_Buff,uint16_t Re_Lens)
{
	uint8_t cData=0;
	uint16_t Datat_cnt=0;
    uint16_t Effect_Data_Lens=0;//��Ч�ճ���
    static uint8_t Temp_Dest[640]={0};
    static u16 TotalCnt=0;//��̬�����ܼ���
    static uint8_t Rev_Data_Flag=0;
	static uint8_t get_spec_flag=0;
	uint16_t FE_Cnt=0;
	uint16_t FE_PLUS=0;//У���
	Effect_Data_Lens = Re_Lens;//��ȡ��Ч���ݳ���
	if(Effect_Data_Lens)//�ַ�������
	{	
		for(Datat_cnt=0;Datat_cnt<Effect_Data_Lens;Datat_cnt++)//��֤һ��У��һλ
		{
			cData =*(Source_Buff+Datat_cnt);//��ֵ
			switch(Rev_Data_Flag)
		    {	
				case 0:
				{
					if(cData==Tcp_Fram_X7E)//ͷ֡1
					{
						Rev_Data_Flag=1;
						#if take_pictures_debug_printf
                        eat_trace("camer_rev_one_packge--> receive ��ʼ 0X7E!! !");         
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
                    eat_trace("camer_rev_one_packge--> receive ���� 0X7E!! !");         
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
							FE_PLUS = FE_PLUS-2*(FE_Cnt-2);//У��͸ı�
						}
						else if((*(Temp_Dest+TotalCnt-2)==0x7E)||(*(Temp_Dest+TotalCnt-1)==0x7E))
						{
							FE_PLUS = FE_PLUS-2*(FE_Cnt-1);//У��͸ı�
						}
						else
						{
							FE_PLUS = FE_PLUS - FE_Cnt*2;//У��͸ı�
						}
                        Big_End_WordtoByte(FE_PLUS,Temp_Dest+TotalCnt-2);
						analyze_camer_rev_frame(Temp_Dest,TotalCnt);//���ݷ���
						Rev_Data_Flag=0;
						TotalCnt=0;
						return;
					}	
				}
				else
				{
					if(cData==0x1B&&(!get_spec_flag))//�������ַ�
					{
					    TotalCnt++;
						get_spec_flag=1;
						continue;
					}
					if(get_spec_flag)//�����ַ�β�ַ��ж�
					{
					    if(cData==0x65)
					     {
					     	*(Temp_Dest+TotalCnt-1)=0x7E;
							  get_spec_flag=0; 
							  FE_Cnt++;//FE����
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
 *  Description: ����ͷ����
 *  Parameters :  Address    :��ַ
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
void tcp_take_pictures_server(char_t socket,uint8_t *data_buff,uint16_t servn)//ͨ����һ����δִ���겻������һ��
{
    if(take_attributes.take_time_state==EAT_TRUE)
    {
        tcp_take_picture_result_replay(socket,servn,0x01,0);
    }
    else
    {
    	take_attributes.chanel_id= *data_buff;//ͨ����
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
            take_attributes.pictures_num -= 1;//����
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
 *  Description: ���ճ�ʼ��
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
 *  Description:  ����ͼƬ
 *  Parameters :  TCP����Ӧ��			  	
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
        eat_trace("tcp_take_picture_result_replay -->> ͨ����֧�� !!!.");
        #endif
 	}
    Tcp_Message_PacKage_And_Send(socket,&msg_svn,tcp_take_picture_replay,send_data,offfest,0,0,0);
 }

 /* ===  FUNCTION  ====================================================================
 *  Name: Tcp_DuoMei_SuoJian
 *  Description: ��ý�����
 *  Parameters :  Address :��ַ
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
 void tcp_media_search(char_t socket,uint8_t *data_buf,uint16_t msgsn)
 {
 	uint32_t Stat_Time=0;//��ʼʱ��
 	uint32_t End_Time=0;//����ʱ��
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
        eat_trace("tcp_media_search -->> medid type:ͼƬ!!!.");
        #endif
	    media_cnt = pictures_num_total;
	}
	else if(media_typef==0x01) 
	{
        #if take_pictures_debug_printf 
        eat_trace("tcp_media_search -->> medid type:¼��!!!.");
        #endif
	    media_cnt = voic_num_total;
	}
	for(ii=0;ii<media_cnt;ii++)
	{
        if(media_typef==0)
        {
            file_read(pictures_add_sys,ii*sizeof(sys_add)+sizeof(pictures_num_total),&sys_add,sizeof(sys_add));
            #if take_pictures_debug_printf 
            eat_trace("tcp_media_search -->> medid type:ͼƬ!!! .");
            #endif
        }
        else if(media_typef==1) 
        {
            file_read(voice_record_add_sys,ii*sizeof(sys_add)+sizeof(voic_num_total),&sys_add,sizeof(sys_add));
            #if take_pictures_debug_printf 
            eat_trace("tcp_media_search -->> medid type:¼��!!! .");
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
            		file_read(media_Name,0,&multimedia,sizeof(multimedia));//��������Ϣ
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
                        eat_trace("tcp_media_search-->����ID:%u.jpg !",sys_add.media_id);
                        #endif
                    }
                    else if(media_typef==1) 
                    {
                        #if take_pictures_debug_printf
                        eat_trace("tcp_media_search-->����ID:%u.wav!",sys_add.media_id);
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
 *  Description: ��ý������ϴ�
 *  Parameters :  Address :��ַ
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-29			 
 * =====================================================================================
 */
void save_media_tcp_serch_update(uint8_t *data_buf)
{
    uint32_t Stat_Time=0;//��ʼʱ��
 	uint32_t End_Time=0;//����ʱ��
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
            eat_trace("take_pictures_over_ok_proc -->> ͼƬ���ϴ�����!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,media_typef,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("take_pictures_over_ok_proc -->> �����ϴ�ͼƬ��Ϣ��Ϣ.");
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
        }
	}	
            
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: tcp_single_media_Search
 *  Description: ��������ָ��           
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
            eat_trace("tcp_single_media_Search -->> ͼƬ���ϴ�����!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,0x00,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> �����ϴ�ͼƬ��Ϣ��Ϣ.");
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
            eat_trace("tcp_single_media_Search -->> ¼�����ϴ�����!!!.");
            #endif
            media_anaylsis_list_add(&media_send_list,0x01,sys_add.media_id);
            #if take_pictures_debug_printf 
            eat_trace("tcp_single_media_Search -->> �����ϴ�¼����Ϣ��Ϣ.");
            #endif
            task_msg_send(EAT_USER_1,EAT_USER_1,media_update_msg);
            return;
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: carmer_start_timeout_switch
 *  Description: ���ճ�ʱ��           
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
 *  Description: ���ճ�ʱ����            
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
static void camer_start_timeout_handle(void) //��ʱ������
{
    if(rev_take_ret_data_timeout.time_state==EAT_TRUE)
    {
         rev_take_ret_data_timeout.times--;
         if(rev_take_ret_data_timeout.times==0)
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_start_timeout_handle --> �������η��ͻ�ȡ֡ʧ��ɾ����Ƭ��Ϣ��");
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
 *  Description: ��ȡͼƬ���ݴ�         
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
 *  Description: ��ȡͼƬ����֡��ʱ����         
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
 
static void camer_get_farme_timeout_handle(void) //��ʱ������
{
    if(rev_get_data_timeout.time_state==EAT_TRUE)
    {
         rev_get_data_timeout.times--;
         if(rev_get_data_timeout.times==0)
         {
             #if take_pictures_debug_printf 
             eat_trace("camer_get_farme_timeout_handle --> �������η��ͻ�ȡ֡ʧ�ܣ����ս���");
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
 *  Description:  ����ͼƬ
 *  Parameters :  Time_Out P   ��ʱ��־�ṹ��			  	
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
 *  Description: ��ʱ����������       
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
 *  Description: ��ʱ��10����      
 *       author: Jumping create at 2013-12-16
 * =====================================================================================
 */
void take_pictures_time10_pro(void)
{
     camer_start_timeout_handle();
     camer_get_farme_timeout_handle();
}

