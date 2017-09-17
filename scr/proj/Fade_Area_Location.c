
/********************************************************************
 *                Copyright SSI(wuhan)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   Fade_Area_Location.c
 * version       :   0.20
 * Description   :   Fade Area location  save and send;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.20  2015/9/20 19:11:27 (markdown time) , j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/


 /****************************************************
 * Include Files
 ****************************************************/ 
#include "Fade_Area_Location.h"

 /****************************************************
* External variables declaration
*****************************************************/

uint16_t fade_location_head=0;//位置信息保存文件头
uint16_t fade_location_tail=0;//位置信息保存文件尾
uint16_t second_fade_location_head=0;
uint16_t second_fade_location_tail=0;//文件尾

/*****************************************************
* Local variables declaration(Static)
******************************************************/
/*****************************************************************************
* Function : Exit_Location_Buffer_Queue
* Description:出队列储存.//文件中的盲区数据
* Parameters : void
* Returns:
*        void  
* NOTE
*     Sameting you want to tell somebody who modify
*****************************************************************************/
void Clear_Location_data_save(void)
{
      if(fade_location_tail == fade_location_head)
      {
      }
      else
      {
           fade_location_head =(fade_location_head+1)%Fade_Area_Data_MAX;
           file_write(File_Fade_Area_Location,0,&fade_location_head,sizeof(fade_location_head));
      }
}
void second_Clear_Location_data_save(void)
{
      if(second_fade_location_tail == second_fade_location_head)
      {
      }
      else
      {
           second_fade_location_head =(second_fade_location_head+1)%Fade_Area_Data_MAX;
           file_write(Second_File_Fade_Area_Location,0,&second_fade_location_head,sizeof(second_fade_location_head));
      }
}

/*****************************************************************************
* Function : Exit_Location_Buffer_Queue
* Description:出队列储存.//文件中的盲区数据
* Parameters : void
* Returns:
*        void  
* NOTE
*     Sameting you want to tell somebody who modify
*****************************************************************************/

void Save_Fade_Location_data(void)
{
     char FS_Ret;
     uint16_t New_Tail=0;
     New_Tail = (fade_location_tail+ 1)%Fade_Area_Data_MAX;
     if(New_Tail == fade_location_head)
     {
         Clear_Location_data_save();
     }
     FS_Ret=file_write(File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*fade_location_tail*sizeof(Location_Queue.Location_Save_Buffer[0]),&Location_Queue.Location_Save_Buffer[Location_Queue.Head],(Location_Queue_Max - 1)*sizeof(Location_Queue.Location_Save_Buffer[0]));
     if(FS_Ret!=0)
     {
          FS_Ret=file_write(File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*fade_location_tail*sizeof(Location_Queue.Location_Save_Buffer[0]),&Location_Queue.Location_Save_Buffer[Location_Queue.Head],(Location_Queue_Max - 1)*sizeof(Location_Queue.Location_Save_Buffer[0]));
          if(FS_Ret!=0) return;
     }
     fade_location_tail = New_Tail;
     file_write(File_Fade_Area_Location,2,&fade_location_tail,sizeof(fade_location_tail));
}
void Second_Fade_Area_Location_Save(void)
{
    char FS_Ret;
    uint16_t New_Tail=0;
    New_Tail = (second_fade_location_tail+ 1)%Fade_Area_Data_MAX;
    if(New_Tail == second_fade_location_head)
    {
        Clear_Location_data_save();
    }
    FS_Ret=file_write(Second_File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*second_fade_location_tail*sizeof(Second_Location_Queue.Location_Save_Buffer[0]),&Second_Location_Queue.Location_Save_Buffer[Second_Location_Queue.Head],(Location_Queue_Max - 1)*sizeof(Second_Location_Queue.Location_Save_Buffer[0]));
    if(FS_Ret!=0)
    {
         FS_Ret=file_write(Second_File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*second_fade_location_tail*sizeof(Second_Location_Queue.Location_Save_Buffer[0]),&Second_Location_Queue.Location_Save_Buffer[Second_Location_Queue.Head],(Location_Queue_Max - 1)*sizeof(Second_Location_Queue.Location_Save_Buffer[0]));
         if(FS_Ret!=0) return;
    }
    second_fade_location_tail = New_Tail;
    file_write(Second_File_Fade_Area_Location,2,&second_fade_location_tail,sizeof(second_fade_location_tail));
}

/*****************************************************************************
* Function : Fade_Area_Location_Send
* Description:This function is used to save Fade area location send.//文件中的盲区数据
* Parameters : void
* Returns:
*        void  
* NOTE
*     Sameting you want to tell somebody who modify
*****************************************************************************/
void File_Fade_Area_Location_Send(void)
{
     u8 Data_Cnt=0;
     char Fs_Ret=0;
     u16 Data_Lens = 0;
     uint16_t msg_svn;
     static u8 Temp_Send_Data[1024]={0};
     BaseLocationSave_Struct Temp_File_Location_Arry[Location_Queue_Max - 1];///////////
     #if APP_TRACE_DBG 
     eat_trace("First_Server_Idle_Location_Inf_Send --> State:blind location data updata:head:%d,tail:%d",fade_location_head,fade_location_tail);
     #endif
     Data_Cnt = (Location_Queue_Max - 1);
     Fs_Ret=file_read(File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
     if(Fs_Ret!=0)
     {
          Fs_Ret=file_read(File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
          if(Fs_Ret!=0)
          {
              Fs_Ret=file_read(File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
              if(Fs_Ret!=0) return;
          }
     }
     Big_End_WordtoByte(Data_Cnt,Temp_Send_Data);
     Data_Lens +=2;
     *(Temp_Send_Data + Data_Lens) = 0x01;
     Data_Lens += 1;
     for(Data_Cnt=0;Data_Cnt < Location_Queue_Max - 1;Data_Cnt++)
     {
         Big_End_WordtoByte(Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens,Temp_Send_Data + Data_Lens);
         Data_Lens +=2;
         memcpy(Temp_Send_Data + Data_Lens,Temp_File_Location_Arry[Data_Cnt].Location_Inf,Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens);
         Data_Lens += Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens;
     }
     Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msg_svn,0x0704,Temp_Send_Data,Data_Lens,EAT_FALSE,0,0);
}

void Second_File_Fade_Area_Location_Send(void)
{
    u8 Data_Cnt=0;
     char Fs_Ret=0;
     u16 Data_Lens = 0;
     uint16_t msg_svn;
     u8 Temp_Send_Data[768]={0};
     BaseLocationSave_Struct Temp_File_Location_Arry[Location_Queue_Max - 1];///////////
     eat_trace("Second_File_Fade_Area_Location_Send --> State:blind location data updata:head:%d,tail:%d",second_fade_location_head,second_fade_location_tail);
     Data_Cnt = (Location_Queue_Max - 1);
     Fs_Ret=file_read(Second_File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*second_fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
     if(Fs_Ret!=0)
     {
          Fs_Ret=file_read(Second_File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*second_fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
          if(Fs_Ret!=0)
          {
              Fs_Ret=file_read(Second_File_Fade_Area_Location,Location_data_save_offest + (Location_Queue_Max-1)*second_fade_location_head*sizeof(Temp_File_Location_Arry[0]),&Temp_File_Location_Arry[0],(Location_Queue_Max - 1)*sizeof(Temp_File_Location_Arry[0]));
              if(Fs_Ret!=0) return;
          }
     }
     Big_End_WordtoByte(Data_Cnt,Temp_Send_Data);
     Data_Lens +=2;
     *(Temp_Send_Data + Data_Lens) = 0x01;
     Data_Lens += 1;
     for(Data_Cnt=0;Data_Cnt < Location_Queue_Max - 1;Data_Cnt++)
     {
         Big_End_WordtoByte(Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens,Temp_Send_Data + Data_Lens);
         Data_Lens +=2;
         memcpy(Temp_Send_Data + Data_Lens,Temp_File_Location_Arry[Data_Cnt].Location_Inf,Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens);
         Data_Lens += Temp_File_Location_Arry[Data_Cnt].Location_Inf_Lens;
     }
     Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&msg_svn,0x0704,Temp_Send_Data,Data_Lens,EAT_FALSE,0,0);
}

/*****************************************************************************
 * Function : Temp_Fade_Area_Location_Send
 * Description:This function is used to send Temp Fade area location send.
 * Parameters : void
 * Returns:
 *  	  void	
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
void Temp_Fade_Area_Location_Send(void)
{
	uint16_t Data_Cnt=0;
	uint16_t Data_Lens = 0;
	uint16_t msgservn=0;
	uint8_t Temp_Send_Data[1024]={0};
	Data_Cnt =(uint16_t)Location_Queue.Tail;
	Big_End_WordtoByte(Data_Cnt,Temp_Send_Data);
	Data_Lens +=2;
	*(Temp_Send_Data + Data_Lens) = 0x00;
	Data_Lens += 1;
	for(Data_Cnt=0;Data_Cnt < Location_Queue.Tail;Data_Cnt++)
	{
		Big_End_WordtoByte(Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens,Temp_Send_Data + Data_Lens);
		Data_Lens+=2;
		memcpy(Temp_Send_Data + Data_Lens,Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf,Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens);
		Data_Lens += Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens;
	}
	Tcp_Message_PacKage_And_Send(Main_Link.mode_Socket,&msgservn,0x0704,Temp_Send_Data,Data_Lens,EAT_FALSE,0,0);
}
void Second_Temp_Fade_Area_Location_Send(void)
{
    uint16_t Data_Cnt=0;
    uint16_t Data_Lens = 0;
    uint16_t msgservn=0;
    uint8_t Temp_Send_Data[1024]={0};
	Data_Cnt =(uint16_t)Second_Location_Queue.Tail;
	Big_End_WordtoByte(Data_Cnt,Temp_Send_Data);
	Data_Lens +=2;
	*(Temp_Send_Data + Data_Lens) = 0x00;
	Data_Lens += 1;
	for(Data_Cnt=0;Data_Cnt < Second_Location_Queue.Tail;Data_Cnt++)
	{
		Big_End_WordtoByte(Second_Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens,Temp_Send_Data + Data_Lens);
		Data_Lens+=2;
		memcpy(Temp_Send_Data + Data_Lens,Second_Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf,Second_Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens);
		Data_Lens += Second_Location_Queue.Location_Save_Buffer[Data_Cnt].Location_Inf_Lens;
	}
	Tcp_Message_PacKage_And_Send(Second_Link.mode_Socket,&msgservn,0x0704,Temp_Send_Data,Data_Lens,EAT_FALSE,0,0);
}

void fade_data_read(void)
{
    file_read(File_Fade_Area_Location,0,&fade_location_head,sizeof(fade_location_tail));
    file_read(File_Fade_Area_Location,2,&fade_location_tail,sizeof(fade_location_tail));

    file_read(File_Fade_Area_Location,0,&second_fade_location_head,sizeof(second_fade_location_tail));
    file_read(File_Fade_Area_Location,2,&second_fade_location_tail,sizeof(second_fade_location_tail));
}
void fade_data_init(void)
{
    
    if(system_param.param_save.statue.wether_first_power_on==EAT_FALSE)
    {
        
        file_create(File_Fade_Area_Location);
        fade_location_tail=0;
        fade_location_head=0;
        file_write(File_Fade_Area_Location,0,&fade_location_head,sizeof(fade_location_tail));
        file_write(File_Fade_Area_Location,2,&fade_location_tail,sizeof(fade_location_tail));
        
        file_create(Second_File_Fade_Area_Location);
        second_fade_location_head=0;
        second_fade_location_tail=0;//文件尾
        file_write(Second_File_Fade_Area_Location,0,&second_fade_location_head,sizeof(second_fade_location_head));
        file_write(Second_File_Fade_Area_Location,2,&second_fade_location_tail,sizeof(second_fade_location_tail));
    }
    else
    {
        fade_data_read();
    }
}
