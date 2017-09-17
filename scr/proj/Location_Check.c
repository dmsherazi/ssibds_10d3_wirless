#include "Location_Check.h"


 /* 求弧度 */
double radian(double d)
{
	return (d * PI / 180.0);   /* 角度1 = π / 180 */
}

/* 计算距离 */
uint64_t getDistance(double lat1, double lng1, double lat2, double lng2)
{
    double radLat1,radLat2,a,b,dst;
    uint64_t distance = 0;

    radLat1 = radian(lat1);
    radLat2 = radian(lat2);
    a = radLat1 - radLat2;
    b = radian(lng1) - radian(lng2);

    dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));

    distance = dst * EARTH_RADIUS*1000;/*m*/
    
    return dst;
}

void Location_check_pro(uint32_t lat1,uint32_t lng1)
{
    double Latd1=0.0,lngd1=0.0,Latd2=0.0,lngd2=0.0;
    uint64_t distance=0;
    if((Real_Time_Location_Inf.Location_Latitude!=0)&&(Real_Time_Location_Inf.Location_Longitude!=0)&&(lat1!=0)&&(lng1!=0))
    {
        Latd1 = (double)(lat1/1000000.0);
    	lngd1 = (double)(lng1/1000000.0);
    	
        Latd2 = (double)(Real_Time_Location_Inf.Location_Latitude/1000000.0);
    	lngd2 = (double)(Real_Time_Location_Inf.Location_Longitude/1000000.0);
        distance = getDistance(lat1,lng1,Latd2,lngd2);
        #if APP_TRACE_DBG 
        eat_trace("Location_check_pro -->location distance:%u",distance);
        #endif
    	if(distance > far_distance)
    	{
           #if APP_TRACE_DBG 
           eat_trace("Location_check_pro --> erro location.");
           #endif
    	}
    	else
    	{
            #if APP_TRACE_DBG 
            eat_trace("Location_check_pro --> comm location.");
            #endif
            Real_Time_Location_Inf.Location_Latitude=lat1;
            Real_Time_Location_Inf.Location_Longitude=lng1;
    	}
    }
}




