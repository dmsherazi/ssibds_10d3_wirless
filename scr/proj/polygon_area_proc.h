
#ifndef POLYGON_AREA_PROC_H 
  #define POLYGON_AREA_PROC_H

  

  
  #include <stdio.h>
  #include <string.h>
  #include"eat_interface.h"
  #define polygon_area_proc_printf 1

  
  #define Duo_Max 2
  #define Duo_Point_Max 20
  
  #define Tcp_QuYu_Duobian_Set_ID 0x8604//�������������ID
  #define Tcp_QuYu_Duobian_Delet_ID 0x8605//���������ɾ��ID
  
  #define Qu_Yu_Save_File_duobian L"Qu_Yu_Save_File_duobian.bin"//��������򴢴�
  
  typedef struct _Qu_YU_Duo //Բ������ṹ��
  {
      uint32_t Duo_ID;//id
      uint16_t DUO_Shuxing;//����
      uint8_t  Time_On[6];
      uint8_t  Time_OFF[6];
      uint16_t Max_Speed;//����ٶ�
      uint8_t  Time_O;//����ʱ�� ����
      uint16_t Point_Cnt;//�������
      uint32_t Point_Savep[Duo_Point_Max];//����ʮ����
  }Qu_YU_Duo;
  
  extern Qu_YU_Duo QuYu_Duo[Duo_Max];
  extern uint8_t Duo_QuYu_Cnt;
  extern void Tcp_QuYu_Duo_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
  extern void  Tcp_QuYu_Delet_Duo_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
  extern void Analyze_Duobian_QuYu(Qu_YU_Duo *Dest_Struct,u8 *psouce);
  extern void File_Analyze_QuYu_Struct_Duo(void);

  #endif
