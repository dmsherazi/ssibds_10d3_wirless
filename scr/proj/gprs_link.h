/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   xxxxxx.c
 * version       :   0.1
 * Description   :   Founction of this Source of C;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2016/9/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef GPRS_LINK_H
   #define GPRS_LINK_H



/********************************************************************
 * Include Files
 ********************************************************************/
       #include "eat_interface.h"
 

#define gprs_link_debug_printf 1
/********************************************************************
* Macros
 ********************************************************************/

/********************************************************************
* Types
********************************************************************/
/*����״̬����*/
typedef enum
{
    deactivated = 0x00, // ע��
    activating = 0x01,  //���ڼ���
    activated = 0x02,   //����OK
    deactivating =0x04,
    bear_other
}gprs_bear_cb;

/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/


extern PresentLinkParameter_Struct Main_Link;//������
extern PresentLinkParameter_Struct Second_Link;


extern GprsBearerParameter_Struct MainBeareParameter; //����APN����
extern GprsBearerParameter_Struct BackupBeareParameter;//��APN����
/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void gprs_bearer_open(u8 *apn,u8 *user, u8 *password);
extern void link_first_server_proc(void);   
extern void link_second_server_proc(void);
extern eat_bool close_gprs_bearer(void);
extern eat_bool socket_close(char_t local_socket);
extern void close_secondip_link(void);
extern void close_frist_iplink(void);
extern char wehther_launch_tcp_conect(LinkIndex_Enum index);


/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    


