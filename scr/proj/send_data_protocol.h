/********************************************************************
 *                Copyright co. Ltd.                  
 *---------------------------------------------------------------------
 * FileName      :   xxxxxx.c
 * version       :   0.1
 * Description   :   Founction of this Source of C;
 * Authors       :   j.x.b
 * Notes         :
 *---------------------------------------------------------------------
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2016/3/9, j.x.b originally.
 *--------------------------------------------------------------------
 * File Description
 *
 *--------------------------------------------------------------------
 ********************************************************************/

#ifndef SEND_DATA_PROTOCOL_H
   #define SEND_DATA_PROTOCOL_H
   
   #include "eat_interface.h"

#define  Tcp_Fram_X7E            0x7e   //TCP帧头0x7e
#define  Tcp_Fram_X7D            0x7d   //TCP帧头0x7d
#define  Tcp_Fram_X7E_Tail       0x02   //TCP帧头0x7e后
#define  Tcp_Fram_X7D_Tail       0x01   //TCP帧头0x7d尾


#define TcpSendMaxByte 1536

/********************************************************************
 * Include Files
 ********************************************************************/
 

/********************************************************************
* Macros
 ********************************************************************/
#define Tcp_Heart_Beat_ID     0x0002//心跳

/********************************************************************
* Types
********************************************************************/
typedef  union _Message_Attribute//消息结构
{
    uint16_t Msg_Abbri;
    struct
    {
        uint16_t   MessLens        : 10;      //消息体长度
        uint16_t   EncryType       : 3;        //消息体加密方式
        uint16_t   SubpackFlag     : 1;        //分包标志
        uint16_t   Rsv             : 2;        //保留位           
    }Bits;
}Msg_Attribute;
       
/*------------------------包信息属性---------------------*/
typedef  struct  _Package_Info                 //包信息属性
{
    uint16_t PackTotalNum;//包总计数
    uint16_t PackSN      ;//包序号
}Package_Info;
       
       
/*-----------------------消息头结构-----------------------*/          
typedef  struct _Message_Head            //消息头结构
{
    uint16_t    MsgID;      //消息ID
    Msg_Attribute MsgAttr;//消息属性
    uint8_t PhoneID[6];//手机号
    uint16_t ProductID;//产品流水号
    Package_Info   PackInfo;//分包信息
}Msg_Head;

/********************************************************************
* Extern Variables (Extern /Global)
********************************************************************/


/********************************************************************
* Local Variables:  STATIC
********************************************************************/
    
/********************************************************************
* External Functions declaration
********************************************************************/
extern void Tcp_Message_PacKage_And_Send(int16_t socket ,uint16_t * msg_servn,uint16_t Msg_Id,uint8_t *Psource,uint16_t Plens,char_t Package_Flag,uint16_t PackageNum,uint16_t Package_Cnt);//分包数目

    
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    





