#ifndef LOCATION_CHECK_H
    #define LOCATION_CHECK_H

#include <stdio.h>
#include <math.h>
#include"eat_interface.h"

#define PI              3.1415926         /* ¦ÐÖµ */
#define EARTH_RADIUS    6378.137          /* µØÇò½üËÆ°ë¾¶ */
#define far_distance    50

extern void Location_check_pro(uint32_t lat1,uint32_t lng1);

#endif
