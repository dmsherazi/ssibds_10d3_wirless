

#ifndef _VOICE_RECORD_PROC_H
    #define _VOICE_RECORD_PROC_H

/********************************************************************
 * Include Files
 ********************************************************************/
 
#include "eat_interface.h"


/********************************************************************
* Macros
 ********************************************************************/
#define voice_record_debug_printf 1

#define voice_record_add_sys	L"D:\\REC\\voice_record_add.bin" 
#define voice_rec_fold     L"D:\\REC"

/********************************************************************
* Types
********************************************************************/

/*¼����ʼ���� ���ֽ�Ϊ������Ϣ*/
extern uint8_t at_cmd_record_fram[29];
extern uint16_t voice_Name[22]; //��ǰ��������
extern uint16_t voice_Name_add[22]; //¼����Ϣ������Ϣ

/*¼����Ϣ������Ϣ*/
extern multimedia_add_struct multimedia_voice;
/*¼����Ϣ������*/
extern uint32_t voic_num_total;

/*¼����Ϣ��*/
extern media_add_struct voice_record_sys_add;

/* 
* ===  FUNCTION  ======================================================================
*  Name: voice_record_system_add_init
*  Description: ¼��ϵͳ��ʼ��
*  Parameters :  Data    void
*  Return     :  void 
*  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
* =====================================================================================
*/ 
extern void voice_record_rename(uint32_t voice_id);
extern void voice_record_add_rename(uint32_t voice_id);
extern void voice_record_system_add_init(void);
extern uint8_t voice_record_trigged(uint8_t media_event,uint16_t record_timer,uint8_t quation,uint8_t process_type);
extern void strstr_voice_record_cmd_cb_proc(uint8_t *p_str);
extern void tcp_voice_record_proc(char socket,uint8_t *tcp_buffer,uint16_t msg_servn);

#endif

