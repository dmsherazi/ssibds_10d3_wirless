#ifndef DEMARCATE_DATA_H
    #define DEMARCATE_DATA_H


    #include "eat_interface.h"

#define demarcate_debug_printf 1

#define test_tf_file     L"D:\\test_tf_file.bin"

/***�궨��ϢӦ��***/
#define demarcate_ask 0x85

/***������Ϣ����***/
#define test_data_ret 0x6D
/**�����Ϣ����***/
#define test_data_rev 0x7D

/**��Ʒ�궨***/
#define prout_dedmter 0x7F

/********************************************************************
* External Functions declaration
********************************************************************/
/*�������Ա����*/
extern uint8_t record_test_flag;
extern void board_demarcate_data_pro(uint8_t *param_point,uint16_t data_len);
extern void loca_record_test(uint8_t *pscr);
extern void system_prouct_test(uint8_t data);
extern void system_prouct_demate(uint8_t *data);
extern void test_result_send(uint8_t test_id,uint8_t reslut);
extern void create_send_rs485_frame(void);


#endif
