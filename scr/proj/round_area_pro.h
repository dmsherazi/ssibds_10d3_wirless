
#ifndef ROUND_AREA_H
    #define ROUND_AREA_H
#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define roud_area_pro_printf 1

#define Tcp_QuYu_Yuan_Set_ID 0x8600//Բ����������ID
#define Tcp_QuYu_Yuan_Delet_ID 0x8601//Բ������ɾ��ID

#define Yuan_Max 10
#define Qu_Yu_Save_File_yuan L"Qu_Yu_Save_File_yuan.bin"//Բ�����򴢴�

typedef struct _Qu_YU_yun //Բ������ṹ��
{
	u32 Yuan_ID;//id
	u16 Yuan_Shuxing;//����
	u32 Cemt_Point_WeiDu;//���ĵ� γ��
	u32 Cemt_Point_JingDu;//���ĵ� ����
	u32 Banjing_R;//�뾶
	u8  Time_On[6];
	u8  Time_OFF[6];
	u16 Max_Speed;//����ٶ�
	u8  Time_O;//����ʱ�� ����
}Qu_YU_yun;

extern Qu_YU_yun QuYu_Yuan[Yuan_Max];
extern uint8_t Yuan_QuYu_Cnt;
extern void Tcp_QuYu_Yuan_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
extern void Tcp_QuYu_Delet_Yuan_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
extern void Analyze_YuanXing_QuYu(Qu_YU_yun *Dest_Struct,u8 *psouce,u8 Qu_Cnt,u8 Offest);
extern void  Analyze_YuanXing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt);
extern void Yuan_Comimt_QuYu_File(void);
#endif

