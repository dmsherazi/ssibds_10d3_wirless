

#ifndef _TAKE_PICTURE_H
    #define _TAKE_PICTURE_H

	#include <stdio.h>
	#include <string.h>
	#include"eat_interface.h"
    #include "public_interface.h"
#define take_pictures_debug_printf 1

#define pictures_add_sys	L"D:\\PIC\\pictures_add.bin" 
#define pictures_pic_fold     L"D:\\PIC"

#define PIC_Size_320X240 0x01
#define PIC_Size_640X480 0x02
#define PIC_Size_176X164 0x05
#define PIC_Size_352X248 0x06

#define take_pictures_max 10
/*��������Ӧ��*/
#define tcp_take_picture_replay 0x0805
#define tcp_media_search_replay_id 0x0802

/*����������ʱ*/
typedef struct 
{
  uint8_t time_state; //��־λ״̬
  uint8_t times;//����
  uint32_t send_data_delay;//����������ʱ
}take_pic_time_process;

/***����***/
typedef struct _take_pictures_list
{
   multimedia_add_struct pictures_add;//ͼƬ������Ϣ
   media_add_struct sys_add_pictures;//ϵͳ������Ϣ
   struct list_head re_list;
}take_pictures_list;//tcp���ջ���

/***��������***/
typedef struct 
{
    uint8_t take_time_state;//����
    uint8_t chanel_id;//ͨ��
    uint8_t picture_size;
    uint8_t picture_qulity;
    uint8_t media_event;
    uint8_t process_type;
    uint16_t take_frze;//����Ƶ�ʣ�����
    uint16_t pictures_num;//��Ƭ����
}take_pictures_attributes;//��������

/***��Ƭ����***/
extern uint32_t pictures_num_total;

/***��������***/
extern struct list_head camer_take_list;
extern uint32_t data_delay_test;

/* 
* ===  FUNCTION  ======================================================================
*         Name: take_picture_action_proc
*  Description: ���մ���
*       Output:
*       Return:              
*       author: Jumping create at 2016-12-16
* =====================================================================================
*/
extern void take_picture_action_proc(void);
extern void take_picture_system_add_init(void);
extern void take_pictures_time10_pro(void);
extern void take_pictures_regularly(void);
extern void tcp_take_picture_result_replay(char_t socket,uint16_t msg_svn,uint8_t result,uint16_t pic_cnt);
extern void tcp_take_pictures_server(char_t socket,uint8_t *data_buff,uint16_t servn);
extern void save_media_tcp_serch_update(uint8_t *data_buf);
extern void tcp_single_media_search(uint8_t *scr_data);
extern void tcp_media_search(char_t socket,uint8_t *data_buf,uint16_t msgsn);
extern void camer_rev_one_packge(uint8_t *Source_Buff,uint16_t Re_Lens);
extern void carmer_start_timeout_switch(uint8_t switch_cmd);
extern void camer_get_farme_timeout_switch(uint8_t switch_cmd);


#endif
