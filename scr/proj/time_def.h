/*
 * File      :rtc.h
 * Change Logs:
 * Date           Author       Notes
 * 2014-02-26     B.QIAN       first  version
 */
#ifndef __TIME_DEF_H__
#define __TIME_DEF_H__

#include <stdio.h> 
#include <string.h>
#include"eat_interface.h"

#define YEAR 2000

/*****hex format*****/
typedef struct
{
 u16 year;
 u8 month;
 u8 day;
 u8 hour;
 u8 min;
 u8 sec;
} date;



date sec2date(long sec);
uint32_t date2sec(date d);
/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : bcd_timer_to_sec
 *  Description: bcd_timer 将BCD码的时间指针
 *  Parameters : void
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */
extern uint32_t bcd_timer_to_sec(uint8_t *bcd_timer);

#endif



