
#ifndef ROUT_RANGE_H
    #define ROUT_RANGE_H

#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define route_range_proc_printf 1

#define LuXian_Max 2
#define Guai_MAX 20

#define Tcp_LuXian_Set_ID 0x8606//·������ID
#define Tcp_LuXian_Delet_ID 0x8607//·��ɾ��ID

#define Qu_Yu_Save_File_Luxian L"Qu_Yu_Save_File_Luxian.bin"//·�ߴ���



typedef struct _Guai_Dian //Բ������ṹ��
{
	u32 GuaiDian_ID;
	u32 LuDuan_ID;//id
	u32 GUAI_Point_WeiDu;//�յ� γ��
	u32 GUAI_Point_JingDu;//�� ����
	u8  Lu_Kuan;//·��
	u8  GUAI_Shuxing;//����
	u16 Max_Time;//·����ʻʱ�������ֵ
	u16 Time_Buzu;//·����ʻ������
	u16 Max_Speed;//�������
	u8  ChaoSu_Time;//����ʱ��
}Guai_Dian;

typedef struct _Lu_Xian //·�߽ṹ��
{
	u32 LuXian_ID;//id
	u16 LuXian_Shuxing;//����
	u8  Time_On[6];
	u8  Time_OFF[6];
	u16 Guai_Cnt;//�յ����
	Guai_Dian Guaidian[Guai_MAX];//�յ�ṹ������
}Lu_Xian_Road;

extern Lu_Xian_Road LuXian_Date[LuXian_Max];

extern u8 Luxian_QuYu_Cnt; 
extern void Tcp_LuXian_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
void Tcp_Delet_LuXian_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
void Analyze_LuXian(Lu_Xian_Road *Dest_Struct,u8 *psouce);
void Analyze_LuXian_GuaiDian(Guai_Dian *Dest_Struct,u8 *psouce,u8 Qu_Cnt);
extern void File_Luxian_Iint(void);


#endif