
#ifndef ROUT_RANGE_H
    #define ROUT_RANGE_H

#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define route_range_proc_printf 1

#define LuXian_Max 2
#define Guai_MAX 20

#define Tcp_LuXian_Set_ID 0x8606//路线设置ID
#define Tcp_LuXian_Delet_ID 0x8607//路线删除ID

#define Qu_Yu_Save_File_Luxian L"Qu_Yu_Save_File_Luxian.bin"//路线储存



typedef struct _Guai_Dian //圆形区域结构体
{
	u32 GuaiDian_ID;
	u32 LuDuan_ID;//id
	u32 GUAI_Point_WeiDu;//拐点 纬度
	u32 GUAI_Point_JingDu;//拐 经度
	u8  Lu_Kuan;//路宽
	u8  GUAI_Shuxing;//属性
	u16 Max_Time;//路段行驶时间过长阈值
	u16 Time_Buzu;//路段行驶不足阈
	u16 Max_Speed;//顶点个数
	u8  ChaoSu_Time;//超速时间
}Guai_Dian;

typedef struct _Lu_Xian //路线结构体
{
	u32 LuXian_ID;//id
	u16 LuXian_Shuxing;//属性
	u8  Time_On[6];
	u8  Time_OFF[6];
	u16 Guai_Cnt;//拐点个数
	Guai_Dian Guaidian[Guai_MAX];//拐点结构体数组
}Lu_Xian_Road;

extern Lu_Xian_Road LuXian_Date[LuXian_Max];

extern u8 Luxian_QuYu_Cnt; 
extern void Tcp_LuXian_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
void Tcp_Delet_LuXian_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
void Analyze_LuXian(Lu_Xian_Road *Dest_Struct,u8 *psouce);
void Analyze_LuXian_GuaiDian(Guai_Dian *Dest_Struct,u8 *psouce,u8 Qu_Cnt);
extern void File_Luxian_Iint(void);


#endif