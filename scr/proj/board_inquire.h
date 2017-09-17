
#ifndef _BOARD_INQUIRE_H
    #define _BOARD_INQUIRE_H


    #include "eat_interface.h"

/***查询终端ID IP 手机号应答***/
#define send_id_ip_inquire 0x66
/***查询终SIM800软件版本应答***/
#define send_sim800_software_vison_inquire 0x84
extern void board_inquire_id_ip_phone_num(void);
/*****************************************************************************
* Function :board_inquire_sim800_software_version
* Description:SIM800软件版本.
* Parameters : void
* Returns:
*      Void    
*****************************************************************************/
void board_inquire_sim800_software_version(void);
    #endif
