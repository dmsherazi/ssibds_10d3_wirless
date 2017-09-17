/****************************************************************
 * File      :time_def.c
 * Change Logs:
 * Date           Author       Notes
 * 2014-02-26     B.QIAN       first  version
 *****************************************************************/
#include   "time_def.h"

const u8 days[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 

int isLeapYear(int year)  
{ 
 if(((year%4==0)&&(year%100!=0))
    ||(year%400==0)) 
 {
  return 1; 
 }
 return 0; 
}

/*����ת��2000��1��1���������*/ 
uint32_t date2sec(date d)
{ 
	 u32 sum=0; 
	 u32 i,backup;

	 //�ۼ��������������
	 for(i=YEAR;i<d.year;i++) 
	 { 
		  sum+=365; 
		  if(isLeapYear(i)) 
		  {//�����һ��
		   sum+=1; 
		  }
	 }
		backup=i;

	 //�ۼƵ����������µ�����
	 for(i=1;i<d.month;i++)
	 {
		  sum+=days[i-1];
	}
	 if(d.month>2)
	 {
		  if(isLeapYear(backup)) 
		  {//�����һ��
		   sum+=1; 
		  }
	 }
	 //�ۼƵ��굱�µ�����
	 sum+=d.day-1; 
	 //ת������
	 sum=sum*24*60*60; 
 
	 //�ӵ����Сʱ�����ӣ���
	 sum+=d.hour*60*60+d.min*60+d.sec;
	 //����������
 	return sum; 
} 



/*����ת��2000��1��1���������*/ 
date sec2date(long sec)
{
 date d={YEAR,1,1,0,0,0};
 u32 ds,sum;

 ds=sec/(24*60*60);
 sec-=ds*(24*60*60);
 d.hour=sec/(60*60);
 sec-=d.hour*(60*60);
 d.min=sec/60;
 sec-=d.min*60;
 d.sec=sec;
 //�����ж�����
 while(1)
 {
  sum=365; 
  if(isLeapYear(d.year)) 
  {//�����һ��
   sum+=1; 
  }
  if(ds<sum)
  {//����һ����
   break;
  }
  d.year++;
  ds-=sum;
 }

 //�����ж�����
 while(1)
 {
  sum=days[d.month-1];
  if(d.month==2)
  {
   if(isLeapYear(d.year)) 
   {//�����һ��
    sum+=1; 
   }
  }
  if(ds<sum)
  {//����һ����
	  d.day+= ds;
	  break;
  }
  ds-=sum;
  d.month++;
 }
 return d;
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name       : bcd_timer_to_sec
 *  Description: bcd_timer ��BCD���ʱ��ָ��
 *  Parameters : void
 *       author: J.x.b  create at 2014-5-28
 * =====================================================================================
 */
uint32_t bcd_timer_to_sec(uint8_t *bcd_timer)
{
	date dec_data;//����ʱ��
 	uint8_t rtc_time[6]={0};
 	uint32_t data_sec=0;
 	BCD_To_Dec(bcd_timer,rtc_time,6);
 	dec_data.year=YEAR+rtc_time[0];
 	dec_data.month=rtc_time[1];
 	dec_data.day=rtc_time[2];
 	dec_data.hour=rtc_time[3];
 	dec_data.min=rtc_time[4];
 	dec_data.sec=rtc_time[5];
 	data_sec=date2sec(dec_data);
 	return data_sec;
}

