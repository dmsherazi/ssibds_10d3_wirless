
#ifndef  _RECTANGLE_H
    #define _RECTANGLE_H

    #include <stdio.h>
    #include <string.h>
    #include"eat_interface.h"

    #define rectangle_are_pro_printf 1
    
    #define Tcp_QuYu_JuXing_Set_ID 0x8602// ������������ID
    #define Tcp_QuYu_JuXing_Delet_ID 0x8603//��������ɾ��ID

    #define Ju_Max 10
    #define Qu_Yu_Save_File_juxing L"Qu_Yu_Save_File_juxing.bin"//���򴢴��ļ�

    typedef struct _Qu_YU_JU //Բ������ṹ��
    {
    	uint32_t Ju_ID;//id
    	uint16_t Ju_Shuxing;//����
    	uint32_t ZUO_Point_WeiDu;//���� γ��
    	uint32_t ZUO_Point_JingDu;//���¾���
    	uint32_t YOU_Point_WeiDu;//����γ��
    	uint32_t YOU_Point_JingDu;//���� ����
    	uint8_t  Time_On[6];
    	uint8_t  Time_OFF[6];
    	uint16_t Max_Speed;//����ٶ�
    	uint8_t  Time_O;//����ʱ�� ����
    }Qu_YU_JU;

    extern Qu_YU_JU QuYu_Juxing[Ju_Max];
    extern uint8_t Juxing_QuYu_Cnt;
    void Tcp_QuYu_Ju_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
    void Tcp_QuYu_Delet_Ju_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
    void Analyze_Juxing_QuYu(Qu_YU_JU *Dest_Struct,u8 *psouce,u8 Qu_Cnt,u8 Offest);
    void Analyze_QuYu_Struct_Ju(Qu_YU_JU *Psource_Struct,u8 *Pdest,u16 *OFest);
    void  Analyze_Juxing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt);


    #endif

    