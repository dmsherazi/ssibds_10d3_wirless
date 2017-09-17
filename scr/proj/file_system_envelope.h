/********************************************************************
 *                Copyright SSI(wuhan)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   file_system_envelope
 * version       :   0.11
 * Description   :   ��װϵͳ�Դ��ļ�ϵͳ��
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    ��ʷ�仯��¼
 *---------------------------------------------------------------------
 *0.10  2015/9/20 19:11:27 (markdown time) , j.x.b originally.
 *--------------------------------------------------------------------
 *0.11  2016/7/20 08:11:27 (markdown time) , j.x.b originally.
 *1���޸ķ�װ��ʽ���Ӵ��󷵻�
 *--------------------------------------------------------------------
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef _FILE_SYSTEM_ENVELOPE_H_
#define _FILE_SYSTEM_ENVELOPE_H_

#define File_Fade_Area_Location	L"D:\\first_ip_fade_date.bin" //ä�����ݣ�
#define Second_File_Fade_Area_Location	L"D:\\second_ip_fade_date.bin" //ä�����ݣ�
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
/*�ļ�ϵͳ��������*/
typedef enum
{
    OPERATION_NO_ERROR = 0,//�����޴���
    OPEN_FILE_ERROR = -1,  //�򿪴���
    FILE_SEEK_ERROR = -2,
    FILE_COMMIT_ERROR = -3,
    WRITE_FILE_ERROR = -4, //д�ļ�����
    READ_FILE_ERROR = -5,  //���ļ�����
}file_system_cb;



extern void fs_Create_file_flod(const uint16_t *FileName);
extern void file_create(const uint16_t *FileName);
extern file_system_cb get_file_size(const uint16_t *FileName,uint32_t *Size);
extern file_system_cb file_write(const uint16_t *FileName,int32_t Ofest,void *writeBuf, uint32_t Len);
extern file_system_cb file_read(const uint16_t *FileName,int32_t Ofest,void *Readbuffer, uint32_t Len);
#endif
