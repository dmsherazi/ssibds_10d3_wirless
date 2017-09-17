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

#ifndef ANALYZE_SEND_CAN_DATA_H
   #define ANALYZE_SEND_CAN_DATA_H



/********************************************************************
 * Include Files
 ********************************************************************/
 #include "eat_interface.h"
 #include "math.h"

/********************************************************************
* Macros
 ********************************************************************/
#define One_Package_Max 960
#define Max_para_Send 160

#define can_param_max 200
#define can_send_list_max 101

#define tcp_can_data_send_id  0xd003//0x0705
#define dbc_data_load 0xd101
#define Remote_update_id 0x8108
/********************************************************************
* Types
********************************************************************/
/***解析后要发送数据***/
typedef struct
{
 uint16_t param_num;
 uint8_t curren_time[6];
 uint8_t param_buff[can_param_max*6];
}analysised_can_param_struct;


typedef struct 
{
    analysised_can_param_struct analysised_can_param;
    struct list_head re_list;
}analysised_can_param_list;
/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/

extern analysised_can_param_struct can_data_send;
/***待发送解析CAN数据链表***/
extern struct list_head analysised_can_list;
/***发送CAN数据流水号***/
extern uint16_t can_data_send_serial_num;
extern uint16_t second_can_data_send_serial_num;


extern uint16_t can_update_fre_zero;
extern uint16_t can_update_fre_one;

/********************************************************************
* Local Variables:  STATIC
********************************************************************/
typedef struct 
{
    uint8_t byte_num;
    uint8_t offest;
}Can_data_struct;
/********************************************************************
* External Functions declaration
********************************************************************/


    
/********************************************************************
* Local Function declaration
********************************************************************/
extern void can_send_pro_list_init(void);
extern void idle_update_can_fade_data(void);
extern void analyze_can_data(uint16_t id_num,uint8_t *data_buf);
extern void idle_update_can_data_proc(int16_t d_socket);
extern void can_send_pro_list_add(struct list_head *list_parm);
extern void can_send_pro_list_del(struct list_head *list_parm);
extern void can_send_update_pro(int16_t t_socket,struct list_head *list_parm);  
#endif



