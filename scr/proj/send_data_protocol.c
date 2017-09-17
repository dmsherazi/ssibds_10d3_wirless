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
static uint8_t Send_Pdest[TcpSendMaxByte]={0};//���͵�����
static uint8_t Send_Temp_Pdest[TcpSendMaxByte]={0};//��ʱ������

/* 
 * ===  FUNCTION  ======================================================================
 *  Name: FrameSend_Switch
 *  Description:  TCP��������ת�� �ӱ�ʶͷ��ʶβ
 *  Parameters :  �β�1-- pScr -> Դ����    
 *				  �β�2 -- pDes -> Ŀ������   
 *				  �β�3 -- sLens -> Դ���鳤�� 
 *		
 *	
 *  Return     :   word -> ����ת���ĳ���
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
	*pDes_temp=Tcp_Fram_X7E;//β�ӱ�ʾ��
	*pDes=Tcp_Fram_X7E;//ͷ�ӱ�ʾ��
	return  temp_lens+2;
}


/* 
 * ===  FUNCTION  ======================================================================
 *  Name: Create__Send_Communication_Frame
 *  Description:  TCP��Ϣ��װ�ͷ��� 
 *  Parameters :  Msg_Id           :��ϢID
 *				  Msg_Property     :��Ϣ����(���ܣ����ȣ��ְ�)
 *				  *Phone_Num	   :����绰���������
 *				  Msg_Sn		   :��Ϣ��ˮ��
 *				  *Pdes            :Ŀ�괢��BUFFER
 *				  Package_Flag     :�Ƿ�ְ���־
 *				  PackageNum       :�ְ���Ŀ
 *				  PackageCnt       :���͵ĵڼ���
 *  Return     :  void 
 *  author     :  J.x.b create at SSI_1D02 at 2014-6-7			 
 * =====================================================================================
 */
void Tcp_Message_PacKage_And_Send(int16_t socket ,uint16_t * msg_servn,uint16_t Msg_Id,uint8_t *Psource,uint16_t Plens,char_t Package_Flag,uint16_t PackageNum,uint16_t Package_Cnt)//�ְ���Ŀ
{
	uint8_t CheckSum=0;//У������
	uint8_t *pscr = NULL;
	uint16_t Temp_Lens = 0;
	uint16_t pack_offest = 0;
	static Msg_Head Msg_Head_Data;//��Ϣͷ�ṹ��
	pscr=Psource;//ԭʼ����
	Temp_Lens=Plens;//���ݳ���
	/******************��ϢID***********************/
	Msg_Head_Data.MsgID = Msg_Id;//��ϢID
	Big_End_WordtoByte(Msg_Head_Data.MsgID,Send_Temp_Pdest);
	pack_offest +=sizeof(Msg_Head_Data.MsgID);
	
	/******************��Ϣ������***********************/
	Msg_Head_Data.MsgAttr.Bits.Rsv =0x00;//������Ϣд��
	if(Package_Flag==EAT_TRUE)
	{
        Msg_Head_Data.MsgAttr.Bits.SubpackFlag=0x01;//�ְ� SUB_PACKAGE_Num
	}
	else
	{
        Msg_Head_Data.MsgAttr.Bits.SubpackFlag=0x00;//���ְ� SUB_PACKAGE_Num
	}
	Msg_Head_Data.MsgAttr.Bits.EncryType =0x00;//�޼���
    Msg_Head_Data.MsgAttr.Bits.MessLens = Plens;//��Ϣ�峤��
    Big_End_WordtoByte(Msg_Head_Data.MsgAttr.Msg_Abbri,Send_Temp_Pdest+pack_offest);//��Ϣ������
    pack_offest += sizeof(Msg_Head_Data.MsgAttr.Msg_Abbri);
    
	/********************�ն��ֻ���********************/
	memcpy(Send_Temp_Pdest+pack_offest,system_param.SaveTerminlPhone.terminl_phone_numd,6);
    pack_offest += 6;
    /********************��Ϣ��ˮ��***********************/
    subcontract_servn_num_pro(Msg_Id,Package_Cnt,msg_servn,&(Msg_Head_Data.ProductID));
    Big_End_WordtoByte(Msg_Head_Data.ProductID,Send_Temp_Pdest+pack_offest);//��Ϣ������
    pack_offest += sizeof(Msg_Head_Data.ProductID);

    /********************�ְ���Ϣ***********************/	 
    if(Msg_Head_Data.MsgAttr.Bits.SubpackFlag == 0x01)
    {
		Msg_Head_Data.PackInfo.PackTotalNum = PackageNum;// ������
		Big_End_WordtoByte(Msg_Head_Data.PackInfo.PackTotalNum,Send_Temp_Pdest+pack_offest);
        pack_offest += sizeof(Msg_Head_Data.PackInfo.PackTotalNum);

		Msg_Head_Data.PackInfo.PackSN =Package_Cnt;//�����
		Big_End_WordtoByte(Msg_Head_Data.PackInfo.PackSN,Send_Temp_Pdest+pack_offest);
        pack_offest += sizeof(Msg_Head_Data.PackInfo.PackSN);
    }
    
    /********************��Ϣ����***********************/	 
    memcpy(Send_Temp_Pdest+pack_offest,pscr,Msg_Head_Data.MsgAttr.Bits.MessLens);
    pack_offest +=Msg_Head_Data.MsgAttr.Bits.MessLens;
    
    /********************У����Ϣ***********************/
    CheckSum=CrcXOR(Send_Temp_Pdest,pack_offest);//���У��
    *(Send_Temp_Pdest+pack_offest)=CheckSum;
    pack_offest += sizeof(CheckSum);
    
    /********************ת��*********************/
    pack_offest = FrameSend_Switch(Send_Temp_Pdest,Send_Pdest,pack_offest);//�������ݳ���

    /********************��������*********************/
    eat_soc_send(socket,Send_Pdest,pack_offest);//��������
    #if APP_TRACE_DBG 
    eat_trace("Tcp_Message_PacKage_And_Send PAKAGED--> mode id:%d ,msg_id:%0x ,len:%u,package num:%u",socket,Msg_Head_Data.MsgID,pack_offest,Msg_Head_Data.PackInfo.PackSN);
    HexToAsicc(Send_Pdest,pack_offest);
    #endif
}


