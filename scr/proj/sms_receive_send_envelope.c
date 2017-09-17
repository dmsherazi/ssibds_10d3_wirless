
#include "sms_receive_send_envelope.h"


const u8 GBK[]="���ƺű�������������������غڼ��ɼ�������ԥ³��������մ�ǭ���������̨��۰Ľ������߶Ͽ���ײ����Ԥƣ�ͼ�ʻǷѹ����෭��������ض��Բ�ηǷ�λ�Ƶ��ʱͣ���ۼƶ�·ƫ����м䲻�����������ܼҡ����Ľ������ǣ�";
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


/***���ŷ�����������***/
struct list_head sms_send_list;

/***���ŷ���״̬***/
static uint8_t sms_send_state=EAT_FALSE;
static void eat_sms_read_cb(EatSmsReadCnf_st  smsReadCnfContent)
{
    u8 format =0;
    eat_get_sms_format(&format);
    if(1 == format)//TEXTģʽ
    {
        eat_trace("eat_sms_read_cb, format->>text ģʽ");
    }
    else//PDUģʽ
    {
        eat_trace("eat_sms_read_cb, format->>PDU ģʽ:%s",smsReadCnfContent.data);
        receive_pdu_sms_pro(smsReadCnfContent.data,&smsReadCnfContent.len);
    }
}
static eat_sms_flash_message_cb(EatSmsReadCnf_st smsFlashMessage)
{
    u8 format =0;
    eat_get_sms_format(&format);
    eat_trace("eat_sms_flash_message_cb, format=%d",format);
    if(1 == format)//TEXTģʽ
    {
    }
    else//PDUģʽ
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
			sms_send_state = EAT_FALSE;//���ŷ��ͳɹ�
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
    eat_sms_register_new_message_callback(eat_sms_new_message_cb);//�յ�����
    eat_sms_register_flash_message_callback(eat_sms_flash_message_cb);
    eat_sms_register_send_completed_callback(eat_sms_send_cb);
    enter_at_cmd_send_queue(&at_cmd,AT_DELET_ALL_SMS);
}

/* ===================================================================================== 
* ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��  
* �磺"8613722216254" --> "683127226152F4"  
* pSrc: Դ�ַ���ָ��  
* pDst: Ŀ���ַ���ָ��  
* nSrcLength: Դ�ַ�������  
* ����: Ŀ���ַ�������  
* =====================================================================================
*/ 
int gsmInvertNumbers(u8* pSrc,u8* pDst,int nSrcLength)  
{  
	int nDstLength;   // Ŀ���ַ�������  
	char ch;    // ���ڱ���һ���ַ�  
	int i; 
	// ���ƴ�����  
	nDstLength = nSrcLength;  
	  
	// �����ߵ�  
	for(i=0; i<nSrcLength;i+=2)  
	{  
	   ch = *pSrc++;   // �����ȳ��ֵ��ַ�  
	   *pDst++ = *pSrc++; // ���ƺ���ֵ��ַ�  
	   *pDst++ = ch;   // �����ȳ��ֵ��ַ�  
	}  
	  
	// Դ��������������  
	if(nSrcLength & 1)  
	{  
	   *(pDst-2) = 'F'; // ��'F'  
	   nDstLength++;   // Ŀ�괮���ȼ�1  
	}  
	  
	// ����ַ����Ӹ�������  
	*pDst = '/0';  
	  
	// ����Ŀ���ַ�������  
	return nDstLength;  
}
/* ===================================================================================== 
* 7-bit����  
* pSrc: Դ�ַ���ָ��  
* pDst: Ŀ����봮ָ��  
* nSrcLength: Դ�ַ�������  
* ����: Ŀ����봮����
* =====================================================================================
*/
int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)  
{  
    int nSrc;        // Դ�ַ����ļ���ֵ  
    int nDst;        // Ŀ����봮�ļ���ֵ  
    int nChar;       // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7  
    unsigned char nLeft;    // ��һ�ֽڲ��������  
      
    // ����ֵ��ʼ��  
    nSrc = 0;  
    nDst = 0;  
      
    // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�  
    // ѭ���ô�����̣�ֱ��Դ����������  
    // ������鲻��8�ֽڣ�Ҳ����ȷ����  
    while(nSrc<nSrcLength)  
    {  
        // ȡԴ�ַ����ļ���ֵ�����3λ  
        nChar = nSrc & 7;  
    
        // ����Դ����ÿ���ֽ�  
        if(nChar == 0)  
        {  
            // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��  
            nLeft = *pSrc;  
        }  
        else  
        {  
            // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�  
            *pDst = (*pSrc << (8-nChar)) | nLeft;  
     
            // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������  
            nLeft = *pSrc >> nChar;  
            // �޸�Ŀ�괮��ָ��ͼ���ֵ pDst++;  
            nDst++;   
        }   
          
        // �޸�Դ����ָ��ͼ���ֵ  
        pSrc++; nSrc++;  
    }  
      
    // ����Ŀ�괮����  
    return nDst;   
}
/* ===================================================================================== 
*  7-bit����  
*  pSrc: Դ���봮ָ��  
*  pDst: Ŀ���ַ���ָ��  
*  nSrcLength: Դ���봮����  
*  ����: Ŀ���ַ�������  
* =====================================================================================
*/
int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
    int nSrc;        // Դ�ַ����ļ���ֵ  
    int nDst;        // Ŀ����봮�ļ���ֵ  
    int nByte;       // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6  
    unsigned char nLeft; // ��һ�ֽڲ��������  
      
    // ����ֵ��ʼ��  
    nSrc = 0;  
    nDst = 0;  
      
    // �����ֽ���źͲ������ݳ�ʼ��  
    nByte = 0;  
    nLeft = 0;  
      
    // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�  
    // ѭ���ô�����̣�ֱ��Դ���ݱ�������  
    // ������鲻��7�ֽڣ�Ҳ����ȷ����  
    while(nSrc<nSrcLength)  
    {  
        // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�  
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;  
        // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������  
        nLeft = *pSrc >> (7-nByte);  
    
        // �޸�Ŀ�괮��ָ��ͼ���ֵ  
        pDst++;  
        nDst++;  
    
        // �޸��ֽڼ���ֵ  
        nByte++;  
    
        // ����һ������һ���ֽ�  
        if(nByte == 7)  
        {  
            // ����õ�һ��Ŀ������ֽ�  
            *pDst = nLeft;  
     
            // �޸�Ŀ�괮��ָ��ͼ���ֵ  
            pDst++;  
            nDst++;  
     
            // �����ֽ���źͲ������ݳ�ʼ��  
            nByte = 0;  
            nLeft = 0;  
        }  
    
        // �޸�Դ����ָ��ͼ���ֵ  
        pSrc++;  
        nSrc++;  
    }  
      
    *pDst = 0;  
      
    // ����Ŀ�괮����  
    return nDst;  
}  
/* ===================================================================================== 
*   8bit����   
*   ����:   pSrc   -   Դ�ַ���ָ��   
*           nSrcLength   -   Դ�ַ�������   
*   ���:   pDst   -   Ŀ����봮ָ��   
*   ����:   Ŀ����봮���� 
* =====================================================================================
*/
int gsmEncode8bit(const char* pSrc,unsigned char* pDst,int nSrcLength)   
{   
	//   �򵥸���   
	memcpy(pDst,pSrc,nSrcLength);   
	return   nSrcLength;   
}  
/* =====================================================================================     
* UCS2����  
* ����: pSrc - Դ�ַ���ָ��   
* nSrcLength - Դ�ַ�������  
* ���: pDst - Ŀ����봮ָ��  
* ����: Ŀ����봮����  
* =====================================================================================
*/
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{  
  
     int nDstLength;        // UNICODE���ַ���Ŀ  
     WCHAR wchar[128]; // UNICODE��������  
     int i;
     nDstLength = mbstowcs(wchar,pSrc,nSrcLength);  //�ɶ��ֽ�ת��λUNICOUDE
    // eat_trace("PDUת���ַ�������%d",nDstLength);
     // �ߵ��ֽڶԵ������  
     for(i=0; i<nDstLength; i++)  
     {  
         *pDst++ = wchar[i] >> 8;         // �������λ�ֽ�  
         *pDst++ = wchar[i] & 0xff;       // �������λ�ֽ�  
     }  
       
	// ����Ŀ����봮����  
     return nDstLength * 2;  
}  
/* =====================================================================================       
* UCS2����   
* ����: pSrc - Դ���봮ָ��  
* nSrcLength - Դ���봮����  
* ���: pDst - Ŀ���ַ���ָ��  
* ����: Ŀ���ַ�������  
	ԭ��
	size_t wcstombs(char *dest, const wchar_t *src, size_t n);
	˵����
	��dest ��NULL����wcstombs() function�ѿ��ַ�srcת���ɶ��ַ�dest�����ת��n���ֽ�(�����n��ת����dest���ֽ���)��
* =====================================================================================
*/   
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
     int nDstLength;        // UNICODE���ַ���Ŀ  
     WCHAR wchar[128]; // UNICODE��������  
     int i;
     // �ߵ��ֽڶԵ���ƴ��UNICODE  
     for(i=0; i<nSrcLength/2; i++)  
     {  
         wchar[i] = *pSrc++ << 8;    // �ȸ�λ�ֽ�  
         wchar[i] |= *pSrc++;        // ���λ�ֽ�  
     }  
     // UNICODE��-->�ַ��� 
     setlocale(LC_ALL,"chs");
     nDstLength = wcstombs(pDst,wchar,128);  
     // ����ַ����Ӹ�������  
     pDst[nDstLength] = '/0';  
      
// ����Ŀ���ַ�������  
     return nDstLength;  
}
/* =====================================================================================       
* �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����  
* �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}  
* pSrc: Դ�ַ���ָ��  
* pDst: Ŀ������ָ��  
* nSrcLength: Դ�ַ�������  
* ����: Ŀ�����ݳ��� 
* =====================================================================================
*/  
int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)  
{  
	int i;
    for(i=0; i<nSrcLength; i+=2)  
    {  
        // �����4λ  
        if(*pSrc>='0' && *pSrc<='9')  
        {  
            *pDst = (*pSrc - '0') << 4;  
        }  
        else  
        {  
            *pDst = (*pSrc - 'A' + 10) << 4;  
        }  
    
        pSrc++;  
    
        // �����4λ  
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
      
    // ����Ŀ�����ݳ���  
    return nSrcLength / 2;  
} 
/* =====================================================================================         
* �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���  
* �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"   
* pSrc: Դ����ָ��  
* pDst: Ŀ���ַ���ָ��  
* nSrcLength: Դ���ݳ���  
* ����: Ŀ���ַ�������
* =====================================================================================
*/ 
int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)  
{  
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�  
    int i;  
    for(i=0; i<nSrcLength; i++)  
    {  
        // �����4λ  
        *pDst++ = tab[*pSrc >> 4];  
    
        // �����4λ  
        *pDst++ = tab[*pSrc & 0x0f];  
    
        pSrc++;  
    }  
      
    // ����ַ����Ӹ�������  
    *pDst = '/0';  
      
    // ����Ŀ���ַ�������  
    return nSrcLength * 2;  
} 
/* =====================================================================================     
* UCS2����  
* ����: pSrc - Դ�ַ���ָ��   
* nSrcLength - Դ�ַ�������  
* ���: pDst - Ŀ����봮ָ��  
* ����: Ŀ����봮����  
	����ԭ�ͣ�
	size_t mbstowcs(wchar_t *dest, const char *src, size_t n);
	˵����
	��dest ��NULL����mbstowcs() �����Ѷ��ַ�srcת���ɿ��ַ�dest�����ת��n�ֽڡ�
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
	int Re_len=0;//�ֽ���
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
* Uncode �ַ�����
* ����: pSrc - buf
* nSrcLength - Դ�ַ�������  
* ���: pDst - Ŀ����봮ָ��  
* ����: Ŀ����봮����  
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
* PDU���룬���ڱ��ơ����Ͷ���Ϣ  
* pSrc: ԴPDU����ָ��  
* pDst: Ŀ��PDU��ָ��  
* ����: Ŀ��PDU������  
* =====================================================================================
*/
static int gsmEncodePdu(SM_PARAM* pSrc, char* pDst)  
{  
  
    int nLength=0;             // �ڲ��õĴ�����  
    int nDstLength=0;          // Ŀ��PDU������  
    int Ret=0;
    unsigned char buf[256]={0}; // �ڲ��õĻ�����  
    nLength = strlen(pSrc->TPA);    // TP-DA��ַ�ַ����ĳ���  
    buf[0] = 0x00;
    buf[1] = 0x11;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)  
    buf[2] = 0x00;               // TP-MR=0  
    buf[3] = (char)(nLength+2); // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)  
    buf[4] = 0x91;              // �̶�: �ù��ʸ�ʽ����  
    buf[5] = 0x68;
    nDstLength +=gsmBytes2String(buf, &pDst[0],6); // ת��4���ֽڵ�Ŀ��PDU��  
    nDstLength +=gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength],nLength); // ת��TP-DA��Ŀ��PDU��   
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��  
    nLength = strlen(pSrc->TP_UD);    // �û���Ϣ�ַ����ĳ���  
    buf[0] = pSrc->TP_PID;        // Э���ʶ(TP-PID)  
    buf[1] = pSrc->TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)  
    buf[2] = 0x01;            // ��Ч��(TP-VP)Ϊ5����  
    buf[3] = ASCALL_COUNT(pSrc->TP_UD,nLength);
    nDstLength +=gsmBytes2String(buf, &pDst[nDstLength],4);        // ת���ö����ݵ�Ŀ��PDU��  
    if(pSrc->TP_DCS == GSM_7BIT)      
    {  
        // 7-bit���뷽ʽ  
        nDstLength +=eat_acsii_to_gsm7bit(pSrc->TP_UD,nLength,&pDst[nDstLength]);  // ת��TP-DA��Ŀ��PDU��
        
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
        // 8-bit���뷽ʽ  
       nDstLength +=gsmEncode8bit(pSrc->TP_UD,&pDst[nDstLength],nLength);    // ת��TP-DA��Ŀ��PDU��  
    }  
    
    // ����Ŀ���ַ�������  
    return nDstLength;  
} 

/* �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ�ַ�������
//����: Ŀ���ַ�������*/
static int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
        int nDstLength;   // Ŀ���ַ�������
        char ch;          // ���ڱ���һ���ַ�
        int i=0;
        // ���ƴ�����
        nDstLength = nSrcLength;
        // �����ߵ�
        for(i=0; i<nSrcLength;i+=2)
        {
            ch = *pSrc++;        // �����ȳ��ֵ��ַ�
            *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
            *pDst++ = ch;        // �����ȳ��ֵ��ַ�
        }
        // �����ַ���'F'��
        if(*(pDst-1) == 'F')
        {
            pDst--;
            nDstLength--;        // Ŀ���ַ������ȼ�1
        }
        // ����ַ����Ӹ�������
        *pDst = '\0';
        // ����Ŀ���ַ�������
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
        int nDstLength;          // Ŀ��PDU������
        unsigned char tmp;       // �ڲ��õ���ʱ�ֽڱ���
        unsigned char buf[512]; // �ڲ��õĻ�����
        // SMSC��ַ��Ϣ��
        gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
        tmp = (tmp - 1) * 2;    // SMSC���봮����
        pSrc += 4;              // ָ�����
        gsmSerializeNumbers(pSrc, pDst->SCA, tmp);    // ת��SMSC���뵽Ŀ��PDU��
        pSrc += tmp;        // ָ�����
        // TPDU�λ����������ظ���ַ��
        gsmString2Bytes(pSrc, &tmp, 2);    // ȡ��������
        pSrc += 2;        // ָ�����
        //if(tmp & 0x80)
        //{
            // �����ظ���ַ��ȡ�ظ���ַ��Ϣ
            gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
            if(tmp & 1) tmp += 1;    // ������ż��
            pSrc += 4;          // ָ�����
            gsmSerializeNumbers(pSrc, pDst->TPA, tmp);    // ȡTP-RA����
            pSrc += tmp;        // ָ�����
        //}
        // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
        gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // ȡЭ���ʶ(TP-PID)
        pSrc += 2;        // ָ�����
        gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // ȡ���뷽ʽ(TP-DCS)
        pSrc += 2;        // ָ�����
        gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // ����ʱ����ַ���(TP_SCTS)
        pSrc += 14;       // ָ�����
        gsmString2Bytes(pSrc, &tmp, 2);    // �û���Ϣ����(TP-UDL)
        pSrc += 2;        // ָ�����
        if((pDst->TP_DCS == 0x00)||(pDst->TP_DCS == 0x00)||(pDst->TP_DCS == 0xF0)||(pDst->TP_DCS == 0xF1)||(pDst->TP_DCS == 0xF2)||(pDst->TP_DCS == 0xF3))   
        {
            // 7-bit����
            nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4); // ��ʽת��
            gsmDecode7bit(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
            nDstLength = tmp;
        }
        else if((pDst->TP_DCS == 0xF4)||(pDst->TP_DCS == 0xF5)||(pDst->TP_DCS == 0xF6)||(pDst->TP_DCS == 0xF7))
        {
            // 8-bit����
            nDstLength = DeCODE_8bit((u8 *)pSrc,buf,tmp * 2);
            memcpy(pDst->TP_UD,buf,nDstLength);// ת����TP-DU
        }
        else
        {
            // UCS2����
            nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);        // ��ʽת��
            HexToAsicc(buf,nDstLength);
            nDstLength = DEnCODE_USC2(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
        }
        // ����Ŀ���ַ�������
        return nDstLength;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: sms_msg_send_envelope
 *  Description: sms_msg_send_envelope 
 *        Input: *pSrc :��������,  Dest:���ŵ�ַ         
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
	uint8_t scNumber[40]={"+8613800270500"};//�������ĺ���
	memset(pDst,0,256);
	eat_set_sms_sc(scNumber);
	eat_set_sms_format(0);//����ΪPUD����	
	memset(Msg_Informatin.TPA,0,16);
	memcpy(Msg_Informatin.TPA,Dest,strlen(Dest));//�����ֻ���
	memset(Msg_Informatin.TP_UD,0,161);
	memcpy(Msg_Informatin.TP_UD,pSrc,strlen(pSrc));//ԭʼ����	
	Msg_Informatin.TP_PID=0;
	Msg_Informatin.TP_DCS=GSM_UCS2;
	PDU_Data_Length=gsmEncodePdu(&Msg_Informatin,pDst); //����ת��eat_set_sms_format(0);
	if(PDU_Data_Length<0) return;
	eat_send_pdu_sms(pDst,(PDU_Data_Length/2)-1);

}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name: void sms_time_out_handle(void)
 *  Description: ���ŷ��ͳ�ʱ����
 *        Input: ÿ�γ���30s ������        
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
           if(sms_send_times==2)//���γ�ʱ���ٷ��͸ú���
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
 *  Description: ��װ������Ϣ
 *        Input: data,������Ϣ���ݣ�buffer,��װ�������       
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
 *  Description: ���ͱ�����Ϣ
 *        Input: ucode_data,������Ϣ����     
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
        eat_trace("worning_sms_send--> ���ź���Ϊ�� .");
        #endif
    }
}
/* =====================================================================================           
* ���ڶ������ݽ���ƥ��  
* pSrc: ԴPDU����ָ��  
* Data_Len:   �ַ�������
* ����: NULL
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
/***���ŷ�������***/
/* 
 * ===  FUNCTION  ======================================================================
 *  Name: void sms_send_list_init(void)
 *  Description:  ��ʼ�����ŷ�������
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
 *  Description:  ��ӷ��Ͷ�����Ϣ
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
 *  Description:  ���Ͷ���
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
        sms_msg_send_envelope(mem_prt->data_buf,mem_prt->send_addr);//��������
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
 *  Description: ����ŷ���������ѹ����Ϣ
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
 *  Description:  ��������ɾ�����ͳɹ��Ķ���
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

