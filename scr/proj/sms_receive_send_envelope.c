
#include "sms_receive_send_envelope.h"


const u8 GBK[]="车牌号报警京沪津渝新蒙宁桂藏黑吉辽冀甘青陕豫鲁晋皖鄂湘苏川黔滇浙赣粤闽台琼港澳紧急天线断开碰撞超速预疲劳驾驶欠压掉电侧翻进出区域矩多边圆形非法位移点火时停当累计短路偏离段行间不足过长【东风管家】您的解锁码是：";
const u8 PUD_STRING[]={0x8F,0x66,0x72,0x4C,0x53,0xF7,0x62,0xA5,0x8B,0x66,0x4E,0xAC,0x6C,0xAA,0x6D,0x25,\
						0x6E,0x1D,0x65,0xB0,0x84,0x99,0x5B,0x81,0x68,0x42,0x85,0xCF,0x9E,0xD1,0x54,0x09,0x8F,0xBD,0x51,0x80,0x75,\
						0x18,0x97,0x52,0x96,0x55,0x8C,0x6B,0x9C,0x81,0x66,0x4B,0x76,0x96,0x91,0x02,0x6E,0x58,0x82,0xCF,0x5D,0xDD,\
						0x9E,0xD4,0x6E,0xC7,0x6D,0x59,0x8D,0x63,0x7C,0xA4,0x95,0xFD,0x53,0xF0,0x74,0x3C,0x6E,0x2F,0x6F,0xB3,0x7D,\
						0x27,0x60,0x25,0x59,0x29,0x7E,0xBF,0x65,0xAD,0x5F,0x00,0x78,0xB0,0x64,0x9E,0x8D,0x85,0x90,0x1F,0x98,0x84,\
						0x75,0xB2,0x52,0xB3,0x9A,0x7E,0x9A,0x76,0x6B,0x20,0x53,0x8B,0x63,0x89,0x75,0x35,0x4F,0xA7,0x7F,0xFB,\
						0x8F,0xDB,0x51,0xFA,0x53,0x3A,0x57,0xDF,0x77,0xE9,0x59,0x1A,0x8F,0xB9,0x57,0x06,0x5F,0x62,0x97,0x5E,\
						0x6C,0xD5,0x4F,0x4D,0x79,0xFB,0x70,0xB9,0x70,0x6B,0x65,0xF6,0x50,0x5C,0x5F,0x53,0x7D,0x2F,\
						0x8B,0xA1,0x77,0xED,0x8D,0xEF,0x50,0x4F,0x79,0xBB,0x6B,0xB5,0x88,0x4C,0x95,0xF4,0x4E,0x0D,\
						0x8D,0xB3,0x8F,0xC7,0x95,0x7F,0x30,0x10,0x4E,0x1C,0x98,0xCE,0x7B,0xA1,0x5B,0xB6,0x30,0x11,\
						0x60,0xA8,0x76,0x84,0x89,0xE3,0x95,0x01,0x78,0x01,0x66,0x2F,0xFF,0x1A};


/***短信发送数据链表***/
struct list_head sms_send_list;

/***短信发送状态***/
static uint8_t sms_send_state=EAT_FALSE;
static void eat_sms_read_cb(EatSmsReadCnf_st  smsReadCnfContent)
{
    u8 format =0;
    eat_get_sms_format(&format);
    if(1 == format)//TEXT模式
    {
        eat_trace("eat_sms_read_cb, format->>text 模式");
    }
    else//PDU模式
    {
        eat_trace("eat_sms_read_cb, format->>PDU 模式:%s",smsReadCnfContent.data);
        receive_pdu_sms_pro(smsReadCnfContent.data,&smsReadCnfContent.len);
    }
}
static eat_sms_flash_message_cb(EatSmsReadCnf_st smsFlashMessage)
{
    u8 format =0;
    eat_get_sms_format(&format);
    eat_trace("eat_sms_flash_message_cb, format=%d",format);
    if(1 == format)//TEXT模式
    {
    }
    else//PDU模式
    {
    }
}

static void eat_sms_delete_cb(eat_bool result)
{
    eat_trace("eat_sms_delete_cb, result=%d",result);
}

static void eat_sms_send_cb(eat_bool result)
{
    eat_trace("eat_sms_send_cb, result=%d",result);
    if(result == EAT_TRUE)
    {
        if(sms_cmd_rest_cmd==EAT_TRUE)
        {
            //eat_reset_module();
        }
    	if(sms_send_state == EAT_TRUE)
		{
			sms_send_list_del(&sms_send_list);
			sms_send_state = EAT_FALSE;//短信发送成功
		}
    }
}
Sms_Delete_Completed Sms_Delete_Completed_cb(bool_t result)
{
    static uint16_t ii=1;
    #if sms_receive_send_debuge_pintf
    eat_trace("Sms_Delete_Completed->>resust:%d !",result);
    #endif
    if(result==EAT_TRUE)
    {
        ii++;
        sms_stoarg_delet(ii);
    }
    else
    {
         ii=10;  
    }
}
void sms_stoarg_delet(uint16_t index)
{
    eat_delete_sms(index,Sms_Delete_Completed_cb);
    #if sms_receive_send_debuge_pintf
    eat_trace("sms_stoarg_delet ->> resust  !!");
    #endif
}
static eat_sms_new_message_cb(EatSmsNewMessageInd_st smsNewMessage)
{
    #if sms_receive_send_debuge_pintf
	eat_trace("eat_sms_new_message_cb, storage=%d,index=%d",smsNewMessage.storage,smsNewMessage.index);
    #endif
    eat_read_sms(smsNewMessage.index,eat_sms_read_cb);
}

static void eat_sms_ready_cb(eat_bool result)
{
    #if sms_receive_send_debuge_pintf
    eat_trace("eat_sms_ready_cb, result=%d",result);
    #endif
    if(result==EAT_TRUE)
    {
        sms_stoarg_delet(1);
    }
}
void init_sms_setting(void)
{
	eat_set_sms_operation_mode(EAT_TRUE);
	eat_set_sms_cnmi(2,1,0,0,0);
    eat_sms_register_sms_ready_callback(eat_sms_ready_cb);
    eat_sms_register_new_message_callback(eat_sms_new_message_cb);//收到短信
    eat_sms_register_flash_message_callback(eat_sms_flash_message_cb);
    eat_sms_register_send_completed_callback(eat_sms_send_cb);
    enter_at_cmd_send_queue(&at_cmd,AT_DELET_ALL_SMS);
}

/* ===================================================================================== 
* 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数  
* 如："8613722216254" --> "683127226152F4"  
* pSrc: 源字符串指针  
* pDst: 目标字符串指针  
* nSrcLength: 源字符串长度  
* 返回: 目标字符串长度  
* =====================================================================================
*/ 
int gsmInvertNumbers(u8* pSrc,u8* pDst,int nSrcLength)  
{  
	int nDstLength;   // 目标字符串长度  
	char ch;    // 用于保存一个字符  
	int i; 
	// 复制串长度  
	nDstLength = nSrcLength;  
	  
	// 两两颠倒  
	for(i=0; i<nSrcLength;i+=2)  
	{  
	   ch = *pSrc++;   // 保存先出现的字符  
	   *pDst++ = *pSrc++; // 复制后出现的字符  
	   *pDst++ = ch;   // 复制先出现的字符  
	}  
	  
	// 源串长度是奇数吗？  
	if(nSrcLength & 1)  
	{  
	   *(pDst-2) = 'F'; // 补'F'  
	   nDstLength++;   // 目标串长度加1  
	}  
	  
	// 输出字符串加个结束符  
	*pDst = '/0';  
	  
	// 返回目标字符串长度  
	return nDstLength;  
}
/* ===================================================================================== 
* 7-bit编码  
* pSrc: 源字符串指针  
* pDst: 目标编码串指针  
* nSrcLength: 源字符串长度  
* 返回: 目标编码串长度
* =====================================================================================
*/
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)  
{  
    int nSrc;        // 源字符串的计数值  
    int nDst;        // 目标编码串的计数值  
    int nChar;       // 当前正在处理的组内字符字节的序号，范围是0-7  
    unsigned char nLeft;    // 上一字节残余的数据  
      
    // 计数值初始化  
    nSrc = 0;  
    nDst = 0;  
      
    // 将源串每8个字节分为一组，压缩成7个字节  
    // 循环该处理过程，直至源串被处理完  
    // 如果分组不到8字节，也能正确处理  
    while(nSrc<nSrcLength)  
    {  
        // 取源字符串的计数值的最低3位  
        nChar = nSrc & 7;  
    
        // 处理源串的每个字节  
        if(nChar == 0)  
        {  
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用  
            nLeft = *pSrc;  
        }  
        else  
        {  
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节  
            *pDst = (*pSrc << (8-nChar)) | nLeft;  
     
            // 将该字节剩下的左边部分，作为残余数据保存起来  
            nLeft = *pSrc >> nChar;  
            // 修改目标串的指针和计数值 pDst++;  
            nDst++;   
        }   
          
        // 修改源串的指针和计数值  
        pSrc++; nSrc++;  
    }  
      
    // 返回目标串长度  
    return nDst;   
}
/* ===================================================================================== 
*  7-bit解码  
*  pSrc: 源编码串指针  
*  pDst: 目标字符串指针  
*  nSrcLength: 源编码串长度  
*  返回: 目标字符串长度  
* =====================================================================================
*/
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
    int nSrc;        // 源字符串的计数值  
    int nDst;        // 目标解码串的计数值  
    int nByte;       // 当前正在处理的组内字节的序号，范围是0-6  
    unsigned char nLeft; // 上一字节残余的数据  
      
    // 计数值初始化  
    nSrc = 0;  
    nDst = 0;  
      
    // 组内字节序号和残余数据初始化  
    nByte = 0;  
    nLeft = 0;  
      
    // 将源数据每7个字节分为一组，解压缩成8个字节  
    // 循环该处理过程，直至源数据被处理完  
    // 如果分组不到7字节，也能正确处理  
    while(nSrc<nSrcLength)  
    {  
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节  
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;  
        // 将该字节剩下的左边部分，作为残余数据保存起来  
        nLeft = *pSrc >> (7-nByte);  
    
        // 修改目标串的指针和计数值  
        pDst++;  
        nDst++;  
    
        // 修改字节计数值  
        nByte++;  
    
        // 到了一组的最后一个字节  
        if(nByte == 7)  
        {  
            // 额外得到一个目标解码字节  
            *pDst = nLeft;  
     
            // 修改目标串的指针和计数值  
            pDst++;  
            nDst++;  
     
            // 组内字节序号和残余数据初始化  
            nByte = 0;  
            nLeft = 0;  
        }  
    
        // 修改源串的指针和计数值  
        pSrc++;  
        nSrc++;  
    }  
      
    *pDst = 0;  
      
    // 返回目标串长度  
    return nDst;  
}  
/* ===================================================================================== 
*   8bit编码   
*   输入:   pSrc   -   源字符串指针   
*           nSrcLength   -   源字符串长度   
*   输出:   pDst   -   目标编码串指针   
*   返回:   目标编码串长度 
* =====================================================================================
*/
int gsmEncode8bit(const char* pSrc,unsigned char* pDst,int nSrcLength)   
{   
	//   简单复制   
	memcpy(pDst,pSrc,nSrcLength);   
	return   nSrcLength;   
}  
/* =====================================================================================     
* UCS2编码  
* 输入: pSrc - 源字符串指针   
* nSrcLength - 源字符串长度  
* 输出: pDst - 目标编码串指针  
* 返回: 目标编码串长度  
* =====================================================================================
*/
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{  
  
     int nDstLength;        // UNICODE宽字符数目  
     WCHAR wchar[128]; // UNICODE串缓冲区  
     int i;
     nDstLength = mbstowcs(wchar,pSrc,nSrcLength);  //吧多字节转换位UNICOUDE
    // eat_trace("PDU转化字符串长度%d",nDstLength);
     // 高低字节对调，输出  
     for(i=0; i<nDstLength; i++)  
     {  
         *pDst++ = wchar[i] >> 8;         // 先输出高位字节  
         *pDst++ = wchar[i] & 0xff;       // 后输出低位字节  
     }  
       
	// 返回目标编码串长度  
     return nDstLength * 2;  
}  
/* =====================================================================================       
* UCS2解码   
* 输入: pSrc - 源编码串指针  
* nSrcLength - 源编码串长度  
* 输出: pDst - 目标字符串指针  
* 返回: 目标字符串长度  
	原型
	size_t wcstombs(char *dest, const wchar_t *src, size_t n);
	说明：
	如dest 非NULL，则wcstombs() function把宽字符src转换成多字符dest，最多转换n个字节(这里的n是转换后dest的字节数)。
* =====================================================================================
*/   
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
     int nDstLength;        // UNICODE宽字符数目  
     WCHAR wchar[128]; // UNICODE串缓冲区  
     int i;
     // 高低字节对调，拼成UNICODE  
     for(i=0; i<nSrcLength/2; i++)  
     {  
         wchar[i] = *pSrc++ << 8;    // 先高位字节  
         wchar[i] |= *pSrc++;        // 后低位字节  
     }  
     // UNICODE串-->字符串 
     setlocale(LC_ALL,"chs");
     nDstLength = wcstombs(pDst,wchar,128);  
     // 输出字符串加个结束符  
     pDst[nDstLength] = '/0';  
      
// 返回目标字符串长度  
     return nDstLength;  
}
/* =====================================================================================       
* 可打印字符串转换为字节数据  
* 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}  
* pSrc: 源字符串指针  
* pDst: 目标数据指针  
* nSrcLength: 源字符串长度  
* 返回: 目标数据长度 
* =====================================================================================
*/  
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)  
{  
	int i;
    for(i=0; i<nSrcLength; i+=2)  
    {  
        // 输出高4位  
        if(*pSrc>='0' && *pSrc<='9')  
        {  
            *pDst = (*pSrc - '0') << 4;  
        }  
        else  
        {  
            *pDst = (*pSrc - 'A' + 10) << 4;  
        }  
    
        pSrc++;  
    
        // 输出低4位  
        if(*pSrc>='0' && *pSrc<='9')  
        {  
            *pDst |= *pSrc - '0';  
        }  
        else  
        {  
            *pDst |= *pSrc - 'A' + 10;  
        }  
        pSrc++;  
        pDst++;  
    }  
      
    // 返回目标数据长度  
    return nSrcLength / 2;  
} 
/* =====================================================================================         
* 字节数据转换为可打印字符串  
* 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"   
* pSrc: 源数据指针  
* pDst: 目标字符串指针  
* nSrcLength: 源数据长度  
* 返回: 目标字符串长度
* =====================================================================================
*/ 
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表  
    int i;  
    for(i=0; i<nSrcLength; i++)  
    {  
        // 输出低4位  
        *pDst++ = tab[*pSrc >> 4];  
    
        // 输出高4位  
        *pDst++ = tab[*pSrc & 0x0f];  
    
        pSrc++;  
    }  
      
    // 输出字符串加个结束符  
    *pDst = '/0';  
      
    // 返回目标字符串长度  
    return nSrcLength * 2;  
} 
/* =====================================================================================     
* UCS2编码  
* 输入: pSrc - 源字符串指针   
* nSrcLength - 源字符串长度  
* 输出: pDst - 目标编码串指针  
* 返回: 目标编码串长度  
	函数原型：
	size_t mbstowcs(wchar_t *dest, const char *src, size_t n);
	说明：
	如dest 非NULL，则mbstowcs() 函数把多字符src转换成宽字符dest，最多转换n字节。
* =====================================================================================
*/
int EnCODE_USC2(u8* pSrc, char* pDst,u8 Length) 
{
	u8 Len=0;
	u16 II=0;
	int Re_len=0;
	int nLength=0;
	u8 SIG=EAT_FALSE;
	u8 Data_Pest[256]={0};
	for(Len=0;Len<Length;Len++)
	{
		if(*(pSrc+Len)<128)
		{			
			*(Data_Pest+Re_len)=0x00;
			*(Data_Pest+Re_len+1)=*(pSrc+Len);
			Re_len+=2;
		}
		else
		{
			for(II=0;II<strlen(GBK);II+=2)
			{
				if((*(pSrc+Len)==GBK[II])&&(*(pSrc+Len+1)==GBK[II+1]))
				{
					*(Data_Pest+Re_len)=PUD_STRING[II];
					*(Data_Pest+Re_len+1)=PUD_STRING[II+1];
					SIG=EAT_TRUE;
					break;
				}
			}
			if(EAT_TRUE==SIG)
			{
				Len++;
				Re_len+=2;
				SIG=EAT_FALSE;
			}
			else
			{
				return -1;
			}
		}
	}
	*Data_Pest=0;
	nLength=gsmBytes2String(Data_Pest,pDst,Re_len);  
	return nLength;
}
int DEnCODE_USC2(u8* pSrc, char* pDst,u8 Length) 
{
	u8 Len=0;
	u16 II=0;
	int Re_len=0;
	u8 SIG=EAT_FALSE;
for(Len=0;Len<Length;Len++)
	{
        if(*(pSrc+Len)==0x00)
        {
            if(*(pSrc+Len+1)!=0x00)
            {
                *pDst++=*(pSrc+Len+1);
                 Re_len+=1;
                 Len++;
            }
            else
            {
				return -1;
            }
        }
		else
		{
			for(II=0;II<strlen(GBK);II+=2)
			{
				if((*(pSrc+Len)==PUD_STRING[II])&&(*(pSrc+Len+1)==PUD_STRING[II+1]))
				{
					*pDst++=GBK[II];
					*pDst++=GBK[II+1];
					SIG=EAT_TRUE;
					break;
				}
			}
			if(EAT_TRUE==SIG)
			{
				Len++;
				Re_len+=2;
				SIG=EAT_FALSE;
			}
			else
			{
				return -1;
			}
		}
	}
	*pDst++=0;
	return Re_len;
}

int DeCODE_8bit(u8* pSrc, char* pDst,u8 Length) 
{
	u8 Len=0;
	u16 II=0;
	int Re_len=0;//字节数
	u8 SMS_Buf[140]={0};
	gsmString2Bytes(pSrc,SMS_Buf,Length);
	for(Len=0;Len<Length/2;Len+=2)
	{
		if((*(SMS_Buf+Len)==0x00)&&(*(SMS_Buf+Len+1)!=0x00))
		{
			*pDst++=*(SMS_Buf+Len+1);
			Re_len+=1;
		}
		else
		{
		    *pDst++=*(SMS_Buf+Len);
		    *pDst++=*(SMS_Buf+Len+1);
		    Re_len+=2;
		}
	}
	*pDst='/0';
	return Re_len;
}
/* =====================================================================================     
* Uncode 字符个数
* 输入: pSrc - buf
* nSrcLength - 源字符串长度  
* 输出: pDst - 目标编码串指针  
* 返回: 目标编码串长度  
* =====================================================================================
*/
u8 ASCALL_COUNT(u8*buff,u8 Lenghth)
{
	u8 II=0;
	u8 Ret_Len=0;
	for(II=0;II<Lenghth;II++)
	{
		if(*(buff+II)>=128)
		{
			II++;	
		}
		Ret_Len+=2;
	}
	return Ret_Len;
}
/* =====================================================================================           
* PDU编码，用于编制、发送短消息  
* pSrc: 源PDU参数指针  
* pDst: 目标PDU串指针  
* 返回: 目标PDU串长度  
* =====================================================================================
*/
static int gsmEncodePdu(SM_PARAM* pSrc, char* pDst)  
{  
  
    int nLength=0;             // 内部用的串长度  
    int nDstLength=0;          // 目标PDU串长度  
    int Ret=0;
    unsigned char buf[256]={0}; // 内部用的缓冲区  
    nLength = strlen(pSrc->TPA);    // TP-DA地址字符串的长度  
    buf[0] = 0x00;
    buf[1] = 0x11;            // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)  
    buf[2] = 0x00;               // TP-MR=0  
    buf[3] = (char)(nLength+2); // 目标地址数字个数(TP-DA地址字符串真实长度)  
    buf[4] = 0x91;              // 固定: 用国际格式号码  
    buf[5] = 0x68;
    nDstLength +=gsmBytes2String(buf, &pDst[0],6); // 转换4个字节到目标PDU串  
    nDstLength +=gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength],nLength); // 转换TP-DA到目标PDU串   
    // TPDU段协议标识、编码方式、用户信息等  
    nLength = strlen(pSrc->TP_UD);    // 用户信息字符串的长度  
    buf[0] = pSrc->TP_PID;        // 协议标识(TP-PID)  
    buf[1] = pSrc->TP_DCS;        // 用户信息编码方式(TP-DCS)  
    buf[2] = 0x01;            // 有效期(TP-VP)为5分钟  
    buf[3] = ASCALL_COUNT(pSrc->TP_UD,nLength);
    nDstLength +=gsmBytes2String(buf, &pDst[nDstLength],4);        // 转换该段数据到目标PDU串  
    if(pSrc->TP_DCS == GSM_7BIT)      
    {  
        // 7-bit编码方式  
        nDstLength +=eat_acsii_to_gsm7bit(pSrc->TP_UD,nLength,&pDst[nDstLength]);  // 转换TP-DA到目标PDU串
        
    }  
    else if(pSrc->TP_DCS == GSM_UCS2)  
    {  
		Ret=EnCODE_USC2(pSrc->TP_UD,&pDst[nDstLength],nLength);
		if(Ret>0)
		{
			nDstLength=nDstLength + Ret;
		}
		else
		{
			nDstLength=-1;
		}
    }  
    else  
    {  
        // 8-bit编码方式  
       nDstLength +=gsmEncode8bit(pSrc->TP_UD,&pDst[nDstLength],nLength);    // 转换TP-DA到目标PDU串  
    }  
    
    // 返回目标字符串长度  
    return nDstLength;  
} 

/* 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"
// pSrc: 源字符串指针
// pDst: 目标字符串指针
// nSrcLength: 源字符串长度
//返回: 目标字符串长度*/
static int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
        int nDstLength;   // 目标字符串长度
        char ch;          // 用于保存一个字符
        int i=0;
        // 复制串长度
        nDstLength = nSrcLength;
        // 两两颠倒
        for(i=0; i<nSrcLength;i+=2)
        {
            ch = *pSrc++;        // 保存先出现的字符
            *pDst++ = *pSrc++;   // 复制后出现的字符
            *pDst++ = ch;        // 复制先出现的字符
        }
        // 最后的字符是'F'吗？
        if(*(pDst-1) == 'F')
        {
            pDst--;
            nDstLength--;        // 目标字符串长度减1
        }
        // 输出字符串加个结束符
        *pDst = '\0';
        // 返回目标字符串长度
        return nDstLength;
    }
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: sms_setting_proc
 *  Description: system clock time base 1s
 *        Input:            
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
        int nDstLength;          // 目标PDU串长度
        unsigned char tmp;       // 内部用的临时字节变量
        unsigned char buf[512]; // 内部用的缓冲区
        // SMSC地址信息段
        gsmString2Bytes(pSrc, &tmp, 2);    // 取长度
        tmp = (tmp - 1) * 2;    // SMSC号码串长度
        pSrc += 4;              // 指针后移
        gsmSerializeNumbers(pSrc, pDst->SCA, tmp);    // 转换SMSC号码到目标PDU串
        pSrc += tmp;        // 指针后移
        // TPDU段基本参数、回复地址等
        gsmString2Bytes(pSrc, &tmp, 2);    // 取基本参数
        pSrc += 2;        // 指针后移
        //if(tmp & 0x80)
        //{
            // 包含回复地址，取回复地址信息
            gsmString2Bytes(pSrc, &tmp, 2);    // 取长度
            if(tmp & 1) tmp += 1;    // 调整奇偶性
            pSrc += 4;          // 指针后移
            gsmSerializeNumbers(pSrc, pDst->TPA, tmp);    // 取TP-RA号码
            pSrc += tmp;        // 指针后移
        //}
        // TPDU段协议标识、编码方式、用户信息等
        gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // 取协议标识(TP-PID)
        pSrc += 2;        // 指针后移
        gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // 取编码方式(TP-DCS)
        pSrc += 2;        // 指针后移
        gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // 服务时间戳字符串(TP_SCTS)
        pSrc += 14;       // 指针后移
        gsmString2Bytes(pSrc, &tmp, 2);    // 用户信息长度(TP-UDL)
        pSrc += 2;        // 指针后移
        if((pDst->TP_DCS == 0x00)||(pDst->TP_DCS == 0x00)||(pDst->TP_DCS == 0xF0)||(pDst->TP_DCS == 0xF1)||(pDst->TP_DCS == 0xF2)||(pDst->TP_DCS == 0xF3))   
        {
            // 7-bit解码
            nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4); // 格式转换
            gsmDecode7bit(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
            nDstLength = tmp;
        }
        else if((pDst->TP_DCS == 0xF4)||(pDst->TP_DCS == 0xF5)||(pDst->TP_DCS == 0xF6)||(pDst->TP_DCS == 0xF7))
        {
            // 8-bit解码
            nDstLength = DeCODE_8bit((u8 *)pSrc,buf,tmp * 2);
            memcpy(pDst->TP_UD,buf,nDstLength);// 转换到TP-DU
        }
        else
        {
            // UCS2解码
            nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);        // 格式转换
            HexToAsicc(buf,nDstLength);
            nDstLength = DEnCODE_USC2(buf, pDst->TP_UD, nDstLength);    // 转换到TP-DU
        }
        // 返回目标字符串长度
        return nDstLength;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: sms_msg_send_envelope
 *  Description: sms_msg_send_envelope 
 *        Input: *pSrc :短信内容,  Dest:短信地址         
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void sms_msg_send_envelope(u8*pSrc,u8*Dest)
{
	int PDU_Data_Length=0;
	SM_PARAM Msg_Informatin;
	uint8_t pDst[512]={0};
	uint8_t format = 0;  
	uint8_t scNumber[40]={"+8613800270500"};//短信中心号码
	memset(pDst,0,256);
	eat_set_sms_sc(scNumber);
	eat_set_sms_format(0);//设置为PUD短信	
	memset(Msg_Informatin.TPA,0,16);
	memcpy(Msg_Informatin.TPA,Dest,strlen(Dest));//接收手机号
	memset(Msg_Informatin.TP_UD,0,161);
	memcpy(Msg_Informatin.TP_UD,pSrc,strlen(pSrc));//原始数据	
	Msg_Informatin.TP_PID=0;
	Msg_Informatin.TP_DCS=GSM_UCS2;
	PDU_Data_Length=gsmEncodePdu(&Msg_Informatin,pDst); //数制转换eat_set_sms_format(0);
	if(PDU_Data_Length<0) return;
	eat_send_pdu_sms(pDst,(PDU_Data_Length/2)-1);

}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void sms_time_out_handle(void)
 *  Description: 短信发送超时处理
 *        Input: 每次超市30s 工两次        
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void sms_send_time_out_handle(void)
{
    static uint8_t sms_send_time=0;
    static uint8_t sms_send_times=0;
    if(sms_send_state==EAT_FALSE)
    {
        sms_send_time =0;
        sms_send_times =0;
        #if sms_receive_send_debuge_pintf 
        eat_trace("sms_send_time_out_handle--> send_msg.");
        #endif
        sms_send_action_pro(&sms_send_list);
    }
    else
    {
        sms_send_time++;
        if(sms_send_time==30)
        {
           sms_send_time=0;
           sms_send_times++;
           if(sms_send_times==2)//两次超时不再发送该函数
           {
              sms_send_times=0;
              sms_send_list_del(&sms_send_list);
           }
           sms_send_state=EAT_FALSE;
        }
    }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void package_warning_sms_Infor(u8*Data,u8 *buffer)
 *  Description: 封装报警信息
 *        Input: data,报警信息内容，buffer,封装数据输出       
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
static void package_warning_sms_Infor(u8*Data,u8 *buffer)
{
	uint8_t Year[3]={0};
	uint8_t Mon[3]={0};
	uint8_t Day[3]={0};
	uint8_t H[3]={0};
	uint8_t F[3]={0};
	uint8_t S[3]={0};
	EatRtc_st rtc = {0};
	eat_get_rtc(&rtc);
	Data_TO_Char(rtc.year,Year);
	Data_TO_Char(rtc.mon,Mon);
	Data_TO_Char(rtc.day,Day);
	Data_TO_Char(rtc.hour,H);
	Data_TO_Char(rtc.min,F);
	Data_TO_Char(rtc.sec,S); 
	if(system_param.terminl_registe.plate_coror != 0)
	{
		sprintf(buffer,"20%s-%s-%s %s:%s:%s %s:%s",Year,Mon,Day,H,F,S,system_param.terminl_registe.plate_number,Data);
	}
	else
	{
		sprintf(buffer,"20%s-%s-%s %s:%s:%s %s:%s",Year,Mon,Day,H,F,S,system_param.terminl_registe.vehicle_vin,Data);
	}
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void worning_sms_send(u8* ucode_data)
 *  Description: 发送报警信息
 *        Input: ucode_data,报警信息内容     
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void worning_sms_send(u8* ucode_data)
{
    uint8_t warnin_dat[128]={0};
	if((strlen(ucode_data)!=0)&&(strlen(system_param.phone_number_param.terminal_sms_send)!=0))
	{
	    memset(warnin_dat,0,sizeof(warnin_dat));
	    package_warning_sms_Infor(ucode_data,warnin_dat);
	    send_sms_data_pro(warnin_dat,system_param.phone_number_param.terminal_sms_send);
	}
	else
    {
        #if sms_receive_send_debuge_pintf 
        eat_trace("worning_sms_send--> 短信号码为空 .");
        #endif
    }
}
/* =====================================================================================           
* 用于短信内容解析匹配  
* pSrc: 源PDU参数指针  
* Data_Len:   字符串长度
* 返回: NULL
* =====================================================================================
*/
void receive_pdu_sms_pro(u8 *pScr,u16 *nLength)
{
    uint8_t data_ret;
    SM_PARAM Rev_SMS;
    memset(&Rev_SMS,0,sizeof(Rev_SMS));
    *nLength=gsmDecodePdu(pScr,&Rev_SMS);
    data_ret=sms_action_proc(Rev_SMS.TP_UD,*nLength,Rev_SMS.TPA + 2);
}
/***短信发送链表***/
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_list_init(void)
 *  Description:  初始化短信发送链表
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void sms_send_list_init(void)
{
    INIT_LIST_HEAD(&sms_send_list);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_list_add(struct list_head *list_parm,uint8_t *buff,uint16_t data_cnt)
 *  Description:  添加发送短信信息
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void sms_send_list_add(struct list_head *list_parm,uint8_t *buff,uint8_t *dest)
{
   sms_send_pack_list *mem_prt;
   mem_prt =(sms_send_pack_list *)eat_mem_alloc(sizeof(sms_send_pack_list));
   memset(mem_prt,0,sizeof(sms_send_pack_list));
   memcpy(mem_prt->data_buf,buff,strlen(buff));
   memcpy(mem_prt->send_addr,dest,strlen(dest));
   if(is_list_enful(list_parm,sms_send_max)==EAT_TRUE)
   {
       list_del(list_parm);
   }
   tail_list_add(&(mem_prt->re_list),list_parm,sms_send_max);
}
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  发送短信
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void sms_send_action_pro(struct list_head *list_parm)
{
    sms_send_pack_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (sms_send_pack_list *)list_entry(list_parm->next,sms_send_pack_list,re_list);
        sms_msg_send_envelope(mem_prt->data_buf,mem_prt->send_addr);//发送数据
        sms_send_state=EAT_TRUE;
    }
        else
        {
            #if sms_receive_send_debuge_pintf 
            eat_trace("sms_send_action_pro--> list is empty.");
            #endif
        }
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void send_sms_data_pro(uint8_t *buff,uint8_t *dest)
 *  Description: 向短信发送链表中压入信息
 *        Input:       
 *       Output:
 *       Return:               
 *       author: jxb.
 * =====================================================================================
 */
void send_sms_data_pro(uint8_t *buff,uint8_t *dest)
{
    sms_send_list_add(&sms_send_list,buff,dest);
}

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_action_pro(struct list_head *list_parm)
 *  Description:  从链表中删除发送成功的短信
 *  Parameters : *q 
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-10-8
 * =====================================================================================
 */ 
void sms_send_list_del(struct list_head *list_parm)
{
    sms_send_pack_list *mem_prt=NULL;
    if(list_empty(list_parm)!=EAT_TRUE)
    {
        mem_prt = (sms_send_pack_list *)list_entry(list_parm->next,sms_send_pack_list,re_list);
        list_del(list_parm->next);
        eat_mem_free(mem_prt);
    }
    else
    {
        #if sms_receive_send_debuge_pintf 
        eat_trace("sms_send_list_del--> list is empty.");
        #endif
    }
}

