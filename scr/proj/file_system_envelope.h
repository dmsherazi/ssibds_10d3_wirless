/********************************************************************
 *                Copyright SSI(wuhan)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   file_system_envelope
 * version       :   0.11
 * Description   :   封装系统自带文件系统。
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    历史变化记录
 *---------------------------------------------------------------------
 *0.10  2015/9/20 19:11:27 (markdown time) , j.x.b originally.
 *--------------------------------------------------------------------
 *0.11  2016/7/20 08:11:27 (markdown time) , j.x.b originally.
 *1、修改封装方式增加错误返回
 *--------------------------------------------------------------------
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef _FILE_SYSTEM_ENVELOPE_H_
#define _FILE_SYSTEM_ENVELOPE_H_

#define File_Fade_Area_Location	L"D:\\first_ip_fade_date.bin" //盲区数据；
#define Second_File_Fade_Area_Location	L"D:\\second_ip_fade_date.bin" //盲区数据；
#define Sleep_Location_Save L"C:\\Sleep_Location_Save.bin"

#include <stdlib.h>
#include <string.h>
#include "eat_modem.h"
#include "eat_interface.h"
#include "eat_fs_errcode.h" 
#include "eat_fs_type.h" 

/********************************************************************
* Types
********************************************************************/
/*文件系统操作返回*/
typedef enum
{
    OPERATION_NO_ERROR = 0,//操作无错误
    OPEN_FILE_ERROR = -1,  //打开错误
    FILE_SEEK_ERROR = -2,
    FILE_COMMIT_ERROR = -3,
    WRITE_FILE_ERROR = -4, //写文件错误
    READ_FILE_ERROR = -5,  //读文件错误
}file_system_cb;



extern void fs_Create_file_flod(const uint16_t *FileName);
extern void file_create(const uint16_t *FileName);
extern file_system_cb get_file_size(const uint16_t *FileName,uint32_t *Size);
extern file_system_cb file_write(const uint16_t *FileName,int32_t Ofest,void *writeBuf, uint32_t Len);
extern file_system_cb file_read(const uint16_t *FileName,int32_t Ofest,void *Readbuffer, uint32_t Len);
#endif
