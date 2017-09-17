
#ifndef REGIONAL_JUDGMENT_H
    #define REGIONAL_JUDGMENT_H

#include <stdio.h>
#include <math.h>
#include"eat_interface.h"

#define regional_judment_debug_printf 1
#define QuYu_Max 10

#define PI              3.1415926         /* π值 */
#define EARTH_RADIUS    6378.137          /* 地球近似半径 */


/* 行驶路线结构体 */
typedef struct{
	u32 Latitude;                  /* 拐点纬度 */
	u32 Longtitude;                /* 拐点经度 */
	u8 lenght;                     /* 路宽 */
}Route_struct;

/*直线结构体 Ax+By+C=0*/
typedef struct{
	double A;                     /* A */
	double B;                     /* B */
	double C;                     /* C */
}Line_struct;

extern u8 Lu_State;


extern double g_Distance(double doX1, double doY1, double doX2, double doY2);
extern double radian(double d);
extern double GetDistance(double lat1, double lng1, double lat2, double lng2);
extern void GaussCalc(double L, double B, double* X, double* Y);
extern u8 pointInPolygon(double x,double y,u8 Offest);
extern u8 JuXing_Judgment(u32 Weidu,u32 JingDu,u32 LWeidu,u32 LJingDu,u32 RWeidu,u32 RJingDu);
extern u8 Luxian_Judgmentu(u32 Weidu,u32 JingDu,u8 Ofest);
extern u8 Yuan_Judgment(u32 Weidu,u32 JingDu,u32 YWeidu,u32 YJingDu,u32 YBanjING);
extern void Yuan_QuYU_ACTION(u32 Wei,u32 Jing);//圆形区域动作
extern void Ju_QuYU_ACTION(u32 Wei,u32 Jing);//圆形区域动作
extern void Duo_QuYU_ACTION(u32 Wei,u32 Jing);//圆形区域动作
extern void LuXian_QuYU_ACTION(u32 Wei_d,u32 Jing_d);//圆形区域动作

extern void initiative_close_gprs_data(void);
extern void regional_judgment_pro(void);
extern void  regional_file_init(void);
extern void regional_file_read(void);
extern void seting_data_to_board(uint8_t Data);

#endif 

