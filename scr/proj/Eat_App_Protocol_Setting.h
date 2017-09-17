
 #ifndef EAT_APP_PROTOCOL_SETTING_H
	#define EAT_APP_PROTOCOL_SETTING_H

	#include <stdio.h>
	#include <string.h>
	#include"eat_interface.h"


#define remote_data_setting 1
extern void tcp_seting_param_proc(uint8_t socket,uint8_t *Temp_data_buff);
extern void tcp_all_data_request(int16_t l_socket,u16 Servn);
extern void tcp_request_param_serven(int16_t l_socket,uint8_t *Temp_data_buff,uint16_t Servn);
extern void seting_data_to_board(uint8_t Data);
extern void tcp_vdr_attributes_request(uint8_t socket);
extern void tcp_car_ctrl_proc(uint8_t socket,uint16_t msg_svn,int8_t *temp_buf);
extern void remote_vdr_ctrl_proc(uint8_t *data_rev);


#endif 

