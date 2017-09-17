
#ifndef PUBLIC_TOOL_H
	#define PUBLIC_TOOL_H
#include <stdio.h>
#include <math.h>
#include"eat_interface.h"



extern char * strstrLast(const char *pStr1,const char *pStr2);
extern unsigned char CrcXOR (unsigned char * pScr,unsigned int lens);
extern void HexToAsicc(unsigned char *SourceData, unsigned short int len );
extern unsigned short int PLUS_XOR (unsigned char * pScr,unsigned int lens);//和校验
extern void Dec_To_BCD(unsigned char *PDec,unsigned char Dec_Len,unsigned char *Bcd);
extern void String_To_BCD(u8 *asc,u8 *bcd,u32 len,u32 fmt);
extern void BCD_TO_String(u8 *Pbcd,u8 *PStr,u32 Bcd_Len);
extern void Time_Minte_Add(u8 *STime,u8*PTime,u8 Madd); 
extern void BCD_To_Dec(u8 *Pbcd,u8 *PStr,u8 Bcd_Len);
extern void Str2Double(char *pbuf,double *ret);
extern void Str2Float(char *pbuf,float * ret);
extern unsigned char GetComma(unsigned char  num,char *str,unsigned char  cha);
extern void tool_str2double(char *s,double *ret);
extern void tool_str2float(char *s,float * ret);
extern void  int2arr(int src,char *pDes,unsigned char store_type);
extern void  short2arr(short src,char *pDes,unsigned char store_type);
extern void Data_TO_Char(unsigned int Src,char *pDes);
extern void Time_Second_Add(u8 *STime,u8*PTime,u16 Seadd);
extern double min(double a,double b);
extern double max(double a,double b);
extern int Porotorl_Phone_Num_Conver(u8 *Phone_Scr,u8 Scr_Len,u8 *Bcd_Dest);


extern void Big_End_DwordtoByte(u32 Pscr,u8 *Start_Point);
extern void Big_End_WordtoByte(u16 Pscr,u8 *Start_Point);
extern void Big_End_BytetoDword(u8 *Start_Point,u32 *Pscr);
extern void Big_End_BytetoWord(u8 *Start_Point,u16 *Pscr);
extern void byete_change_int64(uint8_t mode,uint8_t *pscr,uint64_t *data);
extern void int32_change_byete(uint8_t mode,uint64_t data,uint8_t *pscr);
extern char itoa(int num,char str[]);
extern void FloatToByte(float32_t floatNum,unsigned char* byteArry);////浮点数到十六进制转换2

extern void int32_change_uint16_ascall(uint32_t Data,uint16_t *S);
extern void int8_change_uint16_ascall(uint8_t Data,uint16_t *S);
extern void hex_string_to_ascall(uint8_t *data,uint8_t *des,uint16_t len);

#endif


