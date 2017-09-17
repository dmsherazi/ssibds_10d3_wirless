
#ifndef _DERIVER_IC_CARD_UPDATE_H
#define _DERIVER_IC_CARD_UPDATE_H

#define SSI_EAT_IC_DEBUG 1

#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

/*IC����Ϣ����*/
#define tcp_ic_ask_id  0x8702	

/*IC����Ϣ�ظ�*/
#define tcp_ic_replay_id  0x0702	

/*�����˵��ϱ�*/
#define tcp_dianzi_yundan_id 0x0701 

typedef struct _ic_card_data /*IC������*/
{
	uint8_t Lens;//���ݳ���
	uint8_t IC_Data_buf[70];
}driver_ic_card_data;

extern driver_ic_card_data ic_card_data;

extern void driver_ic_card_data_report(uint8_t *buffer,uint8_t Dection);
extern void tcp_driver_data_ask_proc(uint8_t socket);

#endif

