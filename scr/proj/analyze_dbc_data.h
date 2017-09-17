#ifndef ANALYZE_CAN_DATA_H
   #define ANALYZE_CAN_DATA_H



/********************************************************************
 * Include Files
 ********************************************************************/
 
   #include "eat_interface.h"
   #include <stdlib.h>

/********************************************************************
* Macros
 ********************************************************************/
#define Signal_Max 64

#define Message_Max 60 


#define dbc_data_Issued 0xD101
#define dbc_data_reslut_up 0xD001
#define dbc_data_vision_up 0xD002
#define dbc_data_vision_ask 0xD102


#define DBC_FILE	L"D:\\DBC_FILE.DBC" //dbc数据

#define DBC_MDF_FILE	L"D:\\DBC_mdf.mdf" //dbc数据

#define dbc_struct_save L"C:\\dbc_struct_save.bin"

/********************************************************************
* Types
********************************************************************/


/********************************************************************
typedef struct;
标准:
signal = 'SG_' signal_name multiplexer_indicator ':' start_bit '|'
       signal_size '@' byte_order value_type '(' factor ',' offset ')'
       '[' minimum '|' maximum ']' unit receiver {',' receiver} ;
signal_name = C_identifier ;
自定义:

BO_ 2365484270 TC01: 8 ECU
SG_ 450◎车速_W : 48|16@1+(0.00390625,0)[0|250.996] "km/h" Vector__XXX
*********************************************************************/

typedef struct
{
    uint16_t signal_id;//参数ID对
    uint8_t start_bit;//
    uint8_t signal_size;
    uint8_t byte_order;
    float32_t factor;
    float32_t offset;
}CanSig_Struct;
typedef struct 
{
  uint32_t message_id;
  uint8_t message_size;
  uint8_t message_attribute;
  uint16_t message_get_fre;
  uint8_t chanal;//通道；
  uint8_t signal_num;//参数个数；
  CanSig_Struct signal_array[Signal_Max];
}DbcData_Struct;
typedef struct
{
    char_t HSBaudrate;
    char_t MSBaudrate;
    char_t LSBaudrate;
    char_t ESBaudrate;
}CanBusBaudrate;

typedef struct 
{
   uint8_t dbc_vison[64];
   uint8_t save_message_num;
   CanBusBaudrate chanel_baudrate;
   DbcData_Struct DbcParmSaveArray[Message_Max];
}DbcDataSave_Struct;
typedef struct
{
    uint8_t block_buffer[250];
    uint8_t block_length;
}DBCFileRead_Struct;


extern DbcDataSave_Struct DbcDataSave;

extern void dbc_find_start(void);
extern void dbc_file_analyze(void);
extern void dbc_file_read_line(void);
extern void board_can_inf_setting(void);
extern void dbc_recive_and_save(const unsigned short *FileName,uint16_t package_num,uint16_t package_cn,uint8_t *buffer,uint16_t len);
extern void dbc_file_read_init(void);//dbc 配置文件读取 分组
extern void dbc_receive_reslut_update(uint8_t reslut);
extern void dbc_version_update(void);


#endif
