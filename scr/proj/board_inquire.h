
#ifndef _BOARD_INQUIRE_H
    #define _BOARD_INQUIRE_H


    #include "eat_interface.h"

/***��ѯ�ն�ID IP �ֻ���Ӧ��***/
#define send_id_ip_inquire 0x66
/***��ѯ��SIM800����汾Ӧ��***/
#define send_sim800_software_vison_inquire 0x84
extern void board_inquire_id_ip_phone_num(void);
/*****************************************************************************
* Function :board_inquire_sim800_software_version
* Description:SIM800����汾.
* Parameters : void
* Returns:
*      Void    
*****************************************************************************/
void board_inquire_sim800_software_version(void);
    #endif
