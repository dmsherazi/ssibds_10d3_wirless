
#include "file_system_envelope.h"
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void Eat_FS_File_read(const unsigned short *FileName,void *DataPtr, UINT Length)//
 *  Description: 
 *             *FileName： the return of Eat_FSFile_Create_and_open()
 *			   Ofest   ：   Ofest of data to be read//偏移量
 *			   Readbuffer    ： the point of buffer which store data to be read
 *			   Len:the size of data to be read
 *		 return void
 *       author: J.x.b  create at 2014-5-21
 * =====================================================================================
 */
file_system_cb get_file_size(const uint16_t *FileName,uint32_t *Size)
 {
 	int32_t FileHandle;
 	FileHandle= eat_fs_Open(FileName,FS_READ_WRITE);//打开文件
 	if(FileHandle < EAT_FS_NO_ERROR)
	{
        #if file_sytem_debug
        eat_trace("get_file_size --> open error:%d !",OPEN_FILE_ERROR);
        #endif
        eat_fs_Close(FileHandle);
        return OPEN_FILE_ERROR;
	}
	else
	{
		eat_fs_GetFileSize(FileHandle,Size);
        #if file_sytem_debug
        eat_trace("get_file_size --> sucess 。");
        #endif
		eat_fs_Close(FileHandle);
		return OPERATION_NO_ERROR;
    }
 }
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void Eat_FS_Create_FOLD(const unsigned short *FileName)
 *  Description: 
 *             FileName： The name of file to be delete     // 内部
 *		
		 return FileHandle
 *       author: J.x.b  create at 2014-5-21
 * =====================================================================================
 */
void fs_Create_file_flod(const uint16_t *FileName)
{
	eat_fs_error_enum fs_Op_ret;//返回值结构体
	fs_Op_ret = (eat_fs_error_enum)fs_Op_ret = eat_fs_CreateDir(FileName);
	if(fs_Op_ret < EAT_FS_NO_ERROR)
	{
		#if file_sytem_debug >0
        eat_trace("fs_Create_file_flod --> fail 。");
		#endif	
	}
	else
	{
		#if file_sytem_debug >0
        eat_trace("fs_Create_file_flod --> sucess 。");
		#endif	
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: int file_create
 *  Description: 
 *             创建文件
 *				
 *			 return FileHandle
 *       author: J.x.b  create at 2014-5-21
 * =====================================================================================
 */
void file_create(const uint16_t *FileName)
{
	int32_t FileHandle;
	FileHandle= eat_fs_Open(FileName,FS_CREATE);
	if(FileHandle < EAT_FS_NO_ERROR)
	{
        #if file_sytem_debug
		eat_trace("file_create --> error_id:%d!",FileHandle);
		#endif
	}
	else
	{
        #if file_sytem_debug
		eat_trace("file_create --> creat sucess。");
		#endif
    }
	eat_fs_Close(FileHandle);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void Eat_FS_File_write(int FileHandle,void *DataPtr, UINT Length)//向文件中写数据
 *  Description: 
 *             FileHandle： the return of Eat_FSFile_Create_and_open()
 *			   DataPtr   ： the point of buffer which store data to be writen  
 *			   Length    ： the size of data to be writen
 *
 *		 return void
 *       author: J.x.b  create at 2014-5-21
 * =====================================================================================
 */
file_system_cb file_write(const uint16_t *FileName,int32_t Ofest,void *writeBuf, uint32_t Len)
{
  int32_t FileHandle;
  uint32_t writedLen;
  int32_t SeekRet;
  eat_fs_error_enum fs_Op_ret;//返回值结构体
  FileHandle= eat_fs_Open(FileName,FS_READ_WRITE);
  if(FileHandle>EAT_FS_NO_ERROR)
  {
  	SeekRet=eat_fs_Seek(FileHandle,Ofest,EAT_FS_FILE_BEGIN);//偏移指针
	if(SeekRet<0)
	{	
        #if file_sytem_debug
        eat_trace("file_write --> seek error id:%d !",FILE_SEEK_ERROR);
        #endif
		eat_fs_Close(FileHandle);
		return FILE_SEEK_ERROR;
	}
	else
	{
		fs_Op_ret=eat_fs_Write(FileHandle,writeBuf,Len,&writedLen);//写bufer
        if(EAT_FS_NO_ERROR!=fs_Op_ret)
        {
            #if file_sytem_debug
            eat_trace("file_write --> write error id:%d !",WRITE_FILE_ERROR);
            #endif
			eat_fs_Close(FileHandle);
            return WRITE_FILE_ERROR;
        }
        else
        {
            fs_Op_ret = eat_fs_Commit(FileHandle);
            if(EAT_FS_NO_ERROR != fs_Op_ret )
            {   
                 #if file_sytem_debug
                 eat_trace("file_write --> Commit error id:%d !",FILE_COMMIT_ERROR);
                 #endif
                 eat_fs_Close(FileHandle);
                 return FILE_COMMIT_ERROR;
            }
            else
            {
                 #if file_sytem_debug
                 eat_trace("file_write --> sucess 。");
                 #endif
                 eat_fs_Close(FileHandle);
                 return OPERATION_NO_ERROR;
            }
        }
	}
  }
  else
  {
    #if file_sytem_debug
    eat_trace("file_write --> open file error id:%d !",OPEN_FILE_ERROR);
    #endif
    return OPEN_FILE_ERROR;
  }

}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void Eat_FS_File_read(const unsigned short *FileName,void *DataPtr, UINT Length)//
 *  Description: 
 *             *FileName： the return of Eat_FSFile_Create_and_open()
 *			   Ofest   ：   Ofest of data to be read//偏移量
 *			   Readbuffer    ： the point of buffer which store data to be read
 *			   Len:the size of data to be read
 *		 return void
 *       author: J.x.b  create at 2014-5-21
 * =====================================================================================
 */
file_system_cb file_read(const uint16_t *FileName,int32_t Ofest,void *Readbuffer, uint32_t Len)
{
  uint32_t readLen;
  int32_t SeekRet;
  int32_t FileHandle;
  eat_fs_error_enum fs_Op_ret;//返回值结构体
  FileHandle= eat_fs_Open(FileName,FS_READ_ONLY);//打开文件
  if(FileHandle > EAT_FS_NO_ERROR)
  {
	SeekRet=eat_fs_Seek(FileHandle,Ofest,EAT_FS_FILE_BEGIN);//偏移指针
	if(SeekRet<0)
	{	
        #if file_sytem_debug
        eat_trace("file_read --> seek error id:%d !",FILE_SEEK_ERROR);
        #endif
		eat_fs_Close(FileHandle);
		return FILE_SEEK_ERROR;
	}
	else
	{
		fs_Op_ret =eat_fs_Read(FileHandle,Readbuffer,Len,&readLen);	
		if(EAT_FS_NO_ERROR!=fs_Op_ret)
		{
            #if file_sytem_debug
            eat_trace("file_read --> read error id:%d !",READ_FILE_ERROR);
            #endif
			eat_fs_Close(FileHandle);
			return READ_FILE_ERROR;
		}
		else
		{
            #if file_sytem_debug
            eat_trace("file_read --> sucess 。");
            #endif
			eat_fs_Close(FileHandle);
			return OPERATION_NO_ERROR;
		}
	}
  }
  else
  {
      #if file_sytem_debug
      eat_trace("file_read --> open file error id:%d !",OPEN_FILE_ERROR);
      #endif
      return OPEN_FILE_ERROR;
  }
}


