
#ifndef ROUND_AREA_H
    #define ROUND_AREA_H
#include <stdio.h>
#include <string.h>
#include"eat_interface.h"

#define roud_area_pro_printf 1

#define Tcp_QuYu_Yuan_Set_ID 0x8600//圆形区域设置ID
#define Tcp_QuYu_Yuan_Delet_ID 0x8601//圆形区域删除ID

#define Yuan_Max 10
#define Qu_Yu_Save_File_yuan L"Qu_Yu_Save_File_yuan.bin"//圆形区域储存

typedef struct _Qu_YU_yun //圆形区域结构体
{
	u32 Yuan_ID;//id
	u16 Yuan_Shuxing;//属性
	u32 Cemt_Point_WeiDu;//中心点 纬度
	u32 Cemt_Point_JingDu;//中心点 经度
	u32 Banjing_R;//半径
	u8  Time_On[6];
	u8  Time_OFF[6];
	u16 Max_Speed;//最大速度
	u8  Time_O;//持续时间 超速
}Qu_YU_yun;

extern Qu_YU_yun QuYu_Yuan[Yuan_Max];
extern uint8_t Yuan_QuYu_Cnt;
extern void Tcp_QuYu_Yuan_Server(uint8_t l_socket,u8 *Temp_data_buff,u16 Lens,u16 SN);
extern void Tcp_QuYu_Delet_Yuan_Servn(uint8_t l_socket,u8 *Psouce,u16 SN);
extern void Analyze_YuanXing_QuYu(Qu_YU_yun *Dest_Struct,u8 *psouce,u8 Qu_Cnt,u8 Offest);
extern void  Analyze_YuanXing_QuYu_XiuGai(u8 *psouce,u8 Qu_Cnt);
extern void Yuan_Comimt_QuYu_File(void);
#endif

