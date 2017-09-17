#include "pbulic_tool.h"

/*******************************************************************
*功    能：在指定的字符串找某字符第一次出现的位置
*入口参数：pStr1----指定的字符串起始位置
*			pStr2-----要查找的字符串
*返 回 值：该字符结束的偏移地址，找不到，返回NULL
*全局变量：无
*备    注: 和库函数strstr的区别是返回的是找到字符串的结束地址

********************************************************************/
 u8 TargetData[1024]={0}; 
char * strstrLast(const char *pStr1,const char *pStr2)
{
	char cData,cData1,cData2,*pOffset1=NULL,*pOffset2=NULL;

	while(pStr1!=NULL)
	{
		cData=*pStr1;
		if(cData==*pStr2)
		{
			pOffset1=(char *)pStr1;
			pOffset2=(char *)pStr2;
			while(1)
			{
				if((cData2=*pOffset2++)==0)	return pOffset1;
				if((cData1=*pOffset1++)==0)	return NULL;
				if(cData1!=cData2)	break;
			}
		}
		else if(cData==0)
			break;
		pStr1++;
	}
	return NULL;
}

/*******************************************************************
异或校验
******************************************************************/
unsigned char CrcXOR (unsigned char * pScr,unsigned int lens)
{
	unsigned short int temp;
	unsigned char ret=0;
	for(temp=0;temp<lens;temp++)
	{
		ret^=*(pScr++);
	}
	return ret;
}
/*******************************************************************
和校验
******************************************************************/
unsigned short int PLUS_XOR (unsigned char * pScr,unsigned int lens)
{
	unsigned short int temp;
	unsigned short int ret=0;
	for(temp=0;temp<lens;temp++)
	{
		ret+=*(pScr++);
	}
	return ret;
}
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: HexToAsicc
 *  Description:  将16位数转化为两个字符加空格 
 *  Parameters :  Data    :原始数据
 *				  H_Byte  :高8位
 *				  L_Byte  :低八位
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */ 
void HexToAsicc(u8 *SourceData, u16 len)
{
	const unsigned char asicc[] = "0123456789ABCDEF";
	unsigned short int i;
	unsigned short int Ser_Num;//分包信息
	unsigned short int Rev_Num;//剩余数据
	static unsigned short int Cnt,Num,Dcnt;
	Dcnt=0;
	if(len > 64)
	{
		Ser_Num =(unsigned short int)(len/64);//分包数
		Rev_Num =(unsigned short int)(len%64);//分包后剩余
		if(Rev_Num!=0)//分包有剩余
		{
			for(Num=0;Num<Ser_Num;Num++)
			{
				Cnt=0;
				memset(TargetData,0,1024);
				for(i=0;i<64;i++)
				{		
					*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) >> 4]; //高4位
					 Cnt++;
					*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) & 0x0f]; //低4位
					 Cnt++;
					*(TargetData+Cnt)=' ';
					 Cnt++;
					 Dcnt++;//总计数		
				}
				eat_trace("%s",TargetData);
			}
			Cnt=0;
			memset(TargetData,0,1024);
			for(i=0;i<Rev_Num;i++)
			{		
				*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) >> 4]; //高4位
				 Cnt++;
				*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) & 0x0f]; //低4位
				 Cnt++;
				*(TargetData+Cnt)=' ';
				 Cnt++;
				 Dcnt++;//总计数
			}
			*(TargetData+Cnt)='\0';
			eat_trace("%s",TargetData);
		
		}
		else//无分包
		{
			for(Num=0;Num<Ser_Num;Num++)
			{
				Cnt=0;
				memset(TargetData,0,1024);
				for(i=0;i<64;i++)
				{		
					*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) >> 4]; //高4位
					 Cnt++;
					*(TargetData+Cnt)= asicc[(*(SourceData+Dcnt)) & 0x0f]; //低4位
					 Cnt++;
					*(TargetData+Cnt)=' ';
					 Cnt++;
					 Dcnt++;//总计数
				}
				*(TargetData+Cnt)='\0';
				eat_trace("%s",TargetData);
			}
		}
	}
	else
	{
		Cnt=0;
		memset(TargetData,0,1024);
		for(i=0;i<len;i++)
		{		
			*(TargetData+Cnt)= asicc[(*(SourceData+i)) >> 4]; //高4位
			 Cnt++;
			*(TargetData+Cnt)= asicc[(*(SourceData+i)) & 0x0f]; //低4位
			 Cnt++;
			*(TargetData+Cnt)=' ';
			 Cnt++;
		}
		*(TargetData+Cnt)='\0';
		eat_trace("%s",TargetData);
	}
}
 
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: Data_Change
 *  Description:  将32位数转化字符
 *  Parameters :  Data    :原始数据
 *				  *S        :字符串指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */ 
 void Dec_To_BCD(unsigned char *PDec,unsigned char Dec_Len,unsigned char *Bcd)
 {
 	unsigned char Cnt=0;
 	unsigned char H_data=0,L_data=0;
 	for(Cnt=0;Cnt<Dec_Len;Cnt++)
 	{
 		H_data = *(PDec+Cnt)/10;//高字节
 		L_data = *(PDec+Cnt)%10;//低字节
 		*(Bcd+Cnt) = (H_data<<4) + L_data;
 	}
 }
 void String_To_BCD(u8 *asc,u8 *bcd,u32 len,u32 fmt)
 {
    s32 i, odd;
    s8 ch; 
    if(len)
    {
    	odd = len%2;
	    if (odd)
	    {   
	        *bcd++ = (*asc++) & 0x0F;
	         for (i = 0; i < ((len-1)/2); i++)
	    	 {   
		        ch = (*asc++) << 4;        
		        ch |= (*asc++) & '\x0F';       
		        *bcd++ = ch; 
	    	 }
	    }
	    else
	    {
	    	 for (i = 0; i <(len/2); i++)
	    	 {   
		        ch = (*asc++) << 4;        
		        ch |= (*asc++) & '\x0F';       
		        *bcd++ = ch; 
	    	 }	
	    }
    }
 }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: BCD_To_String
 *  Description:  将BCD转化为字符串
 *  Parameters :  Data    :原始数据
 *				  *S        :字符串指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */
  void BCD_To_Dec(u8 *Pbcd,u8 *PStr,u8 Bcd_Len)
 {
 	u8 Cnt=0;
 	u8 H_data=0,L_data=0;
 	for(Cnt=0;Cnt<Bcd_Len;Cnt++)
 	{
 		H_data = ((*(Pbcd+Cnt))>>4)&0x0F;
 		L_data = (*(Pbcd+Cnt))&0x0F;
 		*(PStr+Cnt) = H_data*10 + L_data;
 	}
 }

 void BCD_TO_String(u8 *Pbcd,u8 *PStr,u32 Bcd_Len)
 {
 	u32 Cnt=0;
 	for(Cnt=0;Cnt<Bcd_Len;Cnt++)
 	{
 		*(PStr+2*Cnt) = (((*(Pbcd+Cnt))>>4)&0x0F)+0x30;
 		*(PStr+2*Cnt+1) = ((*(Pbcd+Cnt))&0x0F)+0x30;
 	}
 }
void Time_Minte_Add(u8 *STime,u8*PTime,u8 Madd)
{
	u8 Temp_Ttim[3]={0};
	u16 Hour_Cnt=0;
	u16 Min_Cnt=0;
	BCD_To_Dec(STime,Temp_Ttim,3);
	Min_Cnt=Temp_Ttim[1]+Madd;
	if(Min_Cnt<60)Temp_Ttim[1]=Min_Cnt;
	else
	{
		Temp_Ttim[1]=(u8)(Min_Cnt%60);
		Hour_Cnt=Temp_Ttim[0]+(u8)(Min_Cnt/60);
		if(Hour_Cnt>=24)
		{
			Temp_Ttim[0]=Hour_Cnt-24;
		}
		else
		{
			Temp_Ttim[0]=Hour_Cnt;
		}
	}
	Dec_To_BCD(Temp_Ttim,3,PTime);
}
void Time_Second_Add(u8 *STime,u8*PTime,u16 Seadd)
{
	u8 Temp_Ttim[3]={0};
	u16 Hour_Cnt=0;
	u16 Min_Cnt=0;
	u16 Secn_Cnt=0;
	BCD_To_Dec(STime,Temp_Ttim,3);
	Secn_Cnt=Temp_Ttim[2]+Seadd;
	if(Secn_Cnt<60)
	{
		Temp_Ttim[2]=(u8)Secn_Cnt;
	}
	else
	{
		Temp_Ttim[2]=(u8)(Secn_Cnt%60);
		Min_Cnt= Temp_Ttim[1]+(u8)(Secn_Cnt/60);
		if(Min_Cnt>60) 
		{
			Temp_Ttim[1]=(u8)(Min_Cnt%60);
			Hour_Cnt=Temp_Ttim[0]+(u8)(Min_Cnt/60);
			if(Hour_Cnt>=24)
			{
				Temp_Ttim[0]=Hour_Cnt-24;
			}
		}
		else
		{
			Temp_Ttim[1]=Min_Cnt;
		}	
	}
	Dec_To_BCD(Temp_Ttim,3,PTime);
}
void Str2Double(char *pbuf,double *ret)
{
    double rev = 0;
    double dat;
    int integer = 1;
    char *str = pbuf;
    int i;
    while(*str != '\0')
    {
        switch(*str)
        {
            case '0':
                dat = 0;
                break;
            case '1':
                dat = 1;
                break;
            case '2':
                dat = 2;
                break;        
            case '3':
                dat = 3;
                break;
            case '4':
                dat = 4;
                break;
            case '5':
                dat = 5;
                break;
            case '6':
                dat = 6;
                break;
            case '7':
                dat = 7;
                break;
            case '8':
                dat = 8;
                break;
            case '9':
                dat = 9;
                break;
            case '.':
                dat = '.';
                break;
        }
        if(dat == '.')
        {
            integer = 0;
            i = 1;
            str ++;
            continue;
        }
        if( integer == 1 )
        {
            rev = rev * 10 + dat;
        }
        else
        {
            rev = rev + dat / (10 * i);
            i = i * 10 ;
        }
        str ++;
    }
    *ret=rev;
}



void Str2Float(char *pbuf,float * ret)
 {
     float rev = 0;
    float dat;
   int integer = 1;
     char *str = pbuf;
    int i;
     while(*str != '\0')
     {
       switch(*str)
        {
            case '0':
                dat = 0;
                break;
             case '1':
                 dat = 1;
               break;
             case '2':
                dat = 2;
               break;        
             case '3':
                 dat = 3;
                break;
            case '4':
                dat = 4;
                break;
             case '5':
                 dat = 5;
                 break;
             case '6':
                dat = 6;
               break;
           case '7':
                dat = 7;
                 break;
            case '8':
                 dat = 8;
                break;
            case '9':
                dat = 9;
                 break;
             case '.':
                 dat = '.';
                 break;
         }
         if(dat == '.')
         {
            integer = 0;
             i = 1;
             str ++;
             continue;
         }
         if( integer == 1 )
         {
             rev = rev * 10 + dat;
         }
         else
         {
            rev = rev + dat / (10 * i);
             i = i * 10 ;
         }
         str ++;
     }
	*ret= rev ;
     //return rev;
}
unsigned char GetComma(unsigned char  num,char *str,unsigned char  cha)
{
	unsigned char i,j = 0;
	int len=strlen(str);
  //RT_ASSERT(str!=RT_NULL);
	for(i = 0;i < len;i ++)
	{
	if(str[i] ==cha)
	j++;
	if(j == num)
	 return i + 1;    
	}	 
	return 0;    
}
void tool_str2double(char *s,double *ret)
{
    char buf[10];
    unsigned char i;
    i=GetComma(1, s,',');
	  if(!i)return;
    i = i - 1;
    strncpy(buf, s, i);
    buf[i] = 0;
    Str2Double(buf,ret);
    return;    
}

void tool_str2float(char *s,float * ret)
{
    char buf[10];
    unsigned char i;
    //float rev=0.0;
    i=GetComma(1, s,',');
		if(!i)return;
    i = i - 1;
    strncpy(buf, s, i);
    buf[i] = 0;
    Str2Float(buf,ret);
    return ;    
}

void  int2arr(int src,char *pDes,unsigned char store_type)
{
	if(store_type==0)
	{
		 (*pDes++) =(char)(src>>24);
		 (*pDes++) =(char)(src>>16);
		 (*pDes++) =(char)(src>>8);
		 (*pDes) =(char)(src>>0);
	}
	else
	{
		 (*pDes++) =(char)(src>>0);
		 (*pDes++) =(char)(src>>8);
		 (*pDes++) =(char)(src>>16);
		 (*pDes) =(char)(src>>24);		
	}
}

void  short2arr(short src,char *pDes,unsigned char store_type)
{
	if(store_type==0)
	{
		 (*pDes++) =(char)(src>>8);
		 (*pDes) =(char)(src>>0);
	}
	else
	{
		 (*pDes++) =(char)(src>>0);
		 (*pDes++) =(char)(src>>8);		
	}
}

void Data_TO_Char(unsigned int Src,char *pDes)
{
	unsigned  char  char10=0;	
unsigned  char  char9=0;
  unsigned  char  char8=0;
  unsigned  char  char7=0;	
	unsigned  char  char6=0;	
	
  unsigned char w_char=0;//
  unsigned char t_char=0;//
	unsigned char h_char=0;//
	unsigned char d_char=0;//
	unsigned char l_char=0;//
	unsigned short temp=0;
	unsigned char flag=0;
	
	


	
	w_char=Src/10000; //
	if(w_char)
	{
		w_char+=0x30;
		*(pDes++)=w_char;	
	}
	else flag=1;//

  //-----
   temp=Src%10000;  //
   t_char=temp/1000;

   if(t_char)
   {
   	 	t_char+=0x30;
		*(pDes++)=t_char;
		flag=0;
   }
   else
   {
   		if(!flag)
		{
   	 	t_char+=0x30;
		*(pDes++)=t_char;
		//flag=0;			
		}
   }

//----
   temp=temp%1000;
   h_char=temp/100;

   if(h_char)
   {
   	   	 h_char+=0x30;
		*(pDes++)=h_char;
		flag=0;	
   }
   else
   {
   		if(!flag)
		{
   	 	h_char+=0x30;
		*(pDes++)=h_char;
		//flag=0;			
		}   		
   }
//----
   temp=temp%100;
	d_char=temp/10;

    if(d_char)
   {
   	   	 d_char+=0x30;
		*(pDes++)=d_char;	
   }
   else
   {
   		if(!flag)
		{
   	 	d_char+=0x30;
		*(pDes++)=d_char;			
		}   		
   }

//----
	l_char=temp%10;
	l_char+=0x30;
	*(pDes++)=l_char;
							
	*pDes=0;
}

double min(double a,double b)
{
	if(a <= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
double max(double a,double b)
{
	if(a >= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: Porotorl_Phone_Num_Conver
 *  Description:  将字符串转换为指定的808协议格式
 *  Parameters :  u8 *Phone_Scr    :原始数据
 *				  *S        :字符串指针
 				  Scr_Len	;原始字符串长度
 				  Bcd_Dest	: 目的字字符串
 *  Return     :  -1 EREOR,1OK;
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */
 int Porotorl_Phone_Num_Conver(u8 *Phone_Scr,u8 Scr_Len,u8 *Bcd_Dest)
 {
 	u8 Temp_Num[12]={0};
 	if((Scr_Len==0)||(Scr_Len>12))
 	{
 		return-1;
 	}
 	else
 	{
 		memcpy(Temp_Num+12-Scr_Len,Phone_Scr,Scr_Len);
 		String_To_BCD(Temp_Num,Bcd_Dest,12,6);//转化BCD
		return 1;
 	}
 }

 /*****************************************************************************
 * Function : Big_End_DwordtoByte
 * Description:This function is used to Big end unsigned int to byte.
 * Parameters : 
 *				Pscr:原始数据
 *				Start_Point:开始指针
 * Returns:void
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
 void Big_End_DwordtoByte(u32 Pscr,u8 *Start_Point)
 {
 	*Start_Point = (unsigned char)(Pscr>>24);
 	*(Start_Point + 1) = (unsigned char)((Pscr>>16)&0x000000FF);
	*(Start_Point + 2) = (unsigned char)((Pscr>>8)&0x000000FF);
	*(Start_Point + 3) = (unsigned char)(Pscr&0x000000FF);
 }

  /*****************************************************************************
 * Function : Big_End_WordtoByte
 * Description:This function is used to Big end unsigned short int to byte.
 * Parameters : 
 *				Pscr:原始数据
 *				Start_Point:开始指针
 * Returns:void
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
 void Big_End_WordtoByte(u16 Pscr,u8 *Start_Point)
 {
 	*Start_Point = (unsigned char)(Pscr>>8);
	*(Start_Point + 1) = (unsigned char)(Pscr&0x00ff);
 }

   /*****************************************************************************
 * Function : Big_End_WordtoByte
 * Description:This function is used to Big end byte to u32.
 * Parameters : 
 *				Start_Point:Byte开始指针
 *				Pscr:目标指针
 * Returns:void
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
 void Big_End_BytetoDword(u8 *Start_Point,u32 *Pscr)
 {
 	*Pscr = (((unsigned int)(*Start_Point))<<24)+(((unsigned int)*(Start_Point+1))<<16)+(((unsigned int)*(Start_Point+2))<<8)+(unsigned int)*(Start_Point+3);	
 }

 /*****************************************************************************
 * Function : Big_End_BytetoWord
 * Description:This function is used to Big end byte to u16.
 * Parameters : 
 *				Start_Point:Byte开始指针
 *				Pscr:目标指针
 * Returns:void
 * NOTE
 *     Sameting you want to tell somebody who modify
 *****************************************************************************/
 void Big_End_BytetoWord(u8 *Start_Point,u16 *Pscr)
 {
	*Pscr = *Start_Point*256 + *(Start_Point+1);
 }

void byete_change_int32(uint8_t mode,uint8_t *pscr,uint64_t *data)
{
    if(mode)//big endian
    {
        *data=(uint32_t)((*pscr)<<0)+(uint32_t)((*(pscr+1))<<8)+(uint32_t)((*(pscr+2))<<16)+(uint32_t)((*(pscr+3))<<24);
    }
    else
    {
        *data=(uint32_t)((*(pscr+0))<<24)+(uint32_t)((*(pscr+1))<<16)+(uint32_t)((*(pscr+2))<<8)+(uint32_t)(*(pscr+3));    
    }
}
void int64_change_byete(uint8_t mode,uint64_t data,uint8_t *pscr)
{
    if(mode)//big endian
    {
        *pscr = (unsigned char)(data>>56);
        *(pscr + 1) = (unsigned char)((data>>48)&0x000000FF);
        *(pscr + 2) = (unsigned char)((data>>40)&0x000000FF);
        *(pscr + 3) = (unsigned char)((data>>32)&0x000000FF);
        *(pscr + 4) = (unsigned char)((data>>24)&0x000000FF);
        *(pscr + 5) = (unsigned char)((data>>16)&0x000000FF);
        *(pscr + 6) = (unsigned char)((data>>8)&0x000000FF);
        *(pscr + 7) = (unsigned char)((data>>0)&0x000000FF);
    }
    else
    {
        *(pscr+ 7) = (unsigned char)(data>>56);
        *(pscr + 6) = (unsigned char)((data>>48)&0x000000FF);
        *(pscr + 5) = (unsigned char)((data>>40)&0x000000FF);
        *(pscr + 4) = (unsigned char)((data>>32)&0x000000FF);
        *(pscr + 3) = (unsigned char)((data>>24)&0x000000FF);
        *(pscr + 2) = (unsigned char)((data>>16)&0x000000FF);
        *(pscr + 1) = (unsigned char)((data>>8)&0x000000FF);
        *pscr  = (unsigned char)((data>>0)&0x000000FF);
    }
}
char itoa(int num,char *str)
{
    char i = 0,j = 0;
    char temp[11]={0};
    char data_t=0;
    int sign = 0;
    sign = num;
    if(sign<0)//判断是否是一个负数
    {
        num = -num;
    };
    do
    {   data_t = (char)(num%10);
        temp[i] = data_t + 0x30;        
        num =(int)(num/10);
        i++;
    }while(num>0);
    if(sign<0)
    {
        temp[i++] = '-';//对于负数,要加以负号
    }
    temp[i] = '\0';
    for(j=0;j<i;j++)
    {
        *(str+j) = temp[i-j-1];
    }
    *(str+j) = '\0';
    return j;
}


void FloatToByte(float32_t floatNum,unsigned char* byteArry)////浮点数到十六进制转换2
{
    int i=0;
    char* pchar=(char*)&floatNum;
    for(i=0;i<sizeof(float32_t);i++)
    {
        *byteArry=*pchar;
        pchar++;
        byteArry++;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Change_U8_CHAR
 *  Description:  将数值转化为ASCLL 
 *  Parameters :  Data    :原始数据
 *				  S        :字符
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */ 
 void change_dec_to_uint16(uint8_t dec_data,uint16_t *S)
 {
	switch(dec_data)
	{
		case 0:
    		*S='0';
    		break;
		case 1:
    		*S='1';
    		break;
		case 2:
    		*S='2';
    		break;
		case 3:
    		*S='3';
    		break;
		case 4:
    		*S='4';
    		break;
		case 5:
    		*S='5';
    		break;
		case 6:
    		*S='6';
    		break;
		case 7:
    		*S='7';
    		break;
		case 8:
    		*S='8';
    		break;
		case 9:
    		*S='9';
    		break;
		default:
		break;	
	}
 }
 /* 
 * ===  FUNCTION  ======================================================================
 *  Name: int8_change_uint16_ascall
 *  Description:  修改TXT文档名称
 *  Parameters :  Data    :原始数据
 *				  *S        :字符串指针
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */ 
void int8_change_uint16_ascall(uint8_t Data,uint16_t *S)
 {
	change_dec_to_uint16((unsigned char)(Data/100),S);
 	change_dec_to_uint16((unsigned char)((Data%100)/10),S+1);
 	change_dec_to_uint16((unsigned char)(Data%10),(S+2));
 }
  /* 
  * ===  FUNCTION  ======================================================================
  *  Name: hex_string_to_ascall
  *  Description:  hex to ascall
  *  Parameters :  Data    :原始数据
  *                *S        :字符串指针
  *  Return     :  void 
  *  author     :  J.x.b create at SSI_1D02 at 2014-6-10          
  * =====================================================================================
  */ 
void hex_string_to_ascall(uint8_t *data,uint8_t *des,uint16_t len)
{
   uint16_t ii;
   const uint8_t asicc[] = "0123456789abcdef";
   if((data != NULL)&&(des!=NULL))
   {
       for(ii=0;ii<len;ii++)
       {
           *(des+ii*2) = asicc[(uint8_t)(*(data+ii)>>4)];
           *(des+ii*2+1) = asicc[(uint8_t)(*(data+ii)&0x0f)];
       }
   }
}
