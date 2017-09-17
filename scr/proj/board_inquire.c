

#include "board_inquire.h"

/*****************************************************************************
 * Function :board_inquire_id_ip_phone_num
 * Description:查询ID IP 端口号 手机号码.
 * Parameters : void
 * Returns:
 *  	Void 	
 *****************************************************************************/
void board_inquire_id_ip_phone_num(void)
{
	uint8_t offest=0;
	uint8_t send_data[128];
	uint8_t chang_string[10]={0};
	
	memcpy(send_data,system_param.SaveTerminlPhone.terminl_phone_nums,16);
	offest += 16;

	memset(chang_string,0,strlen(chang_string));
	Data_TO_Char(system_param.SaveServerParameterArray[MAIN_SERVER].server_socket,chang_string);
	memcpy(send_data + offest,chang_string,6);
	offest +=6;

	memcpy(send_data + offest,system_param.SaveServerParameterArray[MAIN_SERVER].server_domin,32);
	offest +=32;
	
	memset(chang_string,0,strlen(chang_string));
	Data_TO_Char(system_param.SaveServerParameterArray[SECOND_SERVER].server_socket,chang_string);
	memcpy(send_data + offest,chang_string,6);
	offest +=6;
	
	memcpy(send_data + offest,system_param.SaveServerParameterArray[SECOND_SERVER].server_domin,32);
	offest +=32;
	
	*(send_data + offest) = 0;
	offest +=1;
	
    uart1_send_data_frame(send_id_ip_inquire,send_data,offest);
}
/*****************************************************************************
 * Function :board_inquire_sim800_software_version
 * Description:SIM800软件版本.
 * Parameters : void
 * Returns:
 *  	Void 	
 *****************************************************************************/
void board_inquire_sim800_software_version(void)
{
	uint8_t offest=0;
	uint8_t send_data[128];
 	send_data[0]=strlen(system_param.terminl_registe.softerware_vison);
	offest +=1;
 	memcpy(send_data+offest,system_param.terminl_registe.softerware_vison,strlen(system_param.terminl_registe.softerware_vison));
 	offest +=strlen(system_param.terminl_registe.softerware_vison);
    uart1_send_data_frame(send_sim800_software_vison_inquire,send_data,offest);
}

