/****************************************************
 * Include Files
 ****************************************************/ 
 
#include "send_data_protocol.h"



 /****************************************************
* External variables declaration
*****************************************************/


/*****************************************************
* Local variables declaration(Static)
******************************************************/
static uint8_t Send_Pdest[TcpSendMaxByte]={0};//发送的数据
static uint8_t Send_Temp_Pdest[TcpSendMaxByte]={0};//临时数据体

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: FrameSend_Switch
 *  Description:  TCP发送数据转义 加标识头标识尾
 *  Parameters :  形参1-- pScr -> 源数组    
 *				  形参2 -- pDes -> 目标数组   
 *				  形参3 -- sLens -> 源数组长度 
 *		
 *	
 *  Return     :   word -> 返回转义后的长度
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-10			 
 * =====================================================================================
 */
uint16_t FrameSend_Switch(uint8_t * pScr,uint8_t * pDes,uint16_t sLens)
{
	uint16_t  temp_lens=0;
	uint16_t  cnt;
	uint8_t * pScr_temp;
	uint8_t * pDes_temp;
	pScr_temp=pScr;
	pDes_temp=pDes+1;
	for(cnt=0;cnt<sLens;cnt++)
    {
		switch(*pScr_temp)
		{
			case Tcp_Fram_X7E:
			    *(pDes_temp++)=Tcp_Fram_X7D;
			    *(pDes_temp++)=Tcp_Fram_X7E_Tail;
				temp_lens+=2;
				pScr_temp++;
				break;
			case Tcp_Fram_X7D:
				*(pDes_temp++)=Tcp_Fram_X7D;
				*(pDes_temp++)=Tcp_Fram_X7D_Tail;		
				temp_lens+=2;
				pScr_temp++;
				break;
			default:
				*(pDes_temp++)=*(pScr_temp++);
				temp_lens++;
				break;
			
		}
	}
	*pDes_temp=Tcp_Fram_X7E;//尾加标示符
	*pDes=Tcp_Fram_X7E;//头加标示符
	return  temp_lens+2;
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Create__Send_Communication_Frame
 *  Description:  TCP信息封装和发送 
 *  Parameters :  Msg_Id           :消息ID
 *				  Msg_Property     :消息属性(加密，长度，分包)
 *				  *Phone_Num	   :储存电话号码的数组
 *				  Msg_Sn		   :信息流水号
 *				  *Pdes            :目标储存BUFFER
 *				  Package_Flag     :是否分包标志
 *				  PackageNum       :分包数目
 *				  PackageCnt       :发送的第几包
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void Tcp_Message_PacKage_And_Send(int16_t socket ,uint16_t * msg_servn,uint16_t Msg_Id,uint8_t *Psource,uint16_t Plens,char_t Package_Flag,uint16_t PackageNum,uint16_t Package_Cnt)//分包数目
{
	uint8_t CheckSum=0;//校验数据
	uint8_t *pscr = NULL;
	uint16_t Temp_Lens = 0;
	uint16_t pack_offest = 0;
	static Msg_Head Msg_Head_Data;//消息头结构体
	pscr=Psource;//原始数据
	Temp_Lens=Plens;//数据长度
	/******************消息ID***********************/
	Msg_Head_Data.MsgID = Msg_Id;//消息ID
	Big_End_WordtoByte(Msg_Head_Data.MsgID,Send_Temp_Pdest);
	pack_offest +=sizeof(Msg_Head_Data.MsgID);
	
	/******************消息体属性***********************/
	Msg_Head_Data.MsgAttr.Bits.Rsv =0x00;//保留信息写无
	if(Package_Flag==EAT_TRUE)
	{
        Msg_Head_Data.MsgAttr.Bits.SubpackFlag=0x01;//分包 SUB_PACKAGE_Num
	}
	else
	{
        Msg_Head_Data.MsgAttr.Bits.SubpackFlag=0x00;//不分包 SUB_PACKAGE_Num
	}
	Msg_Head_Data.MsgAttr.Bits.EncryType =0x00;//无加密
    Msg_Head_Data.MsgAttr.Bits.MessLens = Plens;//消息体长度
    Big_End_WordtoByte(Msg_Head_Data.MsgAttr.Msg_Abbri,Send_Temp_Pdest+pack_offest);//消息体属性
    pack_offest += sizeof(Msg_Head_Data.MsgAttr.Msg_Abbri);
    
	/********************终端手机号********************/
	memcpy(Send_Temp_Pdest+pack_offest,system_param.SaveTerminlPhone.terminl_phone_numd,6);
    pack_offest += 6;
    /********************消息流水号***********************/
    subcontract_servn_num_pro(Msg_Id,Package_Cnt,msg_servn,&(Msg_Head_Data.ProductID));
    Big_End_WordtoByte(Msg_Head_Data.ProductID,Send_Temp_Pdest+pack_offest);//消息体属性
    pack_offest += sizeof(Msg_Head_Data.ProductID);

    /********************分包信息***********************/	 
    if(Msg_Head_Data.MsgAttr.Bits.SubpackFlag == 0x01)
    {
		Msg_Head_Data.PackInfo.PackTotalNum = PackageNum;// 包总数
		Big_End_WordtoByte(Msg_Head_Data.PackInfo.PackTotalNum,Send_Temp_Pdest+pack_offest);
        pack_offest += sizeof(Msg_Head_Data.PackInfo.PackTotalNum);

		Msg_Head_Data.PackInfo.PackSN =Package_Cnt;//包序号
		Big_End_WordtoByte(Msg_Head_Data.PackInfo.PackSN,Send_Temp_Pdest+pack_offest);
        pack_offest += sizeof(Msg_Head_Data.PackInfo.PackSN);
    }
    
    /********************消息数据***********************/	 
    memcpy(Send_Temp_Pdest+pack_offest,pscr,Msg_Head_Data.MsgAttr.Bits.MessLens);
    pack_offest +=Msg_Head_Data.MsgAttr.Bits.MessLens;
    
    /********************校验信息***********************/
    CheckSum=CrcXOR(Send_Temp_Pdest,pack_offest);//异或校验
    *(Send_Temp_Pdest+pack_offest)=CheckSum;
    pack_offest += sizeof(CheckSum);
    
    /********************转换*********************/
    pack_offest = FrameSend_Switch(Send_Temp_Pdest,Send_Pdest,pack_offest);//返回数据长度

    /********************发送数据*********************/
    eat_soc_send(socket,Send_Pdest,pack_offest);//发送数据
    #if APP_TRACE_DBG 
    eat_trace("Tcp_Message_PacKage_And_Send PAKAGED--> mode id:%d ,msg_id:%0x ,len:%u,package num:%u",socket,Msg_Head_Data.MsgID,pack_offest,Msg_Head_Data.PackInfo.PackSN);
    HexToAsicc(Send_Pdest,pack_offest);
    #endif
}


