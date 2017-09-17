
#ifndef _MEDIA_UPDATE_H
#define _MEDIA_UPDATE_H

#include "voice_record_proc.h"


#define media_updata_debug_printf 1

/***�ϴ���ý����Ϣ***/
#define media_send_list_max 20

/***��ý����Ϣÿ���������***/
#define media_package_max 870

/***��ý�峤��ID***/
#define tcp_media_update 0x0801



/***��ý���ϴ�***/
typedef struct _media_send_list
{
   uint8_t media_type;
   uint16_t media_id;//����ͷָ��
   struct list_head re_list;
}media_send_data_pack_list;//tcp���ջ���

/***��ý���ϴ���������***/
extern struct list_head media_send_list;

extern void media_update_pro_list_init(void);
extern void media_update_pro(uint16_t socket_t);
extern void media_anaylsis_list_add(struct list_head *list_parm,uint8_t media_type,uint16_t media_id);

#endif

