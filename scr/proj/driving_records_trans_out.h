#ifndef _DRING_RECORDS_TRANS_OUT_H
    #define _DRING_RECORDS_TRANS_OUT_H


#include"eat_interface.h"

#define dring_record_debug_printf 0

/*SIM800���յ�������������*/
#define sim800_dring_record_inquire 0x62 

#define inquire_driver_data_ref 0x0700

/*������ʻ��¼�����ݿ���*/
#define gb19056_dring_data_switch 0

/*808��ʻ��¼�����ݿ���*/
#define jt_808dring_data_switch 0

/*������ʻ��¼�����ļ�*/
#define package_driver_record_file L"D:\\driver_record\\package_driver_record.bin"

/*****��ʻ�ٶȼ�¼(08H)�ļ�****/
#define driver_record_08H L"D:\\driver_record\\driver_record_08H.bin"
/*****λ����Ϣ��¼(09H)�ļ�****/
#define driver_record_09H L"D:\\driver_record\\driver_record_09H.bin"
/*****�¹��ɵ��¼(10H)****/
#define driver_record_10H L"D:\\driver_record\\driver_record_10H.bin"
/*****�¹��ɵ��¼(11H)****/
#define driver_record_11H L"D:\\driver_record\\driver_record_11H.bin"
/*��ʻ����ݼ�¼(12H)*/
#define driver_record_12H L"D:\\driver_record\\driver_record_12H.bin"
/*�ⲿ�����¼(13H)*/
#define driver_record_13H L"D:\\driver_record\\driver_record_13H.bin"
/*�����޸ļ�¼(14H)*/
#define driver_record_14H L"D:\\driver_record\\driver_record_14H.bin"
/*�ٶ�״̬��־(15H)*/
#define driver_record_15H L"D:\\driver_record\\driver_record_15H.bin"



/*��ʻ��¼������У��ö��*/
typedef enum
{
    check_ok=0,
    check_set_error=0x01,
    check_inquire=0x02
}driver_data_chek_enum;

/*�ɼ���ʻ��¼���ݶ�����Ϣ*/
#define dring_record_max 11 
/*808��ʻ��¼�����ݻ���*/
typedef struct
{
    uint8_t dring_record_cmd;
    uint16_t inquire_servn_num;
    uint8_t data_len;
    uint8_t data_buff[64];
}dring_record_sturct;

/*808��ʻ��¼�ǲ�ѯ����*/
typedef struct
{
    uint8_t queuesize;   //����Ĵ�С
    uint8_t head, tail;  //���е�ͷ��β�±�
    dring_record_sturct dring_recod_arry[dring_record_max];
    uint8_t inquire_queue_state;
}inquire_dring_record_queue;

extern inquire_dring_record_queue inquire_dring_record;
extern uint8_t inquire_record_ip;
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Tcp_Driver_Data_Trans
 *  Description:  ��ʻ��¼���ݵ���
 *  Parameters :  
 *				  *Pbuffer ��Դ����Pubffer
 *                *Lens    ���ݳ���
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void tcp_driver_data_update(uint8_t *buffer,uint16_t Lens);

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: hy_dring_record_data_inquire_proc
 *  Description:  ����ƽ̨��ѯ��ʻ��¼����
 *  Parameters :  
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-7-7			 
 * =====================================================================================
 */
extern void hy_dring_record_data_inquire_proc(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_proc
  *  Description:  ������ʻ��¼����
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void inquire_dring_record_queue_proc(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: inquire_dring_record_queue_init
  *  Description:  ��ʼ��inquire_dring_record ���� ��ʻ��¼���ݲ�ѯ����
  *  Parameters :void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void inquire_dring_record_queue_init(void);

/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  ������ʻ��¼���ݲ�ѯ����
  *  Parameters :dr_cmd:��ʽ��¼���servn_num:��ˮ�� data:���ݰ����� data_len:���ݳ���
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern  void enter_inquire_dring_record_queue(uint8_t dr_cmd,uint16_t servn_num,uint8_t *data,uint8_t data_len);
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: enter_inquire_dring_record_queue
  *  Description:  ����ʻ��¼���ݲ�ѯ����
  *  Parameters : void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern void exit_inquire_dring_record_queue(void);
/* 
  * ===  FUNCTION  ======================================================================
  *  Name: is_inquire_dring_record_queue_empty
  *  Description:  ��ʻ��¼��ѯ�����Ƿ�Ϊ��
  *  Parameters : void
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
  * =====================================================================================
  */ 
extern  char_t is_inquire_dring_record_queue_empty(void);
 /* 
   * ===  FUNCTION  ======================================================================
   *  Name: is_inquire_dring_record_queue_full
   *  Description:  ��ʻ��¼��ѯ�����Ƿ�Ϊ��
   *  Parameters : void
   *  Return     :  void 
   *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
   * =====================================================================================
   */ 
extern char_t is_inquire_dring_record_queue_full(void);

#endif
