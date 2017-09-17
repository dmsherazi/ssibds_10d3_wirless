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
 *0.10  2016/3/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef THREAD_COMUN_MSG
   #define THREAD_COMUN_MSG



/********************************************************************
 * Include Files
 ********************************************************************/
 #include "eat_interface.h"
 

/********************************************************************
* Macros
 ********************************************************************/
#define thread_msg_por_debug_printf 1

/********************************************************************
* Types
********************************************************************/
typedef enum
{
    StartBearerOpen=0,//��ʼ����
    LinkFisrtServer,    //���ӵ�һ������
    LinkSecondServer,    //���ӵڶ�������
    FirstPlatformReg,    //ע���һ������
    SecondPlatformReg,    //ע��ڶ�������
    closet_socket,//�رն˿�
    Timer3Start,
    Timer4Start,
    Timer5Start,
    Timer6Start,
    
    SystemSleepMsg, //ϵͳ����USER0
    SystemWakeup,
    
    FirstSeverSleepMsgProc,//���ߴ����һ������
    SeconSeverdSleepMsgProc,//���ߴ���ڶ�������
    dbc_file_read_msg,
    dbc_file_analyze_msg,
    dbc_file_analyze_end_msg,
    analyze_can_data_msg,

    driver_record_data_proc,//��ʻ��¼���ݵ�������
    media_update_msg,//��ý�������ϴ�
    take_pictures_timeout,//���ճ�ʱ����
    take_pictures_action,//����
    uart_485_data_send
}ThreadMsgName;

/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/

/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void thread_msg_proc_main(uint8_t msg_id);
extern void thread_msg_proc_user1(uint8_t msg_id);
extern void thread_msg_proc_user2(uint8_t msg_id);
extern void thread_msg_proc_user3(uint8_t msg_id);
extern void system_user1_wake_up(void);
extern void system_user0_wake_up(void);
extern void task_msg_send(EatUser_enum user_src, EatUser_enum user_dst,ThreadMsgName data);

    
/********************************************************************
* Local Function declaration
********************************************************************/

     
#endif


