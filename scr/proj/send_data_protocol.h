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

#define  Tcp_Fram_X7E            0x7e   //TCP֡ͷ0x7e
#define  Tcp_Fram_X7D            0x7d   //TCP֡ͷ0x7d
#define  Tcp_Fram_X7E_Tail       0x02   //TCP֡ͷ0x7e��
#define  Tcp_Fram_X7D_Tail       0x01   //TCP֡ͷ0x7dβ


#define TcpSendMaxByte 1536

/********************************************************************
 * Include Files
 ********************************************************************/
 

/********************************************************************
* Macros
 ********************************************************************/
#define Tcp_Heart_Beat_ID     0x0002//����

/********************************************************************
* Types
********************************************************************/
typedef  union _Message_Attribute//��Ϣ�ṹ
{
    uint16_t Msg_Abbri;
    struct
    {
        uint16_t   MessLens        : 10;      //��Ϣ�峤��
        uint16_t   EncryType       : 3;        //��Ϣ����ܷ�ʽ
        uint16_t   SubpackFlag     : 1;        //�ְ���־
        uint16_t   Rsv             : 2;        //����λ           
    }Bits;
}Msg_Attribute;
       
/*------------------------����Ϣ����---------------------*/
typedef  struct  _Package_Info                 //����Ϣ����
{
    uint16_t PackTotalNum;//���ܼ���
    uint16_t PackSN      ;//�����
}Package_Info;
       
       
/*-----------------------��Ϣͷ�ṹ-----------------------*/          
typedef  struct _Message_Head            //��Ϣͷ�ṹ
{
    uint16_t    MsgID;      //��ϢID
    Msg_Attribute MsgAttr;//��Ϣ����
    uint8_t PhoneID[6];//�ֻ���
    uint16_t ProductID;//��Ʒ��ˮ��
    Package_Info   PackInfo;//�ְ���Ϣ
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
extern void Tcp_Message_PacKage_And_Send(int16_t socket ,uint16_t * msg_servn,uint16_t Msg_Id,uint8_t *Psource,uint16_t Plens,char_t Package_Flag,uint16_t PackageNum,uint16_t Package_Cnt);//�ְ���Ŀ

    
/********************************************************************
* Local Function declaration
********************************************************************/
     
#endif

    





